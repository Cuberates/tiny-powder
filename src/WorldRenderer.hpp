#ifndef POWDER_WORLD_RENDERER
#define POWDER_WORLD_RENDERER

/**
 * @brief Renders only the visual representation of the world and does not modify its state. It handles Raylib rendering of the world
 * @attention Do not modify the world state within the class. If you need to change some representation, modify its corrosponding Physics engine.
 */

#include "raylib.h"
#include "World.hpp"
#include <map>
#include <vector> 

inline std::map<Particle, Color> particleColors = { 
  {Particle::SAND, YELLOW},
  {Particle::WATER, BLUE},
  {Particle::AIR, BLACK}, 
};

class WorldRenderer {
  private: 
  int render_width_, render_height_; 
  std::shared_ptr<World> world;
  public:
  WorldRenderer(std::shared_ptr<World> world);
  int getRenderWidth() const;
  int getRenderHeight() const;  
  void render() const; 
}; 
#endif
