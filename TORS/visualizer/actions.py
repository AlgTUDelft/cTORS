from flask_restful import Resource
from flask import request, Response, current_app
import json

from pyTORS import ScenarioFailedError, ArriveAction, ExitAction, BeginMoveAction, EndMoveAction,\
    SplitAction, CombineAction, WaitAction, ServiceAction, MoveAction

def get_valid_actions():
    if not current_app.result is None:
        actions = current_app.result.plan.get_actions()
        if current_app.action_index < len(actions):
            a = actions[current_app.action_index]  
            _a = current_app.engine.generate_action(current_app.state, a.action)
            return [_a]
        return []
    else:
        return current_app.engine.get_valid_actions(current_app.state)


class Actions(Resource):

    def get(self) -> str:
        """
        Get all possible actions and return them in json

        :return: all possible actions in json response
        """

        actions = {}
        try:
            if not current_app.done:
                valid_actions = get_valid_actions() 
                if len(valid_actions) == 0:
                    current_app.done = True
                    if len(current_app.state.incoming_trains) == 0 and len(current_app.state.outgoing_trains) == 0:
                        current_app.message = "Scenario solved!"
                    else:
                        current_app.message = "Scenario failed! There are remaining incoming or outgoing trains."
        except ScenarioFailedError as e:
            current_app.done = True
            current_app.message = "Scenario failed!"
        if current_app.done:
            return Response(json.dumps({"message": current_app.message}), mimetype='application/json')
        for idx, a in enumerate(valid_actions):
            # get shunting units from action
            shunting_units = [a.shunting_unit] # add other shunting units with combine
            shunting_units = [{
                "uuid": str(su.id),
                "id": str(su.id),
                "trains": "-".join([str(tu.type) for tu in su.trains])
            } for su in shunting_units]
            

            actions[idx] = {"type": a.__class__.__name__,
                            "train_ids": [str(su["uuid"]) for su in shunting_units],                            
                            "trains": shunting_units,
                            "tracks": [str(t.id) for t in a.reserved_tracks],
                            "track_names": [str(t.name) for t in a.reserved_tracks],
                            "duration": a.duration,
                            "employees": [str(e) for e in a.employees],
                            "reward": 0}

            if isinstance(a, ServiceAction):
                actions[idx]["task"] = str(a.task)

        return Response(json.dumps(actions), mimetype='application/json')

    def put(self):
        """
        Perform an action
        """

        action_id = int(request.args["action"])
        valid_actions = get_valid_actions()
        if action_id < 0 or action_id >= len(valid_actions):
            return Response("Invalid action id", status=400)
        action = valid_actions[action_id]
        
        current_app.engine.apply_action(current_app.state, action)
        try:
            current_app.engine.step(current_app.state)
        except ScenarioFailedError as e:
            current_app.done = True
            current_app.message = "Scenario failed! " + str(e) 

        if not current_app.result is None:
            current_app.action_index += 1
