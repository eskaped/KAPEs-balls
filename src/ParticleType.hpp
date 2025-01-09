#ifndef PARTICLE_TYPE_HPP
#define PARTICLE_TYPE_HPP

namespace kape {
class ParticleType
{
 public:
  explicit ParticleType(const char* name, double mass, int charge);
  explicit ParticleType();
  virtual ~ParticleType();

  const char* GetName() const;
  double GetMass() const;
  int GetCharge() const;
  
  virtual double GetWidth() const;
  virtual void Print() const;

 private:
  const char* fName;
  const double fMass;
  const int fCharge;
};
} // namespace kape

#endif