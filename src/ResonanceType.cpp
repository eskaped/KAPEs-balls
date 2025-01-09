#include "ResonanceType.hpp"
#include <iostream>
#include <stdexcept>

namespace kape {
ResonanceType::ResonanceType(const char* name, double mass, int charge,
                             double width)
    : ParticleType(name, mass, charge)
    , fWidth{width}
{
  if (width <= 0) {
    throw std::invalid_argument{"width can't be negative or null"};
  }
}

ResonanceType::ResonanceType()
    : ParticleType()
    , fWidth{}
{}

double ResonanceType::GetWidth() const
{
  return fWidth;
}

void ResonanceType::Print() const
{
  ParticleType::Print();
  std::cout << "Width:\t" << fWidth << '\n';
}
} // namespace kape
