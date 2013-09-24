#ifndef poutTree_
#define poutTree_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TChain.h"
#include "TFile.h"
#include "LVec.h"
#include "Geom.h"
#include "TH2F.h"
#include "TPaveText.h"
#include "TGraph.h"


class poutTree : public TObject
{
 public:
  poutTree(char* filelist);
  ~poutTree();
  Int_t GetEntry(Int_t evt);
  TLorentzVector vec[80];
  Int_t tpes[320];
  Int_t spin;
  Int_t nlv[10];
  Int_t plv[10][10];
  Int_t nph2[10];
  Int_t pph2[10][10];
  Int_t nph3[10];
  Int_t pph3[10][10];
  Int_t nphotons;
  Int_t TrigBits;
  TObjArray* ClusterScratch(Float_t maxsep=.15);
  TLorentzVector ClusterHardE(Int_t ClusterListNumber);
  TLorentzVector ClusterSoftE(Int_t ClusterListNumber,Float_t maxsep);
  Float_t MinEnergy[10];
  Float_t pxyzt[320];
  Int_t nwrds;
  Int_t strig;
  Float_t BBcVertex;
  void ClusterwithYPhi(Bool_t UseYPhi=true){ClusterYPhi=UseYPhi;};
  Char_t BBcSums[5];
  //The following are the EDep Gains from SPin08 (module corrections)
  Float_t EDepSlope[8];
  Float_t EDepInt[8];
  //
  TLorentzVector EdepCorr(TLorentzVector vold,Int_t vtype);
  Int_t EnableEdepCorr;
  Int_t EventN;
  Int_t Rnum;
  Int_t ievt;
  Int_t Bunchid7bit;
  Float_t adc[98];
  //Define a Tree
  TChain* p_out;
  Int_t entry;
  Int_t nentries;
  void Print();
  Bool_t YellowUp;
  Bool_t YellowDn;
  Bool_t BlueUp;
  Bool_t BlueDn;
  Int_t Nphotons;
  Int_t Ndet;
  TVector3 PosInDet(LVec* lv,Geom* pgeom,Float_t zinteraction=0.);
  Int_t WhatEW(LVec* lv);
  Int_t WhatNSTB(LVec* lv);
  Int_t WhatRow0(LVec* lv,Geom* pgeom,Float_t zinteraction=0.);
  Int_t WhatCol0(LVec* lv,Geom* pgeom,Float_t zinteraction=0.);
  Int_t NearEdge(LVec* lv,Geom* pgeom,Float_t d=.5, Float_t zinteraction=0.);
  TLorentzVector* p_photvec[80];
  TLorentzVector* p_detvec[80];
  Int_t GetNPhotVec(Int_t iew,Int_t nstb);
  Int_t GetNDetVec(Int_t iew,Int_t nstb);
  Float_t TotaldetE;
  Float_t TotalphotE;
  TLorentzVector Pair4V(LVec* lv);
  TLorentzVector GetPhotVec(Int_t iew,Int_t nstb,Int_t index);
  TLorentzVector GetDetVec(Int_t iew,Int_t nstb,Int_t index);
  LVec* GetPairNearMass(TObjArray* pvec,Float_t massgoal=.135 );
  void ClearScratch(){scratchlist->RemoveAll();};
  TObjArray* AddToScratch(Int_t vtyp);
  TObjArray* RemoveFromScratch(LVec* v);
  TLorentzVector SumScratch(); 
  TLorentzVector SumList(TObjArray* p_list);
  TObjArray* AllToScratch(Bool_t includesoft=false);
  void histpair(LVec* vv,Geom* p_geom, Float_t ziniteraction=0.,Int_t hnum=0);
  TObjArray* vlist;
  TObjArray* scratchlist;
  TObjArray* dlist;
  TObjArray* softlist;
  TObjArray* ClusterList;
  TH2F* p_fms[4][2];
  void PrintList(TObjArray* plist);
  TVector3 reframe(TLorentzVector vec,TLorentzVector zvec,TLorentzVector bvec);
  unsigned int SavedHits[1000];
  Int_t nSavedHits;
  TMatrix FillFMSADC(Int_t NSTB);
  void DrawFMSADC(Int_t iew, Int_t instb,Geom*);

 private:
  TGraph* gr[4];
  TH2F* gradc[4];
  TLorentzVector nullvec;
  Bool_t ClusterYPhi;

  ClassDef(poutTree,3); 
};

#endif
