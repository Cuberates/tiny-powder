#ifndef POWDER_WORLD_RENDERER_IMPLEMENTATION
#define POWDER_WORLD_RENDERER_IMPLEMENTATION

#include "WorldRenderer.hpp"

int WorldRenderer::getRenderWidth() const {
  return render_width_;
}

int WorldRenderer::getRenderHeight() const {
  return render_height_;
}

WorldRenderer::WorldRenderer(int renderWidth, int renderHeight, std::shared_ptr<World> world) : 
  render_width_ { renderWidth }, 
  render_height_ { renderHeight }, 
  world { world } {
  scale_width_ = render_width_ / (world->getCellSize() * world->getNumCols()); 
  scale_height_ = render_height_ / (world->getCellSize() * world->getNumRows());
}

int WorldRenderer::getScaleHeight() const { return scale_height_; }
int WorldRenderer::getScaleWidth() const { return scale_width_; }

void WorldRenderer::render() const {
  int cellWidth = world->getCellSize() * scale_width_; 
  int cellHeight = world->getCellSize() * scale_height_;
  for (int row = 0; row < world->getNumRows(); ++row) { 
    for (int col = 0; col < world->getNumCols(); ++col) {   
      const Particle& particle = world->getParticle(row, col);
      Color color = particleColors[particle];
      int x = col * cellWidth; 
      int y = row * cellHeight; 
      DrawRectangle(x, y, cellWidth, cellHeight, color);
    }
  }
}
#endif 
