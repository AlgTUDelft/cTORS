from pyTORS import Engine, ScenarioFailedError
import importlib
from manager.scenario_generator import ScenarioGeneratorFromFolder

class Simulator:
    
    def __init__(self, config):
        self.config = config
        self.engine = None
        self.state = None
        self.result = 1
        self.n_trains = 1
        self.scenario = None
        self.scenario_generator = None
    
    def start(self):
        self.assert_start_conditions()
        self.engine = Simulator.load_engine(self.config['data folder'])
        
    def reset(self):
        if not self.state is None:
            self.engine.end_session(self.state)
        del self.scenario

        self.scenario = self.scenario_generator.get_scenario()
        if self.config.verbose >= 1:
            self.scenario.print_scenario_info()
        self.state = self.engine.start_session(self.scenario)
        self.engine.step(self.state)
        self.result = 0
        if self.config.verbose >= 2:
            self.state.print_state_info()
    
    def get_state(self):
        if not self.is_active():
            self.result = self.calculate_reward()
        return self.state

    def is_active(self):
        return self.engine.is_state_active(self.state)
    
    def calculate_reward(self):
        if len(self.state.incoming_trains) == 0 and len(self.state.outgoing_trains) == 0: return 1
        #result = min(0, ((self.state.time - self.state.start_time) / (self.state.end_time - self.state.start_time))-1)
        return 0#result         
    
    def apply_action(self, action):
        self.print("S [{}]> Applying action {}".format(self.state.time, str(action)))
        try:
            self.engine.apply_action_and_step(self.state, action)
            if self.config.verbose >= 2:
                self.state.print_state_info()
            return True
        except ScenarioFailedError:
            self.print("S [{}]> Scenario failed".format(self.state.time))
            return False

    def get_time(self):
        return self.state.time
    
    def get_total_reward(self):
        return self.reward
    
    def get_location(self):
        return self.engine.get_location()

    def get_engine(self):
        return self.engine
    
    def get_max_trains(self):
        return self.config["max_trains"]
    
    def set_n_trains(self, n):
        self.n_trains = n
        self.scenario_generator = self.get_generator(n)
        self.scenario_generator.initialize(self.engine, self.config['scenario'])
        
    def get_result(self):
        return self.result
        
    def assert_start_conditions(self):
        assert self.engine is None
        assert self.state is None
        
    def print(self, m):
        if self.config.verbose >= 1: print(m)
    
    @staticmethod
    def load_engine(path) -> Engine:
        return Engine(path)
    
    def get_generator(self, n_trains):
        generator_str = self.config.generator['class']
        generator_lst = generator_str.split('.')
        _module = importlib.import_module(".".join(generator_lst[:-1]))
        _class = getattr(_module, generator_lst[-1])
        config = self.config.generator.copy()
        del config['class']
        if generator_str in self.config:
            config.update(self.config[generator_str])
        return ScenarioGeneratorFromFolder(_class, n_trains=n_trains, **config)
    
def _has_matching_shunting_unit(o_su, sus):
        for s_su in sus:
            if len(o_su.trains) != len(s_su.trains): continue
            match = True
            for i in range(len(o_su.trains)):
                o_tu = o_su.trains[i]
                s_tu = s_su.trains[i]
                if (o_tu.id is None and o_tu.type != s_tu.type) or \
                    (not o_tu.id is None and o_tu.id != s_tu.id):
                    match = False
                    break
            if match: return True 
        return False    
