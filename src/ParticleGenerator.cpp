#include "Particle.hpp"
#include "TFile.h"
#include "TH1.h"
#include "TRandom.h"
#include "TRandom3.h"
#include <array>
#include <cmath> //M_PI
#include <iostream>
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

void kapef()
{
  kape::Particle::AddParticleType("pi+", 0.13957, +1);      // pione +
  kape::Particle::AddParticleType("pi-", 0.13957, -1);      // pione -
  kape::Particle::AddParticleType("K+", 0.49367, +1);       // kaone +
  kape::Particle::AddParticleType("K-", 0.49367, -1);       // kaone -
  kape::Particle::AddParticleType("p+", 0.93827, +1);       // protone +
  kape::Particle::AddParticleType("p-", 0.93827, -1);       // protone -
  kape::Particle::AddParticleType("K*", 0.89166, 0, 0.050); // K*

  delete gRandom;
  gRandom = new TRandom3();
  gRandom->SetSeed(136279841);

  // chose 300 because all particles could be a k* and decay in two more
  // particles
  std::array<kape::Particle, 300> eventParticles;

  TH1F* hParticleTypes = new TH1F("hParticleTypes", "Generated particle12 types", 7, -0.5, 6.5);
  TH1F* hPhi           = new TH1F("hPhi", "Generated phi angles", 1e5, 0., 2. * M_PI);
  TH1F* hTheta         = new TH1F("hTheta", "Generated theta angles", 1e5, 0., M_PI);
  TH1F* hP             = new TH1F("hP", "Generated p magnitudes", 1e5, 0., 5.);
  TH1F* hPTrasverse    = new TH1F("hPTrasverse", "Generated p trasverses", 1e5, 0., 5.);
  TH1F* hEnergy        = new TH1F("hEnergy", "Generated particle energies", 1000, 0., 6.);

  TH1F* hInvariantMass              = new TH1F("hInvariantMass", "Generated invariant masses", 1e5, 0., 8.);
  TH1F* hInvariantMassDiscordant    = new TH1F("hInvariantMassDiscordant", "Generated discordant invariant masses", 1e4, 0., 8.);
  TH1F* hInvariantMassConcordant    = new TH1F("hInvariantMassConcordant", "Generated concordant invariant masses", 1e4, 0., 8.);
  TH1F* hInvariantMassDiscordantPiK = new TH1F("hInvariantMassDiscordantPiK", "Generated discordant invariant masses pi/K", 1e4, 0., 8.);
  TH1F* hInvariantMassConcordantPiK = new TH1F("hInvariantMassConcordantPiK", "Generated concordant invariant masses pi/K", 1e4, 0., 8.);
  TH1F* hInvariantMassDecayed       = new TH1F("hInvariantMassDecayed", "Generated decayed invariant masses", 100, 0., 8.);

  hParticleTypes->Sumw2();
  hPhi->Sumw2();
  hTheta->Sumw2();
  hP->Sumw2();
  hPTrasverse->Sumw2();
  hEnergy->Sumw2();
  hInvariantMass->Sumw2();
  hInvariantMassDiscordant->Sumw2();
  hInvariantMassConcordant->Sumw2();
  hInvariantMassDiscordantPiK->Sumw2();
  hInvariantMassConcordantPiK->Sumw2();
  hInvariantMassDecayed->Sumw2();

  for (int eventIndex = 0; eventIndex != 1e5; ++eventIndex) {
    // index for the next free space where decayed particles can be placed
    int arrayEnd = 100;
    for (int arrayIndex = 0; arrayIndex < 100; ++arrayIndex) {
      double phi   = gRandom->Uniform(0., 2. * M_PI);
      double theta = gRandom->Uniform(0., M_PI);
      double p     = gRandom->Exp(1.);

      eventParticles[arrayIndex].SetP(p * std::sin(theta) * std::cos(phi), p * std::sin(theta) * std::sin(phi), p * std::cos(theta));

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

        // choose decayed particle types randomly
        if (gRandom->Rndm() <= 0.5) {
          eventParticles[arrayEnd].SetIndex(PI_PLUS);
          eventParticles[arrayEnd + 1].SetIndex(K_MINUS);
        } else {
          eventParticles[arrayEnd].SetIndex(PI_MINUS);
          eventParticles[arrayEnd + 1].SetIndex(K_PLUS);
        }

        int error = eventParticles[arrayIndex].Decay2body(eventParticles[arrayEnd], eventParticles[arrayEnd + 1]);
        if (error != 0) {
          std::cout << "decayed to body failed with error " << error << '\n';
          throw std::runtime_error{"decayed to body failed, check terminal for more info.\n"};
        }

        arrayEnd += 2;
      }

      hParticleTypes->Fill(eventParticles[arrayIndex].GetIndex());
      hPhi->Fill(phi);
      hTheta->Fill(theta);
      hP->Fill(p);
      hPTrasverse->Fill(std::sqrt(std::pow(eventParticles[arrayIndex].GetPx(), 2) + std::pow(eventParticles[arrayIndex].GetPy(), 2)));
      hEnergy->Fill(eventParticles[arrayIndex].GetEnergy());
    }

    // inv masses between particles
    for (int i = 0; i != arrayEnd - 1; ++i) {
      if (eventParticles[i].GetIndex() == K_STAR) { // ignore k_star
        continue;
      }
      for (int j = i + 1; j != arrayEnd; ++j) {
        if (eventParticles[j].GetIndex() == K_STAR) { // ignore k_star
          continue;
        }

        double invMass = eventParticles[i].InvMass(eventParticles[j]);
        hInvariantMass->Fill(invMass);

        int i_type = eventParticles[i].GetIndex();
        int j_type = eventParticles[j].GetIndex();

        if (eventParticles[i].GetCharge() * eventParticles[j].GetCharge() < 0) {
          hInvariantMassDiscordant->Fill(invMass);

          if ((i_type == PI_PLUS && j_type == K_MINUS) || (i_type == PI_MINUS && j_type == K_PLUS) || (j_type == PI_PLUS && i_type == K_MINUS) || (j_type == PI_MINUS && i_type == K_PLUS)) {
            hInvariantMassDiscordantPiK->Fill(invMass);
          }
        } else { // concordant
          hInvariantMassConcordant->Fill(invMass);
          if ((i_type == PI_PLUS && j_type == K_PLUS) || (i_type == PI_MINUS && j_type == K_MINUS) || (j_type == PI_PLUS && i_type == K_PLUS) || (j_type == PI_MINUS && i_type == K_MINUS)) {
            hInvariantMassConcordantPiK->Fill(invMass);
          }
        }
      }
    }

    int i = 100;
    while (i < arrayEnd) {
      hInvariantMassDecayed->Fill(eventParticles[i].InvMass(eventParticles[i + 1]));
      i += 2;
    }
  }


  TFile* file = new TFile("histo.root", "RECREATE");

  hParticleTypes->Write();
  hPhi->Write();
  hTheta->Write();
  hP->Write();
  hPTrasverse->Write();
  hEnergy->Write();
  hInvariantMass->Write();
  hInvariantMassDiscordant->Write();
  hInvariantMassConcordant->Write();
  hInvariantMassDiscordantPiK->Write();
  hInvariantMassConcordantPiK->Write();
  hInvariantMassDecayed->Write();

  file->Close();
}