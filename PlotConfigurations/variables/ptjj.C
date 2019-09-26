#include "TLorentzVector.h"

double ptjj(double jetpt1, double jeteta1, double jetphi1, double jetpt2, 
	   double jeteta2, double jetphi2){
  TLorentzVector jet1;
  jet1.SetPtEtaPhiM(jetpt1,jeteta1,jetphi1,0);
  TLorentzVector jet2;
  jet2.SetPtEtaPhiM(jetpt2,jeteta2,jetphi2,0);

  return fabs((jet1+jet2).Pt());
}
