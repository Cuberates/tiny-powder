#include "raylib.h"
#include "src/World.hpp"
#include "src/Physics.hpp"
#include "src/WorldRenderer.hpp"
#include <memory>

const int WINDOW_WIDTH { 1280 }; 
const int WINDOW_HEIGHT { 720 }; 
const int WINDOW_FPS { 1024 };

int main(void) {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "powder-engine");
  SetTargetFPS(WINDOW_FPS);
  
  WorldPtr world = std::make_shared<World>(WINDOW_WIDTH / 5, WINDOW_HEIGHT / 5, 5); 
  WorldRenderer worldRenderer { world };   
  PhysicsEngine physicsEngine { world };  

  while (!WindowShouldClose()) {
    Vector2 mousePosition = GetMousePosition();
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      int cellX = static_cast<int>(mousePosition.y) / 5;
      int cellY = static_cast<int>(mousePosition.x) / 5;
      if (cellX < worldRenderer.getRenderHeight() && cellY < worldRenderer.getRenderWidth() && cellX >= 0 && cellY >= 0) {
        // world->setParticle(cellX, cellY, Particle::SAND);
        for (int row = cellX - 20; row < cellX + 20 && row < worldRenderer.getRenderHeight(); row++) {
          if (row < 0) continue;  
          for (int col = cellY - 20; col < cellY + 20 && col < worldRenderer.getRenderWidth(); col++) {
            if (col < 0) continue;  
            // world->setParticle(row, col, Particle::SAND)
            int d = (row - cellX) * (row - cellX) + (col - cellY) * (col - cellY);
            if (d <= 400) {  // 20 * 20
              world->setParticle(row, col, Particle::SAND);
            } 
          }
        }
      } 
    }
    BeginDrawing();
    ClearBackground(BLACK);
    physicsEngine.update();
    worldRenderer.render();
    
    EndDrawing();
  }

  CloseWindow();
}
