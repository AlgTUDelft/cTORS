from abc import ABC, abstractmethod
import random
import multiprocessing
import time
#from tors.core.location.global_location_container import GlobalLocation

class Planner(ABC):
    
    def __init__(self, seed, verbose, config):
        super(Planner, self).__init__()
        self.random = random.Random(seed)
        self.location = None
        self.verbose = verbose
        self.config = config
    
    def get_random(self):
        return self.random
    
    def set_location(self, location):
        self.location = location
        #GlobalLocation(location)
        
    def get_location(self):
        return self.location
    
    def report_result(self, result):
        pass
    
    @abstractmethod
    def get_action(self, state, actions): pass
    
    @abstractmethod
    def reset(self): pass
    
    def print(self, m):
        if self.verbose >= 1: print(m)

def record_time(return_type=None):
    def _record_time(func):
        def wrapper(self, *args, **kwargs):
            assert isinstance(self, RemotePlanner)
            t1 = time.time_ns()
            func(self, *args, **kwargs)
            if not return_type is None:
                if self.connection.poll(max(0, (self.time_limit-self.time_taken)/1e9) + 0.1):
                    result = self.connection.recv()
                    if not isinstance(result, return_type):
                        raise Exception("Unexpected return type {}. Expected {}.".format(type(result), return_type))
                    return result
                else:
                    self.subprocess.terminate()
                    self.connection.close()
                    raise Exception("All available time is used")
            self.time_taken += (time.time_ns()-t1) 
        return wrapper
    return _record_time

class RemotePlanner(Planner):
    
    def __init__(self, planner, config):
        super(RemotePlanner, self).__init__(config.planner.seed, config.planner.verbose, {})
        self.connection, child_conn = multiprocessing.Pipe()
        self.subprocess = multiprocessing.Process(target=run, args=(planner, child_conn,), daemon=True)
        self.config = config
        self.reset()
        self.subprocess.start()
    
    @record_time()
    def set_location(self, location):
        self.connection.send(("set_location", (location,)))
    
    @record_time ()
    def resport_result(self, result):
        self.connection.send(("report_result", (result, )))
        
    @record_time(tuple)
    def get_action(self, state, actions):
        self.connection.send(("get_action", (state, actions)))

    @record_time()
    def reset(self):
        self.time_limit = self.config.planner.time_limit * 1e9
        self.time_taken = 0
        self.connection.send(("reset", ()))
    
def run(planner, conn):
    while True:
        try:
            func, args = conn.recv()
            print("Recv: {}, {}".format(func, args))
        except EOFError:
            return
        if not hasattr(planner, func) or not callable(getattr(planner, func)):
            raise Exception("Planner of type {} does not have the method {}".format(type(planner), func))
        result = getattr(planner, func)(*args)
        if not result is None: conn.send(result)
    
