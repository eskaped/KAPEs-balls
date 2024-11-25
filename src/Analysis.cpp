#include "TF1.h"
#include "TFile.h"
#include <iostream>
#include <string>
std::string ExpectedWithError(int nTot, double probability)
{
  return std::string{std::to_string(nTot * probability) + " ± " + std::to_string(std::sqrt((1. - probability) * (nTot)*probability))};
}

enum ParticlesIndexesAnalysis
{
  PI_PLUS = 0,
  PI_MINUS,
  K_PLUS,
  K_MINUS,
  P_PLUS,
  P_MINUS,
  K_STAR
};
void Analysis()
{
  TFile* file = new TFile("histo.root");

  TH1F* hParticleTypes              = (TH1F*)file->Get("hParticleTypes");
  TH1F* hPhi                        = (TH1F*)file->Get("hPhi");
  TH1F* hTheta                      = (TH1F*)file->Get("hTheta");
  TH1F* hP                          = (TH1F*)file->Get("hP");
  TH1F* hPTrasverse                 = (TH1F*)file->Get("hPTrasverse");
  TH1F* hEnergy                     = (TH1F*)file->Get("hEnergy");
  TH1F* hInvariantMass              = (TH1F*)file->Get("hInvariantMass");
  TH1F* hInvariantMassDiscordant    = (TH1F*)file->Get("hInvariantMassDiscordant");
  TH1F* hInvariantMassConcordant    = (TH1F*)file->Get("hInvariantMassConcordant");
  TH1F* hInvariantMassDiscordantPiK = (TH1F*)file->Get("hInvariantMassDiscordantPiK");
  TH1F* hInvariantMassConcordantPiK = (TH1F*)file->Get("hInvariantMassConcordantPiK");
  TH1F* hInvariantMassDecayed       = (TH1F*)file->Get("hInvariantMassDecayed");

  TCanvas* invariantMassCanvas = new TCanvas("invariantMassCanvas", "invariantMassCanvas", 0, 0, 800, 600);
  hInvariantMass->Draw();
  TCanvas* invariantMassDiscordantCanvas = new TCanvas("invariantMassDiscordantCanvas", "invariantMassDiscordantCanvas", 0, 0, 800, 600);
  hInvariantMassDiscordant->Draw();
  TCanvas* invariantMassConcordantCanvas = new TCanvas("invariantMassConcordantCanvas", "invariantMassConcordantCanvas", 0, 0, 800, 600);
  hInvariantMassConcordant->Draw();


  // check histo entries
  if (hParticleTypes->GetEntries() != 1e7)
    std::cout << "hParticleTypes has the wrong number of entries \n";
  if (hPhi->GetEntries() != 1e7)
    std::cout << "hPhi has the wrong number of entries \n";
  if (hTheta->GetEntries() != 1e7)
    std::cout << "hTheta has the wrong number of entries \n";
  if (hP->GetEntries() != 1e7)
    std::cout << "hP has the wrong number of entries \n";
  if (hPTrasverse->GetEntries() != 1e7)
    std::cout << "hPTrasverse has the wrong number of entries \n";
  if (hEnergy->GetEntries() != 1e7)
    std::cout << "hEnergy has the wrong number of entries \n";

  // particle types proportions
  std::cout << "check that the expected number of particles generated for each type "
               "corresponds to the number of generated particles of that type:\n";
  std::cout << "|Particle Type\t| Expected\t| Generated\t|\n";
  std::cout << "-----------------------------------------------------\n";
  std::cout << "|pi+\t\t|" << ExpectedWithError(1e7, 0.400) << "\t\t|" << hParticleTypes->GetBinContent(PI_PLUS + 1) << " ± " << hParticleTypes->GetBinError(PI_PLUS + 1) << "\t|\n";
  std::cout << "|pi-\t\t|" << ExpectedWithError(1e7, 0.400) << "\t\t|" << hParticleTypes->GetBinContent(PI_MINUS + 1) << " ± " << hParticleTypes->GetBinError(PI_MINUS + 1) << "\t|\n";
  std::cout << "|K+ \t\t|" << ExpectedWithError(1e7, 0.050) << "\t\t|" << hParticleTypes->GetBinContent(K_PLUS + 1) << " ± " << hParticleTypes->GetBinError(K_PLUS + 1) << "\t|\n";
  std::cout << "|K- \t\t|" << ExpectedWithError(1e7, 0.050) << "\t\t|" << hParticleTypes->GetBinContent(K_MINUS + 1) << " ± " << hParticleTypes->GetBinError(K_MINUS + 1) << "\t|\n";
  std::cout << "|p+ \t\t|" << ExpectedWithError(1e7, 0.045) << "\t\t|" << hParticleTypes->GetBinContent(P_PLUS + 1) << " ± " << hParticleTypes->GetBinError(P_PLUS + 1) << "\t|\n";
  std::cout << "|p- \t\t|" << ExpectedWithError(1e7, 0.045) << "\t\t|" << hParticleTypes->GetBinContent(P_PLUS + 1) << " ± " << hParticleTypes->GetBinError(P_PLUS + 1) << "\t|\n";
  std::cout << "|K* \t\t|" << ExpectedWithError(1e7, 0.010) << "\t\t|" << hParticleTypes->GetBinContent(K_STAR + 1) << " ± " << hParticleTypes->GetBinError(K_STAR + 1) << "\t|\n";

  // check phi
  TCanvas* phiCanvas = new TCanvas("phiCanvas", "phiCanvas", 0, 0, 800, 600);

  TF1* phiDistr = new TF1("phiDistr", "[0]", 0, 2 * TMath::Pi());
  hPhi->Fit(phiDistr);
  std::cout << "Phi Distribution Fit: y = A\n";
  std::cout << "Parameter A: " << phiDistr->GetParameter(0) << " ± " << phiDistr->GetParError(0) << "\n";
  std::cout << "Reduced Chi Square: " << phiDistr->GetChisquare()/phiDistr->GetNDF()<< "\n";;

  //bohh
  std::cout << "Chi Square Probability: " << phiDistr->GetProb() << "\n";;


  // check theta
  TCanvas* thetaCanvas = new TCanvas("thetaCanvas", "thetaCanvas", 0, 0, 800, 600);

  TF1* thetaDistr = new TF1("thetaDistr", "[0]", 0, TMath::Pi());
  hTheta->Fit(thetaDistr);
  std::cout << "Theta Distribution Fit: y = A\n";
  std::cout << "Parameter A: " << thetaDistr->GetParameter(0) << " ± " << thetaDistr->GetParError(0) << "\n";
  std::cout << "Reduced Chi Square: " << thetaDistr->GetChisquare()/thetaDistr->GetNDF()<< "\n";;

  //bohh
  std::cout << "Chi Square Probability: " << thetaDistr->GetProb() << "\n";;


  // check p
  TCanvas* pCanvas = new TCanvas("pCanvas", "pCanvas", 0, 0, 800, 600);

  TF1* pDistr = new TF1("pDistr", "[0]*TMath::Exp(-x/[1])", 0., 5.);
  pDistr->SetParameter(0, 500);
  pDistr->SetParameter(1, 1);
  hP->Fit(pDistr);
  std::cout << "P Distribution Fit: y = A*e^(-x/B)\n";
  std::cout << "Parameter A: " << pDistr->GetParameter(0) << " ± " << pDistr->GetParError(0) << "\n";
  std::cout << "Parameter B: " << pDistr->GetParameter(1) << " ± " << pDistr->GetParError(1) << "\n";
  std::cout << "Reduced Chi Square: " << pDistr->GetChisquare()/pDistr->GetNDF()<< "\n";;


  //bohh
  std::cout << "Chi Square Probability: " << pDistr->GetProb() << "\n";;

  TCanvas* diffMassCanvas = new TCanvas("diffMassCanvas", "diffMassCanvas", 0, 0, 800, 600);  
  diffMassCanvas->Divide(3,1);
  TH1F* hDiffMass = new TH1F(*hInvariantMassDiscordant);
  hDiffMass->Add(hInvariantMassConcordant, -1);

  diffMassCanvas->cd(1);
  hInvariantMassDiscordant->Draw();
  diffMassCanvas->cd(2);
  hInvariantMassConcordant->Draw();
  diffMassCanvas->cd(3);
  hDiffMass->Draw();

  TF1* diffMassDistr = new TF1("diffMassDistr", "gaus(0)", 0., 8.);
  hDiffMass->Fit(diffMassDistr);


  TCanvas* diffMassPiKCanvas = new TCanvas("diffMassPiKCanvas", "diffMassPiKCanvas", 0, 0, 800, 600);
  // diffMassPiKCanvas->Divide(3,1);
  TH1F* hDiffMassPiK = new TH1F(*hInvariantMassDiscordantPiK);
  hDiffMassPiK->Add(hInvariantMassConcordantPiK, -1);

  // diffMassPiKCanvas->cd(1);
  // hInvariantMassDiscordantPiK->Draw();
  // diffMassPiKCanvas->cd(2);
  // hInvariantMassConcordantPiK->Draw();
  // diffMassPiKCanvas->cd(3);
  hDiffMassPiK->Draw();

  TF1* DiffMassPiKDistr = new TF1("DiffMassPiKDistr", "gaus(0)", 0., 8.);
  hDiffMassPiK->Fit(DiffMassPiKDistr);




  TCanvas* invariantMassDecayedCanvas = new TCanvas("invariantMassDecayedCanvas", "invariantMassDecayedCanvas", 0, 0, 800, 600);
  hInvariantMassDecayed->Draw();

  //   hParticleTypes
  //   hPhi
  //   hTheta
  //   hP
  //   hPTrasverse
  //   hEnergy
  //   hInvariantMass
  //   hInvariantMassDiscordant
  //   hInvariantMassConcordant
  //   hInvariantMassDiscordantPiK
  //   hInvariantMassConcordantPiK
  //   hInvariantMassDecayed

  //   hParticleTypes->Draw();
  //   hPhi->Draw();
  //   hTheta->Draw();
  //   hP->Draw();
  //   hPTrasverse->Draw();
  //   hEnergy->Draw();
  //   hInvariantMass->Draw();
  //   hInvariantMassDiscordant->Draw();
  //   hInvariantMassConcordant->Draw();
  //   hInvariantMassDiscordantPiK->Draw();
  //   hInvariantMassConcordantPiK->Draw();
  //   hInvariantMassDecayed->Draw();

  //   file->Close();
}