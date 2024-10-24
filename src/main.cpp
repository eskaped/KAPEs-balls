R__LOAD_LIBRARY(ParticleType_cpp.so)
R__LOAD_LIBRARY(Particle_cpp.so)
R__LOAD_LIBRARY(ResonanceType_cpp.so)
#include "Particle.hpp"
#include "TH1.h"
#include "TRandom.h"
#include <array>
#include <cmath> //M_PI

enum ParticlesIndexes
{
  PI_PLUS = 0,
  PI_MINUS,
  K_PLUS,
  K_MINUS,
  P_PLUS,
  P_MINUS,
  K_STAR
};

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

  TH1F* hParticleTypes =
      new TH1F("hParticleTypes", "Generated particle types", 7, -0.5, 6.5);
  TH1F* hPhi   = new TH1F("hPhi", "Generated phi angles", 10e5, 0., 2. * M_PI);
  TH1F* hTheta = new TH1F("hTheta", "Generated theta angles", 10e5, 0., M_PI);
  TH1F* hP     = new TH1F("hP", "Generated p magnitudes", 10e5, 0., 5.);
  TH1F* hPTrasverse =
      new TH1F("hPTrasverse", "Generated p trasverses", 10e5, 0., 5.);
  TH1F* hEnergy =
      new TH1F("hEnergy", "Generated particle energies", 1000, 0., 6.);

  TH1F* hInvariantMass =
      new TH1F("hInvariantMass", "Generated invariant masses", 1000, 0., 12.);
  TH1F* hInvariantMassDiscordant =
      new TH1F("hInvariantMassDiscordant",
               "Generated discordant invariant masses", 1000, 0., 12.);
  TH1F* hInvariantMassConcordant =
      new TH1F("hInvariantMassConcordant",
               "Generated concordant invariant masses", 1000, 0., 12.);
  TH1F* hInvariantMassDiscordantPiK =
      new TH1F("hInvariantMassDiscordantPiK",
               "Generated discordant invariant masses pi/K", 1000, 0., 12.);
  TH1F* hInvariantMassConcordantPiK =
      new TH1F("hInvariantMassConcordantPiK",
               "Generated concordant invariant masses pi/K", 1000, 0., 12.);
  TH1F* hInvariantMassDecayed =
      new TH1F("hInvariantMassDecayed", "Generated decayed invariant masses",
               1000, 0., 12.);

  for (int eventIndex = 0; eventIndex != 10e5; ++eventIndex) {
    // index for the next free space where decayed particles can be placed
    int arrayEnd = 100;
    for (int arrayIndex = 0; arrayIndex < 100; ++arrayIndex) {
      double phi   = gRandom->Uniform(0., 2. * M_PI);
      double theta = gRandom->Uniform(0., M_PI);
      double p     = gRandom->Exp(1.);

      eventParticles[arrayIndex].SetP(p * std::sin(theta) * std::cos(phi),
                                      p * std::sin(theta) * std::sin(phi),
                                      p * std::cos(theta));

      double randomChoice = gRandom->Rndm();
      if (randomChoice < 0.40) {
        // pi+
        eventParticles[arrayIndex].SetIndex(PI_PLUS);
      } else if (randomChoice < 0.80) {
        // pi-
        eventParticles[arrayIndex].SetIndex(PI_MINUS);
      } else if (randomChoice < 0.85) {
        // k+
        eventParticles[arrayIndex].SetIndex(K_PLUS);
      } else if (randomChoice < 0.90) {
        // k-
        eventParticles[arrayIndex].SetIndex(K_MINUS);
      } else if (randomChoice < 0.945) {
        // p+
        eventParticles[arrayIndex].SetIndex(P_PLUS);
      } else if (randomChoice < 0.99) {
        // p-
        eventParticles[arrayIndex].SetIndex(P_MINUS);
      } else {
        // k*
        eventParticles[arrayIndex].SetIndex(K_STAR);

        eventParticles[arrayIndex].Decay2body(eventParticles[arrayEnd],
                                              eventParticles[arrayEnd + 1]);
        arrayEnd += 2;
      }

      hParticleTypes->Fill(eventParticles[arrayIndex].GetIndex());
      hPhi->Fill(phi);
      hTheta->Fill(theta);
      hP->Fill(p);
      hPTrasverse->Fill(
          std::sqrt(std::pow(eventParticles[arrayIndex].GetPx(), 2)
                    + std::pow(eventParticles[arrayIndex].GetPy(), 2)));
      hEnergy->Fill(eventParticles[arrayIndex].GetEnergy());
    }

    // inv masses between particles
    for (int i = 0; i != eventParticles.size() - 1; ++i) {
      for (int j = i + 1; j < eventParticles.size(); ++j) {
        double invMass = eventParticles[i].InvMass(eventParticles[j]);
        hInvariantMass->Fill(invMass);
        if (eventParticles[i].GetCharge() * eventParticles[j].GetCharge() < 0) {
          hInvariantMassDiscordant->Fill(invMass);
          // sum equals 3 if and only if the particles are a pi and k discordant
          // couple
          if ((eventParticles[i].GetIndex() + eventParticles[j].GetIndex())
              == 3) {
            hInvariantMassDiscordantPiK->Fill(invMass);
          }
        } else { // concordant
          hInvariantMassConcordant->Fill(invMass);
          // the 3 conditions are met if and only if the particles are a pi and
          // k concordant couple
          if ((eventParticles[i].GetIndex() != eventParticles[j].GetIndex())
              && ((eventParticles[i].GetIndex() + eventParticles[j].GetIndex())
                      % 2
                  == 0)
              && ((eventParticles[i].GetIndex() + eventParticles[j].GetIndex())
                  <= 4)) {
            hInvariantMassConcordantPiK->Fill(invMass);
          }
        }
      }
    }

    int i = 100;
    while(i<eventParticles.size())
    {
        hInvariantMassDecayed->Fill(eventParticles[i].InvMass(eventParticles[i+1]));
        i+=2;
    }
    
  }

  hParticleTypes->Draw();
  hPhi->Draw();
  //   hTheta->Draw();
  //   hP->Draw();
  //   hPTrasverse->Draw();
  //   hEnergy->Draw();
}