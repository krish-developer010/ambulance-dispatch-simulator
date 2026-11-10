# Ambulance Dispatch & Emergency Routing System

This project is a C++17 console-based simulator for a priority-based ambulance dispatch and road-routing system.

## Features
- Weighted road graph with adjacency-list representation
- Custom binary Min-Heap
- Dijkstra shortest-path routing
- BFS and DFS graph traversal
- Emergency prioritization by severity, then arrival time
- Ambulance availability and status tracking
- Hospital bed capacity tracking
- Dispatch history and basic statistics
- Console menu for interaction

## Project Structure
- `include/` — class and structure declarations
- `src/` — implementations
- `tests/` — test cases
- `CMakeLists.txt` — build configuration

## Build
```bash
cmake -S . -B build
cmake --build build
```

## Run
```bash
./build/ambulance_dispatch
```

## Test
```bash
ctest --test-dir build --output-on-failure
```

## Notes
This project was designed for a beginner-friendly DS/Algorithms lab implementation and follows the requirement to use:
- custom graph
- custom min-heap
- Dijkstra with heap
- emergency priority queue
- ambulance and hospital modules
- route reconstruction and dispatch history
