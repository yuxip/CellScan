/********************************************************************
* FpdchanDict.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error FpdchanDict.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G__ANSIHEADER
#define G__DICTIONARY
#define G__PRIVATE_GVALUE
#include "G__ci.h"
#include "FastAllocString.h"
extern "C" {
extern void G__cpp_setup_tagtableFpdchanDict();
extern void G__cpp_setup_inheritanceFpdchanDict();
extern void G__cpp_setup_typetableFpdchanDict();
extern void G__cpp_setup_memvarFpdchanDict();
extern void G__cpp_setup_globalFpdchanDict();
extern void G__cpp_setup_memfuncFpdchanDict();
extern void G__cpp_setup_funcFpdchanDict();
extern void G__set_cpp_environmentFpdchanDict();
}


#include "TObject.h"
#include "TMemberInspector.h"
#include "Fpdchan.h"
#include "CalibStr.h"
#include "dataSet.h"
#include "FpdMap.h"
#include "Fill.h"
#include "FillData.h"
#include "RunData.h"
#include "Filentry.h"
#include "FilesSet.h"
#include "AnalTools.h"
#include "Geom.h"
#include "FitTower.h"
#include "Trigger.h"
#include "simh112.h"
#include "Sim.h"
#include "Cell.h"
#include "Gen.h"
#include "Qt.h"
#include "poutTree.h"
#include "LVec.h"
#include "ClusterQt.h"
#include "Yiqun.h"
#include "TrigQt.h"
#include "WasExternal.h"
#include "TowerFPD.h"
#include "PhotonHitFPD.h"
#include "HitCluster.h"
#include "TowerUtil.h"
#include "Vertex.h"
#include "Sheet.h"
#include "CellTDep.h"
#include "RunDepCor.h"
#include "O2Output.h"
#include "RunDepMgr.h"
#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__FpdchanDictLN_TClass;
extern G__linked_taginfo G__FpdchanDictLN_TBuffer;
extern G__linked_taginfo G__FpdchanDictLN_TMemberInspector;
extern G__linked_taginfo G__FpdchanDictLN_TObject;
extern G__linked_taginfo G__FpdchanDictLN_TString;
extern G__linked_taginfo G__FpdchanDictLN_vectorlEunsignedsPintcOallocatorlEunsignedsPintgRsPgR;
extern G__linked_taginfo G__FpdchanDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR;
extern G__linked_taginfo G__FpdchanDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__FpdchanDictLN_TObjArray;
extern G__linked_taginfo G__FpdchanDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR;
extern G__linked_taginfo G__FpdchanDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__FpdchanDictLN_TVectorTlEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TVectorTlEdoublegR;
extern G__linked_taginfo G__FpdchanDictLN_TF1;
extern G__linked_taginfo G__FpdchanDictLN_TH1D;
extern G__linked_taginfo G__FpdchanDictLN_TH1;
extern G__linked_taginfo G__FpdchanDictLN_TH1F;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTBaselEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTBaselEdoublegR;
extern G__linked_taginfo G__FpdchanDictLN_TH2F;
extern G__linked_taginfo G__FpdchanDictLN_TH2D;
extern G__linked_taginfo G__FpdchanDictLN_TElementActionTlEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TElementPosActionTlEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTlEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTRow_constlEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTRowlEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTDiag_constlEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTColumn_constlEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTFlat_constlEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTSub_constlEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTSparseRow_constlEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTSparseDiag_constlEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTColumnlEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTDiaglEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTFlatlEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTSublEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTSparseRowlEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_TMatrixTSparseDiaglEfloatgR;
extern G__linked_taginfo G__FpdchanDictLN_FpdMap;
extern G__linked_taginfo G__FpdchanDictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR;
extern G__linked_taginfo G__FpdchanDictLN_CalibStr;
extern G__linked_taginfo G__FpdchanDictLN_TFile;
extern G__linked_taginfo G__FpdchanDictLN_TTree;
extern G__linked_taginfo G__FpdchanDictLN_TBranch;
extern G__linked_taginfo G__FpdchanDictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR;
extern G__linked_taginfo G__FpdchanDictLN_TTreeFormula;
extern G__linked_taginfo G__FpdchanDictLN_TChain;
extern G__linked_taginfo G__FpdchanDictLN_Filentry;
extern G__linked_taginfo G__FpdchanDictLN_FillData;
extern G__linked_taginfo G__FpdchanDictLN_FilesSet;
extern G__linked_taginfo G__FpdchanDictLN_Fill;
extern G__linked_taginfo G__FpdchanDictLN_RunData;
extern G__linked_taginfo G__FpdchanDictLN_TVector3;
extern G__linked_taginfo G__FpdchanDictLN_Geom;
extern G__linked_taginfo G__FpdchanDictLN_Qt;
extern G__linked_taginfo G__FpdchanDictLN_dataSet;
extern G__linked_taginfo G__FpdchanDictLN_TLorentzVector;
extern G__linked_taginfo G__FpdchanDictLN_TMinuit;
extern G__linked_taginfo G__FpdchanDictLN_TF2;
extern G__linked_taginfo G__FpdchanDictLN_TRandom;
extern G__linked_taginfo G__FpdchanDictLN_WasExternal;
extern G__linked_taginfo G__FpdchanDictLN_TowerFPD;
extern G__linked_taginfo G__FpdchanDictLN_PhotonHitFPD;
extern G__linked_taginfo G__FpdchanDictLN_HitCluster;
extern G__linked_taginfo G__FpdchanDictLN_TPaveText;
extern G__linked_taginfo G__FpdchanDictLN_TowerUtil;
extern G__linked_taginfo G__FpdchanDictLN_FitTower;
extern G__linked_taginfo G__FpdchanDictLN_Yiqun;
extern G__linked_taginfo G__FpdchanDictLN_Trigger;
extern G__linked_taginfo G__FpdchanDictLN_TRandom1;
extern G__linked_taginfo G__FpdchanDictLN_simh112;
extern G__linked_taginfo G__FpdchanDictLN_TH3F;
extern G__linked_taginfo G__FpdchanDictLN_Sim;
extern G__linked_taginfo G__FpdchanDictLN_LVec;
extern G__linked_taginfo G__FpdchanDictLN_TGraph;
extern G__linked_taginfo G__FpdchanDictLN_poutTree;
extern G__linked_taginfo G__FpdchanDictLN_TGraphErrors;
extern G__linked_taginfo G__FpdchanDictLN_vectorlEstringcOallocatorlEstringgRsPgR;
extern G__linked_taginfo G__FpdchanDictLN_reverse_iteratorlEvectorlEstringcOallocatorlEstringgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__FpdchanDictLN_CellTDep;
extern G__linked_taginfo G__FpdchanDictLN_vectorlETStringcOallocatorlETStringgRsPgR;
extern G__linked_taginfo G__FpdchanDictLN_reverse_iteratorlEvectorlETStringcOallocatorlETStringgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__FpdchanDictLN_RunDepCor;
extern G__linked_taginfo G__FpdchanDictLN_RunDepMgr;
extern G__linked_taginfo G__FpdchanDictLN_tpCellDat;
extern G__linked_taginfo G__FpdchanDictLN_tpCellRec;
extern G__linked_taginfo G__FpdchanDictLN_CellHit;
extern G__linked_taginfo G__FpdchanDictLN_Cell;
extern G__linked_taginfo G__FpdchanDictLN_Vertex;
extern G__linked_taginfo G__FpdchanDictLN_ClusterQt;
extern G__linked_taginfo G__FpdchanDictLN_TrigQt;
extern G__linked_taginfo G__FpdchanDictLN_TGraph2DErrors;
extern G__linked_taginfo G__FpdchanDictLN_AnalTools;
extern G__linked_taginfo G__FpdchanDictLN_Gen;
extern G__linked_taginfo G__FpdchanDictLN_SheetHistory;
extern G__linked_taginfo G__FpdchanDictLN_Sheet;
extern G__linked_taginfo G__FpdchanDictLN_O2Output;

/* STUB derived class for protected member access */
