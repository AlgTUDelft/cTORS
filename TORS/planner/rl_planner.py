from planner.planner import Planner
from stable_baselines3 import PPO
import importlib
import os
from pyTORS import State, SimpleAction, Engine, Location

class RLPlanner(Planner):
    
    def __init__(self, config, rl_config):
        super(RLPlanner, self).__init__(config)
        assert('model_save' in rl_config)
        self.model_file = rl_config.model_save
        self.model = PPO.load(self.model_file)
        self.rl_config = rl_config

    def initialize(self, engine: Engine, location: Location):
        super().initialize(engine, location)
        self.converter = self._get_converter()
    
    def get_action(self, state: State) -> SimpleAction:
        actions = self.get_valid_actions(state)
        if len(actions) == 0: return None
        m_state = self.converter.convert_state(state)
        m_action, _states = self.model.predict(m_state)
        return self.converter.convert_action(state, m_action)

    def reset(self): pass

    def close(self): pass

    def _get_converter(self):
        converter_str = self.rl_config.converter['class']
        converter_lst = converter_str.split('.')
        _module = importlib.import_module(".".join(converter_lst[:-1]))
        _class = getattr(_module, converter_lst[-1])
        config = self.rl_config.converter.copy()
        del config['class']
        if converter_str in self.rl_config:
            config.update(self.rl_config[converter_str])
        return _class(self._engine, self.location, **config)
