#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TStyle.h"
#include <iostream>
#include <string>

std::string ExpectedWithError(Int_t nTot, Double_t probability)
{
  return std::string{
      std::to_string(nTot * probability) + " ± "
      + std::to_string(std::sqrt((1. - probability) * (nTot)*probability))};
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

  // read the data from file
  TH1F* hParticleTypes           = (TH1F*)file->Get("hParticleTypes");
  TH1F* hPhi                     = (TH1F*)file->Get("hPhi");
  TH1F* hTheta                   = (TH1F*)file->Get("hTheta");
  TH1F* hP                       = (TH1F*)file->Get("hP");
  TH1F* hPTrasverse              = (TH1F*)file->Get("hPTrasverse");
  TH1F* hEnergy                  = (TH1F*)file->Get("hEnergy");
  TH1F* hInvariantMass           = (TH1F*)file->Get("hInvariantMass");
  TH1F* hInvariantMassDiscordant = (TH1F*)file->Get("hInvariantMassDiscordant");
  TH1F* hInvariantMassConcordant = (TH1F*)file->Get("hInvariantMassConcordant");
  TH1F* hInvariantMassDiscordantPiK =
      (TH1F*)file->Get("hInvariantMassDiscordantPiK");
  TH1F* hInvariantMassConcordantPiK =
      (TH1F*)file->Get("hInvariantMassConcordantPiK");
  TH1F* hInvariantMassDecayed = (TH1F*)file->Get("hInvariantMassDecayed");

  // rebinning
  hPhi->Rebin(5);
  hTheta->Rebin(5);
  hP->Rebin(5);
  hInvariantMassDiscordant->Rebin(20);
  hInvariantMassConcordant->Rebin(20);
  hInvariantMassDiscordantPiK->Rebin(10);
  hInvariantMassConcordantPiK->Rebin(10);

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
  std::cout
      << "check that the expected number of particles generated for each type "
         "corresponds to the number of generated particles of that type within "
         "errors:\n";
  std::cout << "|Particle Type\t| Expected\t\t\t\t| Generated\t\t|\n";
  std::cout << "---------------------------------------------------------------"
               "------------------\n";
  std::cout << "|pi+\t\t|" << ExpectedWithError(1e7, 0.400) << "\t\t|"
            << hParticleTypes->GetBinContent(PI_PLUS + 1) << " ± "
            << hParticleTypes->GetBinError(PI_PLUS + 1) << "\t|\n";
  std::cout << "|pi-\t\t|" << ExpectedWithError(1e7, 0.400) << "\t\t|"
            << hParticleTypes->GetBinContent(PI_MINUS + 1) << " ± "
            << hParticleTypes->GetBinError(PI_MINUS + 1) << "\t|\n";
  std::cout << "|K+ \t\t|" << ExpectedWithError(1e7, 0.050) << "\t\t|"
            << hParticleTypes->GetBinContent(K_PLUS + 1) << " ± "
            << hParticleTypes->GetBinError(K_PLUS + 1) << "\t|\n";
  std::cout << "|K- \t\t|" << ExpectedWithError(1e7, 0.050) << "\t\t|"
            << hParticleTypes->GetBinContent(K_MINUS + 1) << " ± "
            << hParticleTypes->GetBinError(K_MINUS + 1) << "\t|\n";
  std::cout << "|p+ \t\t|" << ExpectedWithError(1e7, 0.045) << "\t\t|"
            << hParticleTypes->GetBinContent(P_PLUS + 1) << " ± "
            << hParticleTypes->GetBinError(P_PLUS + 1) << "\t|\n";
  std::cout << "|p- \t\t|" << ExpectedWithError(1e7, 0.045) << "\t\t|"
            << hParticleTypes->GetBinContent(P_MINUS + 1) << " ± "
            << hParticleTypes->GetBinError(P_MINUS + 1) << "\t|\n";
  std::cout << "|K* \t\t|" << ExpectedWithError(1e7, 0.010) << "\t\t|"
            << hParticleTypes->GetBinContent(K_STAR + 1) << " ± "
            << hParticleTypes->GetBinError(K_STAR + 1) << "\t|\n";

  // add all parameters to the output in the figures
  gStyle->SetOptStat(11);
  gStyle->SetOptFit(1111);

  // Figure 1: particle types, p, phi,
  // theta----------------------------------------
  TCanvas* Figure1 = new TCanvas("Figure1", "Figure1", 0, 0, 800, 600);
  Figure1->Divide(2, 2);

  // particle types-------------------------------------------------------
  Figure1->cd(1);

  // normalize
  hParticleTypes->Scale(1. / hParticleTypes->Integral(), "width");

  // fitting
  TF1* particleTypesDistr = new TF1("particleTypesDistr",
                                    "[0]*(x<0.5) + "
                                    "[1]*(0.5<x && x<1.5) +"
                                    "[2]*(1.5<x && x<2.5) + "
                                    "[3]*(2.5<x && x<3.5) + "
                                    "[4]*(3.5<x && x<4.5) + "
                                    "[5]*(4.5<x && x<5.5) + "
                                    "[6]*(5.5<x && x<6.5)",
                                    -0.5, 6.5);

  hParticleTypes->Fit(particleTypesDistr);

  // fit output
  std::cout << "\nParticle Types Distribution Fit: \n"
               "y = \t A if (x<0.5) \n"
               "\t B if (0.5<x and x<1.5)\n "
               "\t C if (1.5<x and x<2.5)\n "
               "\t D if (2.5<x and x<3.5)\n "
               "\t E if (3.5<x and x<4.5)\n "
               "\t F if (4.5<x and x<5.5)\n "
               "\t G if (5.5<x and x<6.5)\n";

  std::cout << "Parameter A: " << particleTypesDistr->GetParameter(0) << " ± "
            << particleTypesDistr->GetParError(0) << '\n';
  std::cout << "Parameter B: " << particleTypesDistr->GetParameter(1) << " ± "
            << particleTypesDistr->GetParError(1) << '\n';
  std::cout << "Parameter C: " << particleTypesDistr->GetParameter(2) << " ± "
            << particleTypesDistr->GetParError(2) << '\n';
  std::cout << "Parameter D: " << particleTypesDistr->GetParameter(3) << " ± "
            << particleTypesDistr->GetParError(3) << '\n';
  std::cout << "Parameter E: " << particleTypesDistr->GetParameter(4) << " ± "
            << particleTypesDistr->GetParError(4) << '\n';
  std::cout << "Parameter F: " << particleTypesDistr->GetParameter(5) << " ± "
            << particleTypesDistr->GetParError(5) << '\n';
  std::cout << "Parameter G: " << particleTypesDistr->GetParameter(6) << " ± "
            << particleTypesDistr->GetParError(6) << '\n';

  std::cout << "Reduced Chi Square: "
            << particleTypesDistr->GetChisquare() / particleTypesDistr->GetNDF()
            << "\n";
  std::cout << "Chi Square Probability: " << particleTypesDistr->GetProb()
            << "\n\n";

  // graphics
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
  hParticleTypes->SetFillColor(kAzure - 2);
  hParticleTypes->SetLineColor(kAzure - 2);
  hParticleTypes->SetBarWidth(0.2);
  hParticleTypes->SetBarOffset(0.8);
  // if sumw2 is set to true the histogram doesn't get filled in
  hParticleTypes->Sumw2(kFALSE);
  hParticleTypes->Draw("b same");

  // p------------------------------------------------------------------
  Figure1->cd(2);

  // normalize
  hP->Scale(1. / hP->Integral(), "width");

  // fitting
  TF1* pDistr = new TF1("pDistr", "TMath::Exp(-x/[0])", 0., 5.);
  pDistr->SetParameter(0, 1);
  pDistr->SetParameter(1, 1);
  hP->Fit(pDistr);

  // fit output
  std::cout << "\nP Distribution Fit: y = e^(-x/A)\n";

  std::cout << "Parameter A: " << pDistr->GetParameter(0) << " ± "
            << pDistr->GetParError(0) << "\n";

  std::cout << "Reduced Chi Square: "
            << pDistr->GetChisquare() / pDistr->GetNDF() << "\n";
  std::cout << "Chi Square Probability: " << pDistr->GetProb() << "\n\n";

  // graphics
  hP->SetTitle("Impulse magnitudes' distribution");
  hP->GetXaxis()->SetTitle("Impulse magnitude");
  hP->GetYaxis()->SetTitle("Probability of impulse magnitude");
  hP->GetXaxis()->SetTitleOffset(1.2);
  hP->SetLineColor(kAzure - 2);
  hP->Draw();

  // phi------------------------------------------------------------------
  Figure1->cd(3);

  // normalize
  hPhi->Scale(1. / hPhi->Integral(), "width");

  // fitting
  TF1* phiDistr = new TF1("phiDistr", "[0]", 0, 2 * TMath::Pi());

  hPhi->Fit(phiDistr);

  // fit output
  std::cout << "\nPhi Distribution Fit: y = A\n";
  std::cout << "Parameter A: " << phiDistr->GetParameter(0) << " ± "
            << phiDistr->GetParError(0) << "\n";

  std::cout << "Reduced Chi Square: "
            << phiDistr->GetChisquare() / phiDistr->GetNDF() << "\n";
  std::cout << "Chi Square Probability: " << phiDistr->GetProb() << "\n\n";

  // graphics
  hPhi->SetTitle("Azimuthal angles' distribution");
  hPhi->GetXaxis()->SetTitle("Azimuthal angle");
  hPhi->GetYaxis()->SetTitle("Probability of azimuthal angle");
  hPhi->GetXaxis()->SetTitleOffset(1.2);
  hPhi->SetLineColor(kAzure - 2);
  hPhi->Draw();

  // theta------------------------------------------------------------------
  Figure1->cd(4);

  // normalize
  hTheta->Scale(1. / hTheta->Integral(), "width");

  // fitting
  TF1* thetaDistr = new TF1("thetaDistr", "[0]", 0, TMath::Pi());
  hTheta->Fit(thetaDistr);

  // fit output
  std::cout << "\nTheta Distribution Fit: y = A\n";
  std::cout << "Parameter A: " << thetaDistr->GetParameter(0) << " ± "
            << thetaDistr->GetParError(0) << "\n";

  std::cout << "Reduced Chi Square: "
            << thetaDistr->GetChisquare() / thetaDistr->GetNDF() << "\n";
  std::cout << "Chi Square Probability: " << thetaDistr->GetProb() << "\n\n";

  // graphics
  hTheta->SetTitle("Polar angles' distribution");
  hTheta->GetXaxis()->SetTitle("Polar angle");
  hTheta->GetYaxis()->SetTitle("Probability of polar angle");
  hTheta->GetXaxis()->SetTitleOffset(1.2);
  hTheta->SetFillColor(kAzure - 2);
  hTheta->SetLineColor(kAzure - 2);
  hTheta->Draw();

  // Figure 2: invariant masses graphs ---------------------------------------
  TCanvas* Figure2 = new TCanvas("Figure2", "Figure2", 0, 0, 800, 600);
  Figure2->Divide(3, 1);

  // Only K*--------------------------------------------------------------
  Figure2->cd(1);

  // fitting
  TF1* invariantMassDecayedDistr =
      new TF1("invariantMassDecayedDistr", "gaus(0)", 0., 8.);
  hInvariantMassDecayed->Fit(invariantMassDecayedDistr);

  // fit output
  std::cout << "\nK* Invariant mass fit: y = A*exp(-0.5*((x-M)/D)**2)\n";
  std::cout << "Parameter A: " << invariantMassDecayedDistr->GetParameter(0)
            << " ± " << invariantMassDecayedDistr->GetParError(0) << "\n";
  std::cout << "Parameter M: " << invariantMassDecayedDistr->GetParameter(1)
            << " ± " << invariantMassDecayedDistr->GetParError(1) << "\n";
  std::cout << "Parameter D: " << invariantMassDecayedDistr->GetParameter(2)
            << " ± " << invariantMassDecayedDistr->GetParError(2) << "\n";
  std::cout << "Reduced Chi Square: "
            << invariantMassDecayedDistr->GetChisquare()
                   / invariantMassDecayedDistr->GetNDF()
            << "\n";
  std::cout << "Chi Square Probability: "
            << invariantMassDecayedDistr->GetProb() << "\n\n";

  // graphics
  hInvariantMassDecayed->SetTitle("K* invariant masses");
  hInvariantMassDecayed->GetXaxis()->SetTitle("Invariant mass");
  hInvariantMassDecayed->GetYaxis()->SetTitle("Entries");
  hInvariantMassDecayed->SetMarkerStyle(kFullSquare);
  hInvariantMassDecayed->SetMarkerSize(0.5f);
  hInvariantMassDecayed->Draw();

  // Difference of discordant particles' invariant masses----------
  // subtract
  TH1F* hDiffMass = new TH1F(*hInvariantMassDiscordant);
  hDiffMass->Add(hInvariantMassConcordant, -1);

  Figure2->cd(2);

  // fitting
  TF1* diffMassDistr = new TF1("diffMassDistr", "gaus(0)", 0., 8.);
  hDiffMass->Fit(diffMassDistr);

  // fit output
  std::cout
      << "\nK* discordant invariant mass fit: y = A*exp(-0.5*((x-M)/D)**2)\n";
  std::cout << "Parameter A: " << diffMassDistr->GetParameter(0) << " ± "
            << diffMassDistr->GetParError(0) << "\n";
  std::cout << "Parameter M: " << diffMassDistr->GetParameter(1) << " ± "
            << diffMassDistr->GetParError(1) << "\n";
  std::cout << "Parameter D: " << diffMassDistr->GetParameter(2) << " ± "
            << diffMassDistr->GetParError(2) << "\n";
  std::cout << "Reduced Chi Square: "
            << diffMassDistr->GetChisquare() / diffMassDistr->GetNDF() << "\n";
  std::cout << "Chi Square Probability: " << diffMassDistr->GetProb() << "\n\n";

  // graphics
  hDiffMass->SetTitle("Discordant particles' invariant masses");
  hDiffMass->GetXaxis()->SetTitle("Invariant mass");
  hDiffMass->GetYaxis()->SetTitle("Entries");
  hDiffMass->SetLineColor(kAzure - 2);
  hDiffMass->Draw();

  // Difference of discordant PiK particles' invariant masses-------
  // subtract
  TH1F* hDiffMassPiK = new TH1F(*hInvariantMassDiscordantPiK);
  hDiffMassPiK->Add(hInvariantMassConcordantPiK, -1);

  Figure2->cd(3);

  // fitting
  TF1* diffMassPiKDistr = new TF1("diffMassPiKDistr", "gaus(0)", 0., 8.);
  hDiffMassPiK->Fit(diffMassPiKDistr);

  // fit output
  std::cout << "\nK* discordant PiK invariant mass fit: y = "
               "A*exp(-0.5*((x-M)/D)**2)\n";
  std::cout << "Parameter A: " << diffMassPiKDistr->GetParameter(0) << " ± "
            << diffMassPiKDistr->GetParError(0) << "\n";
  std::cout << "Parameter M: " << diffMassPiKDistr->GetParameter(1) << " ± "
            << diffMassPiKDistr->GetParError(1) << "\n";
  std::cout << "Parameter D: " << diffMassPiKDistr->GetParameter(2) << " ± "
            << diffMassPiKDistr->GetParError(2) << "\n";
  std::cout << "Reduced Chi Square: "
            << diffMassPiKDistr->GetChisquare() / diffMassPiKDistr->GetNDF()
            << "\n";
  std::cout << "Chi Square Probability: " << diffMassPiKDistr->GetProb()
            << "\n\n";

  // graphics
  hDiffMassPiK->SetTitle("#pi - k invariant masses");
  hDiffMassPiK->GetXaxis()->SetTitle("Invariant mass");
  hDiffMassPiK->GetYaxis()->SetTitle("Entries");
  hDiffMassPiK->SetLineColor(kAzure - 2);
  hDiffMassPiK->Draw();
}