from planner.planner import Planner
import tensorflow as tf
import numpy as np
try:
   import cPickle as pickle
except:
   import pickle

from tensorflow.keras.models import Sequential, load_model
from tensorflow.keras.layers import Dense, Input, Dropout
from keras.backend.tensorflow_backend import set_session
import os
import random
import copy
#from tors.core.location.track_part.track_part import TrackPartType
from pyTORS import TrackPartType, ArriveAction, ExitAction, BeginMoveAction, EndMoveAction, MoveAction, WaitAction, SetbackAction, ServiceAction
from collections import deque
import matplotlib.pyplot as plt

class SimpleRLPlanner(Planner):
    INC_COU = 10
    OUT_COU = 10
    SHU_COU = 10
    session = None
    
    def __init__(self, seed, verbose, config, learning=True):
        super(SimpleRLPlanner, self).__init__(seed, verbose, config)
        self.version = 1
        self.filename = self.config.file_format.format(self.version)
        self.learning = learning
        self.buffer = ReplayBuffer(self.config.buffer_size)
        self.run_buffer = []
        self.nn = None
        self.epsilon = 1
        self.runs = 0
        self.reward = 0
        self.last_state = None
        self.track_map = {}
        self.tracks = []
        if self.buffer.size() == 0 and os.path.isfile(self.config.random_buffer):
            self.buffer.load_from_file(self.config.random_buffer)

    def build_nn(self): 
        state = self.encode(None)
        model = Sequential()
        model.add(Input(shape=state.shape))
        if self.config.dropout > 0:
            model.add(Dropout(self.config.dropout, input_shape=state.shape))
        for i in range(self.config.n_dense_layers):
            amount = self.config["dense{}".format(i+1)]
            model.add(Dense(amount, activation='linear' if i == 0 else leaky_relu))
        model.add(Dense(self.get_action_space_size(), activation='linear' if self.config.n_dense_layers == 0 else leaky_relu, \
                             kernel_initializer='glorot_uniform'))
        model.compile(loss='mean_squared_error', optimizer='adam',\
                           metrics=[])   
        return model
                
    def save(self):
        if self.learning:
            self.nn.save(self.filename)
    
    def load(self):
        try:
            return load_model(self.filename, custom_objects={"tf":tf, "leaky_relu": leaky_relu})
        except:
            model = self.build_nn()
            model.load_weights(self.filename)
            return model
    
    def get_action_space_size(self):
        n_tracks = len(self.track_map)
        return SimpleRLPlanner.INC_COU + SimpleRLPlanner.OUT_COU + SimpleRLPlanner.SHU_COU * (n_tracks + 4)
    
    def train(self, validation_split=0.0):
        result = {"loss": [], "val_loss":[]}
        for _ in range(self.config.n_epochs):
            batch_s, batch_sn, batch_a, batch_r = self.buffer.sample(self.config.sample_size)
            alpha = self.config["alpha"]
            gamma = self.config["gamma"]
            q_s = self.nn.predict(batch_s)
            has_next_states = batch_r==None
            if sum(has_next_states) > 0:
                q_ns = np.zeros((len(batch_sn), self.get_action_space_size()), np.float32)
                q_ns[has_next_states, :] = self.nn.predict(np.asarray([np.array(s, np.float32) for s in batch_sn[has_next_states]], np.float32))
            for i, next_state in enumerate(batch_sn):
                if next_state is None:
                    q_s[i,:] = batch_r[i]
                else:
                    q_s[i][batch_a[i]] = (1.0-alpha) * q_s[i][batch_a[i]] + alpha * gamma * np.max(q_ns[i])  
            batch_result = self.train_model(batch_s, q_s,  validation_split=validation_split, epochs=1)
            result['loss'].append(batch_result['loss'][0])
            if validation_split > 0:
                result['val_loss'].append(batch_result['val_loss'][0])
            #plt.hist(q_s.flatten())
            #plt.show()
            #result = self.nn.train_on_batch(batch_s, q_s)
        self.save()
        return result
    
    def train_model(self, states, y, validation_split=0.0, epochs=1):
        if self.learning:
            hist = self.nn.fit(np.asarray(states), np.asarray(y), validation_split=validation_split, verbose=1, epochs=epochs)
            return hist.history
        return None
        
    def encode(self, state=None):
        TYP_LEN = 5
        OCC_COU = 3
        TRA_COU = 6
        INC_COU = SimpleRLPlanner.INC_COU
        INC_LEN = 1 + OCC_COU * TYP_LEN + TRA_COU 
        OUT_COU = SimpleRLPlanner.OUT_COU
        OUT_LEN = 1 + OCC_COU * TYP_LEN + TRA_COU
        TRA_LEN = OCC_COU * (TYP_LEN + 1) + (OCC_COU-1)
        SHU_COU = SimpleRLPlanner.SHU_COU
        SHU_LEN = TRA_COU + 2
        GEN_LEN = 1
        result = np.zeros(INC_COU*INC_LEN + OUT_COU*OUT_LEN + TRA_COU*TRA_LEN + SHU_LEN * SHU_COU + GEN_LEN)
        if state is None: return result
        for i, incoming in enumerate(state.get_incoming_trains()):
            if i >= INC_COU: break
            pt = i*INC_LEN
            for j, train in enumerate(incoming.shunting_unit.train_units):
                if j >= OCC_COU: break
                result[pt + j*TYP_LEN:pt + (j+1)*TYP_LEN] = self.get_train_type_encoding(train)
            result[pt + OCC_COU*TYP_LEN] = (incoming.time - state.start_time) / (state.end_time - state.start_time)
            result[pt + OCC_COU*TYP_LEN + 1 + self.track_map[incoming.get_parking_track().id]] = 1
        for i, outgoing in enumerate(state.get_outgoing_trains()):
            if i >= OUT_COU: break
            pt = INC_COU * INC_LEN + i*OUT_LEN
            for j, train in enumerate(outgoing.shunting_unit.train_units):
                if j >= OCC_COU: break
                result[pt + j*TYP_LEN:pt + (j+1)*TYP_LEN] = self.get_train_type_encoding(train)
            result[pt + OCC_COU*TYP_LEN] = (outgoing.time - state.start_time) / (state.end_time - state.start_time)
            result[pt + OCC_COU*TYP_LEN + 1 + self.track_map[outgoing.get_parking_track().id]] = 1
        tracks = [track for track in self.get_location().get_track_parts() if track.type == TrackPartType.RAILROAD]
        for i, track in enumerate(tracks):
            if i >= TRA_COU: break
            pt = INC_COU * INC_LEN + OUT_COU*OUT_LEN + i * TRA_LEN
            shunting_units = state.get_occupations(track)
            if len(shunting_units) == 0: continue
            if len(shunting_units) > 1:
                positions = [state.get_position_on_track(su)+0.01*i for i,su in enumerate(shunting_units)]
                shunting_units = list(zip(*sorted(zip(positions, shunting_units))))[1]
            tix = 0
            for j, su in enumerate(shunting_units):
                for k, tu in enumerate(state.get_train_units_in_order(su)):
                    if tix + k >= OCC_COU: continue
                    pt2 = pt + (tix+k) * (TYP_LEN + 2)
                    result[pt2:pt2 + TYP_LEN] = self.get_train_type_encoding(tu)
                    direction = state.get_direction(su)
                    result[pt2 + TYP_LEN] = direction
                    if k>0: result[pt2-1] = 1
                tix += len(su.train_units)
        pos = INC_COU*INC_LEN + OUT_COU*OUT_LEN + TRA_COU*TRA_LEN
        for i, su in enumerate(state.get_shunting_units()):
            if i >= SHU_COU: break
            pt = pos + i * SHU_LEN
            result[pt] = 1 if state.is_moving(su) else 0
            result[pt+1] = 1 if state.has_active_action(su) else 0
            track = state.get_position(su)
            result[pt+2 + self.track_map[track.id]] = 1
        pos = len(result) - GEN_LEN
        result[pos] = (state.time - state.start_time) / (state.end_time - state.start_time)
        return result
    
    def get_action_from_encoding(self, state, actions, encoded):
        index = np.argmax(encoded)
        if index < SimpleRLPlanner.INC_COU:
            inc = state.get_incoming_trains()[index]
            return [a for a in actions if isinstance(a, ArriveAction) and a.get_incoming() == inc][0]
        index -= SimpleRLPlanner.INC_COU
        if index < SimpleRLPlanner.OUT_COU:
            out = state.get_outgoing_trains()[index]
            return [a for a in actions if isinstance(a, ExitAction) and a.get_outgoing() == out][0]
        index -= SimpleRLPlanner.OUT_COU
        n_actions = 5 + len(self.track_map)
        su_ix, a_ix = index // n_actions, index % n_actions
        actions = [a for a in actions if a.get_shunting_unit() == state.get_shunting_units()[su_ix]]
        if a_ix == 0: 
            return [a for a in actions if isinstance(a, BeginMoveAction)][0]
        elif a_ix == 1: 
            return [a for a in actions if isinstance(a, EndMoveAction)][0]
        elif a_ix == 2: 
            return [a for a in actions if isinstance(a, WaitAction)][0]
        elif a_ix == 3: 
            return [a for a in actions if isinstance(a, SetbackAction)][0]
        elif a_ix == 4: 
            return [a for a in actions if isinstance(a, ServiceAction)][0]
        else:
            track = self.tracks[a_ix - 5]
            return [a for a in actions if isinstance(a, MoveAction) and a.get_destination_track() == track][0]

    def get_action_index(self, state, action):
        if isinstance(action, ArriveAction):
            return state.get_incoming_trains().index(action.get_incoming())
        elif isinstance(action, ExitAction):
            return SimpleRLPlanner.INC_COU + state.get_outgoing_trains().index(action.get_outgoing())
        index = SimpleRLPlanner.INC_COU + SimpleRLPlanner.OUT_COU
        su_ix = state.get_shunting_units().index(action.get_shunting_unit())
        actions = 5 + len(self.track_map)
        if isinstance(action, BeginMoveAction): a_ix = 0
        elif isinstance(action, EndMoveAction): a_ix = 1
        elif isinstance(action, WaitAction): a_ix = 2
        elif isinstance(action, SetbackAction): a_ix = 3
        elif isinstance(action, ServiceAction): a_ix = 4
        else:
            a_ix = 5 + self.track_map[action.get_destination_track().id]
        return index + su_ix * actions + a_ix
        
    def get_train_type_encoding(self, train):
        if train.type.display_name == 'SLT4':
            return np.array([1,0,0,0,0])
        elif train.type.display_name == 'SLT6':
            return np.array([0,1,0,0,0])
        elif train.type.display_name == 'SNG3':
            return np.array([0,0,1,0,0])
        return np.array([0,0,0,0,0])
    
    def get_action(self, state, actions):
        s = self.encode(state)            
        if self.learning and not self.last_state is None:
            self.run_buffer.append((self.last_state[0], s, self.last_state[1]))
        if self.learning and self.get_random().random() < self.epsilon:
            ix = self.get_random().randint(0, len(actions)-1)
            a_ix = self.get_action_index(state, actions[ix])
            self.last_state = (s, a_ix)
            return actions[ix]
        else:
            values = self.nn.predict(s[None, :])[0]
            valid_values = np.ones(len(values)) * -np.inf
            for a in actions:
                a_ix = self.get_action_index(state, a)
                valid_values[a_ix] = values[a_ix]
            a_ix = np.argmax(valid_values)
            action = self.get_action_from_encoding(state, actions, valid_values)
            self.last_state = (s, a_ix)
            return action
        
    def report_result(self, result):
        if self.learning and not self.last_state is None:
            print("Add result {} to buffer.".format(result))
            self.buffer.add((self.last_state[0], None, self.last_state[1], result))
        if len(self.run_buffer) > 0:
            sample = random.sample(self.run_buffer, min(len(self.run_buffer), 5))
            for s in sample:
                self.buffer.add((s[0], s[1], s[2], None))
        self.run_buffer = []
        self.reward += result
        self.runs += 1
                     
    def new_epoch(self):
        self.runs = 0
        self.reward = 0
        if not self.learning: return
        
        if self.buffer.size() < self.buffer.buffer_size:
            print("Buffer not yet full, continue random exploration: {}/{}".format(self.buffer.size(), self.buffer.buffer_size))
            return
        elif not os.path.isfile(self.config.random_buffer):
            self.buffer.save_to_file(self.config.random_buffer)
        self.train()
        self.epsilon = max(0.001, self.epsilon*self.config.epsilon_decay)
    
    def can_increase_difficulty(self):
        return self.runs > 0 and self.reward / self.runs > 0.75
    
    def increase_difficulty(self):
        self.epsilon = min(1.0, self.epsilon + 0.3)
        
    def get_performance(self):
        if self.runs == 0: return 0
        return self.reward / self.runs
    
    def get_epsilon(self):
        return self.epsilon
    
    def set_location(self, location):
        Planner.set_location(self, location)
        for i, track in enumerate([track for track in self.get_location().get_track_parts() if track.type == TrackPartType.RAILROAD]):
            self.track_map[track.id] = i
            self.tracks.append(track)
                
    def reset(self, load_from_file=True): 
        if SimpleRLPlanner.session is None:
            config = tf.ConfigProto()
            config.gpu_options.allow_growth = True  # dynamically grow the memory used on the GPU
            config.log_device_placement = False  # to log device placement (on which device the operation ran)
            SimpleRLPlanner.session = tf.Session(config=config)
            set_session(SimpleRLPlanner.session)  # set this TensorFlow session as the default session for Keras
        if self.nn is None:
            if load_from_file and os.path.isfile(self.filename):
                self.nn = self.load()
                self.nn._make_predict_function()
            self.nn = self.build_nn()
            self.save()
        self.last_state = None
        
