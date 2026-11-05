#ifndef POWDER_WORLD_RENDERER_IMPLEMENTATION
#define POWDER_WORLD_RENDERER_IMPLEMENTATION

#include "WorldRenderer.hpp"

int WorldRenderer::getRenderWidth() const {
  return render_width_;
}

int WorldRenderer::getRenderHeight() const {
  return render_height_;
}

WorldRenderer::WorldRenderer(std::shared_ptr<World> world) : world(world) {
  render_width_ = world->getNumCols() * world->getCellSize();
  render_height_ = world->getNumRows() * world->getCellSize();
}

void WorldRenderer::render() const {
  for (int row = 0; row < world->getNumRows(); ++row) { 
    for (int col = 0; col < world->getNumCols(); ++col) {
      Particle particle = world->getParticle(row, col);
      Color color = particleColors[particle];
      int cellSize = world->getCellSize(); 
      int x = col * cellSize; 
      int y = row * cellSize; 
      DrawRectangle(x, y, cellSize, cellSize, color);
    }
  }
}
#endif 
