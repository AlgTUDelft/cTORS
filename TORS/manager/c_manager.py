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
    
    def train(self, visual=True):
        from manager.rl_visualizer import RLVisualizer
        self.simulator.start()
        self.planner.set_location(self.simulator.get_location())
        if visual:
            visualizer = RLVisualizer()
            thread = threading.Thread(target=self.run_train_session, args=(visualizer,), name='train-thread', daemon=True)
            visualizer.start(thread)
        else:
            self.run_train_session()
            
    def run_train_session(self, visualizer=None):
        n_trains = 1
        self.simulator.set_n_trains(n_trains)
        for e in range(self.episode_config.rl.n_epochs):
            if e>0: self.planner.new_epoch()
            self.print("### Epoch {:3}          ###".format(e+1))
            for i in range(self.episode_config.rl.n_episodes_per_epoch): 
                self.run_one()
            if self.planner.can_increase_difficulty():
                n_trains += 1
                self.simulator.set_n_trains(n_trains)
                self.planner.increase_difficulty()
                self.print("###############################")
                self.print("### Testing with {:3} trains ###".format(n_trains))
                self.print("###############################")
            if visualizer:
                visualizer.post("add_point", self.planner.get_performance(), self.planner.get_epsilon())
            
    def tune(self):
        epsilon_decay_options = [0.99, 0.98, 0.95, 0.90]
        memory_size_options = [1000, 5000, 20000]
        episodes_in_epoch_options = [10, 50, 200]
        gamma_options = [0.99, 0.95, 0.9]
        for epsilon_decay in epsilon_decay_options:
            # set epsilon decay
            self.train()
        for memory_size in memory_size_options:
            # set memory size
            self.train()
        for episodes_in_epoch in episodes_in_epoch_options:
            # set episodes_in_epoch
            self.train()
        for gamma in gamma_options:
            # set gamma
            self.train()
            
            
    def tune_offline(self):
        self.simulator.start()
        import matplotlib.pyplot as plt
        config = self.agent_config['planner.simple_rl_planner.SimpleRLPlanner']
        test_name = "dropout"
        test_data = {
            "alpha": ("Alpha: ", [0.001, 0.01, 0.05, 0.1, 0.2], "alpha"),
            "n_dense": ("Layers: ", [0,1,2,3,4], "n_dense_layers"),
            "gamma": ("Gamma: ", [1, 0.99, 0.95, 0.90, 0.75, 0.25], "gamma"),
            "dropout": ("Dropout: ", [0, 0.1, 0.2, 0.4], "dropout"),
            "sample_size": ("Sample size: ", [500, 1000, 2500, 5000, 10000], "sample_size")
           }
        str_format = test_data[test_name][0]
        test_list = test_data[test_name][1]
        test_config = test_data[test_name][2]
        for test_setting in test_list:
            config[test_config] = test_setting
            config.n_epochs = int(200000 / config.sample_size)
            self.planner = self.get_planner()
            self.planner.set_location(self.simulator.get_location())
            self.planner.reset(load_from_file=False)
            result = self.planner.train(validation_split = 0.5)
            plt.plot(result['val_loss'], label='validation loss')
            plt.plot(result['loss'], label='training loss')
            plt.title("{}{}".format(str_format, test_setting))
            plt.legend()
            plt.show()
     
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
    