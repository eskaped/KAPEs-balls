#include "Particle.hpp"
#include "TRandom.h"
#include <array>

int main()
{
  kape::Particle::AddParticleType("pi+", 0.13957, +1);      // pione +
  kape::Particle::AddParticleType("pi-", 0.13957, -1);      // pione -
  kape::Particle::AddParticleType("K+", 0.49367, +1);       // kaone +
  kape::Particle::AddParticleType("K-", 0.49367, -1);       // kaone -
  kape::Particle::AddParticleType("p+", 0.93827, +1);       // protone +
  kape::Particle::AddParticleType("p-", 0.93827, -1);       // protone -
  kape::Particle::AddParticleType("K*", 0.89166, 0, 0.050); // K*

  gRandom->SetSeed(136279841);

  // chose 300 because all particles could be a k* and decay in two more
  // particles
  std::array<kape::Particle, 300> eventParticles;

  for (int eventIndex = 0; eventIndex != 10e5; ++eventIndex) {
    // index for the next free space where decayed particles can be placed
    int arrayEnd = 100;
    for (int arrayIndex = 0; arrayIndex < 100; ++arrayIndex) {
      double phi   = gRandom->Uniform(0., 2. * TMath::Pi);
      double theta = gRandom->Uniform(0., TMath::Pi);
      double p     = gRandom->Exp(1.);

      eventParticles[arrayIndex].SetP(p * std::sin(theta) * std::cos(phi),
                                      p * std::sin(theta) * std::sin(phi),
                                      p * std::cos(theta));

      double randomChoice = gRandom->Rndm();
      if (randomChoice < 0.40) {
        // pi+
        eventParticles[arrayIndex].SetIndex(0);
      } else if (randomChoice < 0.80) {
        // pi-
        eventParticles[arrayIndex].SetIndex(1);
      } else if (randomChoice < 0.85) {
        // k+
        eventParticles[arrayIndex].SetIndex(2);
      } else if (randomChoice < 0.90) {
        // k-
        eventParticles[arrayIndex].SetIndex(3);
      } else if (randomChoice < 0.945) {
        // p+
        eventParticles[arrayIndex].SetIndex(4);
      } else if (randomChoice < 0.99) {
        // p-
        eventParticles[arrayIndex].SetIndex(5);
      } else {
        // k*
        eventParticles[arrayIndex].SetIndex(6);

        eventParticles[arrayIndex].Decay2body(eventParticles[arrayEnd],
                                              eventParticles[arrayEnd + 1]);
        arrayEnd += 2;
      }
    }
  }
}