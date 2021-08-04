from planner.planner import Planner

class RandomPlanner(Planner):
    
    def __init__(self, config, random_config):
        super(RandomPlanner, self).__init__(config)
    
    def get_action(self, state):
        actions = self.get_valid_actions(state)
        if len(actions) == 0: return None
        return self.get_random().choice(actions)

    def reset(self): pass

    def close(self): pass
