from itertools import chain, repeat, islice
import numpy as np
from pyTORS import TrackPartType, InvalidActionError, ScenarioFailedError, \
    Arrive, Exit, BeginMove, EndMove, Service, Split, Setback, Combine, Move, Wait, \
    ArriveAction, ExitAction, BeginMoveAction, EndMoveAction, WaitAction, ServiceAction, SplitAction, SetbackAction, CombineAction, MoveAction, \
    State, Track, Location, Engine, Action, ShuntingUnit, Incoming, Outgoing, Train
from gym import spaces


"""
A class to convert the TORS state to a data tuple
"""
class ITORSConvertor:
    def __init__(self, engine: Engine, location: Location, *args, **kwargs):
        self.engine = engine
        self.location = location
    
    """
    Convert a TORS state object to a data object, e.g. a tuple or a custom data object
    """
    def convert_state(self, state: State):
        raise NotImplementedError("This method has not been implemented.")

    """
    Convert an action to a TORS Action object
    """
    def convert_action(self, action: Action):
        raise NotImplementedError("This method has not been implemented.")
    
    """
    Get the observation space, must be a gym spaces object
    """
    def get_observation_space(self):
        raise NotImplementedError("This method has not been implemented.")
    
    """
    Get the action space, must be a gym spaces object
    """
    def get_action_space(self):
        raise NotImplementedError("This method has not been implemented.")


