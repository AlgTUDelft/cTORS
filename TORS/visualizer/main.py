from flask import Flask, render_template
from flask_restful import Api
from pyTORS import Engine
from pathlib import Path
import os
import json

from location import Location
from actions import Actions
from state import State

path_dir = os.path.dirname(__file__)
#layout = "KleineBinckhorstVisualizer"
layout = "Demo"
data_path = str(Path(path_dir).parent.parent) + "/data/" + layout + "/"
vis_config_path = str(Path(path_dir).parent.parent) + "/data/" + layout + "/vis_config.json"


def create_app() -> Flask:
    """
    Create the flask app and load the engine

    :returns: new app
    """

    app = Flask(__name__)

    # Create the tors instance
    with app.app_context():
        app.engine = Engine(data_path)
        app.state = app.engine.start_session()
        app.engine.step(app.state)
        load_config(app)

    return app


def register_routes(app: Flask):
    """
    Register all routes

    :param app: the flask app
    """

    @app.route('/')
    def hello():
        return render_template("base.html")


def register_resources(api: Api):
    """
    Register all resources for the rest api

    :param api: the flask api
    """
    api.add_resource(Location, '/engine/location')
    api.add_resource(Actions, '/engine/actions')
    api.add_resource(State, '/engine/state')


def load_config(app: Flask):
    """
    Load visualization config

    :param app: the flask app
    """
    with open(vis_config_path) as config_file:
        app.vis_config = json.load(config_file)


# Create the webserver
app = create_app()
api = Api(app)

# Register the urls
register_routes(app)
register_resources(api)
