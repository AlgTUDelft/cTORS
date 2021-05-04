#include "Train.h"

map<string, TrainUnitType *> TrainUnitType::types;

void TrainUnitType::Serialize(PBTrainUnitType* pb_tt) const {
	pb_tt->set_displayname(displayName);
	pb_tt->set_carriages(carriages);
	pb_tt->set_length(length);
	pb_tt->set_combineduration(combineDuration);
	pb_tt->set_splitduration(splitDuration);
	pb_tt->set_backnormtime(backNormTime);
	pb_tt->set_backadditiontime(backAdditionTime);
	pb_tt->set_travelspeed(travelSpeed);
	pb_tt->set_startuptime(startUpTime);
	pb_tt->set_typeprefix(typePrefix);
	pb_tt->set_needsloco(needsLoco);
	pb_tt->set_isloco(isLoco);
	pb_tt->set_needselectricity(needsElectricity);
}

void Task::Serialize(PBTask* pb_task) const {
	auto pb_taskType = pb_task->mutable_type();
	pb_taskType->set_other(taskType);
	pb_task->set_priority(priority);
	pb_task->set_duration(duration);
	for(auto& s: skills) {
		pb_task->add_requiredskills(s);
	}
}

Train::Train(const PBTrainUnit& pb_train) 
	: Train(pb_train.id()=="****" ? -1 : stoi(pb_train.id()), TrainUnitType::types.at(pb_train.typedisplayname())) {}

bool Train::operator==(const Train& train) const {
	return (id != -1 && id == train.id) || this == &train;
}

const string Train::toString() const {
	return "(" + to_string(id) + ", " + type->toString() + ")";
}

void Train::Serialize(PBTrainUnit* pb_t) const {
	pb_t->set_id(to_string(id));
	pb_t->set_typedisplayname(type->displayName);
}
