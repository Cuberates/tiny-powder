#include "raylib.h"
#include <memory>

const int WINDOW_WIDTH { 1280 }; 
const int WINDOW_HEIGHT { 720 }; 
const int WINDOW_FPS { 120 };

int main(void) {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "powder-engine");
  SetTargetFPS(WINDOW_FPS);
  
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
  }

  CloseWindow();
}
