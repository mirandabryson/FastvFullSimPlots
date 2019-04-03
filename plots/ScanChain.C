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
  TH1F *hngoodjets = new TH1F("hngoodjets", "ngoodjets", 40, 0, 100);
  hngoodjets->SetDirectory(rootdir);
  //TH1F *hpt = new TH1F("hpt", "pt", 40, 0, 800);
  //hpt->SetDirectory(rootdir);
  //TH1F *heta - new TH1F("heta", "eta", 40, 0, 800);
  //heta->SetDirectory(rootdir);
  TH1F *hlep1_dphiMET = new TH1F("hlep1_dphiMET", "lep1_dphiMET", 40, 0, 100);
  hlep1_dphiMET->SetDirectory(rootdir);
  //TH1F *hj1dphiMETl = new TH1F("hj1dphiMET", "j1dphiMET", 40, 0, 800);
  //hj1dphiMET->SetDirectory(rootdir);
  //TH1F *hMT2 = new TH1F("hMT2", "MT2", 40, 0, 800);
  //hMT2->SetDirectory(rootdir);
  //TH1F *hMCT = new TH1F("hMCT", "MCT", 40, 0, 800);
  //hMCT->SetDirectory(rootdir);
  TH1F *htopnessMod = new TH1F("htopnessMod", "topnessMod", 40, 0, 100);
  htopnessMod->SetDirectory(rootdir);

  //Canvas definition
  TCanvas *c0 = new TCanvas("c0","c0",800,800);
  //c0->SetDirectory(rootdir);

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
      //hj1dphiMET->Fill(j1dphiMET());
      //hMT2->Fill(MT2());
      //hMCT->Fill(MCT());
      htopnessMod->Fill(topnessMod());
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
  hmt_met_lep->SetStats(false);
  hmt_met_lep->SetLineWidth(3);
  hmt_met_lep->SetLineColor(kBlue);
  hmt_met_lep->DrawNormalized("same");

  TLegend* leg0 = new TLegend(1,1,1,1);
  leg0->AddEntry(hpfmet,"FullSim Sample");
  leg0->AddEntry(hmt_met_lep,"FastSim Sample");
  leg0->Draw();
  

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
  //fNumbers->Add(hj1dphiMET);
  //fNumbers->Add(hMT2);
  //fNumbers->Add(hMCT);
  fNumbers->Add(htopnessMod);
  fNumbers->Add(c0);
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
