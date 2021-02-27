from pyTORS import Engine, ScenarioFailedError
#from tors.core.engine import Engine
#from tors.core.logger.exceptions import ScenarioFailedError
import importlib

class Simulator:
    
    def __init__(self, config):
        self.config = config
        self.engine = None
        self.state = None
        self.result = 1
        self.n_trains = 1
        self.org_scenario = None
        self.scenario = None
        self.scenario_generator = None
    
    def start(self):
        self.assert_start_conditions()
        self.engine = Simulator.load_engine(self.config['data folder'])
        self.org_scenario = self.engine.get_scenario()
        
    def reset(self):
        if not self.state is None:
            self.print("S> End previous session")
            self.engine.end_session(self.state)
        self.print("S> Delete scenario")
        del self.scenario

        # TODO: Shunting units are stored in incoming and outgoin goals, which are deleted when the scenario is deleted,
        # and also in the state, which is deleted when the state is deleted.

        self.scenario = self.scenario_generator.generate_scenario()
        self.state = self.engine.start_session(self.scenario)
        self.result = 0
    
    def get_state(self):
        try:
            self.print("S [{}]> Get actions".format(self.state.time))
            next_actions = self.engine.step(self.state)
            #next_actions = [(a[0], a[1]) for a in next_actions]
        except ScenarioFailedError:
            next_actions = []
            self.print("S [{}]> Scenario failed".format(self.state.time))
        if len(next_actions) == 0:
            self.result = self.calculate_reward()
        return self.state, next_actions
    
    def calculate_reward(self):
        if len(self.state.incoming_trains) == 0 and len(self.state.outgoing_trains) == 0: return 1
        #result = min(0, ((self.state.time - self.state.start_time) / (self.state.end_time - self.state.start_time))-1)
        return 0#result         
    
    def apply_action(self, action):
        self.print("S [{}]> Applying action {}".format(self.state.time, str(action)))
        self.engine.apply_action(self.state, action)
    def get_time(self):
        return self.state.time
    
    def get_total_reward(self):
        return self.reward
    
    def get_location(self):
        return self.engine.get_location()
    
    def get_max_trains(self):
        return self.engine.get_scenario().number_of_trains
    
    def set_n_trains(self, n):
        self.n_trains = n
        self.scenario_generator = self.get_generator(n)
        self.scenario_generator.initialize(self.org_scenario, self.engine.get_location())
        
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
        return _class(n_trains=n_trains, **config)
    
def _has_matching_shunting_unit(o_su, sus):
        for s_su in sus:
            if len(o_su.train_units) != len(s_su.train_units): continue
            match = True
            for i in range(len(o_su.train_units)):
                o_tu = o_su.train_units[i]
                s_tu = s_su.train_units[i]
                if (o_tu.id is None and o_tu.type != s_tu.type) or \
                    (not o_tu.id is None and o_tu.id != s_tu.id):
                    match = False
                    break
            if match: return True 
        return False    