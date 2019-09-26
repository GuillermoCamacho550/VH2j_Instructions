//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  root -l TMVAReader_Top.C
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "TFile.h"
#include "TLorentzVector.h"
#include "TMVA/Reader.h"
#include "TROOT.h"
#include "TString.h"
#include "TSystem.h"
#include "TTree.h"



namespace multidraw {extern thread_local TTree* currentTree;}

TMVA::Reader* myreader = new TMVA::Reader();

bool debug = false;

bool initialized = false;

TString name_temp = "";


// Variables fed into the BDT
float loc_mll;
float loc_mjj;
float loc_mth;
float loc_Lepton_pt1;
float loc_mindetajl;
float loc_dphijj2;
float loc_dphill;
float loc_detall;
//float loc_nvert; //float or int?
float loc_Lepton_eta0;
float loc_CleanJet_pt0;
float loc_CleanJet_eta1;

// Variables read from the tree
float loc0_mll;
float loc0_mjj;
float loc0_mth;
float loc0_detajj;
float loc0_Lepton_pt   [100];
float loc0_Lepton_eta  [100];
float loc0_CleanJet_eta[100];
float loc0_Jet_qgl     [100];
float loc0_pTWW;
float loc0_CleanJet_pt [100];
float loc0_CleanJet_phi[100];
//float loc0_nvert;
float loc0_dphill;

// User defined function
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
//
//float ptjj(float jetpt1,
//	   float jeteta1,
//	   float jetphi1, 
//	   float jetpt2,
//	   float jeteta2,
//	   float jetphi2)
//{
//  TLorentzVector jet1;
//  jet1.SetPtEtaPhiM(jetpt1,jeteta1,jetphi1,0);
//  TLorentzVector jet2;
//  jet2.SetPtEtaPhiM(jetpt2,jeteta2,jetphi2,0);
//
//  return fabs((jet1+jet2).Pt());
//
//}
//

float dphijj2(float jetpt1, float jeteta1, float jetphi1,
	     float jetpt2, float jeteta2, float jetphi2)
{
  TLorentzVector jet1;
  jet1.SetPtEtaPhiM(jetpt1,jeteta1,jetphi1,0);

  TLorentzVector jet2;
  jet2.SetPtEtaPhiM(jetpt2,jeteta2,jetphi2,0);

  return fabs(jet1.DeltaPhi(jet2));
}

float detall(float lepeta1, float lepeta2)
{
  return fabs(lepeta1-lepeta2);
}


// Init
void init_TMVAReader_Top(TTree* tree)
{
  tree->SetBranchAddress("mll",          &loc0_mll);
  tree->SetBranchAddress("mjj",          &loc0_mjj);
  tree->SetBranchAddress("mth",          &loc0_mth);
  tree->SetBranchAddress("Lepton_pt",    &loc0_Lepton_pt);
  tree->SetBranchAddress("dphill",       &loc0_dphill);
  //tree->SetBranchAddress("PV_npvsGood",  &loc0_nvert);
  tree->SetBranchAddress("Lepton_eta",   &loc0_Lepton_eta);
  tree->SetBranchAddress("CleanJet_eta", &loc0_CleanJet_eta);
  tree->SetBranchAddress("CleanJet_pt",  &loc0_CleanJet_pt);
  tree->SetBranchAddress("CleanJet_phi", &loc0_CleanJet_phi);
  // The variables' names and their order have to agree with those in TMVAClassification_VH2j_Top_v1.C
  myreader->AddVariable("mll",          &loc_mll);
  myreader->AddVariable("mjj",          &loc_mjj);
  myreader->AddVariable("mth",          &loc_mth);
  myreader->AddVariable("Lepton_pt[1]", &loc_Lepton_pt1);
  myreader->AddVariable("mindetajl(CleanJet_eta[0],CleanJet_eta[1],Lepton_eta[0],Lepton_eta[1])", &loc_mindetajl);
  myreader->AddVariable("dphijj2(CleanJet_pt[0], CleanJet_eta[0], CleanJet_phi[0], CleanJet_pt[1], CleanJet_eta[1], CleanJet_phi[\
1])",       &loc_dphijj2);
  myreader->AddVariable("dphill",         &loc_dphill);
  myreader->AddVariable("detall(Lepton_eta[0], Lepton_eta[1])",         &loc_detall);
  //myreader->AddVariable("PV_npvsGood",    &loc_nvert);
  myreader->AddVariable("Lepton_eta[0]",  &loc_Lepton_eta0);
  myreader->AddVariable("CleanJet_pt[0]", &loc_CleanJet_pt0);
  myreader->AddVariable("CleanJet_eta[1]",&loc_CleanJet_eta1);

  myreader->BookMVA("BDT", "/afs/cern.ch/user/g/gcamacho/CMSSW_9_4_9/src/PlotsConfigurations/Configurations/WW/Full2017/VH2jBDT/dataset/weights/TMVAClassification_VH2j_Top_BDT.weights.xml");
}


// Main function
float TMVAReader_Top(int entry)
{
  if (name_temp != multidraw::currentTree->GetCurrentFile()->GetName()) {

    std::cout << " name_temp = " << name_temp << std::endl;

    name_temp = multidraw::currentTree->GetCurrentFile()->GetName();

    std::cout << " name_temp = " << name_temp << std::endl;

    initialized = false;
  }

  if (!initialized) {

    delete myreader;

    myreader = new TMVA::Reader();

    init_TMVAReader_Top(multidraw::currentTree);

    std::cout << " check init" << std::endl;

    initialized = true;
  }

  multidraw::currentTree->GetEntry(entry);

  loc_mll        = loc0_mll;
  loc_mjj        = loc0_mjj;
  loc_mth        = loc0_mth;
  loc_Lepton_pt1 = loc0_Lepton_pt[1];
  loc_mindetajl  = mindetajl(loc0_CleanJet_eta[0],loc0_CleanJet_eta[1],loc0_Lepton_eta[0],loc0_Lepton_eta[1]);
  loc_dphijj2     = dphijj2(loc0_CleanJet_pt[0], loc0_CleanJet_eta[0], loc0_CleanJet_phi[0],loc0_CleanJet_pt[1], loc0_CleanJet_eta[1], loc0_CleanJet_phi[1]);
  loc_dphill     = loc0_dphill;
  loc_detall     = detall(loc0_Lepton_eta[0], loc0_Lepton_eta[1]);
  //loc_nvert      = loc0_nvert;
  loc_Lepton_eta0 = loc0_Lepton_eta[0];
  loc_CleanJet_pt0  = loc0_CleanJet_pt[0];
  loc_CleanJet_eta1 = loc0_CleanJet_eta[1];

  float classifier = myreader->EvaluateMVA("BDT");

  if (debug) std::cout << " classifier " << classifier << std::endl;

  return classifier;
}
