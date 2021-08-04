import json
import os
import importlib

class Config(dict):
    """dot.notation access to dictionary attributes"""
    __getattr__ = dict.get
    __setattr__ = dict.__setitem__
    __delattr__ = dict.__delitem__
    
    def __init__(self, **kwargs):
        super(Config, self).__init__()
        self.update(kwargs)
        for k,v in self.items():
            if isinstance(v,dict):
                self[k] = Config(**v)
    
    def __getitem__(self, key):
        splt = key.split("/")
        config = self
        for s in splt:
            if not dict.__contains__(config, s): raise KeyError("{} not in Config".format(key))
            config = dict.__getitem__(config, s)
        return config
    
    def __contains__(self, key):
        splt = key.split("/")
        config = self
        for s in splt:
            if not dict.__contains__(config, s): return False
            config = dict.__getitem__(config, s)
        return True
    
    def __getstate__(self):
        return self

    def __setstate__(self, state):
        self.update(state)
        self.__dict__ = self
    
    @staticmethod
    def load_from_file(filename, typ):
        with open(filename) as json_data_file:
            data = json.load(json_data_file)
        result = Config.__default_values__[typ].copy()
        Config._nested_update(result, data)
        config = Config(**result)
        config._check_required_fields(typ)
        config._check_valid_fields(typ)
        return config
    
    @staticmethod
    def _nested_update(d, u):
        for k,v in u.items():
            if k in d and isinstance(d[k], dict):
                Config._nested_update(d[k], v)
            else:
                d[k] = v
               
    def _check_required_fields(self, typ):
        required_fields = {"episode": ['data folder', 'scenario', 'generator', 'generator/class'],
                            "agent": ['class']}[typ]
        for field in required_fields:
            if not field in self:
                raise Exception("Field {} missing in configuration".format(field))
                        
    def _check_valid_fields(self, typ):
        validations = {
            "episode": {
                'generator/class': Config._valid_class,
                'data folder': Config._valid_data_folder 
            },
            "agent": {
                'class': Config._valid_class
            }
        }[typ]
        for field, validation_function in validations.items():
            if field in self:
                try: validation_function(self[field])
                except Exception as e:
                    raise Exception("Error in configuration.\nInvalid setting for {}: {}\n{}".format(field, self[field], e))
    
    @staticmethod
    def _valid_class(value):
        try:
            planner_lst = value.split('.')
            _module = importlib.import_module(".".join(planner_lst[:-1]))
            _class = getattr(_module, planner_lst[-1])
        except:
            raise Exception("Cannot find file or class: {}".format(value))
    
    @staticmethod
    def _valid_data_folder(value):
        if not os.path.exists(value):
            raise Exception("Path {} does not exist".format(value))
        
    __default_values__ = {
        "episode": {
            "n_runs": 1,
            "max_trains": 1,
            "time_limit": -1,
            "verbose": 1
        },
        "agent": {
            "class": "planner.random_planner.RandomPlanner",
            "seed": 42,
            "verbose": 1
        }
    }
        