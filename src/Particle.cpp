#include "Particle.hpp"
#include <cmath>
#include <cstdlib> //for RAND_MAX
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
  if (fIndex == static_cast<int>(fParticleType.size())
      && name != DEFAULT_NAME) {
    std::cout << name << " is not a defined type of particle\n";
    throw std::runtime_error{"it is not a defined type of particle. Check "
                             "terminal output for the name."};
  }
}

int Particle::Decay2body(Particle& dau1, Particle& dau2) const
{
  if (GetMass() == 0.0)
   {
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }


  double massMot  = GetMass();
  double massDau1 = dau1.GetMass();
  double massDau2 = dau2.GetMass();


  if (fIndex > -1) { // add width effect



    // gaussian random numbers

    float x1, x2, w, y1;

    double invnum = 1. / RAND_MAX;
    do {
      x1 = 2.0 * rand() * invnum - 1.0;
      x2 = 2.0 * rand() * invnum - 1.0;
      w  = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w  = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;

    massMot += fParticleType[fIndex]->GetWidth() * y1;

  //   std::cout<<"x1: "<<x1<<"\n";
  //   std::cout<<"x2: "<<x2<<"\n";
  //   std::cout<<"w: "<<w<<"\n";
  //   std::cout<<"y1: "<<y1<<"\n";
  //   std::cout<<"width: "<<fParticleType[fIndex]->GetWidth()<<'\n';
  // 
  }



  if (massMot < massDau1 + massDau2) {
    // std::cout<<"massMot: "<<massMot<<'\n';
    // std::cout<<"massDau1: "<<massDau1<<'\n';
    // std::cout<<"massDau2: "<<massDau2<<'\n';

    printf("Decayment cannot be preformed because mass is too low in this "
           "channel\n");
    return 2;
  }




  double pout =
      sqrt(
          (massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2))
          * (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2)))
      / massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi   = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.SetP(pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi),
            pout * cos(theta));
  dau2.SetP(-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi),
            -pout * cos(theta));

  double energy = sqrt(fPx * fPx + fPy * fPy + fPz * fPz + massMot * massMot);

  double bx = fPx / energy;
  double by = fPy / energy;
  double bz = fPz / energy;



  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);

  

  return 0;
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
double Particle::GetCharge() const
{
  return fParticleType[fIndex]->GetCharge();
}

void Particle::SetIndex(int index)
{
  if (index >= GetNParticleType() || index < 0) {
    std::cout << "it is not a defined type of particle\n";
    throw std::runtime_error{"it is not a defined type of particle."};
  }

  fIndex = index;
}

double Particle::GetEnergy() const
{
  double m{fParticleType[fIndex]->GetMass()};
  return std::sqrt(m * m + fPx * fPx + fPy * fPy + fPz * fPz);
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

double Particle::InvMass(Particle const& p) const
{
  return sqrt(std::pow(GetEnergy() + p.GetEnergy(), 2)
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
  for (; i != static_cast<int>(fParticleType.size()); ++i) {
    if (std::strcmp(fParticleType[i]->GetName(), name) == 0) {
      break;
    }
  }
  return i;
}

void Particle::Boost(double bx, double by, double bz)
{
  double energy = GetEnergy();

  // Boost this Lorentz vector
  double b2     = bx * bx + by * by + bz * bz;
  double gamma  = 1.0 / sqrt(1.0 - b2);
  double bp     = bx * fPx + by * fPy + bz * fPz;
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  fPx += gamma2 * bp * bx + gamma * bx * energy;
  fPy += gamma2 * bp * by + gamma * by * energy;
  fPz += gamma2 * bp * bz + gamma * bz * energy;
}
} // namespace kape