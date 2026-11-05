#ifndef TINY_POWDER_WORLD
#define TINY_POWDER_WORLD

#include <vector>
#include <memory>

enum class Particle : int { AIR = 0, SAND = 1, WATER = 2};

using Particles = std::vector<Particle>; 

class World { 
  private:
  int rows_, cols_, cell_size_; 
  Particles particles_;
  public:
  World(int cols, int rows, int cellSize = 10) : 
    rows_ { rows },
    cols_ { cols }, 
    cell_size_ { cellSize },
    particles_(rows * cols, Particle::AIR)
    {}
  int getCellSize() const; 
  int getNumRows () const; 
  int getNumCols () const; 
  Particles& getParticles(); 
  void setParticle(int row, int col, Particle particle); 
  Particle getParticle(int row, int col) const;   
};

using WorldPtr = std::shared_ptr<World>;

#endif
