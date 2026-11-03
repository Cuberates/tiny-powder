#define AUTOMATON_TEST
#include "../automaton.cpp"

#include <iostream>
#include <utility>

using Cell = std::pair<i64, i64>;

int failures = 0;

void expect(bool condition, const char* message) {
  if (!condition) {
    std::cerr << "FAIL: " << message << '\n';
    ++failures;
  }
}

void check_sand_move(const char* name, Cell source,
                     const std::vector<Cell>& blockers, Cell destination) {
  init();
  set(source.first, source.second, PAR_TYPE::SAND);
  for (const auto& [row, col] : blockers) {
    set(row, col, PAR_TYPE::BRICK);
  }

  auto expected = particles;
  expected[source.first * numCols + source.second] = PAR_TYPE::EMPTY;
  expected[destination.first * numCols + destination.second] = PAR_TYPE::SAND;

  Render::render_sand(source.first, source.second);
  expect(particles == expected, name);
}

void test_grid_coordinates() {
  init();
  set(numRows - 1, numCols - 1, PAR_TYPE::BRICK);
  expect(particles.back() == PAR_TYPE::BRICK,
         "the final row and column address the final grid cell");
  init();
  expect(particles == std::vector<PAR_TYPE>(numRows * numCols, PAR_TYPE::EMPTY),
         "initialization clears the entire rectangular grid");
}

void test_movement_rules() {
  check_sand_move("sand falls straight down before trying either diagonal",
                  {2, 3}, {}, {3, 3});
  check_sand_move("sand prefers bottom left when both diagonals are empty",
                  {2, 3}, {{3, 3}}, {3, 2});
  check_sand_move("sand tries bottom right after down and bottom left",
                  {2, 3}, {{3, 3}, {3, 2}}, {3, 4});
  check_sand_move("sand stays when all three destinations are occupied",
                  {2, 3}, {{3, 2}, {3, 3}, {3, 4}}, {2, 3});
  check_sand_move("sand can move diagonally into column zero",
                  {2, 1}, {{3, 1}}, {3, 0});
  check_sand_move("sand at the left edge can move bottom right",
                  {2, 0}, {{3, 0}}, {3, 1});
  check_sand_move("blocked sand at the left edge stays in bounds",
                  {2, 0}, {{3, 0}, {3, 1}}, {2, 0});
  check_sand_move("sand at the right edge can move bottom left",
                  {2, numCols - 1}, {{3, numCols - 1}}, {3, numCols - 2});
  check_sand_move("blocked sand at the right edge stays in bounds",
                  {2, numCols - 1}, {{3, numCols - 2}, {3, numCols - 1}},
                  {2, numCols - 1});
  check_sand_move("sand stays at the bottom left corner",
                  {numRows - 1, 0}, {}, {numRows - 1, 0});
  check_sand_move("sand stays on the bottom row",
                  {numRows - 1, 3}, {}, {numRows - 1, 3});
  check_sand_move("sand stays at the bottom right corner",
                  {numRows - 1, numCols - 1}, {},
                  {numRows - 1, numCols - 1});
}

void test_one_step_per_update() {
  init();
  set(0, 2, PAR_TYPE::SAND);
  set(1, 2, PAR_TYPE::SAND);
  set(4, 8, PAR_TYPE::BRICK);

  auto expected = particles;
  expected[2] = PAR_TYPE::EMPTY;
  expected[2 * numCols + 2] = PAR_TYPE::SAND;

  update();
  expect(particles == expected,
         "each grain falls one row per update and bricks remain fixed");
}

void test_competing_particles() {
  init();
  for (i64 col = 0; col < numCols; ++col) {
    if (col != 5) {
      set(numRows - 1, col, PAR_TYPE::BRICK);
    }
  }
  set(numRows - 2, 4, PAR_TYPE::SAND);
  set(numRows - 2, 6, PAR_TYPE::SAND);

  update();

  i64 sandCount = 0;
  for (const auto particle : particles) {
    if (particle == PAR_TYPE::SAND) {
      ++sandCount;
    }
  }
  expect(sandCount == 2, "competing grains do not overwrite one another");
  expect(particles[(numRows - 1) * numCols + 5] == PAR_TYPE::SAND,
         "one grain fills the available space");
  for (i64 col = 0; col < numCols; ++col) {
    if (col != 5) {
      expect(particles[(numRows - 1) * numCols + col] == PAR_TYPE::BRICK,
             "sand does not overwrite the brick floor");
    }
  }
}