"""
An example class implementing the ITORSConvertor
"""
class TORSConverter(ITORSConvertor):
    def __init__(self, location: Location, *args, **kwargs):
        super(TORSConverter, self).__init__(location, *args, **kwargs)
        self.init_tracks()
        self.init_sizes()
    
    def init_tracks(self):
        self.track_map = {}
        self.tracks = []
        for i, track in enumerate([track for track in self.location.track_parts if track.type == TrackPartType.RAILROAD]):
            self.track_map[track] = i
            self.tracks.append(track)

    def init_sizes(self):
        self.n_inc = 3
        self.n_out = 3
        self.n_trains = 3
        self.track_pos_size = len(self.tracks)
        self.train_size = 3
        self.su_size = 4 + self.track_pos_size + 1 + self.n_trains * self.train_size
        self.incoming_size = 1 + self.track_pos_size + self.su_size
        self.outgoing_size = 1 + self.track_pos_size + self.su_size
        self.n_actions = 5 + 2*(self.n_trains-1) + len(self.tracks) # see convert_action for this count of actions
        self.n_total_actions = self.n_inc + self.n_out + self.n_trains * self.n_actions

    def get_observation_space(self, state: State):
        conv = self.convert_state(state)
        return spaces.Box(low=0, high=1, shape=(len(conv),), dtype=np.float16)
    
    def get_action_space(self, state: State):
        return spaces.Discrete(self.n_total_actions)

    """
    Returns the action space dimension:
    Per Shunting Unit:
    Arrive      (n_inc)
    Exit        (n_out)
    BeginMove   (1)
    EndMove     (1) 
    Wait        (1)
    Setback     (1)
    Service     (1)     [Assume there is only one task type]
    Split       (n_trains-1)
    Combine     (n-trains-1)
    Move        (n_tracks)
    """
    def convert_action(self, state: State, action: int):
        if action < self.n_inc:
            incoming = sorted(state.incoming_trains, key=lambda inc: inc.time)
            return Arrive(incoming[action])
        action -= self.n_inc
        if action < self.n_out:
            outgoing = sorted(state.outgoing_trains, key=lambda out: out.time)[action]
            out_su = outgoing.shunting_unit
            for su in state.shunting_units:
                if out_su.matches_shunting_unit(su) and state.get_position(su) == outgoing.parking_track:
                    return Exit(su, outgoing)
        action -= self.n_out
        su_ix = action // self.n_actions
        if su_ix >= len(state.shunting_units):
            raise InvalidActionError("Shunting unit does not exist")
        su = state.shunting_units[su_ix]
        action_ix = action % self.n_actions
        if action_ix == 0: return BeginMove(su)
        elif action_ix == 1: return EndMove(su)
        elif action_ix == 2: return Wait(su)
        elif action_ix == 3: return Setback(su)
        elif action_ix == 4:
            facility = state.get_position(su).facilities[0]
            for tu in su.trains:
                tasks = state.get_tasks_for_train(tu)
                if len(tasks) == 0: continue
                return Service(su, tasks[0], tu, facility)
            raise Exception("Executing service task, but no task to be executed.")
        action_ix -= 5
        if action_ix < self.n_trains-1:
            return Split(su, action_ix + 1)
        action_ix -= self.n_trains-1
        if action_ix < self.n_trains-1:
            if action_ix >= su_ix: action_ix += 1 # Skip its own index
            return Combine(su, state.shunting_units[action_ix])
        action_ix -= self.n_trains-1
        return Move(su, self.tracks[action_ix])

    def convert_state(self, state: State):
        return tuple(chain(
            self.convert_incomings(state),
            self.convert_outgoings(state),
            self.convert_tracks(state),
            self.convert_shunting_units(state),
            self.convert_time(state, state.time),
            self.convert_valid_actions(state))
        )
    
    def convert_incomings(self, state: State):
        return pad(chain.from_iterable([self.convert_incoming(state, inc) for inc in state.incoming_trains]), self.n_inc * self.incoming_size)
        
    def convert_outgoings(self, state: State):
        return pad(chain.from_iterable([self.convert_outgoing(state, out) for out in state.outgoing_trains]), self.n_out * self.outgoing_size)
    
    def convert_incoming(self, state: State, inc: Incoming):
        return chain(
            self.convert_time(state, inc.time),
            self.convert_track_position(inc.parking_track),
            self.convert_trains(inc.shunting_unit.trains)
        )

    def convert_outgoing(self, state: State, out: Outgoing):
        return chain(
            self.convert_time(state, out.time),
            self.convert_track_position(out.parking_track),
            self.convert_trains(out.shunting_unit.trains)
        )
    
    def convert_shunting_units(self, state: State):
    	return pad(chain.from_iterable([self.convert_su(state, su) for su in state.shunting_units]), self.n_trains * self.su_size)
    
    def convert_su(self, state: State, su: ShuntingUnit):
        return pad(chain(
            [
            1 if state.is_moving(su) else 0, 
            1 if state.is_waiting(su) else 0,
            1 if state.is_in_neutral(su) else 0,
            1 if state.has_active_action(su) else 0
            ],
            self.convert_track_position(state.get_position(su)),
            self.convert_direction(state, su),
            self.convert_trains(state.get_train_units_in_order(su))
        ), self.su_size)
    
    def convert_trains(self, trains):
        return pad(chain.from_iterable([self.convert_train(train) for train in trains]), self.n_trains * self.train_size)

    def convert_train(self, train: Train):
        if train.type.display_name == 'SLT4':
            return np.array([1,0,0])
        elif train.type.display_name == 'SLT6':
            return np.array([0,1,0])
        elif train.type.display_name == 'SNG3':
            return np.array([0,0,1])
        return np.array([0,0,0])

    def convert_tracks(self, state: State):
        return chain.from_iterable([self.convert_track(state, track) for track in self.tracks])

    def convert_track(self, state: State, track: Track):
        sus_list = state.shunting_units
        sus = state.get_occupations(track)
        if len(sus) > 1:
            positions = [state.get_position_on_track(su)+0.01*i for i,su in enumerate(sus)]
            shunting_units = list(zip(*sorted(zip(positions, sus))))[1]
        return pad(chain.from_iterable(
            [get_index(len(sus_list), sus_list.index(su)) for su in sus]),
            self.n_trains * self.n_trains)

    def convert_track_position(self, track: Track):
        return get_index(len(self.tracks), self.track_map[track])

    def convert_direction(self, state: State, su: ShuntingUnit):
        pos = state.get_position(su)
        prev = state.get_previous(su)
        if pos.is_a_side(prev): return [1]
        return [0]

    def convert_time(self, state: State, time: int):
        return [(time - state.start_time) / (state.end_time - state.start_time)]

    def convert_valid_actions(self, state: State):
        if True: # if check for valid actions
            result = np.zeros(self.n_total_actions)
            try:
                valid_actions = self.engine.get_valid_actions(state)
            except ScenarioFailedError:
                return result
            valid_numbers = []
            for action in valid_actions:
                a_ix = self.convert_valid_action(state, action)
                result[a_ix] = 1
                valid_numbers.append(a_ix)
            print("valid actions: " + str(valid_numbers))
        else:
            result = np.ones(self.n_total_actions)
        return result

    def convert_valid_action(self, state: State, action: Action):
        base = 0
        if isinstance(action, ArriveAction):
            return base + sorted(state.incoming_trains, key=lambda inc: inc.time).index(action.incoming)
        base += self.n_inc
        if isinstance(action, ExitAction):
            return base + sorted(state.outgoing_trains, key=lambda out: out.time).index(action.outgoing)
        base += self.n_out
        su = action.shunting_unit
        su_ix = state.shunting_units.index(su)
        base += su_ix * self.n_actions
        if isinstance(action, BeginMoveAction):
            return base
        elif isinstance(action, EndMoveAction):
            return base + 1
        elif isinstance(action, WaitAction):
            return base + 2
        elif isinstance(action, SetbackAction):
            return base + 3
        elif isinstance(action, ServiceAction):
            return base + 4
        elif isinstance(action, SplitAction):
            return base + 5 + action.split_index - 1
        base += 5 + self.n_trains-1
        if isinstance(action, CombineAction):
            su2_ix = state.shunting_units.index(action.rear_shunting_unit)
            if su2_ix >= su_ix: su2_ix -= 1
            return base + su2_ix
        base += self.n_trains - 1
        if isinstance(action, MoveAction):
            return base + self.track_map[action.destination_track]
    
def get_index(n: int, i: int):
        a = np.zeros(n)
        a[i] = 1
        return a

def pad_infinite(iterable, padding=0):
   return chain(iterable, repeat(padding))

def pad(iterable, size, padding=0):
   return islice(pad_infinite(iterable, padding), size)
