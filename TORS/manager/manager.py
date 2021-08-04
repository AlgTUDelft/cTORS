from pyTORS import ScenarioFailedError
from manager.simulator import Simulator
import importlib
from time import time
from contextlib import contextmanager

class Manager:
    
    def __init__(self, episode_config, agent_config):
        self.episode_config = episode_config
        self.agent_config = agent_config
        self.simulator = Simulator(episode_config)
        self.planner = self.get_planner()
        self.print_episode_info()
        self.print_agent_info()
    
    def print_episode_info(self):
        self.print("M> ### Episode info ###")
        self.print("M> Data folder: {}".format(self.episode_config['data folder']))
        self.print("M> Scenario(s): {}".format(self.episode_config['scenario']))
        self.print("M> Number of runs: {}".format(self.episode_config['n_runs']))
        self.print("M> Maximum number of trains: {}".format(self.episode_config['max_trains']))
    	
    def print_agent_info(self):
        self.print("M> ### Agent info ###")
        planner_class = self.agent_config['class']
        self.print("M> Agent class: {}".format(planner_class))
        if planner_class in self.agent_config:
            config = self.agent_config[planner_class]
            for key, val in config.items():
                self.print("M> \t{}: {}".format(key, val))
      
    def run(self):
        self.sol_runtime = 0
        self.sim_runtime = 0
        self.gen_runtime = 0
        
        sim_start = time()
        self.simulator.start()
        self.sim_runtime += time() - sim_start

        start = time()
        with timeout(self, 10):
            self.planner.initialize(self.simulator.get_engine(), self.simulator.get_location())
        self.sol_runtime += time() - start

        results = {}
        fails = {}
        for t in range(1, self.simulator.get_max_trains()+1):
            results[t] = 0
            fails[t] = 0
            
            gen_start = time()
            self.simulator.set_n_trains(t)
            self.gen_runtime += time() - gen_start
            
            self.print("M> ###############################")
            self.print("M> ### Testing with {:3} trains ###".format(t))
            self.print("M> ###############################")
            for r in range(self.episode_config.n_runs):
                if self.get_remaining_planning_time() < 0 and self.get_remaining_planning_time() != -1: break
                self.print("M> ### Run {:3}                 ###".format(r+1))
                try:
                    result, failure = self.run_one()
                except TimeoutError:
                    break
                results[t] += result
                fails[t] += failure
                if failure: self.print("M> Scenario failed")
                else: self.print("M> Result: {}".format(result))
            self.print("M> Average score: {}%, Failures: {}%".format(results[t]/self.episode_config.n_runs * 100., fails[t]/self.episode_config.n_runs * 100))
            if fails[t] > 0 or results[t] == 0: break
            if self.get_remaining_planning_time() < 0 and self.get_remaining_planning_time() != -1: break
        with timeout(self, 10):
            self.planner.close()
        for t in results:
            self.print("M> {} Trains |\tAverage score: {}%, Failures: {}%".format(t, results[t]/self.episode_config.n_runs * 100., fails[t]/self.episode_config.n_runs * 100))
        print("M> Time spent in solver: {}s".format(self.sol_runtime))
        print("M> Time spent in simulator: {}s".format(self.sim_runtime))
        print("M> Time spent in scenario generator: {}s".format(self.gen_runtime))
        print("M> Total accounted time: {}s".format(self.sol_runtime + self.sim_runtime + self.gen_runtime))    
    
    def run_one(self):
        failure = 0
        self.simulator.reset()
        start = time()
        with timeout(self, 10):
            self.planner.reset()
        self.sol_runtime += time() - start
        while True:
            sim_start = time()
            state = self.simulator.get_state()
            self.sim_runtime += time() - sim_start
            if not self.simulator.is_active(): break
            try:
                start = time()
                with timeout(self):
                    action = self.planner.get_action(state)
                self.sol_runtime += time() - start
                sim_start = time()
                if action is None:
                    valid_actions = self.simulator.get_engine().get_valid_actions(state)
                    if len(valid_actions) == 0: break
                    raise ScenarioFailedError("The planner returned no action, but valid actions are still available.")
                elif not self.simulator.apply_action(action): break
                self.sim_runtime += time() - sim_start
            except TimeoutError:
                self.sol_runtime += time() - start
                self.print("M> Timeout reached.")
                break
            except Exception as e:
                print(e)
                raise e
                failure = 1
                break
        result = self.simulator.get_result()
        start = time()
        with timeout(self, 10):
            self.planner.report_result(result)
        self.sol_runtime += time() - start
        return result, failure
     
    def print(self, m):
        if self.episode_config.verbose >= 1: print(m)   
                
    def get_planner(self):
        planner_str = self.agent_config['class']
        planner_lst = planner_str.split('.')
        _module = importlib.import_module(".".join(planner_lst[:-1]))
        _class = getattr(_module, planner_lst[-1])
        if planner_str in self.agent_config:
            config = self.agent_config[planner_str]
        else: config = {} 
        self.agent_config["time_limit"] = self.episode_config.time_limit
        planner = _class(self.agent_config, config)
        return planner

    # Get the remaining time left for the planner (in seconds)
    def get_remaining_planning_time(self):
        time_limit = self.episode_config.time_limit
        if time_limit == -1: return -1
        return float(time_limit) - self.sol_runtime

# Code for time out #######################################################################
#From https://www.jujens.eu/posts/en/2018/Jun/02/python-timeout-function/
#From https://stackoverflow.com/questions/492519/timeout-on-a-function-call/494273#494273
#Note, only works on UNIX
try:
    import signal

    def raise_timeout(signum, frame):
        raise TimeoutError

    @contextmanager
    def timeout(manager, minimum=None):
        time = manager.get_remaining_planning_time()
        if time != -1 and not minimum is None and time < minimum:
            time = minimum
        if time != -1 and time <= 0:
            raise TimeoutError()
        if time != -1:
            # Register a function to raise a TimeoutError on the signal.
            signal.signal(signal.SIGALRM, raise_timeout)
            # Schedule the signal to be sent after ``time``.
            signal.alarm(int(time+2)) # add some extra time 
        timeout_error = False
        try:
            yield
        except TimeoutError:
            timeout_error = True
        finally:
            # Unregister the signal so it won't be triggered
            # if the timeout is not reached.
            if time != -1:
                signal.signal(signal.SIGALRM, signal.SIG_IGN)
            if timeout_error:
                raise TimeoutError()

except: # If the package signal cannot be imported
    
    @contextmanager
    def timeout(manager, minimum=None):
        if manager.get_remaining_planning_time() != -1:
            raise NotImplementedError("Time out is only supported on UNIX systems")
        yield

# End code for time out ###################################################################