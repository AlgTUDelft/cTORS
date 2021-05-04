from flask_restful import Resource
from flask import Response, current_app
import json
import os

def get_all_plans():
    return sorted(os.listdir(os.path.join(current_app.static_folder, '../../runs')))

def get_plan(ix):
    plans = get_all_plans()
    if ix < 0 or ix >= len(plans): return None
    return os.path.join(current_app.static_folder, '../../runs',plans[ix])

class Plan(Resource):
    
    def get(self) -> str:
        names = get_all_plans()
        return Response(json.dumps(names), mimetype='application/json')