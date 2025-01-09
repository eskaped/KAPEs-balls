#ifndef RESONANCE_TYPE_HPP
#define RESONANCE_TYPE_HPP
#include "ParticleType.hpp"

namespace kape {
class ResonanceType : public ParticleType
{
 public:
  explicit ResonanceType(const char* name, double mass, int charge,
                         double width);
  explicit ResonanceType();

  virtual double GetWidth() const override;
  void Print() const override;

 private:
  const double fWidth;
};
} // namespace kape

#endif