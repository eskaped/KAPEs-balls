#include "Particle.hpp"
#include <cmath>
#include <cstring> //strcmp
#include <iostream>
#include <stdexcept>

namespace kape {
int Particle::GetNParticleType()
{
  return static_cast<int>(fParticleType.size());
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

void Particle::PrintParticleType()
{
  std::cout << "Particle types:\n";
  for (auto const& p : fParticleType) {
    p->Print();
  }
}

int Particle::GetIndex() const
{
  return fIndex;
}
double Particle::GetPx() const
{
  return fPx;
}
double Particle::GetPy() const
{
  return fPy;
}
double Particle::GetPz() const
{
  return fPz;
}

double Particle::GetMass() const
{
  return fParticleType[fIndex]->GetMass();
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

void Particle::SetP(double px, double py, double pz)
{
  fPx = px;
  fPy = py;
  fPz = pz;
}

double Particle::CalculateE() const
{
  double m{fParticleType[fIndex]->GetMass()};
  return std::sqrt(m * m + fPx * fPx + fPy * fPy + fPz * fPz);
}

double Particle::InvMass(Particle const& p) const
{
  return sqrt(std::pow(CalculateE() + p.CalculateE(), 2)
              - (std::pow(fPx + p.fPx, 2) + std::pow(fPy + p.fPy, 2)
                 + std::pow(fPz + p.fPz, 2)));
}

void Particle::Print() const
{
  std::cout << "Index: " << fIndex << '\n';
  std::cout << "Name: " << fParticleType[fIndex]->GetName() << '\n';
  std::cout << "P = (" << fPx << ", " << fPy << ", " << fPz << ") " << '\n';
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