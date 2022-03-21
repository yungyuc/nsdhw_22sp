================================================
BBGym : The branch and bound framework 
================================================

Project name: `BBGym <https://github.com/ianlienfa/BBGym>`__

This is a branch and bound problem solving framework for all kinds of OR problems (especially scheduling problems)
The goal of this framework is to make it easy to toggle between different search strategies, branching strategies and pruning strategies
The framework aim to construct environment for Reinforcement Learning the branch and search strategies on branch and bound

Problem to Solve
================
Goal of this framework:

To provide a framework for testing out different search, branch and pruning strategies for some general scheduling problems.

When researching on dominance relationships or problem-specific heuristics on OR problems, we often have to:

1. Implement the best heuristic and pruning strategy known to us.
2. Test out different search strategies and branch and pruning strategies.

This framework is designed to be elastic enough to be able to easily insert and remove strategies
  
Prospective Users
=================

1. Researchers that are interested in the framework and want to try out different search, branch and pruning strategies for OR problems.
2. Students who would like to solve problem using branch and bound
3. Industry professionals that would like to try out some ideas before moving onto implementing an state-of-the-art solver
  For example: Industry like TSMC or MediaTek often need to solve scheduling problems for distributing jobs to machines under some given constraints.

System Architecture
===================

`BB_engine`: 
  This is the main engine of the framework.  It is responsible for the following:
  1. Read the input file and parse the input
  2. Construct the environment
  3. Run the search strategy
  4. Output the results

`branch_modules`:
  This is the collection of branch classes
  A branch class should inherit the base class `BranchMod`, which requires the following:
  * Implement the `branch` function

`prune_modules`:
  This is the collection of pruning classes
  A pruning class should inherit the base class `PruneMod`, which requires the following:
  * Implement the `prune` function

`Node`:
  This is the collection of node classes
  A specific problem requires a node class and a graph class to be defined.
  It generically specifies the node information during branch and bound

`Graph`:
  The graph class is used to store the graph information during branch and bound
  Compared to the node class, the graph class has more global information and knows about the state of the search
  For example, a graph class for the :math:`1|\Sigma_Cj` problem, we need:
    * the configuration of the probelm instance
    * the best value founded so far    
    * the data structure for searching

`config.h`:
  The configuration for the problem solving
  Users can twiddle with the configuration in this file, 
  including the problem type, the searching strategy, the branching strategy, and the pruning strategy

In general, the framework calls the BB_engine at the start of the program, 
then for every node visited, the framework calls the branch_modules to branch, 
the prune_modules to prune, updates the graph, then relies on the search_module to search for the next node.

The users can use this system solely from source, twiddle the configuration in the config.h file, and build them from the source.  
For this project, we hope to wrap it in a python package, and provide a gym-like api for RL on branch and bound problem solving.


API Description
===============

The following API follows the OpenAI gym api for environments
The basic usage will be like following:

```python
import gym
env = gym.make('BBGym-v0')
done = False
while not done:
  action = env.action_space.sample()
  observation, reward, done, info = env.step(action)
  print(observation, reward, done, info)

```

Engineering Infrastructure
==========================

Build System:
This project uses cmake for its build system, git as version control system
and uses google test and pytest as testing framework.

The documentation will be presented on github 


Schedule
========

* Planning phase (6 weeks including 2/21, 2/28, 3/7, 3/14, 3/21, 3/28):
* Week 1 (4/4): 
    * Testing environment setup
* Week 2 (4/11):
    * Pybind binding for the framework
* Week 3 (4/18):
    * Pybind binding for the framework
* Week 4 (4/25):
    * Adjust the framework to be supported by the gym api
* Week 5 (5/2):
    * Adjust the framework to be supported by the gym api
* Week 6 (5/9):
    * The final version of the framework
* Week 7 (5/16):
    * documentation
* Week 8 (5/23):
