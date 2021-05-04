# Treinonderhoud- en -rangeersimulator (TORS)
This implementation of TORS consists of a backend written in C++ (cTORS), and a front-end written in python (TORS).

## Project setup
The basic project setup uses the structure provided by cmake. The subfolders are subprojects:
* cTORS: The c++ implementation of TORS
* cTORSTest: The tests for cTORS
* pyTORS: The python interface for cTORS
* TORS: The challenge environment, in python

## Installation
The following section explains how to compile this source code. Alternatively one can run the code in a docker container.
The Dockerfile also shows how this project can be compiled and run step-by-step.
### Install Cmake and Python development libraries
To compile cTORS, cmake 3.11 (or higher) is required and the python development libraries:
```
apt-get install cmake
apt-get install python3-dev
```

For windows:
 * download and install cmake: https://cmake.org/download/
 * python header files already installed with python, see the include folder in your python folder.

### Install Google Protocol Buffers
This project uses Google Protocol Buffers to read the data files. Installation is required to compile the C++ source:
 * Download the source from the [github page](https://github.com/protocolbuffers/protobuf) (version 3.15.6 is used)
 * Follow the [instructions](https://github.com/protocolbuffers/protobuf/blob/master/src/README.md) to install

### Build with setuptools
You can build cTORS and the pyTORS library with the following command.
```sh
mkdir build
python setup.py build
python setup.py install
```

### Compile cTORS from C++ source
In the source directory execute the following commands:
```sh
mkdir build
cd build
cmake ..
cmake --build .
```
This has been tested with gcc 9.3. Older versions may not support the c++17 standard. 

### Alternative: build with docker
You can also run TORS in a docker container. To build and run the container, run:
```sh
docker build -t tors-base .
docker run --network="host" --rm -it tors-base /bin/bash
```

# Basic usage

## Run the challenge environment
To run challenge environment, run the following code

```sh
cd TORS
python run.py
```

Optionally you can change the episode or agent data by changing the parameters
```sh
python run.py --agent agent.json --episode episode.json
```
The `--agent` option sets the file that configures the agent.
The `--episode` option sets the file that configures the episode.

You can also run the file with the `--train` flag to train the agent instead of eveluating its performance.

## Usage in Python
To use cTORS in python, you need to import they `pyTORS` library. E.g.

```python
from pyTORS import Engine

engine = Engine("data/Demo")
state = engine.start_session()

actions = engine.step(state)
engine.apply_action(actions[0])

engine.end_session(state)
```

## Running the visualizer

The visualizer runs as a flask server. Install the dependencies in `TORS/requirements-visualizer` first.
```sh
pip install -r TORS/requirements-visualizer
```
Now flask can be run by running the commands:
```sh
cd TORS/visualizer
export FLASK_APP=main.py
export FLASK_ENV=development
export FLASK_RUN_PORT=5000
python -m flask run
```

## Running the example RL-agent with gym
The repository also includes example code that wraps cTORS in a gym-environment and uses an RL implementation from stable-baselines3 to learn a policy. To run this example, first install the requirements:
```sh
pip install -r TORS/requirements-gym
```
Then run:
```sh
cd TORS
python run_gym.py
```
You can check the learning progress using tensorboard:
```sh
tensorboard --logdir ./log_tensorboard/
```

## Configuration
TORS can be configured through configuration files. Seperate configuration exists for
1. The location
2. The scenario
3. The simulator
3. The episode
4. The agent

### Configuring the location
A location is described by the `location.json` file in the data folder.
It describes the shunting yard: how all tracks are connected, what kind of tracks they are, and distances among tracks.

In order to use the visualizer for that location, you need to provided another file `vis_config.json`. See the folder `data/Demo` and `data/KleineBinckhorstVisualizer` for examples.

### Configuring the scenario
A scenario is described by the `scenario.json` file in the data folder.
It describes the scenario: which employees are available, shunting units' arrivals and departures, and possible disturbances.

### Configuring the simulator
The simulator can be configured by the `config.json` file in the data folder.
It describes which business rules need to be checked and the parameters for the actions

### Configuring the episode
You can provide an episode configuration and pass it to `TORS/run.py` with the `--episode` parameter.
This file describes the evaluation/training episode.
It contains the path to the data folder, the number of runs, RL parameters and parameters for scenario generation.

### Configuring the agent
You can provide an agent configuration and pass it to `TORS/run.py` with the `--agent` parameter.
This file prescribes which agent to use, and passes parameters to the agent.

## Tests
### Run the cTORS tests
To run the cTORS tests, execute the commands
```sh
cd build
ctest
```

## Contributors
* Mathijs M. de Weerdt: Conceptualization, Supervision, Project administration, Funding acquisition, Writing - review & editing
* Bob Huisman: Conceptualization
* Koos van der Linden: Software, Writing - Original draft
* Jesse Mulderij: Writing - Original draft
* Marjan van den Akker: Supervision of the bachelor team
* Han Hoogeveen: Supervision of the bachelor team
* Joris den Ouden: Conceptualization, Supervision of the bachelor team
* Demian de Ruijter: Conceptualization, Supervision of the bachelor team
* Bachelor-team, consisting of Dennis Arets, Sjoerd Crooijmans, Richard Dirven, Luuk Glorie, Jonathan den Herder, Jens Heuseveldt, Thijs van der Horst, Hanno Ottens, Loriana Pascual, Marco van de Weerthof, Kasper Zwijsen: Software, Visualization

