from manager.c_simulator import Simulator
import importlib
import threading
#from planner.planner import RemotePlanner

class Manager:
    
    def __init__(self, episode_config, agent_config):
        self.episode_config = episode_config
        self.agent_config = agent_config
        self.simulator = Simulator(episode_config)
        self.planner = self.get_planner()
        
    def run(self):
        self.simulator.start()
        self.planner.set_location(self.simulator.get_location())
        results = {}
        fails = {}
        for t in range(1, 3):#self.simulator.get_max_trains()+1):
            results[t] = 0
            fails[t] = 0
            self.simulator.set_n_trains(t)
            self.print("M> ###############################")
            self.print("M> ### Testing with {:3} trains ###".format(t))
            self.print("M> ###############################")
            for r in range(self.episode_config.n_runs):
                self.print("M> ### Run {:3}                 ###".format(r+1))
                result, failure = self.run_one()
                results[t] += result
                fails[t] += failure
            self.print("M> Average score: {}%, Failures: {}%".format(results[t]/self.episode_config.n_runs * 100., fails[t]/self.episode_config.n_runs * 100))
        for t in results:
            self.print("M> {} Trains |\tAverage score: {}%, Failures: {}%".format(t, results[t]/self.episode_config.n_runs * 100., fails[t]/self.episode_config.n_runs * 100))
            
    def run_one(self):
        failure = 0
        self.simulator.reset()
        self.planner.reset()
        while True:
            state, actions = self.simulator.get_state()
            if not actions: break
            try:
                action = self.planner.get_action(state, actions)
                self.simulator.apply_action(action)
            except Exception as e:
                print(e)
                raise e
                failure = 1
                break
        result = self.simulator.get_result()
        self.planner.report_result(result)
        return result, failure
     
    def print(self, m):
        if self.episode_config.verbose >= 1: print(m)   
                
    def get_planner(self):
        planner_str = self.agent_config.planner['class']
        planner_lst = planner_str.split('.')
        _module = importlib.import_module(".".join(planner_lst[:-1]))
        _class = getattr(_module, planner_lst[-1])
        if planner_str in self.agent_config:
            config = self.agent_config[planner_str]
        else: config = {} 
        planner = _class(self.agent_config.planner.seed, self.agent_config.planner.verbose, config)
        #if self.agent_config.planner.remote:
        #    planner = RemotePlanner(planner, self.agent_config)
        return planner
    