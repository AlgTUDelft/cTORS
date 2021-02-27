from rl.tors_env import TORSEnv
from manager.config import Config
from stable_baselines3 import PPO
from stable_baselines3.common.monitor import Monitor
from rl.policy import ACPwithInvalidActions

use_save = False

data_folder = "episode.json" 
tensorboard_log = "./log_tensorboard/"
model_save = 'ppo_tors'

config = Config.load_from_file(data_folder, "episode")
env = TORSEnv(config)
env = Monitor(env, tensorboard_log)
if not use_save:
  model = PPO(ACPwithInvalidActions, env, verbose=1, tensorboard_log=tensorboard_log)
  model.learn(total_timesteps=200000)
  model.save(model_save)
else:
  model = PPO.load(model_save)

obs = env.reset()
for i in range(2000):
  action, _states = model.predict(obs)
  obs, rewards, done, info = env.step(action)
  env.render()
  if done:
    obs = env.reset()
