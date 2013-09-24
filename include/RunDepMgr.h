#ifndef RunDepMgr_
#define RunDepMgr_

#include "RunDepCor.h"
#include "CellTDep.h"
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TString.h"
#include "TObjArray.h"
#include "TSystem.h"
#include "dataSet.h"

class RunDepMgr : public TObject
{
 public:
  RunDepMgr();
  RunDepMgr(char* RunDepPath, Int_t BaseOveride=0);
  RunDepCor* SetBase()
  {
    return SetBase(RunDepBaseOverride);
  };
  Int_t Day(Long_t rnum)
  {
    return (rnum-12000000)/1000;
  };
  void SetUnitLedFactors();
  RunDepCor* SetBase(Int_t BaseOveride);
  RunDepCor* SetRdep(Int_t Rnum);
  RunDepCor* Base; // Base for BaseOverride
  RunDepCor* Rdep;
  void Rcorrect(TMatrix* padc,TMatrix* pEmat,Int_t det,Int_t iEW,dataSet* da);
  TString RunDepPath;
  Int_t saveday;
  Int_t RunDepBaseOverride; //if non-zero then this run to be used as  base
  Bool_t UseRunDepCor;
  Float_t ledFactor[4][34][17];//
  void Addmvsday(Int_t nstb,Int_t row,Int_t col,TGraphErrors* gr);
  TObjArray* mvsday;
  void UpdateledFactors();
  void UpdateRdep();
  ~RunDepMgr();
  Int_t MissingRun;
  Bool_t Legal(Int_t iew,Int_t nstb,Int_t row0,Int_t col0);
  void SetName(char* nm){fName=nm;};
  const char* GetName() const {return (const char*) fName;};
 private:
  TString fName;
  Int_t rows[4];
  ClassDef(RunDepMgr,1);
};
#endif
