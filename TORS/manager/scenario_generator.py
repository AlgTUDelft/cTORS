from pyTORS import Scenario, Train, TrainUnitType, Task, ShuntingUnit, Incoming, Outgoing,\
    TrackPartType, Employee, Location
import random
import itertools
from functools import reduce
import operator as op
from abc import ABC, abstractmethod
import os

from warnings import warn

class ScenarioGenerator(ABC):
    
    def __init__(self, n_trains=None, n_disturbances=None, n_workers = None,
                 match_outgoing_trains=False, max_length=None, max_trains_per_track=None):
        self.n_trains = n_trains
        self.n_disturbances = n_disturbances
        self.n_workers = n_workers
        self.original_scenario = None
        self.location = None
        self.match_outgoing_trains = match_outgoing_trains
        self.max_length = n_trains if max_length is None else max_length
        self.enforce_max_length = not max_length is None
        self.max_trains_per_track = max_trains_per_track
    
    def initialize(self, engine, scenario_file_string) -> None:
        self.engine = engine
        self.location = engine.get_location()
        self.scenario_file_string = scenario_file_string
        
    @abstractmethod
    def generate_scenario(self) -> Scenario: pass

    @abstractmethod
    def get_max_trains(self): pass
    
    def get_scenario(self) -> Scenario:
        scenario = self.generate_scenario()
        self.match_trains(scenario)
        return scenario
    
    def match_trains(self, scenario: Scenario) -> None:
        if self.match_outgoing_trains:
            incoming = [t for su in [incoming.shunting_unit for incoming in scenario.incoming_trains] for t in su.trains]
            outgoing = [t for su in [outgoing.shunting_unit for outgoing in scenario.outgoing_trains] for t in su.trains]
            _match_trains(incoming, outgoing)
        else:
            for o in scenario.outgoing_trains:
                for tu in o.shunting_unit.trains:
                    tu.id = -1    

