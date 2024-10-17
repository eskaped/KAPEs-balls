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

  Particle(const char* name, double px = 0., double py = 0., double pz = 0.);
  int GetIndex() const;
  double GetPx() const;
  double GetPy() const;
  double GetPz() const;
  double GetMass() const;
  void SetIndex(int index);
  void SetIndex(const char* name);
  void SetP(double px, double py, double pz);
  double CalculateE() const;
  double InvMass(Particle const& p) const;

  void Print() const;

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