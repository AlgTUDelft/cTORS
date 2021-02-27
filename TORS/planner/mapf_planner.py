from planner.planner import Planner
from tors.core.pathfinder.graph import Node
from tors.core.action.action_type import ActionType
import importlib
import copy
import sys


class MAPFPlanner(Planner):
    
    def __init__(self, seed, verbose, config):
        Planner.__init__(self, seed, verbose, config)
        self.mapf_solver = MAPFPlanner.load_mapf_solver(config.mapf_solver)
        self.reset()
    
    @staticmethod
    def load_mapf_solver(solver_str):
        solver_lst = solver_str.split('.')
        _module = importlib.import_module(".".join(solver_lst[:-1]))
        _func = getattr(_module, solver_lst[-1])
        return _func
    
    def reset(self):
        Planner.reset(self)
        self.plan = None
        
    def get_action(self, state, actions):
        if self.plan is None:
            self.plan = self.make_plan(state)
            for a in self.plan:
                self.print("P> "+str(a))
        return self.get_next_action(state, actions)
    
    def get_next_action(self, state, actions):
        action = None
        if len(self.plan) > 0:
            next_action = self.plan[0]
            if next_action.type == 'arrival':
                action = next((a for a in actions if a[0].type.is_arrive() 
                    and next_action.to == Node(a[0].incoming_train.side_track_id, a[0].incoming_train.parking_track_id)
                    and a[0].incoming_train.time == next_action.begin
                    and a[0].incoming_train.shunting_unit.train_units[0] == next_action.agent.train), None)
            elif next_action.type == 'departure':
                action = next((a for a in actions if a[0].type.is_exit() 
                    and next_action.frm == Node(a[0].outgoing_train.parking_track_id, a[0].outgoing_train.side_track_id)
                    and a[0].outgoing_train.time == next_action.begin
                    and a[0].shunting_unit.train_units[0] == next_action.agent.train), None)
            elif next_action.type == 'move' and next_action.begin <= state.time:
                if len(next_action.extra['route']) == 1: #set back
                    action = next((a for a in actions if a[0].type == ActionType.SetBack 
                    and next_action.frm.current_track == a[0].track_ids[0]
                    and a[0].shunting_unit.train_units[0] == next_action.agent.train), None)
                else:
                    route =  self.get_location().pathfinder.transform_route(next_action.extra['route']).path
                    action = next((a for a in actions if a[0].type.is_move() 
                        and route == a[0].track_ids
                        and a[0].shunting_unit.train_units[0] == next_action.agent.train), None)
        if action is None:
            # perform wait action, or when not present, random action
            return next((a for a in actions if a[0].type == ActionType.Wait), self.random.choice(actions))
        self.plan.pop(0)
        return action
    
    def make_plan(self, state):
        graph = copy.deepcopy(self.get_location().pathfinder.get_graph())
        constraints = []
        agents = []
        trains = [i.shunting_unit.train_units[0] for i in state.get_incoming_trains()]
        start = {}
        goal = {}
        new_vertex_id = len(graph.vertices)+1
        for incoming in state.get_incoming_trains():
            new_incoming_node = Node(new_vertex_id, incoming.side_track_id)
            old_incoming_node = Node(incoming.side_track_id, incoming.parking_track_id)
            new_vertex_id += 1
            start[incoming.shunting_unit.train_units[0]] = new_incoming_node 
            self.add_node_to_graph(graph, new_incoming_node, old_incoming_node)
            if incoming.time > 0:
                constraints.append(Constraint([new_incoming_node, old_incoming_node], end=incoming.time))
            constraints.append(Constraint([new_incoming_node, old_incoming_node], begin=incoming.time+1))
        for outgoing in state.get_outgoing_trains():
            new_outgoing_node = Node(outgoing.side_track_id, new_vertex_id)
            old_outgoing_node = Node(outgoing.parking_track_id, outgoing.side_track_id)
            new_vertex_id += 1
            goal[outgoing.shunting_unit.train_units[0]] = new_outgoing_node
            self.add_node_to_graph(graph, old_outgoing_node, new_outgoing_node)
            constraints.append(Constraint(new_outgoing_node, end=outgoing.time))
            constraints.append(Constraint(new_outgoing_node, begin=outgoing.time+1))
        for train in trains:
            agents.append(Agent(train, start[train], goal[train]))
        return self.mapf_solver(graph, agents, constraints)
    
    def add_node_to_graph(self, graph, frm, to):
        graph.add_edge(frm, to)
        if not frm in graph.vertices:
            graph.vertices.append(frm)
            graph._routes[(frm, frm)] = [frm]
            for n in graph.vertices:
                routes = graph._routes[(to, n)]
                graph._routes[(frm, n)] = [[frm] + r for r in routes]
        if not to in graph.vertices:
            graph.vertices.append(to)
            graph._routes[(to, to)] = [to]
            for n in graph.vertices:
                routes = graph._routes[(n, frm)]
                graph._routes[(n, to)] = [r + [to] for r in routes]

class Agent:
    
    def __init__(self, train, start, goal):
        self.train = train
        self.start = start
        self.goal = goal  
        
    def __hash__(self):
        return hash((self.train, self.start, self.goal))
    
    def __eq__(self, other):
        return isinstance(other, Agent) and self.train == other.train and \
            self.start == other.start and self.goal == other.goal
    
    def __str__(self):
        return "Agent {}, A: {}, G: {}".format(self.train, self.start, self.goal)
  
class Constraint:
    
    def __init__(self, nodes, begin=0, end=sys.maxsize):
        self.nodes =  [nodes] if isinstance(nodes, Node) else nodes
        self.begin = begin
        self.end = end
    
    def is_similar(self, constraint):
        return len(self.nodes) == len(constraint.nodes) and all((self.nodes[i].current_track == constraint.nodes[i].current_track for i in range(len(self.nodes)))) 
    
    def conflicts_with(self, constraint):
        if not isinstance(constraint, Constraint): return False
        if not self.is_similar(constraint): return False
        if self.end <= constraint.begin or self.begin >= constraint.end: return False
        return True
        
    def __str__(self):
        if len(self.nodes) == 1:
            return "C({} [{}-{}])".format(self.nodes[0], self.begin, self.end)
        return "C({} [{}-{}])".format(",".join([str(n) for n in self.nodes]), self.begin, self.end)

class Action:
    
    def __init__(self, agent, frm, to, begin, end, typ='move', **kwargs):
        self.agent = agent
        self.frm = frm
        self.to = to
        self.begin = begin
        self.end = end
        self.type = typ
        self.extra = kwargs
        
    def __str__(self):
        return "{}-action t: {}, {}-->{}, [{}-{}], {}".format(self.type, self.agent.train, self.frm, self.to, self.begin, self.end, self.extra)
        