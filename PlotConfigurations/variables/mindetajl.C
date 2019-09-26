#include "TLorentzVector.h"



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


