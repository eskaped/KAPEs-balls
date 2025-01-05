#include "TF1.h"
#include "TH1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
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

  // hParticleTypes
  hPhi->Rebin(5);
  hTheta->Rebin(5);
  hP->Rebin(5);

  // hPTrasverse->Rebin(5);
  // hEnergy->Rebin(5);
  hInvariantMassDiscordant->Rebin(20);
  hInvariantMassConcordant->Rebin(20);
  hInvariantMassDiscordantPiK->Rebin(10);
  hInvariantMassConcordantPiK->Rebin(10);
  // TCanvas* invariantMassCanvas = new TCanvas("invariantMassCanvas", "invariantMassCanvas", 0, 0, 800, 600);
  // hInvariantMass->Draw();
  // TCanvas* invariantMassDiscordantCanvas = new TCanvas("invariantMassDiscordantCanvas", "invariantMassDiscordantCanvas", 0, 0, 800, 600);
  // hInvariantMassDiscordant->Draw();
  // TCanvas* invariantMassConcordantCanvas = new TCanvas("invariantMassConcordantCanvas", "invariantMassConcordantCanvas", 0, 0, 800, 600);
  // hInvariantMassConcordant->Draw();

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


  //Figure 1: particle types, p, angles
  TCanvas* Figure1 = new TCanvas("Figure1", "Figure1", 0, 0, 800, 600);
  Figure1->Divide(2, 2);

  gStyle->SetOptStat(11);
  gStyle->SetOptFit(1111);


  // particle types
  Figure1->cd(1);

  //normalize
  hParticleTypes->Scale(1. / hParticleTypes->Integral(), "width");
  TF1* particleTypesDistr = new TF1("particleTypesDistr", "[0]*(x<0.5) + [1]*(0.5<x && x<1.5) + [2]*(1.5<x && x<2.5) + [3]*(2.5<x && x<3.5) + [4]*(3.5<x && x<4.5) + [5]*(4.5<x && x<5.5) + [6]*(5.5<x && x<6.5)", -0.5, 6.5);
  hParticleTypes->Fit(particleTypesDistr);
  std::cout << "Particle Types Distribution Fit: y = A*e^(-x/B)\n"; //**************************** */
  // std::cout << "Parameter A: " << pDistr->GetParameter(0) << " ± " << pDistr->GetParError(0) << "\n";
  // std::cout << "Parameter B: " << pDistr->GetParameter(1) << " ± " << pDistr->GetParError(1) << "\n";
  std::cout << "Reduced Chi Square: " << particleTypesDistr->GetChisquare() / particleTypesDistr->GetNDF() << "\n";

  hParticleTypes->SetTitle("Particle types' distribution");
  hParticleTypes->GetXaxis()->SetBinLabel(1, "#pi+");
  hParticleTypes->GetXaxis()->SetBinLabel(2, "#pi-");
  hParticleTypes->GetXaxis()->SetBinLabel(3, "K+");
  hParticleTypes->GetXaxis()->SetBinLabel(4, "K-");
  hParticleTypes->GetXaxis()->SetBinLabel(5, "p+");
  hParticleTypes->GetXaxis()->SetBinLabel(6, "p-");
  hParticleTypes->GetXaxis()->SetBinLabel(7, "K*");
  hParticleTypes->GetXaxis()->SetLabelSize(0.065);
  hParticleTypes->GetXaxis()->SetTitleOffset(1.2);
  hParticleTypes->GetXaxis()->SetTitle("Particle type");
  hParticleTypes->GetYaxis()->SetTitle("Probability of particle type");
  hParticleTypes->SetFillColor(kAzure-2);
  hParticleTypes->SetLineColor(kAzure-2);
  hParticleTypes->SetBarWidth(1);

  hParticleTypes->Draw("b");

  // check p
  Figure1->cd(2);

  hP->Scale(1. / hP->Integral(), "width");

  TF1* pDistr = new TF1("pDistr", "[0]*TMath::Exp(-x/[1])", 0., 5.);
  pDistr->SetParameter(0, 1);
  pDistr->SetParameter(1, 1);
  hP->Fit(pDistr);
  std::cout << "P Distribution Fit: y = A*e^(-x/B)\n";
  std::cout << "Parameter A: " << pDistr->GetParameter(0) << " ± " << pDistr->GetParError(0) << "\n";
  std::cout << "Parameter B: " << pDistr->GetParameter(1) << " ± " << pDistr->GetParError(1) << "\n";
  std::cout << "Reduced Chi Square: " << pDistr->GetChisquare() / pDistr->GetNDF() << "\n";


  // bohh
  std::cout << "Chi Square Probability: " << pDistr->GetProb() << "\n";

  hP->SetTitle("Impulse magnitudes' distribution");
  hP->GetXaxis()->SetTitle("Impulse magnitude");
  hP->GetYaxis()->SetTitle("Probability of impulse magnitude");
  hP->GetXaxis()->SetTitleOffset(1.2);
  hP->SetFillColor(kAzure-2);
  hP->SetLineColor(kAzure-2);
  hP->Draw();


  // check phi
  Figure1->cd(3);
  hPhi->Scale(1. / hPhi->Integral(), "width");


  TF1* phiDistr = new TF1("phiDistr", "[0]", 0, 2 * TMath::Pi());
  hPhi->Fit(phiDistr);
  std::cout << "Phi Distribution Fit: y = A\n";
  std::cout << "Parameter A: " << phiDistr->GetParameter(0) << " ± " << phiDistr->GetParError(0) << "\n";
  std::cout << "Reduced Chi Square: " << phiDistr->GetChisquare() / phiDistr->GetNDF() << "\n";

  // bohh
  std::cout << "Chi Square Probability: " << phiDistr->GetProb() << "\n";

  hPhi->SetTitle("Azimuthal angles' distribution");
  hPhi->GetXaxis()->SetTitle("Azimuthal angle");
  hPhi->GetYaxis()->SetTitle("Probability of azimuthal angle");
  hPhi->GetXaxis()->SetTitleOffset(1.2);
  hPhi->SetFillColor(kAzure-2);
  hPhi->SetLineColor(kAzure-2);
  hPhi->Draw();

  // check theta
  Figure1->cd(4);
  hTheta->Scale(1. / hTheta->Integral(), "width");


  TF1* thetaDistr = new TF1("thetaDistr", "[0]", 0, TMath::Pi());
  hTheta->Fit(thetaDistr);
  std::cout << "Theta Distribution Fit: y = A\n";
  std::cout << "Parameter A: " << thetaDistr->GetParameter(0) << " ± " << thetaDistr->GetParError(0) << "\n";
  std::cout << "Reduced Chi Square: " << thetaDistr->GetChisquare() / thetaDistr->GetNDF() << "\n";

  // bohh
  std::cout << "Chi Square Probability: " << thetaDistr->GetProb() << "\n";

  hTheta->SetTitle("Polar angles' distribution");
  hTheta->GetXaxis()->SetTitle("Polar angle");
  hTheta->GetYaxis()->SetTitle("Probability of polar angle");
  hTheta->GetXaxis()->SetTitleOffset(1.2);
  hTheta->SetFillColor(kAzure-2);
  hTheta->SetLineColor(kAzure-2);
  hTheta->Draw();

  //Figure 2: diff gaussians

  TCanvas* Figure2 = new TCanvas("Figure2", "Figure2", 0, 0, 800, 600);
  Figure2->Divide(3, 1);

  //True K*
  Figure2->cd(1);
  hInvariantMassDecayed->SetTitle("K* invariant masses");
  hInvariantMassDecayed->GetXaxis()->SetTitle("Invariant mass");
  hInvariantMassDecayed->GetYaxis()->SetTitle("Occurrences");
  hInvariantMassDecayed->SetMarkerStyle(kOpenSquare);
  hInvariantMassDecayed->SetMarkerSize(0.4f);
  hInvariantMassDecayed->Draw();
  TF1* invariantMassDecayedDistr = new TF1("invariantMassDecayedDistr", "gaus(0)", 0., 8.);
  hInvariantMassDecayed->Fit(invariantMassDecayedDistr);
 

  //Difference all
  TH1F* hDiffMass = new TH1F(*hInvariantMassDiscordant);
  hDiffMass->Add(hInvariantMassConcordant, -1);

  Figure2->cd(2);
  hDiffMass->SetTitle("Discordant particles' invariant masses");
  hDiffMass->GetXaxis()->SetTitle("Invariant mass");
  hDiffMass->GetYaxis()->SetTitle("Occurrences");
  hDiffMass->SetLineColor(kAzure-2);
  hDiffMass->SetFillColor(kAzure-2);
  hDiffMass->Draw();

  TF1* diffMassDistr = new TF1("diffMassDistr", "gaus(0)", 0., 8.);
  hDiffMass->Fit(diffMassDistr);

  //Difference PiK
  TH1F* hDiffMassPiK = new TH1F(*hInvariantMassDiscordantPiK);
  hDiffMassPiK->Add(hInvariantMassConcordantPiK, -1);

  Figure2->cd(3);
  hDiffMassPiK->SetTitle("#pi - k invariant masses");
  hDiffMassPiK->GetXaxis()->SetTitle("Invariant mass");
  hDiffMassPiK->GetYaxis()->SetTitle("Occurrences");
  hDiffMassPiK->SetLineColor(kAzure-2);
  hDiffMassPiK->SetFillColor(kAzure-2);
  hDiffMassPiK->Draw();

  TF1* diffMassPiKDistr = new TF1("diffMassPiKDistr", "gaus(0)", 0., 8.);
  hDiffMassPiK->Fit(diffMassPiKDistr);


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