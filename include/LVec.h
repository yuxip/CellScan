#ifndef LVec_
#define LVec_
#include <stdio.h>
#include <stdlib.h>
#include "TLorentzVector.h"
#include "TObjArray.h"

class LVec : public TLorentzVector
{
 public:
  
  LVec(Int_t iew,Int_t nstb,TLorentzVector v);
  LVec(Int_t vtyp,TLorentzVector v);
  Int_t Iew; // 1 or 2
  Int_t Nstb; //1, 2, 3 or 4
  Int_t Vtype;
  LVec* Partner; // points to partner in original list 
  //(can be reset externally)
  TLorentzVector NewPartner;//4 Vect of partner set internally
  Float_t PairMass();
  Float_t PairMass(LVec* pv2);
  void SetPartner(LVec* newpartner);
 private:
  ClassDef(LVec,2);
};

#endif
