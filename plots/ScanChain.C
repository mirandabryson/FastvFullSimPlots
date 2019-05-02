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
#include "TRatioPlot.h"

// StopCMS3
#include "StopCMS3.cc"

using namespace std;
using namespace stoptas;

inline bool sortIndexbyCSV( pair<int, float> &vec1, pair<int, float> &vec2 ) {
    return vec1.second > vec2.second;
}

float getdphi( float phi1 , float phi2 ){                                                                                                                                      
  float dphi = fabs( phi1 - phi2 );
  if( dphi > TMath::Pi() ) dphi = TMath::TwoPi() - dphi;
  return dphi;
}

int ScanChain(TChain* chain, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // Example Histograms
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");

  TH1F *hpfmet = new TH1F("hpfmet", "pfmet", 40,0,1200);
  hpfmet->SetDirectory(rootdir);
  TH1F *hmht = new TH1F("hmht", "mht", 40, 0, 800);
  hmht->SetDirectory(rootdir);
  //TH1F *hv2met = new TH1F("hv2met", "v2met", 40, 0, 800);
  //hv2met->SetDirectory(rootdir);
  TH1F *hmt_met_lep = new TH1F("hmt_met_lep", "mt_met_lep", 20, 0, 400);
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
  TH1F *hlep1_dphiMET = new TH1F("hlep1_dphiMET", "lep1_dphiMET", 16, 0, 3.2);
  hlep1_dphiMET->SetDirectory(rootdir);
  TH1F *hmindphi_met_j1_j2 = new TH1F("hmindphi_met_j1_j2", "mindphi_met_j1_j2", 16, 0, 3.2);
  hmindphi_met_j1_j2->SetDirectory(rootdir);
  TH1F *hMT2 = new TH1F("hMT2", "MT2", 40, 0, 800);
  hMT2->SetDirectory(rootdir);
  TH1F *hMT2ll = new TH1F("hMT2ll", "MT2ll", 40, 0, 800);
  hMT2ll->SetDirectory(rootdir);
  TH1F *hMCT = new TH1F("hMCT", "MCT", 40, 0, 800);
  hMCT->SetDirectory(rootdir);
  TH1F *htopnessMod = new TH1F("htopnessMod", "topnessMod", 23, -10, 13);
  htopnessMod->SetDirectory(rootdir);
  TH1F *hngoodleps = new TH1F("hngoodleps", "ngoodleps", 4, 0, 4);
  hngoodleps->SetDirectory(rootdir);
  TH1F *hngoodbtags = new TH1F("hngoodbtags", "ngoodbtags", 6, 0, 6);
  hngoodbtags->SetDirectory(rootdir);
  //TH1F *hgenht = new TH1F("hgenht", "genht", 100, 0, 800);
  //hgenht->SetDirectory(rootdir);
  //TH1F *hmassStop = new TH1F("hmassStop", "mass_stop",100, 0, 1800);
  //hmassStop->SetDirectory(rootdir);
  TH1F *hleadJetPt = new TH1F("hleadJetPt", "leadJetPt", 60, 0, 1200);
  hleadJetPt->SetDirectory(rootdir);
  TH1F *hlep1pt = new TH1F("hlep1pt", "lep1pt", 20, 0, 200);
  hlep1pt->SetDirectory(rootdir);
  TH1F *hlep1eta = new TH1F("hlep1eta", "lep1eta", 20, -2.4, 2.4);
  hlep1eta->SetDirectory(rootdir);
  TH1F *hlep2pt = new TH1F("hlep2pt", "lep2pt", 20, 0, 200);
  hlep1pt->SetDirectory(rootdir);
  TH1F *hlep2eta = new TH1F("hlep2eta", "lep2eta", 20, -2.4, 2.4);
  hlep2eta->SetDirectory(rootdir);
  TH1F *halljetpt = new TH1F("halljetpt", "alljetpt", 20, 0, 400);
  halljetpt->SetDirectory(rootdir);
  TH1F *halljeteta = new TH1F("halljeteta", "alljeteta", 20, -2.4, 2.4);
  halljeteta->SetDirectory(rootdir);
  TH1F *hleadJetEta = new TH1F("hleadJetEta", "leadJetEta", 20, -2.4, 2.4);
  hleadJetEta->SetDirectory(rootdir);
  TH1F *hmetres = new TH1F("hmetres", "metres", 100, -100, 100);
  hmetres->SetDirectory(rootdir);
  //  TH2F *hgenvpfmet = new TH2F("hgenvpfmet", "genvpfmet", 40, 0, 1200, 40, 0, 1200);
  //  hgenvpfmet->SetDirectory(rootdir);

  //Defining fastsim histos
  TH1F *hpfmet_fastsim = new TH1F("hpfmet_fastsim", "pfmet", 40,0,1200);
  hpfmet_fastsim->SetDirectory(rootdir);
  TH1F *hpfmet_fastsim_filt = new TH1F("hpfmet_fastsim_filt", "pfmet", 40,0,1200);
  hpfmet_fastsim->SetDirectory(rootdir);
  TH1F *hmht_fastsim = new TH1F("hmht_fastsim", "mht", 40, 0, 800);
  hmht_fastsim->SetDirectory(rootdir);
  //TH1F *hv2met_fastsim = new TH1F("hv2met_fastsim", "v2met", 40, 0, 800);
  //hv2met_fastsim->SetDirectory(rootdir);
  TH1F *hmt_met_lep_fastsim = new TH1F("hmt_met_lep_fastsim", "mt_met_lep", 20, 0, 400);
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
  TH1F *hlep1_dphiMET_fastsim = new TH1F("hlep1_dphiMET_fastsim", "lep1_dphiMET", 16, 0, 3.2);
  hlep1_dphiMET_fastsim->SetDirectory(rootdir);
  TH1F *hmindphi_met_j1_j2_fastsim = new TH1F("hmindphi_met_j1_j2_fastsim", "hmindphi_met_j1_j2", 16, 0, 3.2);
  hmindphi_met_j1_j2_fastsim->SetDirectory(rootdir);
  TH1F *hMT2_fastsim = new TH1F("hMT2_fastsim", "MT2", 40, 0, 800);
  hMT2_fastsim->SetDirectory(rootdir);
  TH1F *hMT2ll_fastsim = new TH1F("hMT2ll_fastsim", "MT2ll", 40, 0, 800);
  hMT2ll_fastsim->SetDirectory(rootdir);
  TH1F *hMCT_fastsim = new TH1F("hMCT_fastsim", "MCT", 40, 0, 800);
  hMCT_fastsim->SetDirectory(rootdir);
  TH1F *htopnessMod_fastsim = new TH1F("htopnessMod_fastsim", "topnessMod", 23, -10, 13);
  htopnessMod_fastsim->SetDirectory(rootdir);
  TH1F *hngoodleps_fastsim = new TH1F("hngoodleps_fastsim", "ngoodleps", 4, 0, 4);
  hngoodleps_fastsim->SetDirectory(rootdir);
  TH1F *hngoodbtags_fastsim = new TH1F("hngoodbtags_fastsim", "ngoodbtags", 6, 0, 6);
  hngoodbtags_fastsim->SetDirectory(rootdir);
  //TH1F *hgenht_fastsim = new TH1F("hgenht_fastsim", "genht", 100, 0, 800);
  //hgenht_fastsim->SetDirectory(rootdir);
  TH1F *hmassStop_fastsim = new TH1F("hmassStop_fastsim", "mass_stop", 100, 0, 1800);
  hmassStop_fastsim->SetDirectory(rootdir); 
  TH1F *hleadJetPt_fastsim = new TH1F("hleadJetPt_fastsim", "leadJetPt", 60, 0, 1200);
  hleadJetPt_fastsim->SetDirectory(rootdir);
  TH1F *hlep1pt_fastsim = new TH1F("hlep1pt_fastsim", "lep1pt", 20, 0, 200);
  hlep1pt_fastsim->SetDirectory(rootdir);
  TH1F *hlep1eta_fastsim = new TH1F("hlep1eta_fastsim", "lep1eta", 20, -2.4, 2.4);
  hlep1eta_fastsim->SetDirectory(rootdir);
  TH1F *hlep2pt_fastsim = new TH1F("hlep2pt_fastsim", "lep2pt", 20, 0, 200);
  hlep1pt_fastsim->SetDirectory(rootdir);
  TH1F *hlep2eta_fastsim = new TH1F("hlep2eta_fastsim", "lep2eta", 20, -2.4, 2.4);
  hlep2eta_fastsim->SetDirectory(rootdir);
  TH1F *halljetpt_fastsim = new TH1F("halljetpt_fastsim", "alljetpt", 20, 0, 400);
  halljetpt_fastsim->SetDirectory(rootdir);
  TH1F *halljeteta_fastsim = new TH1F("halljeteta_fastsim", "alljeteta", 20, -2.4, 2.4);
  halljeteta_fastsim->SetDirectory(rootdir);
  TH1F *hleadJetEta_fastsim = new TH1F("hleadJetEta_fastsim", "leadJetEta", 20, -2.4, 2.4);
  hleadJetEta_fastsim->SetDirectory(rootdir);
  TH1F *hmetres_fastsim = new TH1F("hmetres_fastsim", "metres", 100, -100, 100);
  hmetres_fastsim->SetDirectory(rootdir);

  //  TH2F *hgenvpfmet_fastsim = new TH2F("hgenvpfmet_fastsim", "genvpfmet", 40, 0, 1200, 40, 0, 1200);
  //  hgenvpfmet->SetDirectory(rootdir);

  //Canvas definition
  TCanvas *cpfmet = new TCanvas("cpfmet","cpfmet",800,800);
  TCanvas *cpfmet_filt = new TCanvas("cpfmet_filt","cpfme_filt",800,800);
  TCanvas *cmt_met_lep = new TCanvas("cmt_met_lep","cmt_met_lep",800,800);
  TCanvas *cngoodjets = new TCanvas("cngoodjets","cngoodjets",800,800);
  TCanvas *clep1_dphiMET = new TCanvas("clep1_dphiMET","clep1_dphiMET",800,800);
  TCanvas *cmindphi_met_j1_j2 = new TCanvas("cmindphi_met_j1_j2","cmindphi_met_j1_j2",800,800);
  TCanvas *ctopnessMod = new TCanvas("ctopnessMod","ctopnessMod",800,800);
  TCanvas *cngoodleps = new TCanvas("cngoodleps","cngoodleps",800,800);
  TCanvas *cngoodbtags = new TCanvas("cngoodbtags","cngoodbtags",800,800);
  TCanvas *cmht = new TCanvas("cmht","cmht",800,800);
  TCanvas *cMT2 = new TCanvas("cMT2","cMT2",800,800);
  TCanvas *cMT2ll = new TCanvas("cMT2ll","cMT2ll",800,800);
  TCanvas *cMCT = new TCanvas("cMCT","cMCT",800,800);
  TCanvas *cleadJetPt = new TCanvas("cleadJetPt","cleadJetPt",800,800);
  TCanvas *clep1pt = new TCanvas("clep1pt","clep1pt",800,800);
  TCanvas *clep1eta = new TCanvas("clep1eta","clep1eta",800,800);
  TCanvas *clep2pt = new TCanvas("clep2pt","clep2pt",800,800);
  TCanvas *clep2eta = new TCanvas("clep2eta","clep2eta",800,800);
  TCanvas *calljetpt = new TCanvas("calljetpt","calljetpt",800,800);
  TCanvas *calljeteta = new TCanvas("calljeteta","calljeteta",800,800);
  TCanvas *cleadJetEta = new TCanvas("cleadJetEta", "cleadJetEta", 800, 800);
  TCanvas *cmetres = new TCanvas("cmetres", "cmetres", 800, 800);
  //  TCanvas *cgenvpfmet = new TCanvas("cgenvpfmet", "cgenvpfmet", 800, 800);

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
//cout<<"good leps = "<<ngoodleps()<<" vetoleps =  "<<nvetoleps()<<" jets = "<<ak4pfjets_p4().size()<<endl;
      if(ngoodleps() >0) continue;
      if((fileName.find(fileCut)!= string::npos) &&  mass_stop() !=1200) continue;
      if((fileName.find(fileCut)!= string::npos) &&  mass_lsp() !=800) continue;
      //cout<<"Gluino Mass"<<mass_stop()<<" LSP mass"<<mass_lsp()<<endl;
      
      // Progress
      StopCMS3::progress( nEventsTotal, nEventsChain );

      // Analysis Code
      // this is where we put cuts like pfmet>150 : if(pfmet()<150) continue;

      TH2D* h_sig_counter_nEvents = nullptr;
      float lumi = 41.5;
      
      if(fileName.find(fileCut) != string::npos){
	//if "fastsim" is in the file title, fill only the fastsim histos

	h_sig_counter_nEvents = (TH2D*) file.Get("histNEvts");
	int nEventsPoint = h_sig_counter_nEvents->GetBinContent(h_sig_counter_nEvents->FindBin(mass_stop(), mass_lsp()));
        float evtweight_ = 0.0985 * 1000 / nEventsPoint;
	float totalWeight = lumi*evtweight_*weight_btagsf();
	
	//Fill Histograms
	hpfmet_fastsim->Fill(pfmet(),totalWeight);
        if(filt_met()) hpfmet_fastsim_filt->Fill(pfmet(),totalWeight);
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
        hMT2ll_fastsim->Fill(MT2_ll(),totalWeight);
        //mct
        //
        if(ak4pfjets_p4().size()>1 && ngoodbtags()>0){
          vector <pair<int, float>> jet_csv_pairs;
          //Only 30 GeV jets passing ID, inside eta 2.4 should survive to babies.
          for(uint ijet=0;ijet<ak4pfjets_deepCSVb().size();ijet++){
            float btagvalue = ak4pfjets_deepCSVb().at(ijet) + ak4pfjets_deepCSVbb().at(ijet);
            jet_csv_pairs.push_back(make_pair(ijet,btagvalue));
          }
          sort( jet_csv_pairs.begin(), jet_csv_pairs.end(), sortIndexbyCSV);
          float ptb1,ptb2,dPhibb;
          ptb1 = ak4pfjets_p4().at(jet_csv_pairs.at(0).first).pt();
          ptb2 = ak4pfjets_p4().at(jet_csv_pairs.at(1).first).pt();
          dPhibb = getdphi(ak4pfjets_p4().at(jet_csv_pairs.at(0).first).phi(),ak4pfjets_p4().at(jet_csv_pairs.at(1).first).phi());
          float mct = sqrt(2*ptb1*ptb2*(1+cos(dPhibb)));  
  	  hMCT_fastsim->Fill(mct,totalWeight);
        }

	htopnessMod_fastsim->Fill(topnessMod(),totalWeight);
	hngoodleps_fastsim->Fill(ngoodleps(),totalWeight);
	hngoodbtags_fastsim->Fill(ngoodbtags(),totalWeight);
	//hgenht_fastsim->Fill(genht(),totalWeight);
	hmassStop_fastsim->Fill(mass_stop(),totalWeight);
	if(ak4pfjets_p4().size()>1) hleadJetPt_fastsim->Fill(ak4pfjets_p4().at(0).pt(),totalWeight);
	if(ak4pfjets_p4().size()>1) hleadJetEta_fastsim->Fill(ak4pfjets_p4().at(0).eta(),totalWeight);
	hlep1pt_fastsim->Fill(lep1_p4().pt(), totalWeight);
	hlep1eta_fastsim->Fill(lep1_p4().eta(), totalWeight);
	hlep2pt_fastsim->Fill(lep2_p4().pt(), totalWeight);
	hlep2eta_fastsim->Fill(lep2_p4().eta(), totalWeight);
	for(unsigned int ijets = 0; ijets < ak4pfjets_p4().size(); ijets++){
	  halljetpt_fastsim->Fill(ak4pfjets_p4().at(ijets).pt(), totalWeight);
	  halljeteta_fastsim->Fill(ak4pfjets_p4().at(ijets).eta(), totalWeight);

	}
	hmetres_fastsim->Fill((genmet()-pfmet()), totalWeight);
	//	hgenvpfmet_fastsim->Fill(genmet(),pfmet(), totalWeight);

      } else{
	//if "fastsim" isn't in the title, then fill these histos instead

	float totalWeightFull = lumi*scale1fb()*weight_btagsf();
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
        hMT2ll->Fill(MT2_ll(),totalWeightFull);
        //mct
        //
        if(ak4pfjets_p4().size()>1 && ngoodbtags()>0){
          vector <pair<int, float>> jet_csv_pairs;
          //Only 30 GeV jets passing ID, inside eta 2.4 should survive to babies.
          for(uint ijet=0;ijet<ak4pfjets_deepCSVb().size();ijet++){
            float btagvalue = ak4pfjets_deepCSVb().at(ijet) + ak4pfjets_deepCSVbb().at(ijet);
            jet_csv_pairs.push_back(make_pair(ijet,btagvalue));
          }
          sort( jet_csv_pairs.begin(), jet_csv_pairs.end(), sortIndexbyCSV);
          float ptb1,ptb2,dPhibb;
          ptb1 = ak4pfjets_p4().at(jet_csv_pairs.at(0).first).pt();
          ptb2 = ak4pfjets_p4().at(jet_csv_pairs.at(1).first).pt();
          dPhibb = getdphi(ak4pfjets_p4().at(jet_csv_pairs.at(0).first).phi(),ak4pfjets_p4().at(jet_csv_pairs.at(1).first).phi());
          float mct = sqrt(2*ptb1*ptb2*(1+cos(dPhibb)));
          hMCT->Fill(mct,totalWeightFull);
        }
	htopnessMod->Fill(topnessMod(),totalWeightFull);
	hngoodleps->Fill(ngoodleps(),totalWeightFull);
	hngoodbtags->Fill(ngoodbtags(),totalWeightFull);
	//hgenht->Fill(genht(),totalWeightFull);
	//hmassStop->Fill(mass_stop(),totalWeightFull);
	if(ak4pfjets_p4().size()>1) hleadJetPt->Fill(ak4pfjets_p4().at(0).pt(),totalWeightFull);
	if(ak4pfjets_p4().size()>1) hleadJetEta->Fill(ak4pfjets_p4().at(0).eta(), totalWeightFull);
	hlep1pt->Fill(lep1_p4().pt(), totalWeightFull);
	hlep1eta->Fill(lep1_p4().eta(), totalWeightFull);
	hlep2pt->Fill(lep2_p4().pt(), totalWeightFull);
	hlep2eta->Fill(lep2_p4().eta(), totalWeightFull);

	hmetres->Fill((genmet()-pfmet()), totalWeightFull);
	//	hgenvpfmet->Fill(genmet(),pfmet(), totalWeightFull);

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
  cpfmet->SetLogy();
 //overflow bin
  hpfmet_fastsim->SetBinContent(hpfmet_fastsim->GetNbinsX(), hpfmet_fastsim->GetBinContent(hpfmet_fastsim->GetNbinsX())+hpfmet_fastsim->GetBinContent(hpfmet_fastsim->GetNbinsX()+1));
  hpfmet->SetBinContent(hpfmet->GetNbinsX(), hpfmet->GetBinContent(hpfmet->GetNbinsX())+hpfmet->GetBinContent(hpfmet->GetNbinsX()+1));
  hpfmet->SetStats(false);
  hpfmet->SetLineWidth(3);
  hpfmet->SetLineColor(kRed);
  hpfmet->GetXaxis()->SetTitle("E_{T}^{miss} [GeV]");
  hpfmet_fastsim->SetStats(false);
  hpfmet_fastsim->SetLineWidth(3);
  hpfmet_fastsim->SetLineColor(kBlue);
  hpfmet->Scale(1./(hpfmet->Integral()));
  hpfmet_fastsim->Scale(1./(hpfmet_fastsim->Integral()));
  hpfmet->Draw();
  hpfmet_fastsim->Draw("same");

  auto rp = new TRatioPlot(hpfmet_fastsim,hpfmet);
  rp->Draw("fhidelow");
  rp->GetLowerRefGraph()->SetMinimum(0.5);
  rp->GetLowerRefGraph()->SetMaximum(1.5);
//  rp->GetLowerRefYaxis()->SetLabelSize(0.1);
  rp->GetLowerRefYaxis()->SetTitle("ratio");
  rp->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp->GetLowerRefYaxis()->SetNdivisions(303);
  rp->SetLowTopMargin(0.3);
  rp->GetUpperPad()->cd();
  TLegend* leg0 = new TLegend(0.55,0.78,0.89,0.89);
  leg0->AddEntry(hpfmet,"FullSim Sample");
  leg0->AddEntry(hpfmet_fastsim,"FastSim Sample");
  leg0->SetBorderSize(0);
  leg0->Draw();
  cpfmet->Update();

  cpfmet_filt->cd();
  cpfmet_filt->SetLogy();
 //overflow bin
  hpfmet_fastsim_filt->SetBinContent(hpfmet_fastsim_filt->GetNbinsX(), hpfmet_fastsim_filt->GetBinContent(hpfmet_fastsim_filt->GetNbinsX())+hpfmet_fastsim_filt->GetBinContent(hpfmet_fastsim_filt->GetNbinsX()+1));
  hpfmet_fastsim->SetBinContent(hpfmet_fastsim->GetNbinsX(), hpfmet_fastsim->GetBinContent(hpfmet_fastsim->GetNbinsX())+hpfmet_fastsim->GetBinContent(hpfmet_fastsim->GetNbinsX()+1));
  hpfmet_fastsim->SetStats(false);
  hpfmet_fastsim->SetLineWidth(3);
  hpfmet_fastsim->SetLineColor(kBlue);
  hpfmet_fastsim->GetXaxis()->SetTitle("E_{T}^{miss} [GeV]");
  hpfmet_fastsim_filt->SetStats(false);
  hpfmet_fastsim_filt->SetLineWidth(3);
  hpfmet_fastsim_filt->SetLineColor(kRed);
  hpfmet_fastsim->Scale(1./(hpfmet_fastsim->Integral()));
  hpfmet_fastsim_filt->Scale(1./(hpfmet_fastsim_filt->Integral()));
  hpfmet_fastsim->Draw();
  hpfmet_fastsim_filt->Draw("same");
  cout<<"MET filt "<<(float)(hpfmet_fastsim_filt->Integral()/hpfmet_fastsim->Integral())<<endl;

  auto rpx = new TRatioPlot(hpfmet_fastsim_filt,hpfmet_fastsim);
  rpx->Draw("fhidelow");
  rpx->GetLowerRefGraph()->SetMinimum(0.8);
  rpx->GetLowerRefGraph()->SetMaximum(1.2);
//  rp->GetLowerRefYaxis()->SetLabelSize(0.1);
  rpx->GetLowerRefYaxis()->SetTitle("ratio");
  rpx->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rpx->GetLowerRefYaxis()->SetLabelSize(0.03);
  rpx->GetLowerRefYaxis()->SetTitleSize(0.03);
  rpx->GetLowerRefYaxis()->SetNdivisions(303);
  rpx->SetLowTopMargin(0.3);
  rpx->GetUpperPad()->cd();
  TLegend* leg0x = new TLegend(0.55,0.78,0.89,0.89);
  leg0x->AddEntry(hpfmet_fastsim_filt,"FastSim w/ MET filt.");
  leg0x->AddEntry(hpfmet_fastsim,"FastSim no MET filt.");
  leg0x->SetBorderSize(0);
  leg0x->Draw();
  cpfmet_filt->Update();

  cmt_met_lep->cd();
  cmt_met_lep->SetLogy();
  hmt_met_lep_fastsim->SetBinContent(hmt_met_lep_fastsim->GetNbinsX(), hmt_met_lep_fastsim->GetBinContent(hmt_met_lep_fastsim->GetNbinsX())+hmt_met_lep_fastsim->GetBinContent(hmt_met_lep_fastsim->GetNbinsX()+1));
  hmt_met_lep->SetBinContent(hmt_met_lep->GetNbinsX(), hmt_met_lep->GetBinContent(hmt_met_lep->GetNbinsX())+hmt_met_lep->GetBinContent(hmt_met_lep->GetNbinsX()+1));
  hmt_met_lep->SetStats(false);
  hmt_met_lep->SetLineWidth(3);
  hmt_met_lep->SetLineColor(kRed);
  hmt_met_lep->GetXaxis()->SetTitle("M_{T} [GeV]");
  hmt_met_lep_fastsim->SetStats(false);
  hmt_met_lep_fastsim->SetLineWidth(3);
  hmt_met_lep_fastsim->SetLineColor(kBlue);
  hmt_met_lep->Scale(1./(hmt_met_lep->Integral()));
  hmt_met_lep_fastsim->Scale(1./(hmt_met_lep_fastsim->Integral()));
  hmt_met_lep->Draw();
  hmt_met_lep_fastsim->Draw("same");

  auto rp1 = new TRatioPlot(hmt_met_lep_fastsim,hmt_met_lep);
  rp1->Draw();
  rp1->GetLowerRefGraph()->SetMinimum(0.5);
  rp1->GetLowerRefGraph()->SetMaximum(1.5);
//  rp->GetLowerRefYaxis()->SetLabelSize(0.1);
  rp1->GetLowerRefYaxis()->SetTitle("ratio");
  rp1->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp1->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp1->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp1->GetLowerRefYaxis()->SetNdivisions(303);
  rp1->SetLowTopMargin(0.3);
  rp1->GetUpperPad()->cd();
  TLegend* leg1 = new TLegend(0.55,0.78,0.89,0.89);
  leg1->SetBorderSize(0);
  leg1->AddEntry(hmt_met_lep,"FullSim Sample");
  leg1->AddEntry(hmt_met_lep_fastsim,"FastSim Sample");
  leg1->Draw();
  cmt_met_lep->Update();
  
  cngoodjets->cd();
  cngoodjets->SetLogy();
  hngoodjets_fastsim->SetBinContent(hngoodjets_fastsim->GetNbinsX(), hngoodjets_fastsim->GetBinContent(hngoodjets_fastsim->GetNbinsX())+hngoodjets_fastsim->GetBinContent(hngoodjets_fastsim->GetNbinsX()+1));
  hngoodjets->SetBinContent(hngoodjets->GetNbinsX(), hngoodjets->GetBinContent(hngoodjets->GetNbinsX())+hngoodjets->GetBinContent(hngoodjets->GetNbinsX()+1));
  hngoodjets->SetStats(false);
  hngoodjets->SetLineWidth(3);
  hngoodjets->SetLineColor(kRed);
  hngoodjets->GetXaxis()->SetTitle("nJets");
  hngoodjets_fastsim->SetStats(false);
  hngoodjets_fastsim->SetLineWidth(3);
  hngoodjets_fastsim->SetLineColor(kBlue);
  hngoodjets->Scale(1./(hngoodjets->Integral()));
  hngoodjets_fastsim->Scale(1./(hngoodjets_fastsim->Integral()));
  hngoodjets->Draw();
  hngoodjets->GetYaxis()->SetRangeUser(0.00001,5.);
  hngoodjets_fastsim->Draw("same");

  auto rp2 = new TRatioPlot(hngoodjets_fastsim,hngoodjets);
  rp2->Draw();
  rp2->GetLowerRefGraph()->SetMinimum(0.5);
  rp2->GetLowerRefGraph()->SetMaximum(1.5);
//  rp->GetLowerRefYaxis()->SetLabelSize(0.1);
  rp2->GetLowerRefYaxis()->SetTitle("ratio");
  rp2->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp2->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp2->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp2->GetLowerRefYaxis()->SetNdivisions(303);
  rp2->SetLowTopMargin(0.3);
  rp2->GetUpperPad()->cd();
  TLegend* leg2 = new TLegend(0.55,0.75,0.89,0.87);
  leg2->SetBorderSize(0);
  leg2->AddEntry(hngoodjets,"FullSim Sample");
  leg2->AddEntry(hngoodjets_fastsim,"FastSim Sample");
  leg2->Draw();
  cngoodjets->Update();

  clep1_dphiMET->cd();
  clep1_dphiMET->SetLogy();
  hlep1_dphiMET_fastsim->SetBinContent(hlep1_dphiMET_fastsim->GetNbinsX(), hlep1_dphiMET_fastsim->GetBinContent(hlep1_dphiMET_fastsim->GetNbinsX())+hlep1_dphiMET_fastsim->GetBinContent(hlep1_dphiMET_fastsim->GetNbinsX()+1));
  hlep1_dphiMET->SetBinContent(hlep1_dphiMET->GetNbinsX(), hlep1_dphiMET->GetBinContent(hlep1_dphiMET->GetNbinsX())+hlep1_dphiMET->GetBinContent(hlep1_dphiMET->GetNbinsX()+1));
  hlep1_dphiMET->SetStats(false);
  hlep1_dphiMET->SetLineWidth(3);
  hlep1_dphiMET->SetLineColor(kRed);
  hlep1_dphiMET->GetXaxis()->SetTitle("#Delta#phi_{E_{T}^{miss},lep}");
  hlep1_dphiMET_fastsim->SetStats(false);
  hlep1_dphiMET_fastsim->SetLineWidth(3);
  hlep1_dphiMET_fastsim->SetLineColor(kBlue);
  hlep1_dphiMET->Scale(1./(hlep1_dphiMET->Integral()));
  hlep1_dphiMET_fastsim->Scale(1./(hlep1_dphiMET_fastsim->Integral()));
  hlep1_dphiMET->Draw();
  hlep1_dphiMET->GetYaxis()->SetRangeUser(0.00001,5.);
  hlep1_dphiMET_fastsim->Draw("same");

  auto rp3 = new TRatioPlot(hlep1_dphiMET_fastsim,hlep1_dphiMET);
  rp3->Draw();
  rp3->GetLowerRefGraph()->SetMinimum(0.5);
  rp3->GetLowerRefGraph()->SetMaximum(1.5);
  rp3->GetLowerRefYaxis()->SetTitle("ratio");
  rp3->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp3->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp3->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp3->GetLowerRefYaxis()->SetNdivisions(303);
  rp3->SetLowTopMargin(0.3);
  rp3->GetUpperPad()->cd();
  TLegend* leg3 = new TLegend(0.55,0.75,0.89,0.87);
  leg3->SetBorderSize(0);
  leg3->AddEntry(hlep1_dphiMET,"FullSim Sample");
  leg3->AddEntry(hlep1_dphiMET_fastsim,"FastSim Sample");
  leg3->Draw();
  clep1_dphiMET->Update();

  cmindphi_met_j1_j2->cd();
  cmindphi_met_j1_j2->SetLogy();
  hmindphi_met_j1_j2_fastsim->SetBinContent(hmindphi_met_j1_j2_fastsim->GetNbinsX(), hmindphi_met_j1_j2_fastsim->GetBinContent(hmindphi_met_j1_j2_fastsim->GetNbinsX())+hmindphi_met_j1_j2_fastsim->GetBinContent(hmindphi_met_j1_j2_fastsim->GetNbinsX()+1));
  hmindphi_met_j1_j2->SetBinContent(hmindphi_met_j1_j2->GetNbinsX(), hmindphi_met_j1_j2->GetBinContent(hmindphi_met_j1_j2->GetNbinsX())+hmindphi_met_j1_j2->GetBinContent(hmindphi_met_j1_j2->GetNbinsX()+1));
  hmindphi_met_j1_j2->SetStats(false);
  hmindphi_met_j1_j2->SetLineWidth(3);
  hmindphi_met_j1_j2->SetLineColor(kRed);
  hmindphi_met_j1_j2->GetXaxis()->SetTitle("min #Delta#phi_{j_{1},j_{2}}");
  hmindphi_met_j1_j2_fastsim->SetStats(false);
  hmindphi_met_j1_j2_fastsim->SetLineWidth(3);
  hmindphi_met_j1_j2_fastsim->SetLineColor(kBlue);
  hmindphi_met_j1_j2->Scale(1./(hmindphi_met_j1_j2->Integral()));
  hmindphi_met_j1_j2_fastsim->Scale(1./(hmindphi_met_j1_j2_fastsim->Integral()));
  hmindphi_met_j1_j2->Draw();
  hmindphi_met_j1_j2->GetYaxis()->SetRangeUser(0.01,0.5);
  hmindphi_met_j1_j2_fastsim->Draw("same");

  auto rp4 = new TRatioPlot(hmindphi_met_j1_j2_fastsim,hmindphi_met_j1_j2);
  rp4->Draw();
  rp4->GetLowerRefGraph()->SetMinimum(0.5);
  rp4->GetLowerRefGraph()->SetMaximum(1.5);
  rp4->GetLowerRefYaxis()->SetTitle("ratio");
  rp4->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp4->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp4->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp4->GetLowerRefYaxis()->SetNdivisions(303);
  rp4->SetLowTopMargin(0.3);
  rp4->GetUpperPad()->cd();
  TLegend* leg4 = new TLegend(0.55,0.75,0.89,0.87);
  leg4->SetBorderSize(0);
  leg4->AddEntry(hmindphi_met_j1_j2,"FullSim Sample");
  leg4->AddEntry(hmindphi_met_j1_j2_fastsim,"FastSim Sample");
  leg4->Draw();
  cmindphi_met_j1_j2->Update();

  ctopnessMod->cd();
  ctopnessMod->SetLogy();
  htopnessMod_fastsim->SetBinContent(htopnessMod_fastsim->GetNbinsX(), htopnessMod_fastsim->GetBinContent(htopnessMod_fastsim->GetNbinsX())+htopnessMod_fastsim->GetBinContent(htopnessMod_fastsim->GetNbinsX()+1));
  htopnessMod->SetBinContent(htopnessMod->GetNbinsX(), htopnessMod->GetBinContent(htopnessMod->GetNbinsX())+htopnessMod->GetBinContent(htopnessMod->GetNbinsX()+1));
  htopnessMod->SetStats(false);
  htopnessMod->SetLineWidth(3);
  htopnessMod->SetLineColor(kRed);
  htopnessMod->GetXaxis()->SetTitle("Modified Topness");
  htopnessMod_fastsim->SetStats(false);
  htopnessMod_fastsim->SetLineWidth(3);
  htopnessMod_fastsim->SetLineColor(kBlue);
  htopnessMod->Scale(1./(htopnessMod->Integral()));
  htopnessMod_fastsim->Scale(1./(htopnessMod_fastsim->Integral()));
  htopnessMod->Draw();
  htopnessMod_fastsim->Draw("same");

  auto rp5 = new TRatioPlot(htopnessMod_fastsim,htopnessMod);
  rp5->Draw();
  rp5->GetLowerRefGraph()->SetMinimum(0.5);
  rp5->GetLowerRefGraph()->SetMaximum(1.5);
  rp5->GetLowerRefYaxis()->SetTitle("ratio");
  rp5->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp5->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp5->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp5->GetLowerRefYaxis()->SetNdivisions(303);
  rp5->SetLowTopMargin(0.3);
  rp5->GetUpperPad()->cd();
  TLegend* leg5 = new TLegend(0.55,0.75,0.89,0.87);
  leg5->SetBorderSize(0);
  leg5->AddEntry(htopnessMod,"FullSim Sample");
  leg5->AddEntry(htopnessMod_fastsim,"FastSim Sample");
  leg5->Draw();
  ctopnessMod->Update();

  cngoodleps->cd();
  cngoodleps->SetLogy();
  hngoodleps_fastsim->SetBinContent(hngoodleps_fastsim->GetNbinsX(), hngoodleps_fastsim->GetBinContent(hngoodleps_fastsim->GetNbinsX())+hngoodleps_fastsim->GetBinContent(hngoodleps_fastsim->GetNbinsX()+1));
  hngoodleps->SetBinContent(hngoodleps->GetNbinsX(), hngoodleps->GetBinContent(hngoodleps->GetNbinsX())+hngoodleps->GetBinContent(hngoodleps->GetNbinsX()+1));
  hngoodleps->SetStats(false);
  hngoodleps->SetLineWidth(3);
  hngoodleps->SetLineColor(kRed);
  hngoodleps->GetXaxis()->SetTitle("nLeptons");
  hngoodleps_fastsim->SetStats(false);
  hngoodleps_fastsim->SetLineWidth(3);
  hngoodleps_fastsim->SetLineColor(kBlue);
  hngoodleps->Scale(1./(hngoodleps->Integral()));
  hngoodleps_fastsim->Scale(1./(hngoodleps_fastsim->Integral()));
  hngoodleps->Draw();
  hngoodleps_fastsim->Draw("same");

  auto rp6 = new TRatioPlot(hngoodleps_fastsim,hngoodleps);
  rp6->Draw();
  rp6->GetLowerRefGraph()->SetMinimum(0.5);
  rp6->GetLowerRefGraph()->SetMaximum(1.5);
  rp6->GetLowerRefYaxis()->SetTitle("ratio");
  rp6->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp6->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp6->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp6->GetLowerRefYaxis()->SetNdivisions(303);
  rp6->SetLowTopMargin(0.3);
  rp6->GetUpperPad()->cd();
  TLegend* leg6 = new TLegend(0.55,0.75,0.89,0.87);
  leg6->SetBorderSize(0);
  leg6->AddEntry(hngoodleps,"FullSim Sample");
  leg6->AddEntry(hngoodleps_fastsim,"FastSim Sample");
  leg6->Draw();
  cngoodleps->Update();

  cngoodbtags->cd();
  cngoodbtags->SetLogy();
  hngoodbtags_fastsim->SetBinContent(hngoodbtags_fastsim->GetNbinsX(), hngoodbtags_fastsim->GetBinContent(hngoodbtags_fastsim->GetNbinsX())+hngoodbtags_fastsim->GetBinContent(hngoodbtags_fastsim->GetNbinsX()+1));
  hngoodbtags->SetBinContent(hngoodbtags->GetNbinsX(), hngoodbtags->GetBinContent(hngoodbtags->GetNbinsX())+hngoodbtags->GetBinContent(hngoodbtags->GetNbinsX()+1));
  hngoodbtags->SetStats(false);
  hngoodbtags->SetLineWidth(3);
  hngoodbtags->SetLineColor(kRed);
  hngoodbtags->GetXaxis()->SetTitle("nb-tags");
  hngoodbtags_fastsim->SetStats(false);
  hngoodbtags_fastsim->SetLineWidth(3);
  hngoodbtags_fastsim->SetLineColor(kBlue);
  hngoodbtags->Scale(1./(hngoodbtags->Integral()));
  hngoodbtags_fastsim->Scale(1./(hngoodbtags_fastsim->Integral()));
  hngoodbtags->Draw();
  hngoodbtags_fastsim->Draw("same");

  auto rp7 = new TRatioPlot(hngoodbtags_fastsim,hngoodbtags);
  rp7->Draw();
  rp7->GetLowerRefGraph()->SetMinimum(0.5);
  rp7->GetLowerRefGraph()->SetMaximum(1.5);
  rp7->GetLowerRefYaxis()->SetTitle("ratio");
  rp7->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp7->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp7->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp7->GetLowerRefYaxis()->SetNdivisions(303);
  rp7->SetLowTopMargin(0.3);
  rp7->GetUpperPad()->cd();
  TLegend* leg7 = new TLegend(0.55,0.75,0.89,0.87);
  leg7->SetBorderSize(0);
  leg7->AddEntry(hngoodbtags,"FullSim Sample");
  leg7->AddEntry(hngoodbtags_fastsim,"FastSim Sample");
  leg7->Draw();
  cngoodbtags->Update();

  cmht->cd();
  cmht->SetLogy();
  hmht_fastsim->SetBinContent(hmht_fastsim->GetNbinsX(), hmht_fastsim->GetBinContent(hmht_fastsim->GetNbinsX())+hmht_fastsim->GetBinContent(hmht_fastsim->GetNbinsX()+1));
  hmht->SetBinContent(hmht->GetNbinsX(), hmht->GetBinContent(hmht->GetNbinsX())+hmht->GetBinContent(hmht->GetNbinsX()+1));
  hmht->SetStats(false);
  hmht->SetLineWidth(3);
  hmht->SetLineColor(kRed);
  hmht->GetXaxis()->SetTitle("H_{T}^{miss} [GeV]");
  hmht_fastsim->SetStats(false);
  hmht_fastsim->SetLineWidth(3);
  hmht_fastsim->SetLineColor(kBlue);
  hmht->Scale(1./(hmht->Integral()));
  hmht_fastsim->Scale(1./(hmht_fastsim->Integral()));
  hmht->Draw();
  hmht_fastsim->Draw("same");

  auto rp8 = new TRatioPlot(hmht_fastsim,hmht);
  rp8->Draw();
  rp8->GetLowerRefGraph()->SetMinimum(0.5);
  rp8->GetLowerRefGraph()->SetMaximum(1.5);
  rp8->GetLowerRefYaxis()->SetTitle("ratio");
  rp8->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp8->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp8->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp8->GetLowerRefYaxis()->SetNdivisions(303);
  rp8->SetLowTopMargin(0.3);
  rp8->GetUpperPad()->cd();
  TLegend* leg71 = new TLegend(0.55,0.75,0.89,0.87);
  leg71->SetBorderSize(0);
  leg71->AddEntry(hmht,"FullSim Sample");
  leg71->AddEntry(hmht_fastsim,"FastSim Sample");
  leg71->Draw();
  cmht->Update();

  cMT2->cd();
  cMT2->SetLogy();
  hMT2_fastsim->SetBinContent(hMT2_fastsim->GetNbinsX(), hMT2_fastsim->GetBinContent(hMT2_fastsim->GetNbinsX())+hMT2_fastsim->GetBinContent(hMT2_fastsim->GetNbinsX()+1));
  hMT2->SetBinContent(hMT2->GetNbinsX(), hMT2->GetBinContent(hMT2->GetNbinsX())+hMT2->GetBinContent(hMT2->GetNbinsX()+1));
  hMT2->SetStats(false);
  hMT2->SetLineWidth(3);
  hMT2->SetLineColor(kRed);
  hMT2->GetXaxis()->SetTitle("MT2 [GeV]");
  hMT2_fastsim->SetStats(false);
  hMT2_fastsim->SetLineWidth(3);
  hMT2_fastsim->SetLineColor(kBlue);
  hMT2->Scale(1./(hMT2->Integral()));
  hMT2_fastsim->Scale(1./(hMT2_fastsim->Integral()));
  hMT2->Draw();
  hMT2_fastsim->Draw("same");

  auto rp9 = new TRatioPlot(hMT2_fastsim,hMT2);
  rp9->Draw();
  rp9->GetLowerRefGraph()->SetMinimum(0.5);
  rp9->GetLowerRefGraph()->SetMaximum(1.5);
  rp9->GetLowerRefYaxis()->SetTitle("ratio");
  rp9->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp9->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp9->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp9->GetLowerRefYaxis()->SetNdivisions(303);
  rp9->SetLowTopMargin(0.3);
  rp9->GetUpperPad()->cd();
  TLegend* leg8 = new TLegend(0.55,0.75,0.89,0.87);
  leg8->SetBorderSize(0);
  leg8->AddEntry(hMT2,"FullSim Sample");
  leg8->AddEntry(hMT2_fastsim,"FastSim Sample");
  leg8->Draw();
  cMT2->Update();

  cMT2ll->cd();
  cMT2ll->SetLogy();
  hMT2ll_fastsim->SetBinContent(hMT2ll_fastsim->GetNbinsX(), hMT2ll_fastsim->GetBinContent(hMT2ll_fastsim->GetNbinsX())+hMT2ll_fastsim->GetBinContent(hMT2ll_fastsim->GetNbinsX()+1));
  hMT2ll->SetBinContent(hMT2ll->GetNbinsX(), hMT2ll->GetBinContent(hMT2ll->GetNbinsX())+hMT2ll->GetBinContent(hMT2ll->GetNbinsX()+1));
  hMT2ll->SetStats(false);
  hMT2ll->SetLineWidth(3);
  hMT2ll->SetLineColor(kRed);
  hMT2ll->GetXaxis()->SetTitle("MT2(l,l) [GeV]");
  hMT2ll_fastsim->SetStats(false);
  hMT2ll_fastsim->SetLineWidth(3);
  hMT2ll_fastsim->SetLineColor(kBlue);
  hMT2ll->Scale(1./(hMT2ll->Integral()));
  hMT2ll_fastsim->Scale(1./(hMT2ll_fastsim->Integral()));
  hMT2ll->Draw();
  hMT2ll_fastsim->Draw("same");

  auto rp9x = new TRatioPlot(hMT2_fastsim,hMT2);
  rp9x->Draw();
  rp9x->GetLowerRefGraph()->SetMinimum(0.5);
  rp9x->GetLowerRefGraph()->SetMaximum(1.5);
  rp9x->GetLowerRefYaxis()->SetTitle("ratio");
  rp9x->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp9x->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp9x->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp9x->GetLowerRefYaxis()->SetNdivisions(303);
  rp9x->SetLowTopMargin(0.3);
  rp9x->GetUpperPad()->cd();
  TLegend* leg8x = new TLegend(0.55,0.75,0.89,0.87);
  leg8x->SetBorderSize(0);
  leg8x->AddEntry(hMT2,"FullSim Sample");
  leg8x->AddEntry(hMT2_fastsim,"FastSim Sample");
  leg8x->Draw();
  cMT2ll->Update();

  cMCT->cd();
  cMCT->SetLogy();
  hMCT_fastsim->SetBinContent(hMCT_fastsim->GetNbinsX(), hMCT_fastsim->GetBinContent(hMCT_fastsim->GetNbinsX())+hMCT_fastsim->GetBinContent(hMCT_fastsim->GetNbinsX()+1));
  hMCT->SetBinContent(hMCT->GetNbinsX(), hMCT->GetBinContent(hMCT->GetNbinsX())+hMCT->GetBinContent(hMCT->GetNbinsX()+1));
  hMCT->SetStats(false);
  hMCT->SetLineWidth(3);
  hMCT->SetLineColor(kRed);
  hMCT->GetXaxis()->SetTitle("MCT [GeV]");
  hMCT_fastsim->SetStats(false);
  hMCT_fastsim->SetLineWidth(3);
  hMCT_fastsim->SetLineColor(kBlue);
  hMCT->Scale(1./(hMCT->Integral()));
  hMCT_fastsim->Scale(1./(hMCT_fastsim->Integral()));
  hMCT->Draw();
  hMCT_fastsim->Draw("same");

  auto rp9xx = new TRatioPlot(hMCT_fastsim,hMCT);
  rp9xx->Draw();
  rp9xx->GetLowerRefGraph()->SetMinimum(0.5);
  rp9xx->GetLowerRefGraph()->SetMaximum(1.5);
  rp9xx->GetLowerRefYaxis()->SetTitle("ratio");
  rp9xx->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp9xx->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp9xx->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp9xx->GetLowerRefYaxis()->SetNdivisions(303);
  rp9xx->SetLowTopMargin(0.3);
  rp9xx->GetUpperPad()->cd();
  TLegend* leg8xx = new TLegend(0.55,0.75,0.89,0.87);
  leg8xx->SetBorderSize(0);
  leg8xx->AddEntry(hMCT,"FullSim Sample");
  leg8xx->AddEntry(hMCT_fastsim,"FastSim Sample");
  leg8xx->Draw();
  cMCT->Update();

  cleadJetPt->cd();
  cleadJetPt->SetLogy();
  hleadJetPt_fastsim->SetBinContent(hleadJetPt_fastsim->GetNbinsX(), hleadJetPt_fastsim->GetBinContent(hleadJetPt_fastsim->GetNbinsX())+hleadJetPt_fastsim->GetBinContent(hleadJetPt_fastsim->GetNbinsX()+1));
  hleadJetPt->SetBinContent(hleadJetPt->GetNbinsX(), hleadJetPt->GetBinContent(hleadJetPt->GetNbinsX())+hleadJetPt->GetBinContent(hleadJetPt->GetNbinsX()+1));
  hleadJetPt->SetStats(false);
  hleadJetPt->SetLineWidth(3);
  hleadJetPt->SetLineColor(kRed);
  hleadJetPt->GetXaxis()->SetTitle("Leading Jet p_{T} [GeV]");
  hleadJetPt_fastsim->SetStats(false);
  hleadJetPt_fastsim->SetLineWidth(3);
  hleadJetPt_fastsim->SetLineColor(kBlue);
  hleadJetPt->Scale(1./(hleadJetPt->Integral()));
  hleadJetPt_fastsim->Scale(1./(hleadJetPt_fastsim->Integral()));
  hleadJetPt->Draw();
  hleadJetPt->GetYaxis()->SetRangeUser(0.000001,5.);
  hleadJetPt_fastsim->Draw("same");

  auto rp10 = new TRatioPlot(hleadJetPt_fastsim,hleadJetPt);
  rp10->Draw();
  rp10->GetLowerRefGraph()->SetMinimum(0.5);
  rp10->GetLowerRefGraph()->SetMaximum(1.5);
  rp10->GetLowerRefYaxis()->SetTitle("ratio");
  rp10->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp10->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp10->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp10->GetLowerRefYaxis()->SetNdivisions(303);
  rp10->SetLowTopMargin(0.3);
  rp10->GetUpperPad()->cd();
  TLegend* leg9 = new TLegend(0.55,0.78,0.89,0.89);
  leg9->SetBorderSize(0);
  leg9->AddEntry(hleadJetPt,"FullSim Sample");
  leg9->AddEntry(hleadJetPt_fastsim,"FastSim Sample");
  leg9->Draw();
  cleadJetPt->Update();

  cleadJetEta->cd();
  cleadJetEta->SetLogy();
  hleadJetEta_fastsim->SetBinContent(hleadJetEta_fastsim->GetNbinsX(), hleadJetEta_fastsim->GetBinContent(hleadJetEta_fastsim->GetNbinsX())+hleadJetEta_fastsim->GetBinContent(hleadJetEta_fastsim->GetNbinsX()+1));
  hleadJetEta->SetBinContent(hleadJetEta->GetNbinsX(), hleadJetEta->GetBinContent(hleadJetEta->GetNbinsX())+hleadJetEta->GetBinContent(hleadJetEta->GetNbinsX()+1));
  hleadJetEta->SetStats(false);
  hleadJetEta->SetLineWidth(3);
  hleadJetEta->SetLineColor(kRed);
  hleadJetEta->GetXaxis()->SetTitle("Leading Jet Eta");
  hleadJetEta_fastsim->SetStats(false);
  hleadJetEta_fastsim->SetLineWidth(3);
  hleadJetEta_fastsim->SetLineColor(kBlue);
  hleadJetEta->Scale(1./(hleadJetEta->Integral()));
  hleadJetEta_fastsim->Scale(1./(hleadJetEta_fastsim->Integral()));
  hleadJetEta->Draw();
  hleadJetEta->GetYaxis()->SetRangeUser(0.000001,5.);
  hleadJetEta_fastsim->Draw("same");

  auto rp11 = new TRatioPlot(hleadJetEta_fastsim,hleadJetEta);
  rp11->Draw();
  rp11->GetLowerRefGraph()->SetMinimum(0.5);
  rp11->GetLowerRefGraph()->SetMaximum(1.5);
  rp11->GetLowerRefYaxis()->SetTitle("ratio");
  rp11->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp11->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp11->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp11->GetLowerRefYaxis()->SetNdivisions(303);
  rp11->SetLowTopMargin(0.3);
  rp11->GetUpperPad()->cd();
  TLegend* leg10 = new TLegend(0.55,0.75,0.89,0.87);
  leg10->SetBorderSize(0);
  leg10->AddEntry(hleadJetEta,"FullSim Sample");
  leg10->AddEntry(hleadJetEta_fastsim,"FastSim Sample");
  leg10->Draw();
  cleadJetEta->Update();

  calljetpt->cd();
  calljetpt->SetLogy();
  halljetpt_fastsim->SetBinContent(halljetpt_fastsim->GetNbinsX(), halljetpt_fastsim->GetBinContent(halljetpt_fastsim->GetNbinsX())+halljetpt_fastsim->GetBinContent(halljetpt_fastsim->GetNbinsX()+1));
  halljetpt->SetBinContent(halljetpt->GetNbinsX(), halljetpt->GetBinContent(halljetpt->GetNbinsX())+halljetpt->GetBinContent(halljetpt->GetNbinsX()+1));
  halljetpt->SetStats(false);
  halljetpt->SetLineWidth(3);
  halljetpt->SetLineColor(kRed);
  halljetpt->GetXaxis()->SetTitle("All Jet p_{T} [GeV]");
  halljetpt_fastsim->SetStats(false);
  halljetpt_fastsim->SetLineWidth(3);
  halljetpt_fastsim->SetLineColor(kBlue);
  halljetpt->Scale(1./(halljetpt->Integral()));
  halljetpt_fastsim->Scale(1./(halljetpt_fastsim->Integral()));
  halljetpt->Draw();
  halljetpt_fastsim->Draw("same");

  auto rp12 = new TRatioPlot(halljetpt_fastsim,halljetpt);
  rp12->Draw();
  rp12->GetLowerRefGraph()->SetMinimum(0.5);
  rp12->GetLowerRefGraph()->SetMaximum(1.5);
  rp12->GetLowerRefYaxis()->SetTitle("ratio");
  rp12->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp12->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp12->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp12->GetLowerRefYaxis()->SetNdivisions(303);
  rp12->SetLowTopMargin(0.3);
  rp12->GetUpperPad()->cd();
  TLegend* leg11 = new TLegend(0.55,0.75,0.89,0.87);
  leg11->SetBorderSize(0);
  leg11->AddEntry(halljetpt,"FullSim Sample");
  leg11->AddEntry(halljetpt_fastsim,"FastSim Sample");
  leg11->Draw();
  calljetpt->Update();

  calljeteta->cd();
  calljeteta->SetLogy();
  halljeteta_fastsim->SetBinContent(halljeteta_fastsim->GetNbinsX(), halljeteta_fastsim->GetBinContent(halljeteta_fastsim->GetNbinsX())+halljeteta_fastsim->GetBinContent(halljeteta_fastsim->GetNbinsX()+1));
  halljeteta->SetBinContent(halljeteta->GetNbinsX(), halljeteta->GetBinContent(halljeteta->GetNbinsX())+halljeteta->GetBinContent(halljeteta->GetNbinsX()+1));
  halljeteta->SetStats(false);
  halljeteta->SetLineWidth(3);
  halljeteta->SetLineColor(kRed);
  halljeteta->GetXaxis()->SetTitle("All Jet Eta");
  halljeteta_fastsim->SetStats(false);
  halljeteta_fastsim->SetLineWidth(3);
  halljeteta_fastsim->SetLineColor(kBlue);
  halljeteta->Scale(1./(halljeteta->Integral()));
  halljeteta_fastsim->Scale(1./(halljeteta_fastsim->Integral()));
  halljeteta->Draw();
  halljeteta_fastsim->Draw("same");

  auto rp13 = new TRatioPlot(halljeteta_fastsim,halljeteta);
  rp13->Draw();
  rp13->GetLowerRefGraph()->SetMinimum(0.5);
  rp13->GetLowerRefGraph()->SetMaximum(1.5);
  rp13->GetLowerRefYaxis()->SetTitle("ratio");
  rp13->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp13->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp13->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp13->GetLowerRefYaxis()->SetNdivisions(303);
  rp13->SetLowTopMargin(0.3);
  rp13->GetUpperPad()->cd();

  TLegend* leg12 = new TLegend(0.55,0.75,0.89,0.87);
  leg12->SetBorderSize(0);
  leg12->AddEntry(halljeteta,"FullSim Sample");
  leg12->AddEntry(halljeteta_fastsim,"FastSim Sample");
  leg12->Draw();
  calljeteta->Update();

  clep1pt->cd();
  clep1pt->SetLogy();
  hlep1pt_fastsim->SetBinContent(hlep1pt_fastsim->GetNbinsX(), hlep1pt_fastsim->GetBinContent(hlep1pt_fastsim->GetNbinsX())+hlep1pt_fastsim->GetBinContent(hlep1pt_fastsim->GetNbinsX()+1));
  hlep1pt->SetBinContent(hlep1pt->GetNbinsX(), hlep1pt->GetBinContent(hlep1pt->GetNbinsX())+hlep1pt->GetBinContent(hlep1pt->GetNbinsX()+1));
  hlep1pt->SetStats(false);
  hlep1pt->SetLineWidth(3);
  hlep1pt->SetLineColor(kRed);
  hlep1pt->GetXaxis()->SetTitle("Lepton 1 p_{T} [GeV]");
  hlep1pt_fastsim->SetStats(false);
  hlep1pt_fastsim->SetLineWidth(3);
  hlep1pt_fastsim->SetLineColor(kBlue);
  hlep1pt->Scale(1./(hlep1pt->Integral()));
  hlep1pt_fastsim->Scale(1./(hlep1pt_fastsim->Integral()));
  hlep1pt->Draw();
  hlep1pt_fastsim->Draw("same");

  auto rp14 = new TRatioPlot(hlep1pt_fastsim,hlep1pt);
  rp14->Draw();
  rp14->GetLowerRefGraph()->SetMinimum(0.5);
  rp14->GetLowerRefGraph()->SetMaximum(1.5);
  rp14->GetLowerRefYaxis()->SetTitle("ratio");
  rp14->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp14->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp14->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp14->GetLowerRefYaxis()->SetNdivisions(303);
  rp14->SetLowTopMargin(0.3);
  rp14->GetUpperPad()->cd();
  TLegend* leg13 = new TLegend(0.55,0.75,0.89,0.87);
  leg13->SetBorderSize(0);
  leg13->AddEntry(hlep1pt,"FullSim Sample");
  leg13->AddEntry(hlep1pt_fastsim,"FastSim Sample");
  leg13->Draw();
  clep1pt->Update();

  clep2pt->cd();
  clep2pt->SetLogy();
  hlep2pt_fastsim->SetBinContent(hlep2pt_fastsim->GetNbinsX(), hlep2pt_fastsim->GetBinContent(hlep2pt_fastsim->GetNbinsX())+hlep2pt_fastsim->GetBinContent(hlep2pt_fastsim->GetNbinsX()+1));
  hlep2pt->SetBinContent(hlep2pt->GetNbinsX(), hlep2pt->GetBinContent(hlep2pt->GetNbinsX())+hlep2pt->GetBinContent(hlep2pt->GetNbinsX()+1));
  hlep2pt->SetStats(false);
  hlep2pt->SetLineWidth(3);
  hlep2pt->SetLineColor(kRed);
  hlep2pt->GetXaxis()->SetTitle("Lepton 2  p_{T} [GeV]");
  hlep2pt_fastsim->SetStats(false);
  hlep2pt_fastsim->SetLineWidth(3);
  hlep2pt_fastsim->SetLineColor(kBlue);
  hlep2pt->Scale(1./(hlep2pt->Integral()));
  hlep2pt_fastsim->Scale(1./(hlep2pt_fastsim->Integral()));
  hlep2pt->Draw();
  hlep2pt_fastsim->Draw("same");

  auto rp15 = new TRatioPlot(hlep2pt_fastsim,hlep2pt);
  rp15->Draw();
  rp15->GetLowerRefGraph()->SetMinimum(0.5);
  rp15->GetLowerRefGraph()->SetMaximum(1.5);
  rp15->GetLowerRefYaxis()->SetTitle("ratio");
  rp15->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp15->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp15->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp15->GetLowerRefYaxis()->SetNdivisions(303);
  rp15->SetLowTopMargin(0.3);
  rp15->GetUpperPad()->cd();
  TLegend* leg14 = new TLegend(0.55,0.75,0.89,0.87);
  leg14->SetBorderSize(0);
  leg14->AddEntry(hlep2pt,"FullSim Sample");
  leg14->AddEntry(hlep2pt_fastsim,"FastSim Sample");
  leg14->Draw();
  clep2pt->Update();

  clep2eta->cd();
  clep2eta->SetLogy();
  hlep2eta_fastsim->SetBinContent(hlep2eta_fastsim->GetNbinsX(), hlep2eta_fastsim->GetBinContent(hlep2eta_fastsim->GetNbinsX())+hlep2eta_fastsim->GetBinContent(hlep2eta_fastsim->GetNbinsX()+1));
  hlep2eta->SetBinContent(hlep2eta->GetNbinsX(), hlep2eta->GetBinContent(hlep2eta->GetNbinsX())+hlep2eta->GetBinContent(hlep2eta->GetNbinsX()+1));
  hlep2eta->SetStats(false);
  hlep2eta->SetLineWidth(3);
  hlep2eta->SetLineColor(kRed);
  hlep2eta->GetXaxis()->SetTitle("Lepton 2 Eta");
  hlep2eta_fastsim->SetStats(false);
  hlep2eta_fastsim->SetLineWidth(3);
  hlep2eta_fastsim->SetLineColor(kBlue);
  hlep2eta->Scale(1./(hlep2eta->Integral()));
  hlep2eta_fastsim->Scale(1./(hlep2eta_fastsim->Integral()));
  hlep2eta->Draw();
  hlep2eta_fastsim->Draw("same");

  auto rp16 = new TRatioPlot(hlep2eta_fastsim,hlep2eta);
  rp16->Draw();
  rp16->GetLowerRefGraph()->SetMinimum(0.5);
  rp16->GetLowerRefGraph()->SetMaximum(1.5);
  rp16->GetLowerRefYaxis()->SetTitle("ratio");
  rp16->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp16->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp16->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp16->GetLowerRefYaxis()->SetNdivisions(303);
  rp16->SetLowTopMargin(0.3);
  rp16->GetUpperPad()->cd();
  TLegend* leg15 = new TLegend(0.55,0.75,0.89,0.87);
  leg15->SetBorderSize(0);
  leg15->AddEntry(hlep2eta,"FullSim Sample");
  leg15->AddEntry(hlep2eta_fastsim,"FastSim Sample");
  leg15->Draw();
  clep2eta->Update();

  clep1eta->cd();
  clep1eta->SetLogy();
  hlep1eta_fastsim->SetBinContent(hlep1eta_fastsim->GetNbinsX(), hlep1eta_fastsim->GetBinContent(hlep1eta_fastsim->GetNbinsX())+hlep1eta_fastsim->GetBinContent(hlep1eta_fastsim->GetNbinsX()+1));
  hlep1eta->SetBinContent(hlep1eta->GetNbinsX(), hlep1eta->GetBinContent(hlep1eta->GetNbinsX())+hlep1eta->GetBinContent(hlep1eta->GetNbinsX()+1));
  hlep1eta->SetStats(false);
  hlep1eta->SetLineWidth(3);
  hlep1eta->SetLineColor(kRed);
  hlep1eta->GetXaxis()->SetTitle("Lepton 1 Eta");
  hlep1eta_fastsim->SetStats(false);
  hlep1eta_fastsim->SetLineWidth(3);
  hlep1eta_fastsim->SetLineColor(kBlue);
  hlep1eta->Scale(1./(hlep1eta->Integral()));
  hlep1eta_fastsim->Scale(1./(hlep1eta_fastsim->Integral()));
  hlep1eta->Draw();
  hlep1eta_fastsim->Draw("same");

  auto rp17 = new TRatioPlot(hlep1eta_fastsim,hlep1eta);
  rp17->Draw();
  rp17->GetLowerRefGraph()->SetMinimum(0.5);
  rp17->GetLowerRefGraph()->SetMaximum(1.5);
  rp17->GetLowerRefYaxis()->SetTitle("ratio");
  rp17->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp17->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp17->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp17->GetLowerRefYaxis()->SetNdivisions(303);
  rp17->SetLowTopMargin(0.3);
  rp17->GetUpperPad()->cd();
  TLegend* leg16 = new TLegend(0.55,0.75,0.89,0.87);
  leg16->SetBorderSize(0);
  leg16->AddEntry(hlep1eta,"FullSim Sample");
  leg16->AddEntry(hlep1eta_fastsim,"FastSim Sample");
  leg16->Draw();
  clep1eta->Update();

  cmetres->cd();
  cmetres->SetLogy();
  hmetres_fastsim->SetBinContent(hmetres_fastsim->GetNbinsX(), hmetres_fastsim->GetBinContent(hmetres_fastsim->GetNbinsX())+hmetres_fastsim->GetBinContent(hmetres_fastsim->GetNbinsX()+1));
  hmetres->SetBinContent(hmetres->GetNbinsX(), hmetres->GetBinContent(hmetres->GetNbinsX())+hmetres->GetBinContent(hmetres->GetNbinsX()+1));
  hmetres->SetStats(false);
  hmetres->SetLineWidth(3);
  hmetres->SetLineColor(kRed);
  hmetres->GetXaxis()->SetTitle("MET Resolution");
  hmetres_fastsim->SetStats(false);
  hmetres_fastsim->SetLineWidth(3);
  hmetres_fastsim->SetLineColor(kBlue);
  hmetres->Scale(1./(hmetres->Integral()));
  hmetres_fastsim->Scale(1./(hmetres_fastsim->Integral()));
  hmetres->Draw();
  hmetres_fastsim->Draw("same");

  auto rp18 = new TRatioPlot(hmetres_fastsim,hmetres);
  rp18->Draw();
  rp18->GetLowerRefGraph()->SetMinimum(0.5);
  rp18->GetLowerRefGraph()->SetMaximum(1.5);
  rp18->GetLowerRefYaxis()->SetTitle("ratio");
  rp18->GetLowerRefYaxis()->SetTitleOffset(1.6);
  rp18->GetLowerRefYaxis()->SetLabelSize(0.03);
  rp18->GetLowerRefYaxis()->SetTitleSize(0.03);
  rp18->GetLowerRefYaxis()->SetNdivisions(303);
  rp18->SetLowTopMargin(0.3);
  rp18->GetUpperPad()->cd();
  TLegend* leg17 = new TLegend(0.55,0.75,0.89,0.87);
  leg17->SetBorderSize(0);
  leg17->AddEntry(hmetres,"FullSim Sample");
  leg17->AddEntry(hmetres_fastsim,"FastSim Sample");
  leg17->Draw();
  cmetres->Update();

  //  cgenvpfmet->cd();
  //  hgenvpfmet_fastsim->SetBinContent(hgenvpfmet_fastsim->GetNbinsX(), hgenvpfmet_fastsim->GetBinContent(hgenvpfmet_fastsim->GetNbinsX())+hgenvpfmet_fastsim->GetBinContent(hgenvpfmet_fastsim->GetNbinsX()+1));
  //  hgenvpfmet->SetBinContent(hgenvpfmet->GetNbinsX(), hgenvpfmet->GetBinContent(hgenvpfmet->GetNbinsX())+hgenvpfmet->GetBinContent(hgenvpfmet->GetNbinsX()+1));
  //  hgenvpfmet->SetFillColor(kRed);
  //  hgenvpfmet->GetXaxis()->SetTitle("genmet v pfmet");
  //  hgenvpfmet_fastsim->SetFillColor(kBlue);
  //  hgenvpfmet->Scale(1./(hgenvpfmet->Integral()));
  //  hgenvpfmet_fastsim->Scale(1./(hgenvpfmet_fastsim->Integral()));
  //  hgenvpfmet->Draw("box");
  //  hgenvpfmet_fastsim->Draw("box same");



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
  fNumbers->Add(hMT2ll);
  fNumbers->Add(hMCT);
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
  fNumbers->Add(hmetres);
  //  fNumbers->Add(hgenvpfmet);

  fNumbers->Add(hpfmet_fastsim);
  fNumbers->Add(hpfmet_fastsim_filt);
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
  fNumbers->Add(hMT2ll_fastsim);
  fNumbers->Add(hMCT_fastsim);
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
  fNumbers->Add(hmetres_fastsim);
  //  fNumbers->Add(hgenvpfmet_fastsim);

  fNumbers->Add(cpfmet);
  fNumbers->Add(cpfmet_filt);
  fNumbers->Add(cmht);
  fNumbers->Add(cmt_met_lep);
  fNumbers->Add(cngoodjets);
  fNumbers->Add(clep1_dphiMET);
  fNumbers->Add(cmindphi_met_j1_j2);
  fNumbers->Add(ctopnessMod);
  fNumbers->Add(cngoodleps);
  fNumbers->Add(cngoodbtags);
  fNumbers->Add(cMT2);
  fNumbers->Add(cMT2ll);
  fNumbers->Add(cMCT);
  fNumbers->Add(cleadJetPt);
  fNumbers->Add(cleadJetEta);
  fNumbers->Add(clep1pt);
  fNumbers->Add(clep1eta);
  fNumbers->Add(clep2pt);
  fNumbers->Add(clep2eta);
  fNumbers->Add(calljetpt);
  fNumbers->Add(calljeteta);
  fNumbers->Add(cmetres);
  //  fNumbers->Add(cgenvpfmet);

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
