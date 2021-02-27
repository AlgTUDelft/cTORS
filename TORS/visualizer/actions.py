from flask_restful import Resource
from flask import request, Response, current_app
import json

from pyTORS import ArriveAction, ExitAction, BeginMoveAction, EndMoveAction, SplitAction, CombineAction, WaitAction, ServiceAction, MoveAction


class Actions(Resource):

    def get(self) -> str:
        """
        Get all possible actions and return them in json

        :return: all possible actions in json response
        """

        actions = {}
        for idx, a in enumerate(current_app.engine.get_valid_actions(current_app.state)):
            # get shunting units from action
            shunting_units = [a.shunting_unit] # add other shunting units with combine
            shunting_units = [{
                "uuid": str(su.id),
                "id": str(su.id),
                "train_units": "-".join([str(tu.type) for tu in su.train_units])
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
        action = current_app.engine.get_valid_actions(current_app.state)[action_id]

        current_app.engine.apply_action(current_app.state, action)
        current_app.engine.step(current_app.state)
