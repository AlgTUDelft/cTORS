from planner.mapf_planner import Action, Constraint
import sys
from itertools import chain, permutations

def mapf_solver(graph, agents, constraints):
    for perm in permutations(agents):
        plans = {}
        constraints_for_agent = {agent: [] for agent in agents}
        done = True
        for i, agent in enumerate(perm):
            _constraints = list(chain(*([constraints] + [constraints_for_agent[perm[j]] for j in range(i)])))
            plans[agent], constraints_for_agent[agent] = solve_for_one(graph, agent, _constraints)
            if plans[agent] is None: 
                done = False
                break
        if done: break            
              
    plan = sum([p for p in plans.values() if not p is None], [])
    plan = sorted(plan, key=lambda a: a.begin)
    #TODO solve for conflicts
    return plan

def solve_for_one(graph, agent, constraints):
    routes = graph.get_routes(agent.start, agent.goal)
    if len(routes) == 0:
        print("No routes from {}-->{}".format(agent.start, agent.goal))
        return None, []
    plans = [get_plan(graph, agent, route, constraints) for route in routes]
    plans = [plan for plan in plans if not plan is None]
    if len(plans) == 0: return None, []
    return min(plans, key=lambda p: p[0][-1].end)

def get_plan(graph, agent, route, constraints):
    arrival_node = route[0]
    arrival_goal = route[1]
    arrival_track = route[2]
    departure_goal = route[-2]
    departure_node = route[-1]
    
    arrival_constraints = [c for c in constraints if arrival_node == c.nodes[0] and arrival_goal == c.nodes[1]]
    time = 0
    again = True
    while again:
        again = False
        for c in arrival_constraints:
            if time >= c.begin and time < c.end:
                time = c.end
                again = True
    if time == sys.maxsize: 
        print("No arrival window at {}-->{} with constraints {}".format(arrival_node, arrival_goal, ", ".join([str(c) for c in arrival_constraints])))
        return None
    arrival_action = Action(agent, arrival_node, arrival_goal, time, time, 'arrival')
    
    move_durations = [graph.lengths[(route[i], route[i+1])] for i in range(len(route)-1)]
    move_durations = [get_setback_cost(agent.train) if d == 'setback' else d for d in move_durations]
    move_duration = sum(move_durations)
    move_action = Action(agent, arrival_track, departure_goal, time, time+move_duration, route=route[2:-2])
    move_constraints = get_constraints_from_move(move_action)
    if in_conflict(move_constraints, constraints):
        conflicting_constraints = [c for c in constraints if any([c.is_similar(c2) for c2 in move_constraints])]
        start = find_time_window(conflicting_constraints, time, move_duration)
        if start is None:
            print("No move window found for route {} starting at {}, with duration {} and with constraints {} ".format(move_action.extra['route'], time, move_duration, ", ".join([str(c) for c in conflicting_constraints])))
            return None
        move_action.begin = start
        move_action.end = start+move_duration
    move_constraints = get_constraints_from_move(move_action)
    time = move_action.end

    departure_constraints = [c for c in constraints if c.nodes[0] == departure_node]
    again = True
    while again:
        again = False
        for c in departure_constraints:
            if time >= c.begin and time < c.end:
                time = c.end
                again = True
    if time == sys.maxsize: 
        print("No departure window at {}-->{} with constraints {}".format(departure_goal, departure_node, ", ".join([str(c) for c in departure_constraints])))
        return None
    departure_action = Action(agent, departure_goal, departure_node, time, time, 'departure')
    plan_constraints = [Constraint(arrival_goal, arrival_action.end, move_action.begin)] + move_constraints + [Constraint(departure_goal, move_action.end, departure_action.begin)]
    return [arrival_action, move_action, departure_action], plan_constraints
 
def get_constraints_from_move(move_action):
    route = move_action.extra['route']
    if len(route) == 1:
        [Constraint(route[0], move_action.begin, move_action.end)]
    return [Constraint(route[i+1], move_action.begin, move_action.end) for i in range(len(route)-1)]
    
def in_conflict(cs1, cs2):
    for c1 in cs1:
        for c2 in cs2:
            if c1.conflicts_with(c2): return True
    return False

def find_time_window(constraints, start_time, duration):
    time = start_time
    again = True
    while again:
        again = False
        for c in constraints:
            if time + duration >= c.begin and time < c.end:
                time = c.end
                again = True
    if time == sys.maxsize: 
        return None
    return time

def get_setback_cost(train):
    return 200