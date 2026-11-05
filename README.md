# Automaton
A 2D falling-particle simulation built with C++23 and raylib. Select sand, water, stone, fire, or air from the material menu, then paint with the left mouse button. Watch sand and water fall and spread, and see sand sink through water. The simulation engine and input logic have no graphics dependency.

## Build and run
From the repository root:
```sh
cmake -S . -B build
cmake --build build --parallel
./build/automaton
```

## Tests
Tests are enabled by default for native builds:

```sh
ctest --test-dir build --output-on-failure
```
To build and run the engine, menu, and brush integration tests without raylib or a display:
```sh
cmake -S . -B build-tests -DBUILD_APP=OFF
cmake --build build-tests --parallel
ctest --test-dir build-tests --output-on-failure
```

