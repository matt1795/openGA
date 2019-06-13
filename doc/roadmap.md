# OpenGA Roadmap

The goal of the project is to provide a header-only Genetic Algorithm framework
that gives users flexibility to match their applications. The idea is to provide
a base "engine" that the user can take and customize behavior, but also to
provide helper classes to make some of those custom behaviors easy to use.

## Things to add

### Selection Strategies
- Fitness Proportionate
- Tournament
- Stochastic Universal Sampling
- Reward Based
- Truncation

### Mutation Strategies

### Multithreading
- Add Strategies
- Add Parallel STL when gcc and clang support it

### Containers (maybe use crtp?)
Containers to help wrap different solutions
- List
- Tree
- Graph
