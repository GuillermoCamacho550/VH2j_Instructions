// To compile 
// root -l
// gSystem->Load("libLatinoAnalysisMultiDraw.so")
// .L hww_detajjllmet.C+ 


#include <TString.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TLorentzVector.h>


namespace multidraw {
  extern thread_local TTree* currentTree;
}


float dRjjllmet(float jet1_pt, float jet1_eta, float jet1_phi,
		  float jet2_pt, float jet2_eta, float jet2_phi,
                  float lepton1_pt, float lepton1_eta, float lepton1_phi,
                  float lepton2_pt, float lepton2_eta, float lepton2_phi,
                  float met_pt, float met_phi)         
{
  TLorentzVector jet1;
  TLorentzVector jet2;
  TLorentzVector lepton1;
  TLorentzVector lepton2;
  TLorentzVector met;

  jet1.SetPtEtaPhiM(jet1_pt, jet1_eta, jet1_phi, 0.);
  jet2.SetPtEtaPhiM(jet2_pt, jet2_eta, jet2_phi, 0.);

  lepton1.SetPtEtaPhiM(lepton1_pt,lepton1_eta,lepton1_phi,0);
  lepton2.SetPtEtaPhiM(lepton2_pt,lepton2_eta,lepton2_phi,0);
  met.SetPtEtaPhiM(met_pt,0,met_phi,0);

  return fabs((jet1+jet2).DeltaR(lepton1+lepton2+met));
}


float hww_dRjjllmet(int entry)
{
  TTreeReader fReader;

  TTreeReaderArray<Float_t> CleanJet_pt  = {fReader, "CleanJet_pt"};
  TTreeReaderArray<Float_t> CleanJet_eta = {fReader, "CleanJet_eta"};
  TTreeReaderArray<Float_t> CleanJet_phi = {fReader, "CleanJet_phi"};

  TTreeReaderArray<Float_t> Lepton_pt  = {fReader, "Lepton_pt"};
  TTreeReaderArray<Float_t> Lepton_eta = {fReader, "Lepton_eta"};
  TTreeReaderArray<Float_t> Lepton_phi = {fReader, "Lepton_phi"};

  TTreeReaderValue<Float_t> MET_pt   = {fReader, "MET_pt"};
  TTreeReaderValue<Float_t> MET_phi  = {fReader, "MET_phi"};
 
  fReader.SetTree(multidraw::currentTree);

  fReader.SetEntry(entry); 

  return dRjjllmet(CleanJet_pt[0], CleanJet_eta[0], CleanJet_phi[0],
		     CleanJet_pt[1], CleanJet_eta[1], CleanJet_phi[1],
		     Lepton_pt[0], Lepton_eta[0], Lepton_phi[0],
		     Lepton_pt[1], Lepton_eta[1], Lepton_phi[1],
		     *MET_pt, *MET_phi);
}

