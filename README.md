# Automaton
A 2D falling-particle simulation built with C++23 and raylib. Spawn balls of sand and water, watch them fall and spread, and see sand sink through water.
## Build and run
From the repository root:
```sh
cmake -S . -B build
cmake --build build --parallel
./build/automaton
```
## Tests
Tests are enabled by default for native builds. After building, run:
```sh
ctest --test-dir build --output-on-failure
```
## Project layout
| Path | Contents |
| --- | --- |
| [automaton.cpp](automaton.cpp) | Particle grid, simulation rules, drawing, and mouse input. |
| [CMakeLists.txt](CMakeLists.txt) | Application build, raylib dependency, and CTest setup. |
| [tests/ParticlesTest.cpp](tests/ParticlesTest.cpp) | Automated simulation and brush tests. |
| `resources/textures/` | Bundled texture assets; the current renderer uses colored rectangles. |
The bundled textures are Kenney's 1-Bit Platformer Pack 1.1, distributed under CC0. See the [asset license](resources/textures/License.txt) for details.
