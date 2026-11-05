#ifndef TINY_POWDER_PHYSICS
#define TINY_POWDER_PHYSICS 

#include "World.hpp"
#include <memory>

class PhysicsEngine { 
  private:
  WorldPtr world_;
  public:
  PhysicsEngine(WorldPtr world);
  void update();
}; 

using PhysicsEnginePtr = std::shared_ptr<PhysicsEngine>;

#endif
