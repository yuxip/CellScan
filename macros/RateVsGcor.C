//correlations between count rate( # over threshold ) vs LED corrected gcorr, to check outliers
//--Yuxi Pan 04/03/2012
  Int_t RateVsGcor(Long_t run = 12098007){

	gSystem->Load("/home/yuxip/FMS/CellScan/lib/libMyCellMgr.so");
	gROOT->LoadMacro("/home/yuxip/FMS/CellScan/macros/Legal.C");
	Long_t baserun = 12095006;
	const char* celldir = "/home/yuxip/FMS/CellScan/cells/psu";
	const char* basedir = "/home/yuxip/FMS/CellScan/cells/psu/base";

	TGraph*	gcor[4];	//nstb 1 2 3 4
	Char_t	gname[50];
	TCanvas* c1 = new TCanvas("c1","c1",900,600);
	c1->Divide(2,2);
	Char_t 	outfilename[40];
	sprintf(outfilename,"gRateVsGcor_run%ld.ps",run);
	TString outfile(outfilename);
	TString outhead = outfile + "[";
	TString outend = outfile + "]";
//	c1->Print(outhead);

	//list of cells that need further notice
	Char_t listname[100];
	sprintf(listname,"checklist_run%ld.txt",run);
	fstream checklist(listname,ios::out);
	Int_t trigcut	= 4000;
	Float_t gcorcut = 1.0;

	//========prepare LED factors array==========//
	TString RunDepPath = gSystem->Getenv("RunDepPath");
	cout<<"RunDepPath: "<<RunDepPath<<endl;
	TFile* RdepFile = new TFile(RunDepPath,"read");
	RunDepMgr* Mgr = (RunDepMgr*)RdepFile->Get("Mgr");
	Mgr->RunDepPath = RunDepPath;
	Mgr->SetRdep(run);
	Mgr->SetBase(baserun);
	//========prepare LED factors array==========//

	TFile*	fdata;
	Char_t	fdatafile[80];
	TFile*	fbase;
	Char_t	fbasefile[80];
	Char_t	cellname[50];
	MyCell* fcell;
	MyCell* bcell;
	Int_t	AdcTh = 100;	//adc threshold
	Int_t	icell = 0;
	for(Int_t nstb = 1; nstb < 5; nstb++){
		
		sprintf(gname,"RateVsCor_run%ld_nstb%d",run,nstb);
		gcor[nstb-1] = new TGraph(0);
		gcor[nstb-1]->SetTitle(gname);
		
		Int_t nrow,ncol;
		if(nstb<3){
			nrow = 34;
			ncol = 17;
		}
		else{
			nrow = 24;
			ncol = 12;
		}
		for(Int_t row = 0; row < nrow; row++){
			for(Int_t col = 0; col < ncol; col++){
				if(!Legal(2,nstb,row,col))continue;
					
					sprintf(cellname,"Cellr%d_c%d_n%d",row,col,nstb);
					sprintf(fdatafile,"%s/%s.root",celldir,cellname);
					fdata = new TFile(fdatafile,"read");
					if(!fdata){
						cout<<fdatafile<<" does not exist!!"<<endl;
						return 0;
					}
					fcell = (MyCell*)fdata->Get(cellname);
					if(!fcell){
						cout<<"ERROR!! "<<cellname<<" object does not exist!!"<<endl;
						return 0;
					}	
					
					Float_t Ntriggers = fcell->GetNth(run,AdcTh);
					//calculate average LED for the run
					TH1F* fLed = fcell->GetLEDSpectrum(run);
					Float_t thisLedAdc = fLed->GetMean();
					Float_t bgcorr = fcell->GetGainCor();
					if(icell%50==0){
						cout<<"debug "<<cellname<<endl;
						cout<<"Ntriggers	= "<<Ntriggers<<endl;
						cout<<"thisLedAdc	= "<<thisLedAdc<<endl;
						cout<<"bgcorr		= "<<bgcorr<<endl;
					}

					//get LED adc of the base run
					sprintf(fbasefile,"%s/%s.root",basedir,cellname);
					fbase = new TFile(fbasefile,"read");
                                        if(!fbase){
                                                cout<<fbasefile<<" does not exist!!"<<endl;
                                                return 0;
                                        }
                                        bcell = (MyCell*)fbase->Get(cellname);
                                        if(!bcell){
                                                cout<<"ERROR!! "<<cellname<<" object does not exist!!"<<endl;
                                                return 0;
                                        }
					TH1F* bLed = bcell->GetLEDSpectrum(baserun);
					Float_t NominalLedAdc = bLed->GetMean();
					Float_t ledfac = Mgr->ledFactor[nstb-1][row][col];
					Float_t ngcorr = bgcorr*ledfac*NominalLedAdc / thisLedAdc;
					
					if(icell%50==0){
                                                cout<<"NominalLedAdc	= "<<NominalLedAdc<<endl;
						cout<<"LED factor	= "<<ledfac<<endl;
                                                cout<<"ngcorr		= "<<ngcorr<<endl;
                                        }
					if(ngcorr>10||Ntriggers==0)continue;
					gcor[nstb-1]->SetPoint(icell,ngcorr,Ntriggers);
					if(Ntriggers>trigcut)checklist<<nstb<<" "<<row<<" "<<col<<" "<<Ntriggers<<" "<<ngcorr<<endl;
					icell++;
					fdata->Close();
					fbase->Close();
					delete fdata;
					delete fbase;
					fdata = 0;
					fbase = 0;
				}
			}
			cout<<"# cells: "<<icell<<endl;
			cout<<"gcor["<<nstb<<"]: nPoits = "<<gcor[nstb-1]->GetN()<<endl;
			c1->cd(nstb);
			c1->GetPad(nstb)->SetLogy();
			gcor[nstb-1]->SetMarkerStyle(2);
			gcor[nstb-1]->SetMarkerColor(nstb);
			gcor[nstb-1]->Draw("Ap");
			//c1->Print(outfile);
			//c1->Clear();
		}
	c1->Print(outfile);	
	RdepFile->Close();
	delete RdepFile;
	return 1;
  }				
