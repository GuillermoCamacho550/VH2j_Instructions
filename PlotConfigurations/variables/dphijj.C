#include "TLorentzVector.h"

float dphijj(float jetpt1, float jeteta1, float jetphi1,
	      float jetpt2, float jeteta2, float jetphi2)
{
  TLorentzVector jet1;
  jet1.SetPtEtaPhiM(jetpt1,jeteta1,jetphi1,0);

  TLorentzVector jet2;
  jet2.SetPtEtaPhiM(jetpt2,jeteta2,jetphi2,0);

  return fabs(jet1.DeltaPhi(jet2));
}
