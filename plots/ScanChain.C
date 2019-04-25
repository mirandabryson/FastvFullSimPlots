// -*- C++ -*-
// Usage:
// > root -b -q doAll.C

#include <iostream>
#include <vector>

// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TCanvas.h"
#include "TLegend.h"

// StopCMS3
#include "StopCMS3.cc"

using namespace std;
using namespace stoptas;

int ScanChain(TChain* chain, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // Example Histograms
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");

  TH1F *hpfmet = new TH1F("hpfmet", "pfmet", 40,0,800);
  hpfmet->SetDirectory(rootdir);
  //TH1F *hmht = new TH1F("hmht", "mht", 40, 0, 800);
  //hmht->SetDirectory(rootdir);
  //TH1F *hv2met = new TH1F("hv2met", "v2met", 40, 0, 800);
  //hv2met->SetDirectory(rootdir);
  TH1F *hmt_met_lep = new TH1F("hmt_met_lep", "mt_met_lep", 40, 0, 800);
  hmt_met_lep->SetDirectory(rootdir);
  //TH1F *hlepkinpt = new TH1F("hlepkinpt", "lepkinpt", 40, 0, 800);
  //hlepkinpt->SetDirectory(rootdir);
  //TH1F *hlepkineta = new TH1F("hlepkineta", "lepkineta", 40, 0, 800);
  TH1F *hngoodjets = new TH1F("hngoodjets", "ngoodjets", 10, 0, 10);
  hngoodjets->SetDirectory(rootdir);
  //TH1F *hpt = new TH1F("hpt", "pt", 40, 0, 800);
  //hpt->SetDirectory(rootdir);
  //TH1F *heta - new TH1F("heta", "eta", 40, 0, 800);
  //heta->SetDirectory(rootdir);
  TH1F *hlep1_dphiMET = new TH1F("hlep1_dphiMET", "lep1_dphiMET", 5, 0, 5);
  hlep1_dphiMET->SetDirectory(rootdir);
  TH1F *hmindphi_met_j1_j2 = new TH1F("hmindphi_met_j1_j2", "mindphi_met_j1_j2", 40, 0, 800);
  hmindphi_met_j1_j2->SetDirectory(rootdir);
  //TH1F *hMT2 = new TH1F("hMT2", "MT2", 40, 0, 800);
  //hMT2->SetDirectory(rootdir);
  //TH1F *hMCT = new TH1F("hMCT", "MCT", 40, 0, 800);
  //hMCT->SetDirectory(rootdir);
  TH1F *htopnessMod = new TH1F("htopnessMod", "topnessMod", 30, -10, 20);
  htopnessMod->SetDirectory(rootdir);

  //Defining fastsim histos
  TH1F *hpfmet_fastsim = new TH1F("hpfmet_fastsim", "pfmet", 40,0,800);
  hpfmet_fastsim->SetDirectory(rootdir);
  //TH1F *hmht_fastsim = new TH1F("hmht_fastsim", "mht", 40, 0, 800);
  //hmht_fastsim->SetDirectory(rootdir);
  //TH1F *hv2met_fastsim = new TH1F("hv2met_fastsim", "v2met", 40, 0, 800);
  //hv2met_fastsim->SetDirectory(rootdir);
  TH1F *hmt_met_lep_fastsim = new TH1F("hmt_met_lep_fastsim", "mt_met_lep", 40, 0, 800);
  hmt_met_lep_fastsim->SetDirectory(rootdir);
  //TH1F *hlepkinpt_fastsim = new TH1F("hlepkinpt_fastsim", "lepkinpt", 40, 0, 800);
  //hlepkinpt_fastsim->SetDirectory(rootdir);
  //TH1F *hlepkineta_fastsim = new TH1F("hlepkineta_fastsim", "lepkineta", 40, 0, 800);
  TH1F *hngoodjets_fastsim = new TH1F("hngoodjets_fastsim", "ngoodjets", 10, 0, 10);
  hngoodjets_fastsim->SetDirectory(rootdir);
  //TH1F *hpt_fastsim = new TH1F("hpt_fastsim", "pt", 40, 0, 800);
  //hpt_fastsim->SetDirectory(rootdir);
  //TH1F *heta_fastsim - new TH1F("heta_fastsim", "eta", 40, 0, 800);
  //heta_fastsim->SetDirectory(rootdir);
  TH1F *hlep1_dphiMET_fastsim = new TH1F("hlep1_dphiMET_fastsim", "lep1_dphiMET", 40, 0, 100);
  hlep1_dphiMET_fastsim->SetDirectory(rootdir);
  TH1F *hmindphi_met_j1_j2_fastsim = new TH1F("hmindphi_met_j1_j2_fastsim", "hmindphi_met_j1_j2", 40, 0, 800);
  hmindphi_met_j1_j2_fastsim->SetDirectory(rootdir);
  //TH1F *hMT2_fastsim = new TH1F("hMT2_fastsim", "MT2", 40, 0, 800);
  //hMT2_fastsim->SetDirectory(rootdir);
  //TH1F *hMCT_fastsim = new TH1F("hMCT_fastsim", "MCT", 40, 0, 800);
  //hMCT_fastsim->SetDirectory(rootdir);
  TH1F *htopnessMod_fastsim = new TH1F("htopnessMod_fastsim", "topnessMod", 30, -10, 20);
  htopnessMod_fastsim->SetDirectory(rootdir);

  //Canvas definition
  TCanvas *c0 = new TCanvas("c0","c0",800,800);

  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  if (nEvents >= 0) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile file(currentFile->GetTitle());
    TTree *tree = (TTree*)file.Get("t");
    if (fast) TTreeCache::SetLearnEntries(10);
    if (fast) tree->SetCacheSize(128*1024*1024);
    stopcms3.Init(tree);

    string fileName = file.GetName();
    string fileCut = "fastsim";
    
    // Loop over Events in current file
    if (nEventsTotal >= nEventsChain) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();
    for (unsigned int event = 0; event < nEventsTree; ++event) {

      // Get Event Content
      if (nEventsTotal >= nEventsChain) continue;
      if (fast) tree->LoadTree(event);
      stopcms3.GetEntry(event);
      ++nEventsTotal;

      // Progress
      StopCMS3::progress( nEventsTotal, nEventsChain );

      // Analysis Code
      // this is where we put cuts like pfmet>150 : if(pfmet()<150) continue;

      if(fileName.find(fileCut) != string::npos){
	//if "fastsim" is in the file title, fill only the fastsim histos
    
	//Fill Histograms
	hpfmet_fastsim->Fill(pfmet());
	//hmht_fastsim->Fill(mht());
	//hv2met_fastsim->Fill(v2met();
	hmt_met_lep_fastsim->Fill(mt_met_lep());
	//hlepkinpt_fastsim->Fill(lepkinpt());
	//hlepkineta_fastsim->Fill(lepkineta());
	hngoodjets_fastsim->Fill(ngoodjets());
	//hpt_fastsim->Fill(pt());
	//heta_fastsim->Fill(eta());
	hlep1_dphiMET_fastsim->Fill(lep1_dphiMET());
	hmindphi_met_j1_j2_fastsim->Fill(j1dphiMET());
	//hMT2_fastsim->Fill(MT2());
	//hMCT_fastsim->Fill(MCT());
	htopnessMod_fastsim->Fill(topnessMod());
      } else{
	//if "fastsim" isn't in the title, then fill these histos instead

	//Fill Histograms
	hpfmet->Fill(pfmet());
	//hmht->Fill(mht());
	//hv2met->Fill(v2met();
	hmt_met_lep->Fill(mt_met_lep());
	//hlepkinpt->Fill(lepkinpt());
	//hlepkineta->Fill(lepkineta());
	hngoodjets->Fill(ngoodjets());
	//hpt->Fill(pt());
	//heta->Fill(eta());
	hlep1_dphiMET->Fill(lep1_dphiMET());
	hmindphi_met_j1_j2->Fill(j1dphiMET());
	//hMT2->Fill(MT2());
	//hMCT->Fill(MCT());
	htopnessMod->Fill(topnessMod());
      }
    }
  
    // Clean Up
    delete tree;
    file.Close();
  }
  if (nEventsChain != nEventsTotal) {
    cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
  }
  
  // Example Histograms
  //  hpfmet->Draw();

  c0->cd();
  hpfmet->SetStats(false);
  hpfmet->SetLineWidth(3);
  hpfmet->SetLineColor(kRed);
  hpfmet->GetXaxis()->SetTitle("MET [GeV]");
  hpfmet->DrawNormalized();
  hpfmet_fastsim->SetStats(false);
  hpfmet_fastsim->SetLineWidth(3);
  hpfmet_fastsim->SetLineColor(kBlue);
  hpfmet_fastsim->DrawNormalized("same");

  TLegend* leg0 = new TLegend(0.1,0.7,0.48,0.9);
  leg0->AddEntry(hpfmet,"FullSim Sample");
  leg0->AddEntry(hpfmet_fastsim,"FastSim Sample");
  leg0->Draw();


  //c0->cd();
  //h_ ->SetStats(false);
  //h_->SetLineWidth(3);
  //h_ ->SetLineColor(kRed);
  //h_ ->GetXaxis()->SetTitle(" ");
  //h_DrawNormalized();
  //h_fastsim->SetStats(false);
  //h_fastsim->SetLineWidth(3);
  //h_fastsim->SetLineColor(kBlue);
  //h_fastsim->DrawNormalized("same");


  //TLegend* leg0 = newTLegend(0.1,0.7.0.48,0.9);
  //leg0->AddEntry( ,"FullSim Sample");
  //leg0->AddEntry( ,"FastSim Sample");
  //leg0->Draw;


  //c1->cd();
  //h_ ->SetStats(false);
  //h_->SetLineWidth(3);
  //h_ ->SetLineColor(kRed);
  //h_ ->GetXaxis()->SetTitle(" ");
  //h_DrawNormalized();
  //h_fastsim->SetStats(false);
  //h_fastsim->SetLineWidth(3);
  //h_fastsim->SetLineColor(kBlue);
  //h_fastsim->DrawNormalized("same");


  //TLegend* leg1 = newTLegend(0.1,0.7.0.48,0.9);
  //leg1->AddEntry( ,"FullSim Sample");
  //leg1->AddEntry( ,"FastSim Sample");
  //leg1->Draw;


  //c2->cd();
  //h_ ->SetStats(false);
  //h_->SetLineWidth(3);
  //h_ ->SetLineColor(kRed);
  //h_ ->GetXaxis()->SetTitle(" ");
  //h_DrawNormalized();
  //h_fastsim->SetStats(false);
  //h_fastsim->SetLineWidth(3);
  //h_fastsim->SetLineColor(kBlue);
  //h_fastsim->DrawNormalized("same");


  //TLegend* leg2 = newTLegend(0.1,0.7.0.48,0.9);
  //leg2->AddEntry( ,"FullSim Sample");
  //leg2->AddEntry( ,"FastSim Sample");
  //leg2->Draw;


  //c3->cd();
  //h_ ->SetStats(false);
  //h_->SetLineWidth(3);
  //h_ ->SetLineColor(kRed);
  //h_ ->GetXaxis()->SetTitle(" ");
  //h_DrawNormalized();
  //h_fastsim->SetStats(false);
  //h_fastsim->SetLineWidth(3);
  //h_fastsim->SetLineColor(kBlue);
  //h_fastsim->DrawNormalized("same");


  //TLegend* leg3 = newTLegend(0.1,0.7.0.48,0.9);
  //leg3->AddEntry( ,"FullSim Sample");
  //leg3->AddEntry( ,"FastSim Sample");
  //leg3->Draw;


  //c4->cd();
  //h_ ->SetStats(false);
  //h_->SetLineWidth(3);
  //h_ ->SetLineColor(kRed);
  //h_ ->GetXaxis()->SetTitle(" ");
  //h_DrawNormalized();
  //h_fastsim->SetStats(false);
  //h_fastsim->SetLineWidth(3);
  //h_fastsim->SetLineColor(kBlue);
  //h_fastsim->DrawNormalized("same");


  //TLegend* leg4 = newTLegend(0.1,0.7.0.48,0.9);
  //leg4->AddEntry( ,"FullSim Sample");
  //leg4->AddEntry( ,"FastSim Sample");
  //leg4->Draw;
  

  //c5->cd();
  //h_ ->SetStats(false);
  //h_->SetLineWidth(3);
  //h_ ->SetLineColor(kRed);
  //h_ ->GetXaxis()->SetTitle(" ");
  //h_DrawNormalized();
  //h_fastsim->SetStats(false);
  //h_fastsim->SetLineWidth(3);
  //h_fastsim->SetLineColor(kBlue);
  //h_fastsim->DrawNormalized("same");


  //TLegend* leg5 = newTLegend(0.1,0.7.0.48,0.9);
  //leg5->AddEntry( ,"FullSim Sample");
  //leg5->AddEntry( ,"FastSim Sample");
  //leg5->Draw;


  //c6->cd();
  //h_ ->SetStats(false);
  //h_->SetLineWidth(3);
  //h_ ->SetLineColor(kRed);
  //h_ ->GetXaxis()->SetTitle(" ");
  //h_DrawNormalized();
  //h_fastsim->SetStats(false);
  //h_fastsim->SetLineWidth(3);
  //h_fastsim->SetLineColor(kBlue);
  //h_fastsim->DrawNormalized("same");


  //TLegend* leg6 = newTLegend(0.1,0.7.0.48,0.9);
  //leg6->AddEntry( ,"FullSim Sample");
  //leg6->AddEntry( ,"FastSim Sample");
  //leg6->Draw;


  //c7->cd();
  //h_ ->SetStats(false);
  //h_->SetLineWidth(3);
  //h_ ->SetLineColor(kRed);
  //h_ ->GetXaxis()->SetTitle(" ");
  //h_DrawNormalized();
  //h_fastsim->SetStats(false);
  //h_fastsim->SetLineWidth(3);
  //h_fastsim->SetLineColor(kBlue);
  //h_fastsim->DrawNormalized("same");


  //TLegend* leg7 = newTLegend(0.1,0.7.0.48,0.9);
  //leg7->AddEntry( ,"FullSim Sample");
  //leg7->AddEntry( ,"FastSim Sample");
  //leg7->Draw;

  //c8->cd();
  //h_ ->SetStats(false);
  //h_->SetLineWidth(3);
  //h_ ->SetLineColor(kRed);
  //h_ ->GetXaxis()->SetTitle(" ");
  //h_DrawNormalized();
  //h_fastsim->SetStats(false);
  //h_fastsim->SetLineWidth(3);
  //h_fastsim->SetLineColor(kBlue);
  //h_fastsim->DrawNormalized("same");


  //TLegend* leg8 = newTLegend(0.1,0.7.0.48,0.9);
  //leg8->AddEntry( ,"FullSim Sample");
  //leg8->AddEntry( ,"FastSim Sample");
  //leg8->Draw;


  //c9->cd();
  //h_ ->SetStats(false);
  //h_->SetLineWidth(3);
  //h_ ->SetLineColor(kRed);
  //h_ ->GetXaxis()->SetTitle(" ");
  //h_DrawNormalized();
  //h_fastsim->SetStats(false);
  //h_fastsim->SetLineWidth(3);
  //h_fastsim->SetLineColor(kBlue);
  //h_fastsim->DrawNormalized("same");


  //TLegend* leg9 = newTLegend(0.1,0.7.0.48,0.9);
  //leg9->AddEntry( ,"FullSim Sample");
  //leg9->AddEntry( ,"FastSim Sample");
  //leg9->Draw;


  //c10->cd();
  //h_ ->SetStats(false);
  //h_->SetLineWidth(3);
  //h_ ->SetLineColor(kRed);
  //h_ ->GetXaxis()->SetTitle(" ");
  //h_DrawNormalized();
  //h_fastsim->SetStats(false);
  //h_fastsim->SetLineWidth(3);
  //h_fastsim->SetLineColor(kBlue);
  //h_fastsim->DrawNormalized("same");


  //TLegend* leg10 = newTLegend(0.1,0.7.0.48,0.9);
  //leg10->AddEntry( ,"FullSim Sample");
  //leg10->AddEntry( ,"FastSim Sample");
  //leg10->Draw;


  //c11->cd();
  //h_ ->SetStats(false);
  //h_->SetLineWidth(3);
  //h_ ->SetLineColor(kRed);
  //h_ ->GetXaxis()->SetTitle(" ");
  //h_DrawNormalized();
  //h_fastsim->SetStats(false);
  //h_fastsim->SetLineWidth(3);
  //h_fastsim->SetLineColor(kBlue);
  //h_fastsim->DrawNormalized("same");


  //TLegend* leg11 = newTLegend(0.1,0.7.0.48,0.9);
  //leg11->AddEntry( ,"FullSim Sample");
  //leg11->AddEntry( ,"FastSim Sample");
  //leg11->Draw;

  //c12->cd();
  //h_ ->SetStats(false);
  //h_->SetLineWidth(3);
  //h_ ->SetLineColor(kRed);
  //h_ ->GetXaxis()->SetTitle(" ");
  //h_DrawNormalized();
  //h_fastsim->SetStats(false);
  //h_fastsim->SetLineWidth(3);
  //h_fastsim->SetLineColor(kBlue);
  //h_fastsim->DrawNormalized("same");


  //TLegend* leg12 = newTLegend(0.1,0.7.0.48,0.9);
  //leg12->AddEntry( ,"FullSim Sample");
  //leg12->AddEntry( ,"FastSim Sample");
  //leg12->Draw;

  //c13->cd();
  //h_ ->SetStats(false);
  //h_->SetLineWidth(3);
  //h_ ->SetLineColor(kRed);
  //h_ ->GetXaxis()->SetTitle(" ");
  //h_DrawNormalized();
  //h_fastsim->SetStats(false);
  //h_fastsim->SetLineWidth(3);
  //h_fastsim->SetLineColor(kBlue);
  //h_fastsim->DrawNormalized("same");


  //TLegend* leg13 = newTLegend(0.1,0.7.0.48,0.9);
  //leg13->AddEntry( ,"FullSim Sample");
  //leg13->AddEntry( ,"FastSim Sample");
  //leg13->Draw;


  //Output file for Histograms
  TFile *fNumbers = new TFile("MirandasPlots.root","recreate");
  fNumbers->Add(hpfmet);
  //fNumbers->Add(hmht);
  //fNumbers->Add(hv2met);
  fNumbers->Add(hmt_met_lep);
  //fNumbers->Add(hlepkinpt);
  //fNumbers->Add(hlepkineta);
  fNumbers->Add(hngoodjets);
  //fNumbers->Add(hpt);
  //fNumbers->Add(heta);
  fNumbers->Add(hlep1_dphiMET);
  fNumbers->Add(hmindphi_met_j1_j2);
  //fNumbers->Add(hMT2);
  //fNumbers->Add(hMCT);
  fNumbers->Add(htopnessMod);
  fNumbers->Add(hpfmet_fastsim);
  //fNumbers->Add(hmht_fastsim);
  //fNumbers->Add(hv2met_fastsim);
  fNumbers->Add(hmt_met_lep_fastsim);
  //fNumbers->Add(hlepkinpt_fastsim);
  //fNumbers->Add(hlepkineta_fastsim);
  fNumbers->Add(hngoodjets_fastsim);
  //fNumbers->Add(hpt_fastsim);
  //fNumbers->Add(heta_fastsim);
  fNumbers->Add(hlep1_dphiMET_fastsim);
  fNumbers->Add(hmindphi_met_j1_j2_fastsim);
  //fNumbers->Add(hMT2_fastsim);
  //fNumbers->Add(hMCT_fastsim);
  fNumbers->Add(htopnessMod_fastsim);
  fNumbers->Add(c0);
  fNumbers->Add(c1);
  fNumbers->Add(c2);
  fNumbers->Add(c3);
  fNumbers->Add(c4);
  fNumbers->Add(c5);
  fNumbers->Add(c6);
  fNumbers->Add(c7);
  fNumbers->Add(c8);
  fNumbers->Add(c9);
  fNumbers->Add(c10);
  fNumbers->Add(c11);
  fNumbers->Add(c12);
  fNumbers->Add(c13);
  fNumbers->Write();  
  fNumbers->Close();

  // return
  bmark->Stop("benchmark");
  cout << endl;
  cout << nEventsTotal << " Events Processed" << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time: " << Form( "%.01f", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time: " << Form( "%.01f", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;
  return 0;
}