void check_water_move(const char* name, Cell source,
                      const std::vector<Cell>& blockers, Cell destination) {
  init();
  set(source.first, source.second, PAR_TYPE::WATER);
  for (const auto& [row, col] : blockers) {
    set(row, col, PAR_TYPE::BRICK);
  }
  auto expected = particles;
  expected[source.first * numCols + source.second] = PAR_TYPE::EMPTY;
  expected[destination.first * numCols + destination.second] = PAR_TYPE::WATER;

  Render::render_water(source.first, source.second);
  expect(particles == expected, name);
}

void test_water_movement() {
  check_water_move("water falls before trying diagonals or spreading sideways",
                   {2, 3}, {}, {3, 3});
  check_water_move("water tries bottom left before bottom right",
                   {2, 3}, {{3, 3}}, {3, 2});
  check_water_move("water tries bottom right before spreading sideways",
                   {2, 3}, {{3, 3}, {3, 2}}, {3, 4});
  check_water_move("water spreads left when downward movement is blocked",
                   {2, 3}, {{3, 2}, {3, 3}, {3, 4}}, {2, 2});
  check_water_move("water spreads right when down and left are blocked",
                   {2, 3}, {{3, 2}, {3, 3}, {3, 4}, {2, 2}}, {2, 4});
  check_water_move("enclosed water stays in place",
                   {2, 3}, {{3, 2}, {3, 3}, {3, 4}, {2, 2}, {2, 4}}, {2, 3});
  check_water_move("water at the bottom left can spread right",
                   {numRows - 1, 0}, {}, {numRows - 1, 1});
  check_water_move("water at the bottom right can spread left",
                   {numRows - 1, numCols - 1}, {}, {numRows - 1, numCols - 2});

  init();
  set(2, 3, PAR_TYPE::WATER);
  for (const Cell cell : {Cell{3, 2}, Cell{3, 3}, Cell{3, 4},
                          Cell{2, 2}, Cell{2, 4}}) {
    set(cell.first, cell.second, PAR_TYPE::SAND);
  }
  const auto expected = particles;
  Render::render_water(2, 3);
  expect(particles == expected, "water cannot displace sand");
}

void test_sand_water_swaps() {
  for (const i64 destinationCol : {i64{3}, i64{2}, i64{4}}) {
    init();
    set(2, 3, PAR_TYPE::SAND);
    set(3, destinationCol, PAR_TYPE::WATER);
    if (destinationCol != 3) {
      set(3, 3, PAR_TYPE::BRICK);
    }
    if (destinationCol == 4) {
      set(3, 2, PAR_TYPE::BRICK);
    }
    auto expected = particles;
    expected[2 * numCols + 3] = PAR_TYPE::WATER;
    expected[3 * numCols + destinationCol] = PAR_TYPE::SAND;

    Render::render_sand(2, 3);
    expect(particles == expected,
           "sand swaps with water in down-left-right priority without losing either particle");
  }
}

void test_water_moves_once_per_update() {
  init();
  set(numRows - 1, 0, PAR_TYPE::WATER);
  auto expected = particles;
  expected[(numRows - 1) * numCols] = PAR_TYPE::EMPTY;
  expected[(numRows - 1) * numCols + 1] = PAR_TYPE::WATER;
  update();
  expect(particles == expected, "water spreads only one column in a frame");

  init();
  set(numRows - 2, 3, PAR_TYPE::SAND);
  set(numRows - 1, 3, PAR_TYPE::WATER);
  expected = particles;
  expected[(numRows - 2) * numCols + 3] = PAR_TYPE::WATER;
  expected[(numRows - 1) * numCols + 3] = PAR_TYPE::SAND;
  update();
  expect(particles == expected,
         "displaced water stays directly above the sand for the rest of the frame");

  expected[(numRows - 2) * numCols + 3] = PAR_TYPE::EMPTY;
  expected[(numRows - 1) * numCols + 2] = PAR_TYPE::WATER;
  update();
  expect(particles == expected, "displaced water can move again on the next frame");

  init();
  set(numRows - 3, 3, PAR_TYPE::SAND);
  set(numRows - 2, 3, PAR_TYPE::SAND);
  set(numRows - 1, 3, PAR_TYPE::WATER);
  set(numRows - 2, 2, PAR_TYPE::BRICK);
  set(numRows - 2, 4, PAR_TYPE::BRICK);
  expected = particles;
  expected[(numRows - 2) * numCols + 3] = PAR_TYPE::WATER;
  expected[(numRows - 1) * numCols + 3] = PAR_TYPE::SAND;
  update();
  expect(particles == expected,
         "stacked sand cannot displace the same water upward twice in one frame");
}

