#include <raylib.h>
#include <algorithm>
#include <cstdint>
#include <vector>
#include <map>

using i64 = std::int64_t;

const int screenWidth = 1280; 
const int screenHeight = 720; 
const int pixelSize = 4;
const int numRows = screenHeight / pixelSize;
const int numCols = screenWidth / pixelSize;
const int gameFps = 120; 
const int brushRadius = std::max(1, 20 / pixelSize);

enum class PAR_TYPE { EMPTY, SAND, BRICK, WATER };
std::map<PAR_TYPE, Color> colors = {
  {PAR_TYPE::EMPTY, BLACK}, 
  {PAR_TYPE::SAND, YELLOW}, 
  {PAR_TYPE::BRICK, RED}, 
  {PAR_TYPE::WATER, BLUE},
}; 

constexpr i64 MAX_PARTICLES { numRows * numCols };  
std::vector<PAR_TYPE> particles(MAX_PARTICLES, PAR_TYPE::EMPTY);
std::vector<bool> moved(MAX_PARTICLES, false);

void set(i64 row, i64 col, PAR_TYPE ptype) {
  particles[row * numCols + col] = ptype;
}

void spawn_ball(i64 centerRow, i64 centerCol, i64 radius, PAR_TYPE type) {
  if (radius < 0) {
    return;
  }
  const i64 firstRow = std::max<i64>(0, centerRow - radius);
  const i64 lastRow = std::min<i64>(numRows - 1, centerRow + radius);
  const i64 firstCol = std::max<i64>(0, centerCol - radius);
  const i64 lastCol = std::min<i64>(numCols - 1, centerCol + radius);
  for (i64 row = firstRow; row <= lastRow; ++row) {
    for (i64 col = firstCol; col <= lastCol; ++col) {
      const i64 rowOffset = row - centerRow;
      const i64 colOffset = col - centerCol;
      if (rowOffset * rowOffset + colOffset * colOffset <= radius * radius &&
          particles[row * numCols + col] == PAR_TYPE::EMPTY) {
        set(row, col, type);
      }
    }
  }
}

void spawn_at(int screenX, int screenY, PAR_TYPE type) {
  if (screenX < 0 || screenX >= screenWidth ||
      screenY < 0 || screenY >= screenHeight) {
    return;
  }
  spawn_ball(screenY / pixelSize, screenX / pixelSize, brushRadius, type);
}

void render() { 
  for (auto row = 0; row < numRows; row++) {
    for (auto col = 0; col < numCols; col++) {
      const auto type = particles[row * numCols + col];
      if (type != PAR_TYPE::EMPTY) {
        DrawRectangle(col * pixelSize, row * pixelSize, pixelSize, pixelSize, colors[type]);
      }
    }
  }
}

void init() { 
  std::fill(particles.begin(), particles.end(), PAR_TYPE::EMPTY);
  std::fill(moved.begin(), moved.end(), false);
}

namespace Render { 
  bool try_move(i64 row, i64 col, i64 nextRow, i64 nextCol,
                bool displaceWater = false) {
    if (nextRow < 0 || nextRow >= numRows || nextCol < 0 || nextCol >= numCols) {
      return false;
    }
    const i64 source = row * numCols + col;
    const i64 destination = nextRow * numCols + nextCol;
    const auto displaced = particles[destination];
    if (displaced != PAR_TYPE::EMPTY &&
        !(displaceWater && displaced == PAR_TYPE::WATER && !moved[destination])) {
      return false;
    }
    std::swap(particles[source], particles[destination]);
    moved[destination] = true;
    // Displaced water has already moved this frame; empty cells remain usable.
    moved[source] = displaced != PAR_TYPE::EMPTY;
    return true;
  }

  void render_sand(i64 row, i64 col) {
    const i64 nextRow = row + 1;
    if (nextRow >= numRows) {
      return;
    }
    // Try directly below, then down-left, then down-right.
    for (const i64 nextCol : {col, col - 1, col + 1}) {
      if (try_move(row, col, nextRow, nextCol, true)) {
        return;
      }
    }
  }

  void render_water(i64 row, i64 col) {
    for (const i64 nextCol : {col, col - 1, col + 1}) {
      if (try_move(row, col, row + 1, nextCol)) {
        return;
      }
    }
    // Water spreads sideways when it cannot fall, including along the floor.
    for (const i64 nextCol : {col - 1, col + 1}) {
      if (try_move(row, col, row, nextCol)) {
        return;
      }
    }
  }
}

void update() { 
  std::fill(moved.begin(), moved.end(), false);
  // Sand sinks first. Then water flows, skipping particles already moved or swapped.
  for (const auto type : {PAR_TYPE::SAND, PAR_TYPE::WATER}) {
    for (auto row = numRows - 1; row >= 0; row--) {
      for (auto col = 0; col < numCols; col++) {
        const i64 index = row * numCols + col;
        if (particles[index] == type && !moved[index]) {
          if (type == PAR_TYPE::SAND) {
            Render::render_sand(row, col);
          } else {
            Render::render_water(row, col);
          }
        }
      }
    }
  }
}

void checkerboard() { 
  for (auto row = 0; row < numRows; row++) {
    for (auto col = 0; col < numCols; col++) {
      if ((row + col) % 2 == 0) {
        set(row, col, PAR_TYPE::SAND);
      } else set(row, col, PAR_TYPE::EMPTY);
    }
  }
}

#ifndef AUTOMATON_TEST
int main(void) {
  InitWindow(screenWidth, screenHeight, "choco-cookies");
  SetTargetFPS(gameFps);

  init();
  while (!WindowShouldClose()) { 
    update();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      spawn_at(GetMouseX(), GetMouseY(), PAR_TYPE::SAND);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
      spawn_at(GetMouseX(), GetMouseY(), PAR_TYPE::WATER);
    }
    BeginDrawing(); 
    ClearBackground(BLACK);
    render(); 
    EndDrawing(); 
  }
  CloseWindow();
}
#endif
