#include "Particle.hpp"
#include <cstring> //strcmp
#include <iostream>
#include <stdexcept>

namespace kape {
int Particle::GetNParticleType()
{
  return fParticleType.size();
}

Particle::Particle(const char* name, double px, double py, double pz)
    : fPx{px}
    , fPy{py}
    , fPz{pz}
{
  fIndex = FindParticle(name);
  // not found
  if (fIndex == fParticleType.size()) {
    std::cout << name << " is not a defined type of particle\n";
    throw std::runtime_error{"it is not a defined type of particle. Check "
                             "terminal output for the name."};
  }
}

void Particle::AddParticleType(const char* name, double mass, int charge,
                               double width)
{
  int index = FindParticle(name);
  if (index == GetNParticleType()) // it's a new Particle Type
  {
    if (width == 0.) { // it's a ParticleType
      fParticleType.push_back(new ParticleType(name, mass, charge));
    } else {
      fParticleType.push_back(new ResonanceType(name, mass, charge, width));
    }
  } else // we're updating a Particle Type
  {
    delete fParticleType[index];
    if (width == 0.) { // it's a ParticleType
      fParticleType[index] = new ParticleType(name, mass, charge);
    } else {
      fParticleType[index] = new ResonanceType(name, mass, charge, width);
    }
  }
}

int Particle::GetIndex() const
{
  return fIndex;
}

void Particle::SetIndex(int index)
{
  if (index >= GetNParticleType() || index < 0) {
    std::cout << "it is not a defined type of particle\n";
    throw std::runtime_error{"it is not a defined type of particle."};
  }

  fIndex = index;
}
void Particle::SetIndex(const char* name)
{
  SetIndex(FindParticle(name));
}

int Particle::FindParticle(const char* name)
{
  int i{0};
  for (; i != fParticleType.size(); ++i) {
    if (std::strcmp(fParticleType[i]->GetName(), name) == 0) {
      break;
    }
  }
  return i;
}

} // namespace kape