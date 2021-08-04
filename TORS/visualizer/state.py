from flask_restful import Resource
from flask import Response, request, current_app
import json
from typing import List
from pyTORS import ShuntingUnit
from plan import get_plan

class State(Resource):

    def get(self) -> str:
        """
        Get current state and put in json

        :return: Part of the state in a json response
        """
        state = {
            "time": self.get_time(),
            "tracks": self.get_tracks(),
            "reserved_tracks": self.get_reserved_tracks(),
            "next_event": self.get_next_event(),
            "goals": self.get_goals()
        }

        return Response(json.dumps(state), mimetype='application/json')

    def put(self):
        """
        Reset the state
        """
        current_app.engine.end_session(current_app.state)
        if "plan" in request.args:
            result_path = None
            try:
                plan_id = int(request.args["plan"])
                result_path = get_plan(plan_id)
                current_app.result = current_app.engine.import_result(result_path)
            finally:
                if result_path is None: return Response("Invalid result id", status=400)
            current_app.action_index = 0
            current_app.state = current_app.engine.start_session(current_app.result.scenario)
        else:
            current_app.state = current_app.engine.start_session(current_app.scenario)
        current_app.done = False
        current_app.message = ""
        current_app.engine.step(current_app.state)
    
    def get_time(self) -> int:
        return current_app.state.time
    
    def get_tracks(self) -> dict:
        coordinates = current_app.vis_config["coordinates"]
        scale = current_app.vis_config["scale"]
        offset_x = current_app.vis_config["offset_x"]
        offset_y = current_app.vis_config["offset_y"]

        tracks = {}
        state = current_app.state
        # get tracks
        for track in current_app.engine.get_location().track_parts:
            id = track.id
            tracks[id] = []

            # get trains on track
            for train in state.get_occupations(track):
                if state.get_position(train) == track:
                    # get direction
                    direction = None
                    previous = state.get_previous(train)
                    if previous and str(previous.id) in coordinates:
                        # direction track is connected to current track (point in common)
                        if coordinates[str(id)][0] in coordinates[str(previous.id)]:
                            direction = coordinates[str(id)][-1]
                        else:
                            direction = coordinates[str(id)][0]

                        direction = [direction[0] * scale + offset_x,
                                     direction[1] * scale + offset_y]

                    train_obj = {
                        "id": train.id,
                        "unique_id": str(train.id),
                        "direction": direction,
                        "in_neutral": state.is_in_neutral(train),
                        "moving": state.is_moving(train),
                        "active": state.has_active_action(train),
                        "trains": [tu.id for tu in train.trains],
			            "train_unit_types": [str(tu.type) for tu in train.trains],
                        "train_unit_tasks": [", ".join([str(task) for task in state.get_tasks_for_train(tu)]) for tu in train.trains],
                        "length": train.length
                    }

                    tracks[id].append(train_obj)
        return tracks
    
    def get_reserved_tracks(self) -> List[int]:
        return [track.id for track in current_app.state.get_reserved_tracks()]

    def get_next_event(self) -> str:
        if current_app.state.peek_event():
            next_event = str(current_app.state.peek_event().type)
        else:
            next_event = "Nothing"
        return next_event
    
    def get_goals(self):
        goals = []
        incoming_list = current_app.state.incoming_trains
        outgoing_list = current_app.state.outgoing_trains
        current_app.engine.get_location()
        for goal in incoming_list + outgoing_list:
            goals.append({
                "type": "incoming" if goal in incoming_list else "outgoing",
                "time": goal.time,
                "from": (goal.side_track if goal in incoming_list else goal.parking_track).name,
                "to": (goal.parking_track if goal in incoming_list else goal.side_track).name,
                "id": goal.shunting_unit.id,
                "trains": [tu.id for tu in goal.shunting_unit.trains],
                "train_unit_types":[str(tu.type) for tu in goal.shunting_unit.trains]
            })
        
        
        return goals
    
