from abc import ABC, abstractmethod
import random
from pyTORS import Engine, InvalidActionError, State, Action, SimpleAction, Location
from typing import Union, List, Tuple, Optional

class Planner(ABC):
    
    def __init__(self, config):
        """
        Initialize the planner with a reference to the engine and a time limit.
        The time limit is just for info. The planner is stopped after this limit. 
        The config parameter contains the contents of the agent config file (default agent.json)
        """
        super(Planner, self).__init__()
        self.random = random.Random(config.seed)
        self.verbose = config.verbose
        self.location = None
        self.base_config = config
        self.time_limit = config.time_limit
    
    def get_random(self):
        """
        Get the random generator initialized with the given seed
        """
        return self.random
    
    def initialize(self, engine: Engine, location: Location):
        """
        Set the engine and location of the planner
        """
        self._engine = engine
        self.location = location
        
    def get_location(self) -> Location:
        """
        Get the location
        """
        return self.location
    
    def report_result(self, result):
        """
        Report the scenario result back to the planner
        """

    def get_valid_actions(self, state: State) -> List[Action]:
        """
        Return a list of all the valid actions in the given state
        """
        return self._engine.get_valid_actions(state)

    def is_valid_action(self, state: State, action: Union[Action, SimpleAction]) -> Tuple[bool, str]:
        """
        Return a tuple (result, reason), with 
        result: true iff the action is valid in the given state
        reason: an explanation why the action is not valid if it is not valid
        """
        return self._engine.is_valid_action(state, action)

    def generate_action(self, state: State, action: SimpleAction) -> Action:
        """
        Generate an Action from the simple action in the given state
        """
        return self._engine.generate_action(state, action)
    
    @abstractmethod
    def get_action(self, state: State) -> Optional[Union[Action, SimpleAction]]: 
        """
        Get an action for the given state, and choose from one of the actions in the list actions.
        Should return None if no valid action is available
        """
    
    @abstractmethod
    def reset(self):
        """
        Reset the planner. This method is called after every run
        """

    @abstractmethod
    def close(self):
        """
        Close the planner and close all resources.
        This method is called at the end of all runs
        """
    
    def print(self, m):
        if self.verbose >= 1: print(m)