class ScenarioGeneratorFromScenario(ScenarioGenerator):
    
    def __init__(self, n_combinations=100, *args, **kwargs):
        super(ScenarioGeneratorFromScenario, self).__init__(*args, **kwargs)
        self.n_combinations = n_combinations
        if not self.match_outgoing_trains and self.enforce_max_length:
            warn("parameter 'max_length' is only supported when matching is enabled")
        if self.max_trains_per_track != self.max_trains_per_track:
            warn("parameter 'max_trains_per_track' not supported for ScenarioGeneratorFromScenario.")
        
    def initialize(self, engine, scenario_file_string) -> None:
        super(ScenarioGeneratorFromScenario, self).initialize(engine, scenario_file_string)
        self.scenario = engine.get_scenario(scenario_file_string)
        self._set_valid_trains()
        self.max_trains = len(self.valid_trains)
        max_disturbances = len(self.scenario.get_disturbance_list())
        max_workers = len(list(self.scenario.employees))
        self.n_trains = self._get_number(self.n_trains, self.max_trains)
        self.n_disturbances = self._get_number(self.n_disturbances, max_disturbances, min=0)
        self.n_workers = self._get_number(self.n_workers, max_workers)
        self.combination_generator = self._combination_generator()

    def _set_valid_trains(self):
        if self.enforce_max_length:
            _match_trains(
                [t for su in [incoming.shunting_unit for incoming in self.scenario.incoming_trains] for t in su.trains], 
                [t for su in [outgoing.shunting_unit for outgoing in self.scenario.outgoing_trains] for t in su.trains])
            valid_inc_trains = []
            for inc in self.scenario.incoming_trains:
                for i, t in enumerate(inc.shunting_unit.trains):
                    if i < self.max_length: valid_inc_trains.append(t)
                    else: break
            self.valid_trains = []
            for out in self.scenario.outgoing_trains:
                for i, t in enumerate(out.shunting_unit.trains):
                    if i < self.max_length:
                        if any((t.id == t2.id for t2 in valid_inc_trains)):
                            self.valid_trains.append(t)
                    else: break
            incoming_trains = []
            outgoing_trains = []
            for inc in self.scenario.incoming_trains:
                inc.shunting_unit.trains = [t for t in inc.shunting_unit.trains if t in self.valid_trains]
                if len(inc.shunting_unit.trains) > 0:
                    incoming_trains.append(inc)
            for out in self.scenario.outgoing_trains:
                out.shunting_unit.trains = [t for t in out.shunting_unit.trains if t in self.valid_trains]
                if len(out.shunting_unit.trains) > 0:
                    outgoing_trains.append(out)
            self.scenario.set_incoming_trains(incoming_trains)
            self.scenario.set_outgoing_trains(outgoing_trains)
        else:
            self.valid_trains = [t for su in [incoming.shunting_unit for incoming in self.scenario.incoming_trains] for t in su.trains]        

    def get_max_trains(self):
        return self.max_trains

    def _combination_generator(self):
        combination_generator = itertools.combinations(range(self.max_trains), self.n_trains)
        combinations_left = self.n_combinations
        seen_combinations = 0
        total_combinations = ScenarioGeneratorFromScenario.__ncr(self.max_trains, self.n_trains)
        while True:
            combination = next(combination_generator, None)
            if combination is None:
                combination_generator = itertools.combinations(range(self.max_trains), self.n_trains)
                combination = next(combination_generator)
            probability = (seen_combinations + combinations_left) / total_combinations 
            if probability >= 1.0 or random.random() < probability:
                combinations_left -= 1
                yield combination
            seen_combinations += 1
            if combinations_left == 0:
                combinations_left = self.n_combinations
                seen_combinations = 0

    def _get_number(self, given, max, min=1):
        if given is None:
            return random.randint(min, max)
        elif given == 'max': return max
        return given

    def generate_scenario(self) -> Scenario:
        scenario = self.scenario.get_copy() 
        self._select_trains(scenario)
        self._select_disturbances(scenario, self.n_disturbances)
        self._select_workers(scenario, self.n_workers)
        return scenario
        
    def _select_trains(self, scenario):
        incoming_shunting_units = [incoming.shunting_unit for incoming in scenario.incoming_trains]
        outgoing_shunting_units = [outgoing.shunting_unit for outgoing in scenario.outgoing_trains] 
        incoming_trains = [u for su in incoming_shunting_units for u in su.trains] 
        outgoing_trains = [u for su in outgoing_shunting_units for u in su.trains]
        
        combination = next(self.combination_generator)
        not_incoming_train_units = [it for i, it in enumerate(incoming_trains) if not i in combination]
        not_outgoing_train_units = []
        
        for u in not_incoming_train_units:
            mu = _find_matching_train(u, outgoing_trains)
            assert not mu is None
            not_outgoing_train_units.append(mu)
            outgoing_trains.remove(mu)
            in_su = _find_matching_shunting_unit(u, incoming_shunting_units)
            _remove_from_shunting_unit(in_su, u)
            out_su = _find_matching_shunting_unit(mu, outgoing_shunting_units)
            _remove_from_shunting_unit(out_su, mu)
                
        scenario.set_incoming_trains([incoming for incoming in scenario.incoming_trains if len(incoming.shunting_unit.trains) > 0])
        scenario.set_outgoing_trains([outgoing for outgoing in scenario.outgoing_trains if len(outgoing.shunting_unit.trains) > 0])
        scenario.set_end_time(max([o.time for o in scenario.outgoing_trains]))
                        
        
    def _select_disturbances(self, scenario, n_disturbances):
        pass #Not implemented
        
    def _select_workers(self, scenario, n_workers):
        pass # Not implemented
    
    @staticmethod
    def __ncr(n, r):
        if r > n or r < 0: return 0
        r = min(r, n-r)
        numer = reduce(op.mul, range(n, n-r, -1), 1)
        denom = reduce(op.mul, range(1, r+1), 1)
        return numer / denom

