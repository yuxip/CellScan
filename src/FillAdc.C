#include "MyCellMgr.h"
#include "TSystem.h"
#include "TCut.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

 Int_t main(){
	
//	gSystem->Load("MyCellMgr.so");
	const char* runlist = "/home/yuxip/FMS/CellScan/fill15408.txt";
	const char* option = "recreate";
	cout<<"running FillAdc()..."<<endl;

	fstream infile(runlist,ios::in);
	if(!infile){
		cout<<"runlist: "<<runlist<<" does not exist!!"<<endl;
		return 0;
	}
	Long_t runnum;
	char outname[30];
	const char* datadir = "/home/yuxip/FMS/CellScan/data";
//	const char* datadir = "/home/yuxip/FMS/root12fms/Output";
	while(infile>>runnum){
	
		cout<<"processing run: "<<runnum<<endl;
		sprintf(outname,"MyCellsNewBBCAsy_run%ld.root",runnum);
		MyCellMgr* cmgr = new MyCellMgr(runnum,outname,option);
//		TCut cuts = "Ntracks==2&&N12==2&&abs(E12-40)<20&&Esoft<1&&Z<.7&&((TrigBits)&(0x200))&&sqrt(pow(Y2-Y1,2)+pow(Phi2-Phi1,2))<.2";
//		TCut cuts = "Ntracks==2&&N12==2&&abs(E12-40)<20&&Esoft<1&&Z<.7";
		TCut cuts = "Ntracks==2&&N12==2&&E12>10"; //pion sample cut, 2-photon event
		cmgr->ReadOutput(datadir,cuts,kFALSE);
		cmgr->ReadADC(datadir,kTRUE);
		cout<<"run: "<<runnum<<" finished..."<<endl;
		delete cmgr;
		cmgr = 0;	
	}
		return 1;
	
  }
