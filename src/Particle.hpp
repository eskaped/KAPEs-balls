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
  static void PrintParticleType();

  Particle(const char* name = DEFAULT_NAME, double px = 0., double py = 0.,
           double pz = 0.);
  int Decay2body(Particle& dau1, Particle& dau2) const;
  int GetIndex() const;
  double GetPx() const;
  double GetPy() const;
  double GetPz() const;
  double GetMass() const;
  double GetEnergy() const;
  double GetCharge() const;
  void SetIndex(int index);
  void SetIndex(const char* name);
  void SetP(double px, double py, double pz);
  double InvMass(Particle const& p) const;

  void Print() const;

 private:
  static inline const char* DEFAULT_NAME{"DEFAULT_NAME"};
  static inline std::vector<ParticleType*> fParticleType{};
  //returns the index of the first particle type named "name"
  //if not found it returns the number of particle types
  static int FindParticle(const char* name);
  void Boost(double bx, double by, double bz);

  int fIndex;
  double fPx;
  double fPy;
  double fPz;
};

} // namespace kape

#endif