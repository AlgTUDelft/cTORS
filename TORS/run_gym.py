from rl.tors_env import TORSEnv
from manager.config import Config
from stable_baselines3 import PPO
from stable_baselines3.common.monitor import Monitor
from rl.policy import ACPwithInvalidActions

use_save = True

tensorboard_log = "./log_tensorboard/"
model_save = 'ppo_tors'

e_config = Config.load_from_file("episode.json", "episode")
a_config = Config.load_from_file("agent.json", "agent")
env = TORSEnv(e_config, a_config)
env = Monitor(env, tensorboard_log)
if not use_save:
  model = PPO(ACPwithInvalidActions, env, verbose=1, tensorboard_log=tensorboard_log)
  model.learn(total_timesteps=200000)
  model.save(model_save)
else:
  model = PPO.load(model_save)

obs = env.reset()
run = 0
for i in range(2000):
  action, _states = model.predict(obs)
  obs, rewards, done, info = env.step(action)
  env.render()
  if done:
    env.write_to_file("./runs/run_"+str(run)+".json")
    obs = env.reset()
    run += 1
