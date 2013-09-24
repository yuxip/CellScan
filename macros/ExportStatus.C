//export cell status for a particluar run to status_run${run}.txt file 
//Yuxi Pan 01/07/2012
#include<fstream>
using namespace std;

  Int_t ExportStatus(Long_t run = 12098007, Int_t instb = 1){

	gROOT->LoadMacro("/home/yuxip/FMS/CellScan/macros/Legal.C");
	gSystem->Load("/home/yuxip/FMS/root12fms/FpdRoot/Fpdchan.so");
	const char* celldir	= "/home/yuxip/FMS/CellScan/cells";
	TString cellstatusdir(celldir);
	const char* outputdir	= "/home/yuxip/FMS/CellScan/status";
	Char_t outfile[100];
	sprintf(outfile,"%s/status_run%ld_n%d.txt",outputdir,run,instb);
	cout<<"outfile: "<<outfile<<endl;
	fstream output(outfile,ios::out);

//	Int_t nstb	= 0;
//	Int_t row0	= 0;
//	Int_t col0	= 0;
	Int_t ch	= 0;
	Char_t 	cellname[30];
	UChar_t	status	= 0;
	Int_t ncell	= 0;
	Int_t nrow	= 0;
	Int_t ncol	= 0;

	for(Int_t nstb = instb; nstb < instb + 1; nstb++){
		
		if(nstb<3){	//large cells
			nrow = 34;
			ncol = 17;
		}
		else{		//small cells
			nrow = 24;
			ncol = 12;
		}

		for(Int_t row0 = 0; row0 < nrow; row0++){
			for(Int_t col0 = 0; col0 < ncol; col0++){
				
				ch = ncol*row0 + col0 + 1;
				if(!Legal(2,nstb,row0,col0)){
				//	cout<<2<<" "<<nstb<<" "<<ch<<" 0xff"<<endl;
					output<<2<<" "<<nstb<<" "<<ch<<" 0xff"<<endl;
					continue;
				}
			
				sprintf(cellname,"Cellr%d_c%d_n%d",row0,col0,nstb);
				TString cn(cellname);
				TString cellfile = cellstatusdir + "/" + cn + ".root";

				TFile* fcell = new TFile(cellfile,"read");
				if(!fcell){
					cout<<"cellfile: "<<cellfile<<" does not exist!!"<<endl;
					return 0;
				}

				MyCell* mcell = (MyCell*)fcell->Get(cellname);
				if(!mcell){
					cout<<"cell object: "<<cellname<<" does not exist!!"<<endl;
					return 0;
				}
				status = mcell->GetStatusBit(run);
				//cout<<2<<" "<<nstb<<" "<<ch<<" 0x"<<hex<<(Int_t)status<<endl;
				output<<2<<" "<<nstb<<" "<<ch<<" 0x"<<hex<<(Int_t)status<<endl;
				//cout<<dec;
				output<<dec;
				fcell->Close();
				delete fcell;
				ncell++;
			}
		}
	}
//	cout<<"ncell: "<<ncell<<endl;
	
	return 1;
  }			
