{

  gROOT->ProcessLine(".L ScanChain.C+");

  TChain *ch = new TChain("t"); 
  ch->Add("~mbryson/StopAnalysis/StopBabyMaker/stopbaby_3.root");
  ScanChain(ch); 
}