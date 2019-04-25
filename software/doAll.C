{

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *ch = new TChain("t"); 
  ch->Add("/home/users/isuarez/MCStudies/T14t_mGluino1200_mLSP800_FullSim_0.root");
  ScanChain(ch); 
}