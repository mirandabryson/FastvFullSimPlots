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
#include "TH2.h"
#include "TLatex.h"
//#include "TRatioPlot.h"

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
  TH1F *hmht = new TH1F("hmht", "mht", 40, 0, 800);
  hmht->SetDirectory(rootdir);
  //TH1F *hv2met = new TH1F("hv2met", "v2met", 40, 0, 800);
  //hv2met->SetDirectory(rootdir);
  TH1F *hmt_met_lep = new TH1F("hmt_met_lep", "mt_met_lep", 30, 0, 400);
  hmt_met_lep->SetDirectory(rootdir);
  //TH1F *hlepkinpt = new TH1F("hlepkinpt", "lepkinpt", 40, 0, 800);
  //hlepkinpt->SetDirectory(rootdir);
  //TH1F *hlepkineta = new TH1F("hlepkineta", "lepkineta", 40, 0, 800);
  TH1F *hngoodjets = new TH1F("hngoodjets", "ngoodjets", 20, 0, 20);
  hngoodjets->SetDirectory(rootdir);
  //TH1F *hpt = new TH1F("hpt", "pt", 40, 0, 800);
  //hpt->SetDirectory(rootdir);
  //TH1F *heta - new TH1F("heta", "eta", 40, 0, 800);
  //heta->SetDirectory(rootdir);
  TH1F *hlep1_dphiMET = new TH1F("hlep1_dphiMET", "lep1_dphiMET", 15, 0, 3.5);
  hlep1_dphiMET->SetDirectory(rootdir);
  TH1F *hmindphi_met_j1_j2 = new TH1F("hmindphi_met_j1_j2", "mindphi_met_j1_j2", 15, 0, 3.5);
  hmindphi_met_j1_j2->SetDirectory(rootdir);
  TH1F *hMT2 = new TH1F("hMT2", "MT2", 40, 0, 800);
  hMT2->SetDirectory(rootdir);
  //TH1F *hMCT = new TH1F("hMCT", "MCT", 40, 0, 800);
  //hMCT->SetDirectory(rootdir);
  TH1F *htopnessMod = new TH1F("htopnessMod", "topnessMod", 23, -10, 13);
  htopnessMod->SetDirectory(rootdir);
  TH1F *hngoodleps = new TH1F("hngoodleps", "ngoodleps", 10, 0, 10);
  hngoodleps->SetDirectory(rootdir);
  TH1F *hngoodbtags = new TH1F("hngoodbtags", "ngoodbtags", 10, 0, 10);
  hngoodbtags->SetDirectory(rootdir);
  //TH1F *hgenht = new TH1F("hgenht", "genht", 100, 0, 800);
  //hgenht->SetDirectory(rootdir);
  //TH1F *hmassStop = new TH1F("hmassStop", "mass_stop",100, 0, 1800);
  //hmassStop->SetDirectory(rootdir);
  TH1F *hleadJetPt = new TH1F("hleadJetPt", "leadJetPt", 15, 0, 300);
  hleadJetPt->SetDirectory(rootdir);
  TH1F *hlep1pt = new TH1F("hlep1pt", "lep1pt", 40, 0, 800);
  hlep1pt->SetDirectory(rootdir);
  TH1F *hlep1eta = new TH1F("hlep1eta", "lep1eta", 20, -3, 3);
  hlep1eta->SetDirectory(rootdir);
  TH1F *hlep2pt = new TH1F("hlep2pt", "lep2pt", 40, 0, 800);
  hlep1pt->SetDirectory(rootdir);
  TH1F *hlep2eta = new TH1F("hlep2eta", "lep2eta", 20, -3, 3);
  hlep2eta->SetDirectory(rootdir);
  TH1F *halljetpt = new TH1F("halljetpt", "alljetpt", 30, 0, 300);
  halljetpt->SetDirectory(rootdir);
  TH1F *halljeteta = new TH1F("halljeteta", "alljeteta", 20, -3, 3);
  halljeteta->SetDirectory(rootdir);
  TH1F *hleadJetEta = new TH1F("hleadJetEta", "leadJetEta", 20, -3, 3);
  hleadJetEta->SetDirectory(rootdir);



  //Defining fastsim histos
  TH1F *hpfmet_fastsim = new TH1F("hpfmet_fastsim", "pfmet", 40,0,800);
  hpfmet_fastsim->SetDirectory(rootdir);
  TH1F *hmht_fastsim = new TH1F("hmht_fastsim", "mht", 40, 0, 800);
  hmht_fastsim->SetDirectory(rootdir);
  //TH1F *hv2met_fastsim = new TH1F("hv2met_fastsim", "v2met", 40, 0, 800);
  //hv2met_fastsim->SetDirectory(rootdir);
  TH1F *hmt_met_lep_fastsim = new TH1F("hmt_met_lep_fastsim", "mt_met_lep", 30, 0, 400);
  hmt_met_lep_fastsim->SetDirectory(rootdir);
  //TH1F *hlepkinpt_fastsim = new TH1F("hlepkinpt_fastsim", "lepkinpt", 40, 0, 800);
  //hlepkinpt_fastsim->SetDirectory(rootdir);
  //TH1F *hlepkineta_fastsim = new TH1F("hlepkineta_fastsim", "lepkineta", 40, 0, 800);
  TH1F *hngoodjets_fastsim = new TH1F("hngoodjets_fastsim", "ngoodjets", 20, 0, 20);
  hngoodjets_fastsim->SetDirectory(rootdir);
  //TH1F *hpt_fastsim = new TH1F("hpt_fastsim", "pt", 40, 0, 800);
  //hpt_fastsim->SetDirectory(rootdir);
  //TH1F *heta_fastsim - new TH1F("heta_fastsim", "eta", 40, 0, 800);
  //heta_fastsim->SetDirectory(rootdir);
  TH1F *hlep1_dphiMET_fastsim = new TH1F("hlep1_dphiMET_fastsim", "lep1_dphiMET", 15, 0, 3.5);
  hlep1_dphiMET_fastsim->SetDirectory(rootdir);
  TH1F *hmindphi_met_j1_j2_fastsim = new TH1F("hmindphi_met_j1_j2_fastsim", "hmindphi_met_j1_j2", 15, 0, 3.5);
  hmindphi_met_j1_j2_fastsim->SetDirectory(rootdir);
  TH1F *hMT2_fastsim = new TH1F("hMT2_fastsim", "MT2", 40, 0, 800);
  hMT2_fastsim->SetDirectory(rootdir);
  //TH1F *hMCT_fastsim = new TH1F("hMCT_fastsim", "MCT", 40, 0, 800);
  //hMCT_fastsim->SetDirectory(rootdir);
  TH1F *htopnessMod_fastsim = new TH1F("htopnessMod_fastsim", "topnessMod", 23, -10, 13);
  htopnessMod_fastsim->SetDirectory(rootdir);
  TH1F *hngoodleps_fastsim = new TH1F("hngoodleps_fastsim", "ngoodleps", 10, 0, 10);
  hngoodleps_fastsim->SetDirectory(rootdir);
  TH1F *hngoodbtags_fastsim = new TH1F("hngoodbtags_fastsim", "ngoodbtags", 10, 0, 10);
  hngoodbtags_fastsim->SetDirectory(rootdir);
  //TH1F *hgenht_fastsim = new TH1F("hgenht_fastsim", "genht", 100, 0, 800);
  //hgenht_fastsim->SetDirectory(rootdir);
  TH1F *hmassStop_fastsim = new TH1F("hmassStop_fastsim", "mass_stop", 100, 0, 1800);
  hmassStop_fastsim->SetDirectory(rootdir); 
  TH1F *hleadJetPt_fastsim = new TH1F("hleadJetPt_fastsim", "leadJetPt", 15, 0, 300);
  hleadJetPt_fastsim->SetDirectory(rootdir);
  TH1F *hlep1pt_fastsim = new TH1F("hlep1pt_fastsim", "lep1pt", 40, 0, 800);
  hlep1pt_fastsim->SetDirectory(rootdir);
  TH1F *hlep1eta_fastsim = new TH1F("hlep1eta_fastsim", "lep1eta", 40, 0, 800);
  hlep1eta_fastsim->SetDirectory(rootdir);
  TH1F *hlep2pt_fastsim = new TH1F("hlep2pt_fastsim", "lep2pt", 20, -3, 3);
  hlep1pt_fastsim->SetDirectory(rootdir);
  TH1F *hlep2eta_fastsim = new TH1F("hlep2eta_fastsim", "lep2eta", 20, -3, 3);
  hlep2eta_fastsim->SetDirectory(rootdir);
  TH1F *halljetpt_fastsim = new TH1F("halljetpt_fastsim", "alljetpt", 15, 0, 300);
  halljetpt_fastsim->SetDirectory(rootdir);
  TH1F *halljeteta_fastsim = new TH1F("halljeteta_fastsim", "alljeteta", 20, -3, -3);
  halljeteta_fastsim->SetDirectory(rootdir);
  TH1F *hleadJetEta_fastsim = new TH1F("hleadJetEta_fastsim", "leadJetEta", 20, -3, 3);
  hleadJetEta_fastsim->SetDirectory(rootdir);




  //Canvas definition
  TCanvas *cpfmet = new TCanvas("cpfmet","cpfmet",800,800);
  TCanvas *cmt_met_lep = new TCanvas("cmt_met_lep","cmt_met_lep",800,800);
  TCanvas *cngoodjets = new TCanvas("cngoodjets","cngoodjets",800,800);
  TCanvas *clep1_dphiMET = new TCanvas("clep1_dphiMET","clep1_dphiMET",800,800);
  TCanvas *cmindphi_met_j1_j2 = new TCanvas("cmindphi_met_j1_j2","cmindphi_met_j1_j2",800,800);
  TCanvas *ctopnessMod = new TCanvas("ctopnessMod","ctopnessMod",800,800);
  TCanvas *cngoodleps = new TCanvas("cngoodleps","cngoodleps",800,800);
  TCanvas *cngoodbtags = new TCanvas("cngoodbtags","cngoodbtags",800,800);
  TCanvas *cmht = new TCanvas("cmht","cmht",800,800);
  TCanvas *cMT2 = new TCanvas("cMT2","cMT2",800,800);
  TCanvas *cleadJetPt = new TCanvas("cleadJetPt","cleadJetPt",800,800);
  TCanvas *clep1pt = new TCanvas("clep1pt","clep1pt",800,800);
  TCanvas *clep1eta = new TCanvas("clep1eta","clep1eta",800,800);
  TCanvas *clep2pt = new TCanvas("clep2pt","clep2pt",800,800);
  TCanvas *clep2eta = new TCanvas("clep2eta","clep2eta",800,800);
  TCanvas *calljetpt = new TCanvas("calljetpt","calljetpt",800,800);
  TCanvas *calljeteta = new TCanvas("calljeteta","calljeteta",800,800);
  TCanvas *cleadJetEta = new TCanvas("cleadJetEta", "cleadJetEta", 800, 800);