class RandomScenarioGenerator(ScenarioGenerator):
    
    def __init__(self, *args, **kwargs):
        ScenarioGenerator.__init__(self, *args, **kwargs)
    
    def generate_scenario(self):
        trains = self._generate_trains()
        incoming = self._generate_incoming(trains)
        outgoing = self._generate_outgoing(trains, incoming)
        disturbances = self._generate_disturbances()
        workers = self._generate_workers()
        return self._generate_scenario(incoming, outgoing, workers, disturbances)
    
    def get_max_trains(self):
        return self.n_trains

    def _generate_trains(self):
        return [Train(i, self._get_random_train_type(), self._get_task_list()) for i in range(1, self.n_trains+1)]
    
    def _get_random_train_type(self):
        options = [
            TrainUnitType("SLT4", 4, 70, 180, 120, 120, 16, 0, 0, "SLT", False, False, True),
            TrainUnitType("SNG3", 3, 35, 180, 120, 120, 16, 0, 0, "SNG", False, False, True)
        ]
        return random.choice(options)
    
    def _get_task_list(self):
        return [self._get_task() for i in range(random.randint(0,2))]
    
    def _get_task(self):
        options = [
            Task("Binnenkant reiniging", 1, 50, ["Binnenkant reiniging"])
        ]
        return random.choice(options)
    
    def _get_parking_tracks(self):
        tracks = [track for track in  self.location.get_track_parts() if track.type == TrackPartType.RAILROAD and track.parking_allowed]
        parking_tracks = {}
        for track in tracks:
            side_tracks =  track.get_neighbors()
            side_tracks = [track for track in side_tracks if track.type == TrackPartType.BUMPER]
            if len(side_tracks) > 0:
                parking_tracks[track] = side_tracks
        return parking_tracks
            
    def _generate_incoming(self, trains):
        types = list(set([train.type for train in trains]))
        trains_per_type = {typ: [train for train in trains if train.type == typ] for typ in types}
        parking_tracks = self._get_parking_tracks()
        trains_per_track = {track: 0 for track in parking_tracks}
        result = []
        id = 1
        last_time = 0
        while sum([len(t) for typ, t in trains_per_type.items()]) > 0:
            types = [typ for typ in types if len(trains_per_type[typ]) > 0]
            typ = random.choice(types)
            remaining_trains = trains_per_type[typ]
            next_len = random.randint(1, min(len(trains_per_type[typ]), self.max_length))
            selected, trains_per_type[typ] = remaining_trains[:next_len], remaining_trains[next_len:]
            track = random.choice(list(parking_tracks.keys()))
            side_track = random.choice(parking_tracks[track])
            su = ShuntingUnit(id, selected)
            time = max(last_time, random.choice([0, random.randint(last_time, last_time + 50)]))
            instanding = random.choice([True, False]) if time == 0 else False
            standing_index = 0 if not instanding else trains_per_track[track] + 1
            incoming = Incoming(id, su, track, side_track, time, instanding, standing_index)
            result.append(incoming)
            trains_per_track[track] += 1
            id += 1
            last_time = time
        return result
    
    def _generate_outgoing(self, trains, incomings):
        types = list(set([train.type for train in trains]))
        trains_per_type = {typ: [train for train in trains if train.type == typ] for typ in types}
        arrival_times = {}
        arrival_track = {}
        for incoming in incomings:
            for train in incoming.shunting_unit.trains:
                arrival_times[train.id] = incoming.time
                arrival_track[train.id] = incoming.parking_track_id
        parking_tracks = self._get_parking_tracks()
        trains_per_track = {track: 0 for track in parking_tracks}
        result = []
        id = 1
        while sum([len(t) for typ, t in trains_per_type.items()]) > 0:
            types = [typ for typ in types if len(trains_per_type[typ]) > 0]
            typ = random.choice(types)
            remaining_trains = trains_per_type[typ]
            next_len = random.randint(1, min(len(trains_per_type[typ]), self.max_length))
            selected, trains_per_type[typ] = remaining_trains[:next_len], remaining_trains[next_len:]
            track = random.choice(list(parking_tracks.keys()))
            side_track = random.choice(parking_tracks[track])
            time = max([arrival_times[train.id] + self.location.pathfinder.get_fastest_route(cur_track_id=arrival_track[train.id], dest_track_id=track.id, dest_dir_id=side_track.id)[1] for train in selected])
            time += random.randint(150, 300*(1+len(trains)))
            outstanding = False
            standing_index = 0 if not outstanding else trains_per_track[track] + 1
            outgoing_trains = [tu.get_copy() for tu in selected]
            if not self.match_outgoing_trains:
                for train in outgoing_trains:
                    train.id = -1
            su = ShuntingUnit(id, outgoing_trains)
            outgoing = Outgoing(id, su, track, side_track, time, outstanding, standing_index, False)
            result.append(outgoing)
            trains_per_track[track] += 1
            id += 1
            if not self.max_trains_per_track is None and trains_per_track[track] >= self.max_trains_per_track:
                del parking_tracks[track]
            
        
        return result
    
    def _generate_workers(self):
        always_shift = [(0, 115200)]
        tracks = [track.id for track in  self.location.get_track_parts() if track.type == TrackPartType.RAILROAD and track.parking_allowed]
        workers = [
            Employee(id, chr(64+id), ["Rangeren"], always_shift, break_windows=[], break_duration=0, start_track_id=random.choice(tracks), end_track_id=random.choice(tracks), can_move_shunting_units=True)
            for id in range(1,self.n_workers+1)
        ]
        id = len(workers) + 1
        
        cleaning_skills = [
            "Binnenkant reiniging",
            "Kop reiniging",
            "Zeep reiniging zijkant",
            "Oxaal reiniging zijkant", 
            "Zeep reiniging",
            "Oxaal reiniging"
            ]
        facilities = self.location.facilities
        cleaning_facilities = [f for f in facilities if any(['reiniging' in tt.lower() for tt in f.task_types])]
        if len(cleaning_facilities) > 0:
            cleaning_facility = random.choice(cleaning_facilities)
            track_id = random.choice(cleaning_facility.related_track_ids)
            workers.append(
                Employee(id, chr(64+id), cleaning_skills, always_shift, break_windows=[], break_duration=0, start_track_id=track_id, end_track_id=track_id, can_move_shunting_units=False)
            )
        return workers
    
    def _generate_disturbances(self):
        return []
    
    def _generate_scenario(self, incoming, outgoing, workers, disturbances):
        scenario = Scenario()
        for i in incoming:
            scenario.add_incoming_train(i)
        for o in outgoing:
            scenario.add_outgoing_train(o)
        for e in workers:
            scenario.add_employee(e)
        for d in disturbances:
            scenario.add_disturbance(d)
        scenario.set_end_time(max([o.time for o in outgoing]))
        return scenario

