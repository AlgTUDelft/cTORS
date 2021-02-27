from planner.planner import Planner

class RandomPlanner(Planner):
    
    def __init__(self, seed, verbose, config):
        super(RandomPlanner, self).__init__(seed, verbose, config)
    
    def get_action(self, state, actions):
        return self.get_random().choice(actions)

    def reset(self): pass
