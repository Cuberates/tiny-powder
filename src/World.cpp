#ifndef TINY_POWDER_WORLD_IMPLEMENTATION
#define TINY_POWDER_WORLD_IMPLEMENTATION

#include "World.hpp"

int World::getNumRows() const {
  return rows_;
}
int World::getNumCols() const {
  return cols_; 
}
int World::getCellSize() const {
  return cell_size_;
} 
void World::setParticle(int row, int col, Particle particle) {
  particles_[row * cols_ + col] = particle;
}
std::vector<Particle>& World::getParticles() {
  return particles_;
}
Particle World::getParticle(int row, int col) const {
  return particles_[row * cols_ + col];
}
#endif
