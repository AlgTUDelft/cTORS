#include "Plan.h"

int POSAction::newUID{ 0 };

const vector<int> GetTrainIDs(const PBList<string>& pb_train_ids) {
    vector<string> trainIDs(pb_train_ids.begin(), pb_train_ids.end());
    vector<int> trains(trainIDs.size());
    transform(trainIDs.begin(), trainIDs.end(), trains.begin(), [](const string& s) -> int {return stoi(s); });
    return trains;
}

POSAction& POSAction::operator=(const POSAction& pa) {
    if(this != &pa) {
        id = pa.id;
        suggestedStart = pa.suggestedStart;
        suggestedEnd = pa.suggestedEnd;
        minDuration = pa.minDuration;
        delete action;
        action = pa.action->Clone();
    }
    return *this;
}

POSAction POSAction::CreatePOSAction(const Location* location, const Scenario* scenario, const PBAction& pb_action) {
    int suggestedStartingTime = pb_action.suggestedstartingtime();
    int suggestedEndingTime = pb_action.suggestedfinishingtime();
    int minDuration = pb_action.minimumduration();
    vector<int> trainIDs = GetTrainIDs(pb_action.trainunitids());
    SimpleAction* action;
    if(pb_action.has_movement()) {
        auto track_id = *(pb_action.movement().path().end()-1);
        const Track* destination = location->GetTrackByID(to_string(track_id));
        action = new Move(trainIDs, destination); //TODO change to multi-move
    } else if(pb_action.has_task()) {
        auto& taskType = pb_action.task().type();
        if(taskType.has_predefined()) {
            auto task = taskType.predefined();
            switch(task) {
                case PBPredefinedTaskType::Arrive: 
                    action = new Arrive(scenario->GetIncomingByTrainIDs(trainIDs)); break;
                case PBPredefinedTaskType::Exit:
                {
                    vector<const TrainUnitType*> types;
                    for(int id: trainIDs)  types.push_back(scenario->GetTrainByID(id)->GetType());
                    auto& outgoingTrains = scenario->GetOutgoingTrains();
                    int start = pb_action.suggestedstartingtime();
                    int end = pb_action.suggestedfinishingtime();
                    auto it = find_if(outgoingTrains.begin(), outgoingTrains.end(),
                        [start, end, &trainIDs, &types] (const Outgoing* out) -> bool { 
                            return out->GetTime() >= start && 
                                    out->GetTime() <= end &&
                                    out->GetShuntingUnit()->MatchesTrainIDs(trainIDs, types);
                        });
                    if(it == outgoingTrains.end())
                        throw invalid_argument("Outgoing Train with ids " + Join(outgoingTrains.begin(), outgoingTrains.end(), ", ") + " does not exist.");
                    action = new Exit(trainIDs, (*it)->GetID());
                    break;
                }
                case PBPredefinedTaskType::Split:
                {
                    // TODO how is the split action defined in protobuf? Current implementation: store the IDs of the first train.
                    vector<int> firstTrainIDs = GetTrainIDs(pb_action.task().trainunitids());
                    int splitIndex = 1;
                    for(auto& id: firstTrainIDs) {
                        auto it = find(trainIDs.begin(), trainIDs.end(), id);
                        if(it == trainIDs.end())
                            throw invalid_argument("Train with id " + to_string(id) + " not found in shunting unit "
                                + Join(trainIDs.begin(), trainIDs.end(), ","));
                        int ix = it - trainIDs.begin();
                        splitIndex = ix + 1 > splitIndex ? ix + 1 : splitIndex;
                    }
                    action = new Split(trainIDs, splitIndex);
                    break;
                }
                case PBPredefinedTaskType::Combine:
                {
                    // TODO how is the combine action defined in protobuf? Current implementation: store the IDs of the second train.
                    auto secondTrainIDs = GetTrainIDs(pb_action.task().trainunitids());
                    action = new Combine(trainIDs, secondTrainIDs); break;
                }
                case PBPredefinedTaskType::Walking:
                    action = new Setback(trainIDs); break;
                case PBPredefinedTaskType::Break:
                    action = new Wait(trainIDs); break;
                case PBPredefinedTaskType::BeginMove: //TEMP
                    action = new BeginMove(trainIDs); break; //TEMP
                case PBPredefinedTaskType::EndMove: //TEMP
                    action = new EndMove(trainIDs); break; //TEMP
                case PBPredefinedTaskType::NonService:
                    throw invalid_argument("Non-service tasks not implemented.");
                case PBPredefinedTaskType::Move:
                    throw invalid_argument("Move task type not used. Use POSMovement instead.");
                default:
                    throw invalid_argument("Invalid predefined task type " + to_string(task));
            }
        } else {
            const Train* train = scenario->GetTrainByID(stoi(pb_action.task().trainunitids().at(0)));
            auto tasks = scenario->GetTasksForTrain(train);
            auto taskTypeString = taskType.other();
            auto it = find_if(tasks.begin(), tasks.end(), [taskTypeString](auto& t) -> bool {return t.taskType == taskTypeString; });
            if(it==tasks.end()) throw invalid_argument("Could not find task " + taskTypeString + " for train " + train->toString());
            const Facility* facility = location->GetFacilityByID(pb_action.task().facilities().at(0).id());
            action = new Service(trainIDs, *it, *train, facility);
        }
    } else {
        action = new Wait(trainIDs);
    } 
    return POSAction(suggestedStartingTime, suggestedEndingTime, minDuration, action); 
}

