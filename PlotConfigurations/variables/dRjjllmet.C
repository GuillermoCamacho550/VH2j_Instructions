#include "TLorentzVector.h"

float dRjjllmet(float jetpt1, float jeteta1, float jetphi1,
	          float jetpt2, float jeteta2, float jetphi2,
                  float leptonpt1, float leptoneta1, float leptonphi1,
                  float leptonpt2, float leptoneta2, float leptonphi2,
                  float metpt, float metphi)         
{
  TLorentzVector jet1;
  jet1.SetPtEtaPhiM(jetpt1,jeteta1,jetphi1,0);

  TLorentzVector jet2;
  jet2.SetPtEtaPhiM(jetpt2,jeteta2,jetphi2,0);

  TLorentzVector lepton1;
  lepton1.SetPtEtaPhiM(leptonpt1,leptoneta1,leptonphi1,0);

  TLorentzVector lepton2;
  lepton2.SetPtEtaPhiM(leptonpt2,leptoneta2,leptonphi2,0);

  TLorentzVector met;
  met.SetPtEtaPhiM(metpt,0,metphi,0);


  return fabs((jet1+jet2).DeltaR(lepton1+lepton2+met));
}
