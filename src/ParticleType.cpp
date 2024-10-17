#include "ParticleType.hpp"
#include <iostream>
#include <stdexcept>
namespace kape {
ParticleType::ParticleType(const char* name, double mass, int charge)
    : fName{name}
    , fMass{mass}
    , fCharge{charge}
{
  if (mass <= 0.) {
    throw std::invalid_argument{"mass can't be negative or null"};
  }

  if (name == nullptr) {
    throw std::invalid_argument{"name can't point to nullptr"};
  }
}

ParticleType::ParticleType()
    : ParticleType("", 1, 0)
{}

ParticleType::~ParticleType()
{}

const char* ParticleType::GetName() const
{
  return fName;
}

double ParticleType::GetMass() const
{
  return fMass;
}

int ParticleType::GetCharge() const
{
  return fCharge;
}

void ParticleType::Print() const
{
  std::cout << "Name:\t" << fName << '\n';
  std::cout << "Mass:\t" << fMass << '\n';
  std::cout << "Charge:\t" << fCharge << '\n';
}
} // namespace kape