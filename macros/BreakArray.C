#include <fstream>
#include "TKey.h"
using namespace::std;
  
  Int_t BreakArray( const char* outfilename = "/home/yuxip/FMS/CellScan/MyCellsNewBBCAsy_Fill15419_4.root" ){
	
	gSystem->Load("/home/yuxip/FMS/CellScan/lib/libMyCellMgr.so");
	gROOT->LoadMacro("/home/yuxip/FMS/CellScan/macros/Legal.C");
	
	TFile* fout = new TFile(outfilename,"read");
	if(!fout){
		cout<<outfilename<<" does not exist!!"<<endl;
		return -1;
	}

//	MyCellMgr* cmgr = new MyCellMgr(0,outfilename,"update");
//	TDirectory* t1 = gDirectory;	
        Geom* mGeom = new Geom("./","geom.txt");
//	fstream inlist(runlist,ios::in);
//	if(!inlist){
//		cout<<runlist<<" does not exist!!"<<endl;
//		return -1;
//	}
//	Char_t 		infilename[100];
//	Char_t 		cn[30];
	Char_t		cellout[100];
	Int_t 		runnum;
	MyCell* 	mcell 	= 0;
	TFile*		fcell 	= 0;
	TIter nextkey(fout->GetListOfKeys());
	TKey* key;
	while(key = (TKey*)nextkey()){
		mcell = (MyCell*)key->ReadObj();
		mcell->SetGeom(mGeom);
		const char* cn = mcell->GetName();
		cout<<"reading "<<cn<<endl;
		sprintf(cellout,"/home/yuxip/FMS/CellScan/cells/%s.root",cn);
		fcell = new TFile(cellout,"recreate");
		mcell->Write("",TObject::kOverwrite);
		fcell->Close();			
		mcell = 0;
	}

	return 1;
  }
