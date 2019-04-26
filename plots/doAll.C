{

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *ch = new TChain("t"); 
  ch->Add("/home/users/isuarez/MCStudies/T14t_mGluino1200_mLSP800_FullSim_0.root");
  ch->Add("/home/users/isuarez/MCStudies/T14t_mGluino1200_mLSP800_FastSim_merged.root ");
  //ch->Add("/home/users/isuarez/MCStudies/T14t_mGluino1200_mLSP800_FastSim.root");
  //ch->Add("stopbaby_3.root");
  //ch->Add("fastsim_stopbaby_119.root");
  //ch->Add("fastsim_stopbaby_120.root");
  ScanChain(ch); 
}
