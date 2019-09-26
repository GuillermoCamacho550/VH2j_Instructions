//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  root -l -b -q VH2j_TMVAMultiClass.C+
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <iostream>

#include "TChain.h"
#include "TFile.h"
#include "TROOT.h"
#include "TString.h"
#include "TSystem.h"
#include "TLorentzVector.h"

#include "TMVA/DataLoader.h"
#include "TMVA/Factory.h"
#include "TMVA/TMVAMultiClassGui.h"
#include "TMVA/Tools.h"

using namespace TMVA;

float mindetajl(float jet1_eta,
                float jet2_eta,
                float lep1_eta,
                float lep2_eta)
{
  float themin = 999;

  float detaj1l1 = fabs(jet1_eta - lep1_eta);
  float detaj1l2 = fabs(jet1_eta - lep2_eta);
  float detaj2l1 = fabs(jet2_eta - lep1_eta);
  float detaj2l2 = fabs(jet2_eta - lep2_eta);

  if (detaj1l1 < themin) themin = detaj1l1;
  if (detaj1l2 < themin) themin = detaj1l2;
  if (detaj2l1 < themin) themin = detaj2l1;
  if (detaj2l2 < themin) themin = detaj2l2;

  return themin;
}

float ptjj(float jetpt1, float jeteta1, float jetphi1, float jetpt2, float jeteta2, float jetphi2)
{
  TLorentzVector jet1;
  jet1.SetPtEtaPhiM(jetpt1,jeteta1,jetphi1,0);
  TLorentzVector jet2;
  jet2.SetPtEtaPhiM(jetpt2,jeteta2,jetphi2,0);

  return fabs((jet1+jet2).Pt());

}



