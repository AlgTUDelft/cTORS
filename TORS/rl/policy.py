from stable_baselines3.common.policies import ActorCriticPolicy
from stable_baselines3.common.torch_layers import MlpExtractor
import numpy as np
import torch as th
import torch.nn as nn
from stable_baselines3.common.distributions import CategoricalDistribution, Distribution, MultiCategoricalDistribution, make_proba_distribution

### Overrides the ActorCriticPolicy, by setting the action logits of invalid actions to -inf.
### Most of the implementation is copied from stable_baselines3.common.policies ActorCriticPolicy
class ACPwithInvalidActions(ActorCriticPolicy):
  
  def _get_action_dist_from_latent(self, obs, latent_pi, latent_sde = None):
    mean_actions = self.action_net(latent_pi)
    #The last bits in the observation vector, tell the agent which actions are valid
    valid = obs[:, -len(mean_actions[0]):] == 1
    if sum(sum(valid)) > 0: #If there are valid actions
        #Make sure that the invalid acctions cannot be chosen.
      mean_actions[~valid] = -np.inf
    if isinstance(self.action_dist, CategoricalDistribution):
        # Here mean_actions are the logits before the softmax
        return self.action_dist.proba_distribution(action_logits=mean_actions)
    elif isinstance(self.action_dist, MultiCategoricalDistribution):
        # Here mean_actions are the flattened logits
        return self.action_dist.proba_distribution(action_logits=mean_actions)
    else:
        raise ValueError("Invalid action distribution")

  def forward(self, obs, deterministic = False):
    """
    Forward pass in all the networks (actor and critic)
    :param obs: Observation
    :param deterministic: Whether to sample or use deterministic actions
    :return: action, value and log probability of the action
    """
    latent_pi, latent_vf, latent_sde = self._get_latent(obs)
    # Evaluate the values for the given observations
    values = self.value_net(latent_vf)
    distribution = self._get_action_dist_from_latent(obs, latent_pi, latent_sde=latent_sde)
    actions = distribution.get_actions(deterministic=deterministic)
    log_prob = distribution.log_prob(actions)
    return actions, values, log_prob

  def _predict(self, observation, deterministic = False) -> th.Tensor:
    """
    Get the action according to the policy for a given observation.
    :param observation:
    :param deterministic: Whether to use stochastic or deterministic actions
    :return: Taken action according to the policy
    """
    latent_pi, _, latent_sde = self._get_latent(observation)
    distribution = self._get_action_dist_from_latent(observation, latent_pi, latent_sde)
    actions = distribution.get_actions(deterministic=deterministic)
    return actions

  def evaluate_actions(self, obs, actions):
    """
    Evaluate actions according to the current policy,
    given the observations.
    :param obs:
    :param actions:
    :return: estimated value, log likelihood of taking those actions
        and entropy of the action distribution.
    """
    latent_pi, latent_vf, latent_sde = self._get_latent(obs)
    distribution = self._get_action_dist_from_latent(obs, latent_pi, latent_sde)
    log_prob = distribution.log_prob(actions)
    values = self.value_net(latent_vf)
    return values, log_prob, distribution.entropy()