class ScenarioGeneratorFromFolder(ScenarioGenerator):

    def __init__(self, subclass, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.subclass = subclass
        self.args = args
        self.kwargs = kwargs

    def initialize(self, engine, scenario_file_string) -> None:
        super().initialize(engine, scenario_file_string)
        self.generators = {}
        scenarios = []
        if isinstance(scenario_file_string, list):
            scenarios = scenario_file_string
        elif os.path.isdir(scenario_file_string):
            for file in os.listdir(scenario_file_string):
                scenarios.append(os.path.join(scenario_file_string, file))
        elif os.path.isfile(scenario_file_string):
            scenarios = [scenario_file_string]
        else:
            raise NotImplementedError("No scenarios can be obtained from " + scenario_file_string)
        to_be_deleted = []
        for scenario in scenarios:
            self.generators[scenario] = self.subclass(*self.args, **self.kwargs)
            self.generators[scenario].initialize(self.engine, scenario)
            if self.generators[scenario].get_max_trains() < self.n_trains:
                to_be_deleted.append(scenario)
        for scenario in to_be_deleted:
            del self.generators[scenario]

    def get_max_trains(self):
        return super().get_max_trains()

    def generate_scenario(self) -> Scenario:
        while True:
            scenario_key = random.choice(list(self.generators.keys()))
            scenario_gen = self.generators[scenario_key]
            return scenario_gen.generate_scenario()


def _find_matching_train(train, train_list):
    mu = None
    if train.id != -1:
        mu = next((t for t in train_list if train.id == t.id), None)
    if mu is None:
        mu = next((t for t in train_list if train.type == t.type), None)
    return mu
    
def _remove_from_shunting_unit(su, train):
    su.trains = [t for t in su.trains if not t == train]

def _find_matching_shunting_unit(train, shunting_units):
    return next((su for su in shunting_units \
        if not next((t for t in su.trains if t == train), None) is None), None)
    
def _match_trains(incoming, outgoing):
        to_be_matched = [tu for tu in outgoing if tu.id == -1]
        available_for_matching = [tu for tu in incoming if not tu.id in [otu.id for otu in outgoing]]
        for tu in to_be_matched:
            mu = _find_matching_train(tu, available_for_matching)
            tu.id = mu.id
            available_for_matching.remove(mu)
        