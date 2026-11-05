#ifndef TINY_POWDER_PHYSICS_IMPLEMENTATION  
#define TINY_POWDER_PHYSICS_IMPLEMENTATION

#include "World.hpp"  
#include "Physics.hpp"

PhysicsEngine::PhysicsEngine(WorldPtr world) : world_ { world } {}

void PhysicsEngine::update() {  
  // Scan upward so a particle moved down is not processed again this update.
  for (int row = world_->getNumRows() - 1; row >= 0; row--) { 
    for (int col = 0; col < world_->getNumCols(); col++) {
      const Particle& particle = world_->getParticle(row, col);
      if (particle == Particle::SAND) {
        if (row + 1 < world_->getNumRows() && world_->getParticle(row + 1, col) == Particle::AIR) {
          world_->setParticle(row + 1, col, Particle::SAND);
          world_->setParticle(row, col, Particle::AIR);
        } else if (row + 1 < world_->getNumRows() && col - 1 >= 0 && world_->getParticle(row + 1, col - 1) == Particle::AIR) {
          world_->setParticle(row + 1, col - 1, Particle::SAND);
          world_->setParticle(row, col, Particle::AIR);
        } else if (row + 1 < world_->getNumRows() && col + 1 < world_->getNumCols() && world_->getParticle(row + 1, col + 1) == Particle::AIR) {
          world_->setParticle(row + 1, col + 1, Particle::SAND);
          world_->setParticle(row, col, Particle::AIR);
        }
      }
    }
  }
}
#endif

