from tors.core.engine import Engine
from tors.core.logger.exceptions import ScenarioFailedError
import importlib

class Simulator:
    
    def __init__(self, config):
        self.config = config
        self.engine = None
        self.state = None
        self.scenario = None
        self.result = 1
        self.n_trains = 1
    
    def start(self):
        self.assert_start_conditions()
        self.engine = Simulator.load_engine(self.config['data folder'])
        
    def reset(self):
        self.scenario = self.engine.generate_scenario()
        self.state = self.engine.start_state(self.scenario)
        self.state.time = 0
        self.result = 0
    
    def get_state(self):
        try:
            next_actions = self.engine.next(self.state, self.scenario)
            next_actions = [(a[0], a[1]) for a in next_actions]
        except ScenarioFailedError:
            next_actions = []
            self.print("S> Scenario failed")
        if len(next_actions) == 0:
            self.result = self.calculate_reward()
        return self.state, next_actions
    
    def calculate_reward(self):
        if len(self.state.get_incoming_trains()) == 0 and len(self.state.get_outgoing_trains()) == 0: result = 1
        else: result = 0
        #result = min(0, ((self.state.time - self.state.start_time) / (self.state.end_time - self.state.start_time))-1)
        #remaining_incoming = sum([len(incoming.shunting_unit.train_units) for incoming in self.state.get_incoming_trains()])
        #result -= 0.5 * remaining_incoming / self.n_trains
        #for outgoing in self.state.get_outgoing_trains():
        #    if _has_matching_shunting_unit(outgoing.shunting_unit, self.state.get_shunting_units_on_track(outgoing.parking_track_id)):
        #        result += 0.2 / self.n_trains
        return result         
    
    def apply_action(self, action):
        self.print("S [{}]> Applying action {}".format(self.state.time, action[0]))
        #input("P> ...")
        self.state = self.engine.apply(self.state, action[0])
        
    def get_time(self):
        return self.state.time
    
    def get_total_reward(self):
        return self.reward
    
    def get_location(self):
        return self.engine.get_location()
    
    def get_max_trains(self):
        return self.engine.get_scenario().get_number_of_trains()
    
    def set_n_trains(self, n):
        self.n_trains = n
        self.engine.set_scenario_generator(self.get_generator(n))
        
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