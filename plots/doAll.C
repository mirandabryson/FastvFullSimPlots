{

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *ch = new TChain("t"); 
  ch->Add("/home/users/isuarez/MCStudies/SMS_T14t_Fall17_mGluino-1200_mLSP-800_fullsim_1.root");
  ch->Add("/home/users/isuarez/MCStudies/SMS_T14t_Fall17_mGluino-1200_mLSP-800_fastsim_1.root");
  //ch->Add("/home/users/isuarez/MCStudies/T14t_mGluino1200_mLSP800_FastSim.root");
  //ch->Add("stopbaby_3.root");
  //ch->Add("fastsim_stopbaby_119.root");
  //ch->Add("fastsim_stopbaby_120.root");
  ScanChain(ch); 
}