void POSAction::Serialize(const LocationEngine& engine, const State* state, PBAction* pb_action) const {
    pb_action->set_suggestedstartingtime(suggestedStart);
    pb_action->set_suggestedfinishingtime(suggestedEnd);
    pb_action->set_minimumduration(minDuration);
    debug_out("Serialize train ids");
    for(auto& t: action->GetTrainIDs()) {
        *(pb_action->add_trainunitids()) = to_string(t);
    }
    if(instanceof<Move>(action)) {
        debug_out("Serialize move action");
        auto move = dynamic_cast<const Move*>(action);
        auto pb_move = pb_action->mutable_movement();
        auto path = engine.GetPath(state, *move);
        auto su = state->GetShuntingUnitByTrainIDs(move->GetTrainIDs());
        for(auto t: path.route) {
            pb_move->add_path(stoi(t->GetID()));
        }
        auto current = state->GetPosition(su);
        auto previous = state->GetPrevious(su);
        auto destination = engine.GetLocation().GetTrackByID(move->GetDestinationID());
        auto prev_destination = prev(path.route.back());
        pb_move->set_fromside(current->IsASide(previous) ? PBSide::B : PBSide::A );
        pb_move->set_toside(destination->IsASide(prev_destination) ? PBSide::A : PBSide::B );
        pb_move->set_order(0);
    } else if(instanceof<Wait>(action)) {
        auto pb_wait = pb_action->mutable_break_();
    } else {
        auto pb_task = pb_action->mutable_task();
        auto pb_task_type = pb_task->mutable_type();
        if(instanceof<Service>(action)) {
            auto service = dynamic_cast<const Service*>(action);
            auto facility = engine.GetLocation().GetFacilityByID(service->GetFacilityID());
            pb_task_type->set_other(service->GetTask().taskType);
            pb_task->set_location(stoi(facility->GetTracks().at(0)->GetID()));
            auto pb_facility = pb_task->add_facilities();
            pb_facility->set_id(facility->GetID());
            pb_task->add_trainunitids(to_string(service->GetTrain().GetID()));
        } else if(instanceof<Split>(action)) {
            pb_task_type->set_predefined(PBPredefinedTaskType::Split);
            auto split = dynamic_cast<const Split*>(action);
            debug_out("Serialize split action " + split->toString() + " #trains: " + to_string(split->GetTrainIDs().size()));
            // TODO how is the split action defined in protobuf? Current implementation: store the IDs of the first train.
            for(int i=0; i<split->GetSplitIndex(); i++) {
                debug_out("Serialize split action: " + to_string(i));
                pb_task->add_trainunitids(to_string(split->GetTrainIDs().at(i)));
            } 
        } else if(instanceof<Combine>(action)) {
            pb_task_type->set_predefined(PBPredefinedTaskType::Combine);
            auto combine = dynamic_cast<const Combine*>(action);
            // TODO how is the combine action defined in protobuf? Current implementation: store the IDs of the second train.
            for(auto& t: combine->GetSecondTrainIDs())
                pb_task->add_trainunitids(to_string(t));
        } else if(instanceof<Setback>(action)) {
            pb_task_type->set_predefined(PBPredefinedTaskType::Walking);
        } else if(instanceof<Arrive>(action)) {
            pb_task_type->set_predefined(PBPredefinedTaskType::Arrive);
        } else if(instanceof<Exit>(action)) {
            pb_task_type->set_predefined(PBPredefinedTaskType::Exit);
        } else if(instanceof<BeginMove>(action)) { //TEMP
            pb_task_type->set_predefined(PBPredefinedTaskType::BeginMove);//TEMP
        } else if(instanceof<EndMove>(action)) {//TEMP
            pb_task_type->set_predefined(PBPredefinedTaskType::EndMove);//TEMP
        } else {
            throw invalid_argument("Action serialization not implemented for action " + action->toString());
        }
    }
}