void VH2j_TMVAMultiClass_v1()
{
  std::cout << "\n ==> Start VH2j_TMVAMultiClass\n" << std::endl;

  TMVA::Tools::Instance();

  TString workdir = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Fall2017_nAOD_v1_Full2017v2/MCl1loose2017v2__MCCorr2017__btagPerEvent__l2loose__l2tightOR2017/nanoLatino";

  TString outfileName = "VH2j_TMVAMultiClass.root";

  TString Jet1_qgl = "(Alt$(Jet_qgl[0],-1)*(CleanJet_jetIdx[0]==0) + Alt$(Jet_qgl[1],-1)*(CleanJet_jetIdx[0]==1) + Alt$(Jet_qgl[2],-1)*(CleanJet_jetIdx[0]==2) + Alt$(Jet_qgl[3],-1)*(CleanJet_jetIdx[0]==3) + Alt$(Jet_qgl[4],-1)*(CleanJet_jetIdx[0]==4) + Alt$(Jet_qgl[5],-1)*(CleanJet_jetIdx[0]==5) + Alt$(Jet_qgl[6],-1)*(CleanJet_jetIdx[0]==6) + Alt$(Jet_qgl[7],-1)*(CleanJet_jetIdx[0]==7) + Alt$(Jet_qgl[8],-1)*(CleanJet_jetIdx[0]==8) + (CleanJet_jetIdx[0]>8)*(-1))";
  TString Jet2_qgl = "(Alt$(Jet_qgl[0],-1)*(CleanJet_jetIdx[1]==0) + Alt$(Jet_qgl[1],-1)*(CleanJet_jetIdx[1]==1) + Alt$(Jet_qgl[2],-1)*(CleanJet_jetIdx[1]==2) + Alt$(Jet_qgl[3],-1)*(CleanJet_jetIdx[1]==3) + Alt$(Jet_qgl[4],-1)*(CleanJet_jetIdx[1]==4) + Alt$(Jet_qgl[5],-1)*(CleanJet_jetIdx[1]==5) + Alt$(Jet_qgl[6],-1)*(CleanJet_jetIdx[1]==6) + Alt$(Jet_qgl[7],-1)*(CleanJet_jetIdx[1]==7) + Alt$(Jet_qgl[8],-1)*(CleanJet_jetIdx[1]==8) + (CleanJet_jetIdx[1]>8)*(-1))";



  TFile* outfile = TFile::Open(outfileName, "recreate");

  TMVA::Factory* factory = new TMVA::Factory("TMVAMultiClass", outfile, "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=multiclass");

  TMVA::DataLoader* dataloader=new TMVA::DataLoader("dataset");

  dataloader->AddVariable("mll",          'F');
  dataloader->AddVariable("mjj",          'F');
  dataloader->AddVariable("mth",          'F');
  dataloader->AddVariable("jet1_qgl:=("+Jet1_qgl+">=0)*("+Jet1_qgl+"+1)-1", 'F');//qgl of jet 1 if its greater than 0
  dataloader->AddVariable("jet2_qgl:=("+Jet2_qgl+">=0)*("+Jet2_qgl+"+1)-1", 'F');
  dataloader->AddVariable("Lepton1_pt:=Lepton_pt[0]", 'F');
  dataloader->AddVariable("Lepton2_pt:=Lepton_pt[1]", 'F');
  dataloader->AddVariable("detajj",       'F');
  dataloader->AddVariable("mindetajl:=mindetajl(CleanJet_eta[0],CleanJet_eta[1],Lepton_eta[0],Lepton_eta[1])", 'F');
  //dataloader->AddVariable("Jet1_qgl:=Jet_qgl[CleanJet_jetIdx[0]]", 'F');
  //  dataloader->AddVariable("Jet2_qgl:=Jet_qgl[CleanJet_jetIdx[1]]", 'F');
  dataloader->AddVariable("pTWW", 'F');
  dataloader->AddVariable("ptjj:=ptjj(CleanJet_pt[0], CleanJet_eta[0], CleanJet_phi[0], CleanJet_pt[1], CleanJet_eta[1], CleanJet_phi[1])", 'F');

  TChain* VH2j = new TChain("Events");
  TChain* ggH  = new TChain("Events");
  TChain* Top  = new TChain("Events");
  TChain* WW   = new TChain("Events");

  for (UInt_t k=0; k<30; k++) {
    if (k < 20) VH2j->Add(Form("%s_HZJ_HToWW_M120__part%d.root",                      workdir.Data(), k));  // k <  20
    if (k < 19) VH2j->Add(Form("%s_GluGluZH_HToWW_M125__part%d.root",                 workdir.Data(), k));  // k <  19
    if (k < 21) VH2j->Add(Form("%s_HWplusJ_HToWW_M125__part%d.root",                  workdir.Data(), k));  // k <  21
    if (k < 29) VH2j->Add(Form("%s_HWminusJ_HToWW_M125__part%d.root",                 workdir.Data(), k));  // k <  29
    if (k < 24) ggH ->Add(Form("%s_GluGluHToWWTo2L2NuPowheg_M125_CP5Up__part%d.root", workdir.Data(), k));  // k <  24
    if (k <  1) Top ->Add(Form("%s_TTTo2L2Nu_PSWeights__part%d.root",                 workdir.Data(), k));  // k < 107
    if (k <  1) WW  ->Add(Form("%s_WWTo2L2Nu__part%d.root",                           workdir.Data(), k));  // k <   3
  }

  TCut myCuts = "PuppiMET_pt > 20 \
            && Lepton_pt[0] > 25 \
            && Lepton_pt[1] > 10 \
            && Alt$(Lepton_pt[2],0) < 10 \
            && (abs(Lepton_pdgId[1]) == 13 || Lepton_pt[1] > 13) \
            && Lepton_pdgId[0]*Lepton_pdgId[1] == -11*13 \
            && mll > 12 \
            && ptll > 30 \
            && drll < 2 \
            && Sum$(CleanJet_pt > 30) >= 2 \
            && Alt$(abs(CleanJet_eta[0]),0) < 2.5 \
            && Alt$(abs(CleanJet_eta[1]),0) < 2.5 \
            && mjj < 200 \
            && abs(detajj) < 3.5 \
            && Alt$(Jet_qgl[CleanJet_jetIdx[0]],1) > 0 \
            && Alt$(Jet_qgl[CleanJet_jetIdx[1]],1) > 0 \
            && (mth > 60 && mth < 125)";



  gROOT->cd(outfileName + ":/");

  dataloader->AddTree(VH2j, "VH2j", 1, myCuts, "Training and Testing");
  dataloader->AddTree(ggH,  "ggH",  1, myCuts, "Training and Testing");
  dataloader->AddTree(Top,  "Top",  1, myCuts, "Training and Testing");
  dataloader->AddTree(WW,   "WW",   1, myCuts, "Training and Testing");
  
  dataloader->PrepareTrainingAndTestTree("", "SplitMode=Random:NormMode=None:!V");

  factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDTG", "!H:!V:NTrees=500:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.50:nCuts=20:MaxDepth=2:SkipNormalization=1");

  factory->TrainAllMethods();     // Train MVAs using the set of training events
  factory->TestAllMethods();      // Evaluate MVAs using the set of test events
  factory->EvaluateAllMethods();  // Evaluate and compare performance of all configured MVAs

  //----------------------------------------------------------------------------

  outfile->Close();

  std::cout << "\n ==> VH2j_TMVAMultiClass is done\n" << std::endl;

  delete factory;
  delete dataloader;
}