class ReplayBuffer:
    """Constructs a buffer object that stores the past actions
    and samples a set of subsamples"""

    def __init__(self, buffer_size):
        self.buffer_size = buffer_size
        self.count = 0
        self.buffer = deque()

    def add(self, experience):
        """Add an experience to the buffer"""
        # S represents current state, a is action,
        # r is reward, d is whether it is the end, 
        if self.count < self.buffer_size:
            self.buffer.append(experience)
            self.count += 1
        else:
            self.buffer.popleft()
            self.buffer.append(experience)

    def size(self):
        return self.count

    def sample(self, batch_size):
        """Samples a total of elements equal to batch_size from buffer
        if buffer contains enough elements. Otherwise return all elements"""

        batch = []
        if self.count <= batch_size:
            batch = self.buffer
        else:
            batch = random.sample(self.buffer, batch_size)

        # Maps each experience in batch in batches of states, actions, rewards
        # and new states
        s_batch, ns_batch, a_batch, r_batch = list(map(np.array, list(zip(*batch))))

        return np.array(s_batch), np.array(ns_batch), np.array(a_batch), np.array(r_batch)

    def clear(self):
        self.buffer.clear()
        self.count = 0
    
    def load_from_file(self, filename):
        with open(filename, "rb") as f:
            data = pickle.load(f)
        print("Loading {} samples from buffer file".format(len(data)))
        for exp in data:
            self.add(exp)
        s_batch, ns_batch, a_batch, r_batch = self.sample(self.size())
        #plt.hist(a_batch, bins=range(min(a_batch), max(a_batch) + 1, 1))
        #plt.show()
        #has_next_states = r_batch ==None
        #runs = r_batch[~has_next_states]
        #plt.hist(runs, bins=np.arange(min(runs), max(runs) + 0.02, 0.02))
        #plt.show()
    
    def save_to_file(self, filename):
        data = list(self.buffer)
        print("Saving {} samples to buffer file".format(len(data)))
        with open(filename, "wb") as f:
            pickle.dump(data, f)
            
def leaky_relu(x): 
    return tf.nn.leaky_relu(x, alpha=0.01)            