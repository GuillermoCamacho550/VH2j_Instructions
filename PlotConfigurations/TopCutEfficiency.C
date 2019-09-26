const TString path1 = "plotStorage/SequentialCuts/IndividualTop/";
const TString path2 = "plotStorage/SequentialCuts/StackingTop/";

void TopCutEfficiency()
{
  TFile* file_Top   = new TFile(path1 + "plots_WW_ALL_top.root", "read");
  // TFile* file_WH0   = new TFile(path1 + "plots_WW_ALL_WH_hww.root", "read");
  //TFile* file_ggZH0 = new TFile(path1 + "plots_WW_ALL_ggZH_hww.root", "read");

  TH1D* events_Top   = (TH1D*)file_Top  ->Get("VH_2j_emu_0/events/histo_top");
  //TH1D* events_WH0   = (TH1D*)file_WH0  ->Get("VH_2j_emu_0/events/histo_WH_hww");
  //TH1D* events_ggZH0 = (TH1D*)file_ggZH0->Get("VH_2j_emu_0/events/histo_ggZH_hww");

  float  Base;
  Base=events_Top->Integral(-1,-1);//+events_ggZH0->Integral(-1,-1)+events_WH0->Integral(-1,-1);
  printf(" Cut efficiency calculator for VH2j analysis \n");
  printf(" Number of events with no cuts applied = %f \n", Base);
  
  float PrevCut;
  PrevCut = Base;

  printf(" -Individual cuts: \n");
  for (Int_t i=1; i<=11; i++) {
   TString number = ""; number +=i;

   TFile* file_Top   = new TFile(path1 + "plots_WW_ALL_top.root", "read");
   //   TFile* file_WH   = new TFile(path1 + "plots_WW_ALL_WH_hww.root", "read");
   //TFile* file_ggZH = new TFile(path1 + "plots_WW_ALL_ggZH_hww.root", "read");

   TH1D* events_Top   = (TH1D*)file_Top  ->Get("VH_2j_emu_" + number + "/events/histo_top");
   //   TH1D* events_WH   = (TH1D*)file_WH  ->Get("VH_2j_emu_" + number + "/events/histo_WH_hww");
   //   TH1D* events_ggZH = (TH1D*)file_ggZH->Get("VH_2j_emu_" + number + "/events/histo_ggZH_hww");

   float  Events;
   Events = events_Top->Integral(-1,-1);//+events_ggZH->Integral(-1,-1)+events_WH->Integral(-1,-1);
   
   float  Eff;
   Eff = Events/Base;
   //   float RelEff;
   //   RelEff = Events/PrevCut; 

   //   PrevCut = Events;

   printf(" Efficiency after cut %d = %f \n", i, Eff);
   //   printf(" Relative efficiency cut %d vs cut %d = %f \n \n", i, i-1, RelEff);
 
  }

  printf("\n -Sequential cuts: \n");
  PrevCut = Base;

  for (Int_t i=1; i<=11; i++) {
   TString number = ""; number +=i;

   TFile* file_Top   = new TFile(path2 + "plots_WW_ALL_top.root", "read");
   //   TFile* file_WH   = new TFile(path2 + "plots_WW_ALL_WH_hww.root", "read");
   //   TFile* file_ggZH = new TFile(path2 + "plots_WW_ALL_ggZH_hww.root", "read");

   TH1D* events_Top   = (TH1D*)file_Top  ->Get("VH_2j_emu_" + number + "/events/histo_top");
   //   TH1D* events_WH   = (TH1D*)file_WH  ->Get("VH_2j_emu_" + number + "/events/histo_WH_hww");
   //TH1D* events_ggZH = (TH1D*)file_ggZH->Get("VH_2j_emu_" + number + "/events/histo_ggZH_hww");

   float  Events;
   Events = events_Top->Integral(-1,-1);//+events_ggZH->Integral(-1,-1)+events_WH->Integral(-1,-1);
   
   float  Eff;
   Eff = Events/Base;

   float RelEff;
   RelEff = Events/PrevCut; 

   PrevCut = Events;

   printf(" Efficiency after cut %d = %f \n", i, Eff);
   printf(" Relative efficiency cut %d vs cut %d = %f \n \n", i, i-1, RelEff);

  }
}
