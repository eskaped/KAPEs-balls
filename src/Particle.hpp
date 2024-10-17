#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "ParticleType.hpp"
#include "ResonanceType.hpp"

#include <vector>
namespace kape {
class Particle
{
 public:
  static int GetNParticleType();
  static void AddParticleType(const char* name, double mass, int charge,
                              double width = 0);

  Particle(const char* name, double px = 0., double py = 0., double pz = 0.);
  int GetIndex() const;
  void SetIndex(int index);
  void SetIndex(const char* name);

 private:
  static std::vector<ParticleType*> fParticleType;
  static int FindParticle(const char* name);

  int fIndex;
  double fPx;
  double fPy;
  double fPz;
};

} // namespace kape

#endif