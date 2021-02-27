from flask_restful import Resource
from flask import Response, current_app
import json
from typing import List
from pyTORS import ShuntingUnit

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
                            direction = coordinates[str(id)][0]
                        else:
                            direction = coordinates[str(id)][-1]

                        direction = [direction[0] * scale + offset_x,
                                     direction[1] * scale + offset_y]

                    train_obj = {
                        "id": train.id,
                        "unique_id": str(train.id),
                        "direction": direction,
                        "in_neutral": state.is_in_neutral(train),
                        "moving": state.is_moving(train),
                        "train_units": [tu.id for tu in train.get_trains()],
			            "train_unit_types": [str(tu.type) for tu in train.train_units],
                        "train_unit_tasks": [", ".join([str(task) for task in state.get_tasks_for_train(tu)]) for tu in train.train_units],
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
                "train_units": [tu.id for tu in goal.shunting_unit.train_units],
                "train_unit_types":[str(tu.type) for tu in goal.shunting_unit.train_units]
            })
        
        
        return goals
    