POSPlan POSPlan::CreatePOSPlan(const Location* location, const Scenario* scenario, const PBPOSPlan& pb_plan) {
    vector<PBAction> pb_actions(pb_plan.actions().begin(), pb_plan.actions().end());
    vector<POSAction> actions;
    transform(pb_actions.begin(), pb_actions.end(), back_inserter(actions),
        [location, scenario](const PBAction& pba) -> const POSAction {return POSAction::CreatePOSAction(location, scenario, pba);});
    return POSPlan(actions);
}

void POSPlan::Serialize(LocationEngine& engine, const Scenario& scenario, PBPOSPlan* pb_plan) const {
    auto state = engine.StartSession(scenario);
    auto it = actions.begin();
    while(it != actions.end()) {
        try {
            debug_out("Serializing T=" + to_string(state->GetTime()) + ". A=" + it->GetAction()->toString() + " at T=" + to_string(it->GetSuggestedStart()) +".");
            engine.Step(state);
            debug_out("Finished Step Update [T="+to_string(state->GetTime())+"].");
            if(state->GetTime() >= it->GetSuggestedStart()) {
                if(instanceof<Wait>(it->GetAction())) {
                    //SKIP
                } else if(true || (!instanceof<BeginMove>(it->GetAction()) && !instanceof<EndMove>(it->GetAction()))) {
                    auto pb_action = pb_plan->add_actions();
                    debug_out("Serialize action");
                    it->Serialize(engine, state, pb_action);
                    debug_out("Finish Serialize action");
                }
                debug_out("Apply action");
                engine.ApplyAction(state, *(it->GetAction()));
                debug_out("End action");
                it++;
            }
        } catch (ScenarioFailedException e) {
			cout << "Scenario failed.\n";
            pb_plan->set_feasible(false);
			break;
		}
    }
    pb_plan->set_feasible(state->GetShuntingUnits().size() == 0);
    engine.EndSession(state);
}

void POSPlan::SerializeToFile(LocationEngine& engine, const Scenario& scenario, const string& outfile) const {
    PBPOSPlan pb_plan;
    debug_out("Start Serializing plan.")
    Serialize(engine, scenario, &pb_plan);
    debug_out("End Serializing plan.")
    parse_pb_to_json(outfile, pb_plan);
}


void RunResult::Serialize(LocationEngine& engine, PBRun* pb_run) const {
    pb_run->set_location(location);
    scenario.Serialize(pb_run->mutable_scenario());
    plan.Serialize(engine, scenario, pb_run->mutable_plan());
    pb_run->set_feasible(feasible);
}

void RunResult::SerializeToFile(LocationEngine& engine, const string& outfile) const {
    PBRun pb_run;
    Serialize(engine, &pb_run);
    parse_pb_to_json(outfile, pb_run);
}

RunResult* RunResult::CreateRunResult(const Engine& engine, const PBRun& pb_run) {
    string locationString = pb_run.location();
    auto& location = engine.GetLocation(locationString);
    return CreateRunResult(&location, pb_run);
}

RunResult* RunResult::CreateRunResult(const Location* location, const PBRun& pb_run) {
    Scenario scenario = Scenario(pb_run.scenario(), *location);
    POSPlan plan = POSPlan::CreatePOSPlan(location, &scenario, pb_run.plan());
    bool feasible = pb_run.feasible();
    return new RunResult(location->GetLocationFilePath(), scenario, plan, feasible);
}