//TCanvas *cmassStop = new TCanvas("cmassStop","cmassStop",800,800);

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


      // LEPTON SKIM
      if(ngoodleps()>0) continue;
      if(pfmet()<50) continue;
      if((fileName.find(fileCut)!= string::npos) &&  mass_stop() !=1200) continue;
      if((fileName.find(fileCut)!= string::npos) &&  mass_lsp() !=800) continue;
      //cout<<"Gluino Mass"<<mass_stop()<<" LSP mass"<<mass_lsp()<<endl;
      
      // Progress
      StopCMS3::progress( nEventsTotal, nEventsChain );

      // Analysis Code
      // this is where we put cuts like pfmet>150 : if(pfmet()<150) continue;

      TH2D* h_sig_counter_nEvents = nullptr;
      
      if(fileName.find(fileCut) != string::npos){
	//if "fastsim" is in the file title, fill only the fastsim histos

	h_sig_counter_nEvents = (TH2D*) file.Get("histNEvts");
	int nEventsPoint = h_sig_counter_nEvents->GetBinContent(h_sig_counter_nEvents->FindBin(mass_stop(), mass_lsp()));
        float evtweight_ = xsec() * 1000 / nEventsPoint;

	float totalWeight = evtweight_*weight_btagsf();
	
	//Fill Histograms
	hpfmet_fastsim->Fill(pfmet(),totalWeight);
	hmht_fastsim->Fill(ak4_MHT_pt(),totalWeight);
	//hv2met_fastsim->Fill(v2met(),totalWeight);
	hmt_met_lep_fastsim->Fill(mt_met_lep(),totalWeight);
	//hlepkinpt_fastsim->Fill(lepkinpt(),totalWeight);
	//hlepkineta_fastsim->Fill(lepkineta(),totalWeight);
	hngoodjets_fastsim->Fill(ngoodjets(),totalWeight);
	//hpt_fastsim->Fill(pt(),totalWeight);
	//heta_fastsim->Fill(eta(),totalWeight);
	hlep1_dphiMET_fastsim->Fill(lep1_dphiMET(),totalWeight);
	hmindphi_met_j1_j2_fastsim->Fill(mindphi_met_j1_j2(),totalWeight);
	hMT2_fastsim->Fill(MT2_had(),totalWeight);
	//hMCT_fastsim->Fill(MCT(),totalWeight);
	htopnessMod_fastsim->Fill(topnessMod(),totalWeight);
	hngoodleps_fastsim->Fill(ngoodleps(),totalWeight);
	hngoodbtags_fastsim->Fill(ngoodbtags(),totalWeight);
	//hgenht_fastsim->Fill(genht(),totalWeight);
	hmassStop_fastsim->Fill(mass_stop(),totalWeight);
	hleadJetPt_fastsim->Fill(ak4pfjets_p4().at(0).pt(),totalWeight);
	hleadJetEta_fastsim->Fill(ak4pfjets_p4().at(0).eta(),totalWeight);
	hlep1pt_fastsim->Fill(lep1_p4().pt(), totalWeight);
	hlep1eta_fastsim->Fill(lep1_p4().eta(), totalWeight);
	hlep2pt_fastsim->Fill(lep2_p4().pt(), totalWeight);
	hlep2eta_fastsim->Fill(lep2_p4().eta(), totalWeight);
	for(unsigned int ijets = 0; ijets < ak4pfjets_p4().size(); ijets++){
	  halljetpt_fastsim->Fill(ak4pfjets_p4().at(ijets).pt(), totalWeight);
	  halljeteta_fastsim->Fill(ak4pfjets_p4().at(ijets).eta(), totalWeight);
	}

      } else{
	//if "fastsim" isn't in the title, then fill these histos instead

	float totalWeightFull = scale1fb()*weight_btagsf();

	//Fill Histograms
	hpfmet->Fill(pfmet(),totalWeightFull);
	hmht->Fill(ak4_MHT_pt(),totalWeightFull);
	//hv2met->Fill(v2met(),totalWeightFull);
	hmt_met_lep->Fill(mt_met_lep(),totalWeightFull);
	//hlepkinpt->Fill(lepkinpt(),totalWeightFull);
	//hlepkineta->Fill(lepkineta(),totalWeightFull);
	hngoodjets->Fill(ngoodjets(),totalWeightFull);
	//hpt->Fill(pt(),totalWeightFull);
	//heta->Fill(eta(),totalWeightFull);
	hlep1_dphiMET->Fill(lep1_dphiMET(),totalWeightFull);
	hmindphi_met_j1_j2->Fill(mindphi_met_j1_j2(),totalWeightFull);
	hMT2->Fill(MT2_had(),totalWeightFull);
	//hMCT->Fill(MCT(),totalWeightFull);
	htopnessMod->Fill(topnessMod(),totalWeightFull);
	hngoodleps->Fill(ngoodleps(),totalWeightFull);
	hngoodbtags->Fill(ngoodbtags(),totalWeightFull);
	//hgenht->Fill(genht(),totalWeightFull);
	//hmassStop->Fill(mass_stop(),totalWeightFull);
	hleadJetPt->Fill(ak4pfjets_p4().at(0).pt(),totalWeightFull);
	hleadJetEta->Fill(ak4pfjets_p4().at(0).eta(), totalWeightFull);
	hlep1pt->Fill(lep1_p4().pt(), totalWeightFull);
	hlep1eta->Fill(lep1_p4().eta(), totalWeightFull);
	hlep2pt->Fill(lep2_p4().pt(), totalWeightFull);
	hlep2eta->Fill(lep2_p4().eta(), totalWeightFull);
	for(unsigned int ijets = 0; ijets < ak4pfjets_p4().size(); ijets++){
	  halljetpt->Fill(ak4pfjets_p4().at(ijets).pt(), totalWeightFull);
	  halljeteta->Fill(ak4pfjets_p4().at(ijets).eta(), totalWeightFull);
	}
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

  cpfmet->cd();
  hpfmet->SetStats(false);
  hpfmet->SetLineWidth(3);
  hpfmet->SetLineColor(kRed);
  hpfmet->GetXaxis()->SetTitle("MET [GeV]");
  hpfmet->DrawNormalized();
  hpfmet_fastsim->SetStats(false);
  hpfmet_fastsim->SetLineWidth(3);
  hpfmet_fastsim->SetLineColor(kBlue);
  hpfmet_fastsim->DrawNormalized("same");

  //auto rp = new TRatioPlot(hpfmet,hpfmet_fastsim);
  //rp->Draw();
  //cpfmet->Update();

  TLegend* leg0 = new TLegend(0.1,0.7,0.48,0.9);
  leg0->AddEntry(hpfmet,"FullSim Sample");
  leg0->AddEntry(hpfmet_fastsim,"FastSim Sample");
  leg0->Draw();

  cmt_met_lep->cd();
  hmt_met_lep->SetStats(false);
  hmt_met_lep->SetLineWidth(3);
  hmt_met_lep->SetLineColor(kRed);
  hmt_met_lep->GetXaxis()->SetTitle("M_{T} [GeV]");
  hmt_met_lep->DrawNormalized();
  hmt_met_lep_fastsim->SetStats(false);
  hmt_met_lep_fastsim->SetLineWidth(3);
  hmt_met_lep_fastsim->SetLineColor(kBlue);
  hmt_met_lep_fastsim->DrawNormalized("same");

  TLegend* leg1 = new TLegend(0.1,0.7,0.48,0.9);
  leg1->AddEntry(hmt_met_lep,"FullSim Sample");
  leg1->AddEntry(hmt_met_lep_fastsim,"FastSim Sample");
  leg1->Draw();
  
  cngoodjets->cd();
  hngoodjets->SetStats(false);
  hngoodjets->SetLineWidth(3);
  hngoodjets->SetLineColor(kRed);
  hngoodjets->GetXaxis()->SetTitle("nJets");
  hngoodjets->DrawNormalized();
  hngoodjets_fastsim->SetStats(false);
  hngoodjets_fastsim->SetLineWidth(3);
  hngoodjets_fastsim->SetLineColor(kBlue);
  hngoodjets_fastsim->DrawNormalized("same");

  TLegend* leg2 = new TLegend(0.1,0.7,0.48,0.9);
  leg2->AddEntry(hngoodjets,"FullSim Sample");
  leg2->AddEntry(hngoodjets_fastsim,"FastSim Sample");
  leg2->Draw();

  clep1_dphiMET->cd();
  hlep1_dphiMET->SetStats(false);
  hlep1_dphiMET->SetLineWidth(3);
  hlep1_dphiMET->SetLineColor(kRed);
  hlep1_dphiMET->GetXaxis()->SetTitle("dPhi_{MET}");
  hlep1_dphiMET->DrawNormalized();
  hlep1_dphiMET_fastsim->SetStats(false);
  hlep1_dphiMET_fastsim->SetLineWidth(3);
  hlep1_dphiMET_fastsim->SetLineColor(kBlue);
  hlep1_dphiMET_fastsim->DrawNormalized("same");

  TLegend* leg3 = new TLegend(0.1,0.7,0.48,0.9);
  leg3->AddEntry(hlep1_dphiMET,"FullSim Sample");
  leg3->AddEntry(hlep1_dphiMET_fastsim,"FastSim Sample");
  leg3->Draw();

  cmindphi_met_j1_j2->cd();
  hmindphi_met_j1_j2->SetStats(false);
  hmindphi_met_j1_j2->SetLineWidth(3);
  hmindphi_met_j1_j2->SetLineColor(kRed);
  hmindphi_met_j1_j2->GetXaxis()->SetTitle("min dPhi_{jets}");
  hmindphi_met_j1_j2->DrawNormalized();
  hmindphi_met_j1_j2_fastsim->SetStats(false);
  hmindphi_met_j1_j2_fastsim->SetLineWidth(3);
  hmindphi_met_j1_j2_fastsim->SetLineColor(kBlue);
  hmindphi_met_j1_j2_fastsim->DrawNormalized("same");

  TLegend* leg4 = new TLegend(0.1,0.7,0.48,0.9);
  leg4->AddEntry(hmindphi_met_j1_j2,"FullSim Sample");
  leg4->AddEntry(hmindphi_met_j1_j2_fastsim,"FastSim Sample");
  leg4->Draw();

  ctopnessMod->cd();
  htopnessMod->SetStats(false);
  htopnessMod->SetLineWidth(3);
  htopnessMod->SetLineColor(kRed);
  htopnessMod->GetXaxis()->SetTitle("Modified Topness");
  htopnessMod->DrawNormalized();
  htopnessMod_fastsim->SetStats(false);
  htopnessMod_fastsim->SetLineWidth(3);
  htopnessMod_fastsim->SetLineColor(kBlue);
  htopnessMod_fastsim->DrawNormalized("same");

  TLegend* leg5 = new TLegend(0.1,0.7,0.48,0.9);
  leg5->AddEntry(htopnessMod,"FullSim Sample");
  leg5->AddEntry(htopnessMod_fastsim,"FastSim Sample");
  leg5->Draw();

  cngoodleps->cd();
  hngoodleps->SetStats(false);
  hngoodleps->SetLineWidth(3);
  hngoodleps->SetLineColor(kRed);
  hngoodleps->GetXaxis()->SetTitle("nLeptons");
  hngoodleps->DrawNormalized();
  hngoodleps_fastsim->SetStats(false);
  hngoodleps_fastsim->SetLineWidth(3);
  hngoodleps_fastsim->SetLineColor(kBlue);
  hngoodleps_fastsim->DrawNormalized("same");

  TLegend* leg6 = new TLegend(0.1,0.7,0.48,0.9);
  leg6->AddEntry(hngoodleps,"FullSim Sample");
  leg6->AddEntry(hngoodleps_fastsim,"FastSim Sample");
  leg6->Draw();

  cngoodbtags->cd();
  hngoodbtags->SetStats(false);
  hngoodbtags->SetLineWidth(3);
  hngoodbtags->SetLineColor(kRed);
  hngoodbtags->GetXaxis()->SetTitle("nb-tags");
  hngoodbtags->DrawNormalized();
  hngoodbtags_fastsim->SetStats(false);
  hngoodbtags_fastsim->SetLineWidth(3);
  hngoodbtags_fastsim->SetLineColor(kBlue);
  hngoodbtags_fastsim->DrawNormalized("same");

  TLegend* leg17 = new TLegend(0.1,0.7,0.48,0.9);
  leg17->AddEntry(hngoodbtags,"FullSim Sample");
  leg17->AddEntry(hngoodbtags_fastsim,"FastSim Sample");
  leg17->Draw();

  cmht->cd();
  hmht->SetStats(false);
  hmht->SetLineWidth(3);
  hmht->SetLineColor(kRed);
  hmht->GetXaxis()->SetTitle("Missing h_{T} [GeV]");
  hmht->DrawNormalized();
  hmht_fastsim->SetStats(false);
  hmht_fastsim->SetLineWidth(3);
  hmht_fastsim->SetLineColor(kBlue);
  hmht_fastsim->DrawNormalized("same");

  TLegend* leg7 = new TLegend(0.1,0.7,0.48,0.9);
  leg7->AddEntry(hmht,"FullSim Sample");
  leg7->AddEntry(hmht_fastsim,"FastSim Sample");
  leg7->Draw();

  cMT2->cd();
  hMT2->SetStats(false);
  hMT2->SetLineWidth(3);
  hMT2->SetLineColor(kRed);
  hMT2->GetXaxis()->SetTitle("MT2 [GeV]");
  hMT2->DrawNormalized();
  hMT2_fastsim->SetStats(false);
  hMT2_fastsim->SetLineWidth(3);
  hMT2_fastsim->SetLineColor(kBlue);
  hMT2_fastsim->DrawNormalized("same");

  TLegend* leg8 = new TLegend(0.1,0.7,0.48,0.9);
  leg8->AddEntry(hMT2,"FullSim Sample");
  leg8->AddEntry(hMT2_fastsim,"FastSim Sample");
  leg8->Draw();

  cleadJetPt->cd();
  hleadJetPt->SetStats(false);
  hleadJetPt->SetLineWidth(3);
  hleadJetPt->SetLineColor(kRed);
  hleadJetPt->GetXaxis()->SetTitle("Leading Jet p_{T} [GeV]");
  hleadJetPt->DrawNormalized();
  hleadJetPt_fastsim->SetStats(false);
  hleadJetPt_fastsim->SetLineWidth(3);
  hleadJetPt_fastsim->SetLineColor(kBlue);
  hleadJetPt_fastsim->DrawNormalized("same");

  TLegend* leg9 = new TLegend(0.1,0.7,0.48,0.9);
  leg9->AddEntry(hleadJetPt,"FullSim Sample");
  leg9->AddEntry(hleadJetPt_fastsim,"FastSim Sample");
  leg9->Draw();

  cleadJetEta->cd();
  hleadJetEta->SetStats(false);
  hleadJetEta->SetLineWidth(3);
  hleadJetEta->SetLineColor(kRed);
  hleadJetEta->GetXaxis()->SetTitle("Leading Jet Eta");
  hleadJetEta->DrawNormalized();
  hleadJetEta_fastsim->SetStats(false);
  hleadJetEta_fastsim->SetLineWidth(3);
  hleadJetEta_fastsim->SetLineColor(kBlue);
  hleadJetEta_fastsim->DrawNormalized("same");

  TLegend* leg10 = new TLegend(0.1,0.7,0.48,0.9);
  leg10->AddEntry(hleadJetEta,"FullSim Sample");
  leg10->AddEntry(hleadJetEta_fastsim,"FastSim Sample");
  leg10->Draw();

  calljetpt->cd();
  halljetpt->SetStats(false);
  halljetpt->SetLineWidth(3);
  halljetpt->SetLineColor(kRed);
  halljetpt->GetXaxis()->SetTitle("All Jet p_{T} [GeV]");
  halljetpt->DrawNormalized();
  halljetpt_fastsim->SetStats(false);
  halljetpt_fastsim->SetLineWidth(3);
  halljetpt_fastsim->SetLineColor(kBlue);
  halljetpt_fastsim->DrawNormalized("same");

  TLegend* leg11 = new TLegend(0.1,0.7,0.48,0.9);
  leg11->AddEntry(halljetpt,"FullSim Sample");
  leg11->AddEntry(halljetpt_fastsim,"FastSim Sample");
  leg11->Draw();

  calljeteta->cd();
  halljeteta->SetStats(false);
  halljeteta->SetLineWidth(3);
  halljeteta->SetLineColor(kRed);
  halljeteta->GetXaxis()->SetTitle("All Jet Eta");
  halljeteta->DrawNormalized();
  halljeteta_fastsim->SetStats(false);
  halljeteta_fastsim->SetLineWidth(3);
  halljeteta_fastsim->SetLineColor(kBlue);
  halljeteta_fastsim->DrawNormalized("same");

  TLegend* leg12 = new TLegend(0.1,0.7,0.48,0.9);
  leg12->AddEntry(halljeteta,"FullSim Sample");
  leg12->AddEntry(halljeteta_fastsim,"FastSim Sample");
  leg12->Draw();

  clep1pt->cd();
  hlep1pt->SetStats(false);
  hlep1pt->SetLineWidth(3);
  hlep1pt->SetLineColor(kRed);
  hlep1pt->GetXaxis()->SetTitle("Lepton 1 p_{T} [GeV]");
  hlep1pt->DrawNormalized();
  hlep1pt_fastsim->SetStats(false);
  hlep1pt_fastsim->SetLineWidth(3);
  hlep1pt_fastsim->SetLineColor(kBlue);
  hlep1pt_fastsim->DrawNormalized("same");

  TLegend* leg13 = new TLegend(0.1,0.7,0.48,0.9);
  leg13->AddEntry(hlep1pt,"FullSim Sample");
  leg13->AddEntry(hlep1pt_fastsim,"FastSim Sample");
  leg13->Draw();

  clep2pt->cd();
  hlep2pt->SetStats(false);
  hlep2pt->SetLineWidth(3);
  hlep2pt->SetLineColor(kRed);
  hlep2pt->GetXaxis()->SetTitle("Lepton 2  p_{T} [GeV]");
  hlep2pt->DrawNormalized();
  hlep2pt_fastsim->SetStats(false);
  hlep2pt_fastsim->SetLineWidth(3);
  hlep2pt_fastsim->SetLineColor(kBlue);
  hlep2pt_fastsim->DrawNormalized("same");

  TLegend* leg14 = new TLegend(0.1,0.7,0.48,0.9);
  leg14->AddEntry(hlep2pt,"FullSim Sample");
  leg14->AddEntry(hlep2pt_fastsim,"FastSim Sample");
  leg14->Draw();

  clep2eta->cd();
  hlep2eta->SetStats(false);
  hlep2eta->SetLineWidth(3);
  hlep2eta->SetLineColor(kRed);
  hlep2eta->GetXaxis()->SetTitle("Lepton 2 Eta");
  hlep2eta->DrawNormalized();
  hlep2eta_fastsim->SetStats(false);
  hlep2eta_fastsim->SetLineWidth(3);
  hlep2eta_fastsim->SetLineColor(kBlue);
  hlep2eta_fastsim->DrawNormalized("same");

  TLegend* leg15 = new TLegend(0.1,0.7,0.48,0.9);
  leg15->AddEntry(hlep2eta,"FullSim Sample");
  leg15->AddEntry(hlep2eta_fastsim,"FastSim Sample");
  leg15->Draw();

  clep1eta->cd();
  hlep1eta->SetStats(false);
  hlep1eta->SetLineWidth(3);
  hlep1eta->SetLineColor(kRed);
  hlep1eta->GetXaxis()->SetTitle("Lepton 1 Eta");
  hlep1eta->DrawNormalized();
  hlep1eta_fastsim->SetStats(false);
  hlep1eta_fastsim->SetLineWidth(3);
  hlep1eta_fastsim->SetLineColor(kBlue);
  hlep1eta_fastsim->DrawNormalized("same");

  TLegend* leg16 = new TLegend(0.1,0.7,0.48,0.9);
  leg16->AddEntry(hlep1eta,"FullSim Sample");
  leg16->AddEntry(hlep1eta_fastsim,"FastSim Sample");
  leg16->Draw();



  //Output file for Histograms
  TFile *fNumbers = new TFile("MirandasPlots.root","recreate");
  fNumbers->Add(hpfmet);
  fNumbers->Add(hmht);
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
  fNumbers->Add(hngoodleps);
  fNumbers->Add(hngoodbtags);
  //fNumbers->Add(hgenht);
  //fNumbers->Add(hmassStop);
  fNumbers->Add(hMT2);
  fNumbers->Add(hleadJetPt);
  fNumbers->Add(hleadJetEta);
  fNumbers->Add(hlep1pt);
  fNumbers->Add(hlep1eta);
  fNumbers->Add(hlep2pt);
  fNumbers->Add(hlep2eta);
  fNumbers->Add(halljetpt);
  fNumbers->Add(halljeteta);


  fNumbers->Add(hpfmet_fastsim);
  fNumbers->Add(hmht_fastsim);
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
  fNumbers->Add(hngoodleps_fastsim);
  fNumbers->Add(hngoodbtags_fastsim);
  //fNumbers->Add(hgenht_fastsim);
  fNumbers->Add(hmassStop_fastsim);
  fNumbers->Add(hMT2_fastsim);
  fNumbers->Add(hleadJetPt_fastsim);
  fNumbers->Add(hleadJetEta_fastsim);
  fNumbers->Add(hlep1pt_fastsim);
  fNumbers->Add(hlep1eta_fastsim);
  fNumbers->Add(hlep2pt_fastsim);
  fNumbers->Add(hlep2eta_fastsim);
  fNumbers->Add(halljetpt_fastsim);
  fNumbers->Add(halljeteta_fastsim);


  fNumbers->Add(cpfmet);
  fNumbers->Add(cmht);
  fNumbers->Add(cmt_met_lep);
  fNumbers->Add(cngoodjets);
  fNumbers->Add(clep1_dphiMET);
  fNumbers->Add(cmindphi_met_j1_j2);
  fNumbers->Add(ctopnessMod);
  fNumbers->Add(cngoodleps);
  fNumbers->Add(cngoodbtags);
  fNumbers->Add(cMT2);
  fNumbers->Add(cleadJetPt);
  fNumbers->Add(cleadJetEta);
  fNumbers->Add(clep1pt);
  fNumbers->Add(clep1eta);
  fNumbers->Add(clep2pt);
  fNumbers->Add(clep2eta);
  fNumbers->Add(calljetpt);
  fNumbers->Add(calljeteta);

  //fNumbers->Add(cmassStop);
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