void test_ball_shape_and_occupied_cells() {
  init();
  set(10, 20, PAR_TYPE::WATER);
  set(10, 21, PAR_TYPE::BRICK);
  auto expected = particles;
  const std::vector<Cell> disk = {
    {8, 20}, {9, 19}, {9, 20}, {9, 21},
    {10, 18}, {10, 19}, {10, 20}, {10, 21}, {10, 22},
    {11, 19}, {11, 20}, {11, 21}, {12, 20}
  };
  for (const auto& [row, col] : disk) {
    if (expected[row * numCols + col] == PAR_TYPE::EMPTY) {
      expected[row * numCols + col] = PAR_TYPE::SAND;
    }
  }
  spawn_ball(10, 20, 2, PAR_TYPE::SAND);
  expect(particles == expected,
         "the brush creates a filled circle while preserving water and bricks");

  init();
  expected = particles;
  for (const Cell cell : {Cell{0, 0}, Cell{0, 1}, Cell{0, 2},
                          Cell{1, 0}, Cell{1, 1}, Cell{2, 0}}) {
    expected[cell.first * numCols + cell.second] = PAR_TYPE::SAND;
    expected[(numRows - 1 - cell.first) * numCols + numCols - 1 - cell.second]
      = PAR_TYPE::WATER;
  }
  spawn_ball(0, 0, 2, PAR_TYPE::SAND);
  spawn_ball(numRows - 1, numCols - 1, 2, PAR_TYPE::WATER);
  expect(particles == expected, "brush circles clip safely at both grid corners");

  init();
  expected = particles;
  expected[4 * numCols + 6] = PAR_TYPE::SAND;
  spawn_ball(4, 6, 0, PAR_TYPE::SAND);
  spawn_ball(10, 20, -1, PAR_TYPE::SAND);
  expect(particles == expected,
         "zero-radius brushes create one particle and negative radii do nothing");
}

void test_mouse_coordinates() {
  init();
  const auto emptyGrid = particles;
  spawn_at(-1, 0, PAR_TYPE::SAND);
  spawn_at(0, -1, PAR_TYPE::SAND);
  spawn_at(screenWidth, 0, PAR_TYPE::SAND);
  spawn_at(0, screenHeight, PAR_TYPE::SAND);
  expect(particles == emptyGrid, "clicks outside the screen do not paint particles");

  const i64 row = numRows / 3;
  const i64 col = numCols / 2;
  spawn_at(col * pixelSize, row * pixelSize, PAR_TYPE::SAND);
  expect(particles[row * numCols + col] == PAR_TYPE::SAND,
         "mouse x maps to column and mouse y maps to row");
  expect(particles[(row - brushRadius) * numCols + col] == PAR_TYPE::SAND &&
         particles[(row + brushRadius) * numCols + col] == PAR_TYPE::SAND &&
         particles[row * numCols + col - brushRadius] == PAR_TYPE::SAND &&
         particles[row * numCols + col + brushRadius] == PAR_TYPE::SAND,
         "a click fills the full brush radius around its center");
  expect(particles[(row - brushRadius - 1) * numCols + col] == PAR_TYPE::EMPTY,
         "a click does not paint beyond the brush radius");
}

int main() {
  expect(numRows == screenHeight / pixelSize,
         "rows follow the screen height");
  expect(numCols == screenWidth / pixelSize,
         "columns follow the screen width");
  test_grid_coordinates();
  test_movement_rules();
  test_one_step_per_update();
  test_competing_particles();
  test_water_movement();
  test_sand_water_swaps();
  test_water_moves_once_per_update();
  test_ball_shape_and_occupied_cells();
  test_mouse_coordinates();
  if (failures != 0) {
    return 1;
  }
  std::cout << "All particle tests passed.\n";
  return 0;
}
