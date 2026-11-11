#include "raylib.h"
#include "src/World.hpp"
#include "src/WorldRenderer.hpp"
#include <memory>

const int WINDOW_WIDTH { 1280 }; 
const int WINDOW_HEIGHT { 720 }; 
const int WINDOW_FPS { 120 };

int main(void) {
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "powder-engine");
  SetTargetFPS(WINDOW_FPS);
  
  WorldPtr world = std::make_shared<World>(World(100, 100, 5));   
  WorldRenderer worldRenderer { WINDOW_WIDTH, WINDOW_HEIGHT, world }; 

  for (int row = 0; row < world->getNumRows(); row++) { 
    for (int col = 0; col < world->getNumCols(); col++) { 
      world->setParticle(row, col, Particle::SAND); 
    }
  }

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    worldRenderer.render();    
    EndDrawing();
  }

  CloseWindow();
}
