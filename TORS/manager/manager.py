from manager.simulator import Simulator
import importlib
import threading
from planner.planner import RemotePlanner

class Manager:
    
    def __init__(self, config):
        self.config = config
        self.simulator = Simulator(config)
        self.planner = self.get_planner()
        
    def run(self):
        self.simulator.start()
        self.planner.set_location(self.simulator.get_location())
        results = {}
        fails = {}
        for t in range(1, 5):#self.simulator.get_max_trains()+1):
            results[t] = 0
            fails[t] = 0
            self.simulator.set_n_trains(t)
            self.print("M> ###############################")
            self.print("M> ### Testing with {:3} trains ###".format(t))
            self.print("M> ###############################")
            for r in range(self.config.n_runs):
                self.print("M> ### Run {:3}                 ###".format(r+1))
                result, failure = self.run_one()
                results[t] += result
                fails[t] += failure
            self.print("M> Average score: {}%, Failures: {}%".format(results[t]/self.config.n_runs * 100., fails[t]/self.config.n_runs * 100))
        for t in results:
            self.print("M> {} Trains |\tAverage score: {}%, Failures: {}%".format(t, results[t]/self.config.n_runs * 100., fails[t]/self.config.n_runs * 100))
            
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
        for e in range(self.config.rl.n_epochs):
            if e>0: self.planner.new_epoch()
            self.print("### Epoch {:3}          ###".format(e+1))
            for i in range(self.config.rl.n_episodes_per_epoch): 
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
        import matplotlib.pyplot as plt
        n_dense_layers_options = [0,1,2,3,4]
        dropout_options = [0, 0.1, 0.2, 0.4]
        gamma_options = [1, 0.99, 0.95, 0.90, 0.75, 0.25]
        self.config['planner.simple_rl_planner.SimpleRLPlanner'].n_epochs = 50
        #for n_dense_layers in n_dense_layers_options:
        #    self.config['planner.simple_rl_planner.SimpleRLPlanner'].n_dense_layers = n_dense_layers
        for gamma in gamma_options:
            self.config['planner.simple_rl_planner.SimpleRLPlanner'].gamma = gamma
            self.planner = self.get_planner()
            self.planner.reset()
            result = self.planner.train(validation_split = 0.5)
            plt.plot(result['val_loss'], label='validation loss')
            plt.plot(result['loss'], label='training loss')
            #plt.title("Layers: {}".format(n_dense_layers))
            plt.title("Gamma: {}".format(gamma))
            plt.legend()
            plt.show()
     
    def print(self, m):
        if self.config.verbose >= 1: print(m)   
                
    def get_planner(self):
        planner_str = self.config.planner['class']
        planner_lst = planner_str.split('.')
        _module = importlib.import_module(".".join(planner_lst[:-1]))
        _class = getattr(_module, planner_lst[-1])
        if planner_str in self.config:
            config = self.config[planner_str]
        else: config = {} 
        planner = _class(self.config.planner.seed, self.config.planner.verbose, config)
        if self.config.planner.remote:
            planner = RemotePlanner(planner, self.config)
        return planner
    