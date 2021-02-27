from manager.c_manager import Manager
from manager.config import Config
import time
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-a", "--agent", \
        help="Path to the agent configuration file (default=agent.json)", \
        required=False, default="agent.json")
    parser.add_argument("-e", "--episode", \
        help="Path to the episode configuration file (default=episode.json)", \
        required=False, default="episode.json")
    parser.add_argument("-t", "--train", action='store_true', \
        help="Train the agent (default=evaluate)", required=False)
    args = parser.parse_args()
    start = time.time()
    manager = Manager(Config.load_from_file(args.episode, "episode"), Config.load_from_file(args.agent, "agent"))
    if args.train:
        manager.train()
    else:
        manager.run()
    print("Total running time: {}".format(time.time() - start))

    #print("\n***** Summary Metrics *****")
    #pprint.pprint(metrics)