//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Root -l TMVAClassification_VH2j.C\(\"BDT\"\)
//  root -l TMVAClassification_VH2j.C\(\"BDT,BDT4,BDT6,BDT12\"\)
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TObjString.h"
#include "TPluginManager.h"
#include "TROOT.h"
#include "TString.h"
#include "TSystem.h"
#include "TTree.h"

#include "TMVA/DataLoader.h"
#include "TMVA/Factory.h"
#include "TMVA/TMVAGui.h"
#include "TMVA/Tools.h"


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

float dphijj(float jetpt1, float jeteta1, float jetphi1,
	      float jetpt2, float jeteta2, float jetphi2)
{
  TLorentzVector jet1;
  jet1.SetPtEtaPhiM(jetpt1,jeteta1,jetphi1,0);

  TLorentzVector jet2;
  jet2.SetPtEtaPhiM(jetpt2,jeteta2,jetphi2,0);

  return fabs(jet1.DeltaPhi(jet2));
}



void TMVAClassification_VH2j_Top_v1(TString myMethodList = "") 
{
  // Load the library
  TMVA::Tools::Instance();
  TMVA::gConfig().GetVariablePlotting().fNbinsMVAoutput = 20; //change binning!
  // Default MVA methods to be trained + tested
  std::map<std::string,int> Use;

  Use["BDT"]   = 1;  // Uses Adaptive Boost
  Use["BDT4"]  = 0;  // Uses Adaptive Boost
  Use["BDT6"]  = 0;  // Uses Adaptive Boost
  Use["BDT12"] = 1;  // Uses Adaptive Boost
  Use["BDTG"] = 0;
  //Use["Fisher"] = 0;
  //Use["RuleFit"] = 0;
  //Use["k-NN"] = 1;
  Use["DNN_CPU"] = 0;
  Use["MLP"] = 0;
  if (myMethodList != "") {

    for (std::map<std::string,int>::iterator it=Use.begin(); it!=Use.end(); it++) it->second = 0;

    std::vector<TString> mlist = TMVA::gTools().SplitString(myMethodList, ',');

    for (UInt_t i=0; i<mlist.size(); i++) {

      std::string regMethod(mlist[i]);

      if (Use.find(regMethod) == Use.end()) {

	std::cout << " Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;

	for (std::map<std::string,int>::iterator it=Use.begin(); it!=Use.end(); it++) std::cout << it->first << " ";

	std::cout << std::endl;

	return;
      }

      Use[regMethod] = 1;
    }
  }








//
//  // Check
//  std::cout << "Use[BDT] = "     << Use["BDT"]     << std::endl;
//  std::cout << "Use[BDT4] = "    << Use["BDT4"]    << std::endl;
//  std::cout << "Use[BDT6] = "    << Use["BDT6"]    << std::endl;
//  std::cout << "Use[BDT12] = "   << Use["BDT12"]   << std::endl;
//  std::cout << "Use[BDTG] = "    << Use["BDTG"]    << std::endl;
//  std::cout << "Use[DNN_CPU] = " << Use["DNN_CPU"] << std::endl;
//  std::cout << "Use[MLP] = "     << Use["MLP"]     << std::endl;
//  return;
//  // Check
//
//
//








  // Output file
  //----------------------------------------------------------------------------
  TString workdir = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Fall2017_nAOD_v1_Full2017v2LP19/MCl1loose2017__MCCorr2017LP19__l2loose__l2tightOR2017__PUFIXLP19";
  //  TString workdirFakes = "/eos/cms/store/group/phys_higgs/cmshww/amassiro/HWWNano/Run2017_nAOD_v1_Full2017v2LP19/DATAl1loose2017LP19__l2loose__fakeWPUFIXLP19";

  TString outfileName("VH2j_TMVA_Top.root");

  TFile* outputFile = TFile::Open(outfileName, "recreate");


  // Create the factory object. The first argument is the base of the name of all the weight files
  //----------------------------------------------------------------------------
  TString factoryName("TMVAClassification_VH2j_Top");

  TMVA::Factory* factory = new TMVA::Factory(factoryName, outputFile,
					     "!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification");

  TMVA::DataLoader* dataloader = new TMVA::DataLoader("dataset");

  dataloader->AddVariable("mll",          'F');
  dataloader->AddVariable("mjj",          'F');
  dataloader->AddVariable("mth",          'F');
  //  dataloader->AddVariable("Lepton_pt[0]", 'F');
  dataloader->AddVariable("Lepton_pt[1]", 'F');
  //  dataloader->AddVariable("detajj",       'F');
  dataloader->AddVariable("mindetajl:=mindetajl(CleanJet_eta[0],CleanJet_eta[1],Lepton_eta[0],Lepton_eta[1])", 'F');
  //  dataloader->AddVariable("Jet_qgl[CleanJet_jetIdx[0]]", 'F');
  //  dataloader->AddVariable("Jet_qgl[CleanJet_jetIdx[1]]", 'F');
  //  dataloader->AddVariable("PuppiMET_pt", 'F');
  dataloader->AddVariable("dphijj:=dphijj(CleanJet_pt[0], CleanJet_eta[0], CleanJet_phi[0], CleanJet_pt[1], CleanJet_eta[1], CleanJet_phi[1])", 'F');
  dataloader->AddVariable("dphill", 'F');
  //  dataloader->AddVariable("Sum$(CleanJet_pt>30)", 'F');
//  dataloader->AddVariable("Alt$(FatJet_pt[0],0)", 'F');
//  dataloader->AddVariable("Alt$(FatJet_phi[0],0)", 'F');
//  dataloader->AddVariable("Alt$(FatJet_eta[0],0)", 'F');
//  dataloader->AddVariable("pTWW", 'F');
  //  dataloader->AddVariable("ptll", 'F');
  //  dataloader->AddVariable("PV_npvsGood", 'F');
  //  dataloader->AddVariable("ptjj:=ptjj(CleanJet_pt[0], CleanJet_eta[0], CleanJet_phi[0], CleanJet_pt[1], CleanJet_eta[1], CleanJet_phi[1])", 'F');
//  dataloader->AddVariable("Sum$(CleanJet_pt>30)", 'F');
//  dataloader->AddVariable("Lepton_pt[0]", 'F');
//  dataloader->AddVariable("Lepton_phi[0]", 'F');
  dataloader->AddVariable("Lepton_eta[0]", 'F');
//  dataloader->AddVariable("Lepton_pt[1]", 'F');
//  dataloader->AddVariable("Lepton_phi[1]", 'F');
//  dataloader->AddVariable("Lepton_eta[1]", 'F');
  dataloader->AddVariable("CleanJet_pt[0]", 'F');
//  dataloader->AddVariable("CleanJet_phi[0]", 'F');
//  dataloader->AddVariable("CleanJet_eta[0]", 'F');
//  dataloader->AddVariable("CleanJet_pt[1]", 'F');
//  dataloader->AddVariable("CleanJet_phi[1]", 'F');
  dataloader->AddVariable("CleanJet_eta[1]", 'F');
//  dataloader->AddVariable("MET_pt", 'F');
//  dataloader->AddVariable("MET_phi", 'F');
//
  //  dataloader->AddVariable("detajjllmet:=detajjllmet(1", 'F');
  // Input files
  //----------------------------------------------------------------------------
  std::vector<TFile*> InputFiles_signal;
  std::vector<TFile*> InputFiles_background;

  InputFiles_signal.clear();
  InputFiles_background.clear();

  for (UInt_t k=0; k<50; k++) {

    if (k < 20) InputFiles_signal.push_back(TFile::Open(Form("%s/nanoLatino_HZJ_HToWW_M120__part%d.root",      workdir.Data(), k)));  // Use M125 when available
    if (k < 19) InputFiles_signal.push_back(TFile::Open(Form("%s/nanoLatino_GluGluZH_HToWW_M125__part%d.root", workdir.Data(), k)));
    if (k < 21) InputFiles_signal.push_back(TFile::Open(Form("%s/nanoLatino_HWplusJ_HToWW_M125__part%d.root",  workdir.Data(), k)));
    if (k < 29) InputFiles_signal.push_back(TFile::Open(Form("%s/nanoLatino_HWminusJ_HToWW_M125__part%d.root", workdir.Data(), k)));

    if (k < 50) InputFiles_signal.push_back(TFile::Open(Form("%s/nanoLatino_GluGluHToWWTo2L2NuPowheg_M125__part%d.root", workdir.Data(), k))); //ggH

    if (k < 28) InputFiles_signal.push_back(TFile::Open(Form("%s/nanoLatino_VBFHToWWTo2L2NuPowheg_M125__part%d.root", workdir.Data(), k))); //VBF/qqH

    if (k < 11) InputFiles_signal.push_back(TFile::Open(Form("%s/nanoLatino_ttHToNonbb_M125__part%d.root", workdir.Data(), k))); //ttH

    if (k < 3)  InputFiles_signal.push_back(TFile::Open(Form("%s/nanoLatino_GluGluHToTauTau_M125__part%d.root", workdir.Data(), k))); //Htautau
    if (k < 11) InputFiles_signal.push_back(TFile::Open(Form("%s/nanoLatino_VBFHToTauTau_M125__part%d.root", workdir.Data(), k))); //Htautau
    if (k < 6)  InputFiles_signal.push_back(TFile::Open(Form("%s/nanoLatino_HZJ_HToTauTau_M125__part%d.root", workdir.Data(), k))); //Htautau
    if (k < 9)  InputFiles_signal.push_back(TFile::Open(Form("%s/nanoLatino_HWplusJ_HToTauTau_M125__part%d.root", workdir.Data(), k))); //Htautau
    if (k < 6)  InputFiles_signal.push_back(TFile::Open(Form("%s/nanoLatino_HWminusJ_HToTauTau_M125__part%d.root", workdir.Data(), k))); //Htautau

    if (k < 10) InputFiles_background.push_back(TFile::Open(Form("%s/nanoLatino_TTTo2L2Nu__part%d.root", workdir.Data(), k))); //Top
    if (k < 12) InputFiles_background.push_back(TFile::Open(Form("%s/nanoLatino_ST_s-channel__part%d.root", workdir.Data(), k)));
    if (k < 9)  InputFiles_background.push_back(TFile::Open(Form("%s/nanoLatino_ST_t-channel_antitop__part%d.root", workdir.Data(), k)));
    if (k < 36) InputFiles_background.push_back(TFile::Open(Form("%s/nanoLatino_ST_t-channel_top__part%d.root", workdir.Data(), k)));
    if (k < 9)  InputFiles_background.push_back(TFile::Open(Form("%s/nanoLatino_ST_tW_antitop__part%d.root", workdir.Data(), k)));
    if (k < 11) InputFiles_background.push_back(TFile::Open(Form("%s/nanoLatino_ST_tW_top__part%d.root", workdir.Data(), k)));


  }


  // Apply cuts on the signal and background samples (can be different)
  //----------------------------------------------------------------------------
  TCut mycuts;
  TCut mycutb;

  mycuts = "PuppiMET_pt > 20 \
            && Alt$(Jet_qgl[CleanJet_jetIdx[0]],1) > 0 \
            && Alt$(Jet_qgl[CleanJet_jetIdx[1]],1) > 0 \
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
            && Sum$(CleanJet_pt > 20 && Jet_btagDeepB[CleanJet_jetIdx] > 0.1522) == 0 \
            && mjj < 200 \
            && abs(detajj) < 3.5 \
            && (mth > 60 && mth < 125)";

           


  mycutb = mycuts;

  
  // Create factory for signal and background samples
  double tmpWeight = 1.;

  for (UInt_t i=0; i<InputFiles_signal.size(); ++i) {
    TTree* tmpsTree = (TTree*)InputFiles_signal.at(i)->Get("Events");
    dataloader->AddSignalTree(tmpsTree, tmpWeight);
  }

  for (UInt_t k=0; k<InputFiles_background.size(); ++k) {
    TTree* tmpbTree = (TTree*)InputFiles_background.at(k)->Get("Events");
    dataloader->AddBackgroundTree(tmpbTree, tmpWeight);
  }

//dataloader->SetSignalWeightExpression    ("XSWeight*GenLepMatch2l*SFweight2l*bPogSF_CMVAL*LepCut2l__ele_mva_90p_Iso2016__mu_cut_Tight80x*LepSF2l__ele_mva_90p_Iso2016__mu_cut_Tight80x*METFilter_MC");
//dataloader->SetBackgroundWeightExpression("XSWeight*GenLepMatch2l*SFweight2l*bPogSF_CMVAL*LepCut2l__ele_mva_90p_Iso2016__mu_cut_Tight80x*LepSF2l__ele_mva_90p_Iso2016__mu_cut_Tight80x*METFilter_MC");


// eleWP='mvaFall17Iso_WP90'
// muWP='cut_Tight_HWWW'
//
// LepWPCut        = 'LepCut'+Nlep+'l__ele_'+eleWP+'__mu_'+muWP
// LepWPweight     = 'LepSF'+Nlep+'l__ele_'+eleWP+'__mu_'+muWP
//
// XSWeight      = 'XSWeight'
// SFweight      = 'SFweight'+Nlep+'l*'+LepWPweight+'*'+LepWPCut+'*PrefireWeight'
// GenLepMatch   = 'GenLepMatch'+Nlep+'l'

//
 dataloader->SetSignalWeightExpression("XSWeight*METFilter_MC*SFweight2l*LepCut2l__ele_mvaFall17Iso_WP90__mu_cut_Tight_HWWW*LepSF2l__ele_mvaFall17Iso_WP90__mu_cut_Tight_HWWW*PrefireWeight*GenLepMatch2l");
 dataloader->SetBackgroundWeightExpression("XSWeight*METFilter_MC*SFweight2l*LepCut2l__ele_mvaFall17Iso_WP90__mu_cut_Tight_HWWW*LepSF2l__ele_mvaFall17Iso_WP90__mu_cut_Tight_HWWW*PrefireWeight*GenLepMatch2l");
//
//dataloader->PrepareTrainingAndTestTree(mycuts, mycutb, "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random::SplitSeed=10:NormMode=None:!V");
//dataloader->PrepareTrainingAndTestTree(mycuts, mycutb, "nTrain_Signal=2404:nTrain_Background=11671:SplitMode=Block::SplitSeed=10:NormMode=EqualNumEvents");
//dataloader->PrepareTrainingAndTestTree(mycuts, mycutb, "SplitMode=Alternate::SplitSeed=10:NormMode=EqualNumEvents");
//dataloader->PrepareTrainingAndTestTree(mycuts, mycutb, "SplitMode=Random:NormMode=NumEvents:!V");
  dataloader->PrepareTrainingAndTestTree(mycuts, mycutb, "SplitMode=Random::SplitSeed=10:NormMode=EqualNumEvents");


  // Book MVA methods
  //----------------------------------------------------------------------------
  if (Use["BDT"]) factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT",
				      "!H:!V:NTrees=250:MinNodeSize=0.5%:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.1:SeparationType=GiniIndex:nCuts=20");

  if (Use["BDT4"]) factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT4",
				       "!H:!V:NTrees=800:MinNodeSize=0.5%:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.1:SeparationType=GiniIndex:nCuts=20");

  if (Use["BDT6"]) factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT6",
				       "!H:!V:NTrees=500:MinNodeSize=2.5%:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.1:SeparationType=GiniIndex:nCuts=500");

  if (Use["BDT12"]) factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT12",
					"!H:!V:NTrees=500:MinNodeSize=0.5%:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.2:SeparationType=GiniIndex:nCuts=20");

  if (Use["BDTG"]) factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDTG",
					"!H:!V:NTrees=500:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.50:nCuts=20:MaxDepth=2:SkipNormalization=1");

  if (Use["MLP"]) // neural network
     factory->BookMethod( dataloader, TMVA::Types::kMLP, "MLP", "!H:!V:NeuronType=tanh:NCycles=1000:HiddenLayers=N+5,5:TestRate=5:EstimatorType=MSE");

  if (Use["DNN_CPU"]) {
    TString layoutString("Layout=TANH|100,TANH|50,TANH|10,LINEAR");
    TString training0("LearningRate=1e-1, Momentum=0.5, Repetitions=1, ConvergenceSteps=10,"
		      " BatchSize=256, TestRepetitions=10, Multithreading=True");
    TString training1("LearningRate=1e-2, Momentum=0.0, Repetitions=1, ConvergenceSteps=10,"
		      " BatchSize=256, TestRepetitions=7, Multithreading=True");
    TString trainingStrategyString("TrainingStrategy=");
    trainingStrategyString += training0 + "|" + training1;
    TString nnOptions("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=N:"
		      "WeightInitialization=XAVIERUNIFORM:Architecture=CPU");
    nnOptions.Append(":");
    nnOptions.Append(layoutString);
    nnOptions.Append(":");
    nnOptions.Append(trainingStrategyString);
    factory->BookMethod(dataloader, TMVA::Types::kDNN, "DNN_CPU", nnOptions);
  }
 
  // Now you can tell the factory to train, test, and evaluate the MVAs
  //----------------------------------------------------------------------------
  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();    


  // Save the output
  //----------------------------------------------------------------------------
  outputFile->Close();

  delete factory;
  delete dataloader;

  
  // Launch the GUI for the root macros
  //----------------------------------------------------------------------------
  if (!gROOT->IsBatch()) TMVA::TMVAGui(outfileName);
}
