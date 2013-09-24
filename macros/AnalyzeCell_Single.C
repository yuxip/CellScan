//report status analysis on a single cell
//Yuxi Pan 02/07/2012

  Int_t AnalyzeCell_Single( Long_t runnum = 12098007,Int_t nstb = 3, Int_t row0 = 4, Int_t col0 = 6){
	
	gStyle->SetPalette(1);
	gSystem->Load("../lib/libMyCellMgr.so");
	gROOT->LoadMacro("/home/yuxip/FMS/CellScan/macros/Legal.C");
	gROOT->LoadMacro("/home/yuxip/FMS/CellScan/macros/ConvertTH2Bin.C");
	mGeom = new Geom("./","geom.txt");
	cout<<"runnum: "<<runnum<<endl;
	cout<<"analyzing Cellr"<<row0<<"_c"<<col0<<"_n"<<nstb<<endl;
	if(!Legal(2,nstb,row0,col0)){
		cout<<"illegal cell index"<<endl;
		return 0;
	}
	const char* celldir = "/home/yuxip/FMS/CellScan/cells/psu";
	Char_t outfilename[50];
	sprintf(outfilename,"CellAnalysisRun%ld_n%d_r%d_c%d.ps",runnum,nstb,row0,col0);
	TString outfile(outfilename);

	//=====prepare LED factors array, calculate t-dependent gcorr================//
	Int_t baserun = 12095006;
	TString RunDepPath = gSystem->Getenv("RunDepPath");
        cout<<"RunDepPath: "<<RunDepPath<<endl;
        TFile* RdepFile = new TFile(RunDepPath,"read");
        RunDepMgr* Mgr = (RunDepMgr*)RdepFile->Get("Mgr");
        Mgr->RunDepPath = RunDepPath;
        Mgr->SetRdep(runnum);
        Mgr->SetBase(baserun);
	
	const char* gaincor = gSystem->Getenv("FMSCORR");
	cout<<"gaincor: "<<gaincor<<endl;
	if(!gaincor){
                cout<<"Please set env. $FMSCORR"<<endl;
                exit(0);
        }
	CalibStr* mGcor = new CalibStr(12000000,gaincor);
	Float_t ogcorr = mGcor->GetValue(2,nstb,row0,col0);
	Float_t ledfac = Mgr->ledFactor[nstb-1][row0][col0];
	Float_t ledcor = Mgr->Rdep->Cdep(nstb,row0,col0)->FixFactor(0,0,1);	//return led corr for the running average
	Float_t ngcorr = ogcorr*ledfac*ledcor;
	cout<<"=======	gaincors: "<<endl;
	cout<<"		ogcorr: "<<ogcorr<<endl;
	cout<<"		ledfac:	"<<ledfac<<endl;
	cout<<"		ledcor: "<<ledcor<<endl;
	cout<<"		ngcorr: "<<ngcorr<<endl;
	cout<<"================="<<endl;		
	//=====prepare LED factors array, calculate t-dependetn gcorr================//

        TCanvas* c1 = new TCanvas("c1","c1",700,1100);
	c1->Divide(2,3);
	
	Char_t cellname[50];
	Char_t cellfile[100];
	TDirectory* h1where = gDirectory;
	TF1*  g1 	= new TF1("g1","gaus",0.1,0.2);
	g1->SetLineWidth(1);
	g1->SetLineColor(kRed);
	TH1F* hmass 	= new TH1F("hmass","hmass",50,0,1);
	TH2F* hxy1	= new TH2F("hxy1","hxy1",34,-98.6,98.6,34,-98.6,98.6);
	TH2F* hxy2	= new TH2F("hxy2","hxy2",34,-98.6,98.6,34,-98.6,98.6);
	TH2F* smallxy1	= new TH2F("smallxy1","smallxy1",52,-98.6,98.6,52,-98.6,98.6);
	TH2F* smallxy2	= new TH2F("smallxy2","smallxy2",52,-98.6,98.6,52,-98.6,98.6);
	Float_t wbox 	= 12.0*3.8;

	Int_t nentries;
	Int_t 	maxy 	= 0;
	Int_t 	maxx 	= 0;
	UChar_t Status = 0x00;
	Char_t  text[30];
	MyCell* mcell 	= new MyCell();
	TTree* Trpi;
	Float_t	x2, y2;
	Float_t ecutL = 25; //large cells are calibrated@25GeV
	Float_t ecutS = 40; //small cells are calibrated@40GeV
	Float_t ewindow = 15; 
	Int_t cnt = 1;
	Int_t n1  = 0;
        Int_t n2  = 0;

	Int_t largeN1 = 0;
	Int_t smallN1 = 0;
	Int_t largeN2 = 0;
	Int_t smallN2 = 0;
	TFile* 	fcell = 0;
	MyCellEvt* mcevt = new MyCellEvt();
	
	sprintf(cellname,"Cellr%d_c%d_n%d",row0,col0,nstb);
	sprintf(cellfile,"%s/Cellr%d_c%d_n%d.root",celldir,row0,col0,nstb);			
	fcell = new TFile(cellfile,"read");
	fcell->ls();
	mcell = (MyCell*)fcell->Get(cellname);
	mcell->SetGeom(mGeom);
	mcell->Print();
	if(!mcell){
		cout<<"ERROR getting "<<cellname<<endl;
		return 0;
	}
	c1->cd(cnt);
	c1->GetPad(cnt)->SetLogy();
	c1->GetPad(cnt)->SetLogx();
	mcell->GetAdcSpectrum(runnum)->Draw();
	cnt++;
	c1->cd(cnt);
	c1->GetPad(cnt)->SetLogy();
	c1->GetPad(cnt)->SetLogx();
	mcell->GetLEDSpectrum(runnum)->Draw();
	mcell->GetLEDSpectrum(runnum)->SetLineColor(kBlue);
	Status = mcell->GetStatusBit(runnum);
	sprintf(text,"0x%x",Status);
	TLatex l(0.5,0.5,text);
	l.SetNDC();
	l.SetTextColor(2);
	l.Draw();
	cnt++;
	c1->cd(cnt);
//	c1->GetPad(cnt)->SetLogy();
	c1->GetPad(cnt)->SetFillColor(37);
	maxy = mcell->GetLEDvsEvt(runnum)->FindLastBinAbove(0,2);
	maxx = mcell->GetLEDvsEvt(runnum)->FindLastBinAbove(0,1);
	cout<<"maxx: "<<maxx<<endl;
	if(maxy < 2000){
		mcell->GetLEDvsEvt(runnum)->GetYaxis()->SetRange(0,2*maxy);
	}
	if(maxx < 2000){
		mcell->GetLEDvsEvt(runnum)->GetXaxis()->SetRange(0,(maxx+100));
	}
				
	mcell->GetLEDvsEvt(runnum)->Draw("COLZ");
	
	Trpi = mcell->GetPionSample("all",runnum);
	nentries = Trpi->GetEntries();
	//	Trpi->Print();
	cout<<"Trpi nentries: "<<nentries<<endl;

	if(nentries!=0){
		Float_t x1;
		Float_t x2;
		Float_t y1;
		Float_t y2;
		Float_t m;
		Float_t e;
		Trpi->ResetBranchAddresses();	//the orginal MergedTr was hooked up with transient member MyCellEvt of MyCell
                Trpi->SetBranchAddress("mycevt",&mcevt);
                Trpi->SetBranchStatus("*",0);
                Trpi->SetBranchStatus("X2",1);
                Trpi->SetBranchStatus("Y2",1);
                Trpi->SetBranchStatus("M",1);
		Trpi->SetBranchStatus("E",1);
//		Trpi->GetEntry(0);
//		Trpi->Show(0);
//		cout<<"X2: "<<(mcevt->X2)<<", Y2: "<<(mcevt->Y2)<<", M: "<<(mcevt->M)<<endl;

		Int_t binx;
		Int_t biny;
		TString partner;
		Int_t pnstb = 0;
		Int_t prow0 = 0;
		Int_t pcol0 = 0;
		Long_t prun = 0;
		for(Int_t i = 0; i < nentries; i++){
			
			Trpi->GetEntry(i);
			x2 = mcevt->X2;
			y2 = mcevt->Y2;
			 m = mcevt->M; 
			 e = mcevt->E;
			
			if(nstb<3){		//large cells
				if(!ConvertTH2Bin(nstb,row0,col0,binx,biny,hxy1)){
					cout<<"ERROR in ConvertTH2Bin()!"<<endl;
					return -1;
				}
				hxy1->SetBinContent(binx,biny,(hxy1->GetBinContent(binx,biny)+1));
				if(abs(e-ecutL)>ewindow)continue;
			}
			if(nstb>2){		//small cells
				if(!ConvertTH2Bin(nstb,row0,col0,binx,biny,smallxy1)){
					cout<<"ERROR in ConvertTH2Bin()!"<<endl;
                                        return -1;
                                }
				smallxy1->SetBinContent(binx,biny,(smallxy1->GetBinContent(binx,biny)+1));
				if(abs(e-ecutS)>ewindow)continue;
			}
//			cout<<"binx: "<<binx<<", biny: "<<biny<<endl;
			
			hmass->Fill(m);
			if(!mGeom->getNSTB(x2,y2))continue;	//bypass abnormal nstb
			partner = mcell->GetPartnerByXY(x2,y2,runnum);
			if(col0==0){
				cout<<"debug: partner: "<<partner<<endl;
			}
			sscanf((const char*)partner,"Cellr%d_c%d_n%d_run%ld",&prow0,&pcol0,&pnstb,&prun);
			if(col0==0){
                        	cout<<"debug: partner (nstb,row0,col0): ("<<pnstb<<", "<<prow0<<", "<<pcol0<<") "<<endl;
                        }

			if(!Legal(2,pnstb,prow0,pcol0)){
				cout<<"invalid partner id"<<endl;
				continue;
			}
			if(pnstb<3){             //large cells
				if(!ConvertTH2Bin(pnstb,prow0,pcol0,binx,biny,hxy2)){
					cout<<"ERROR in ConvertTH2Bin()!"<<endl;
					return -1;
				}
				hxy2->SetBinContent(binx,biny,(hxy2->GetBinContent(binx,biny)+1));
			}
			if(pnstb>2){		//small cells
				if(!ConvertTH2Bin(pnstb,prow0,pcol0,binx,biny,smallxy2)){
					cout<<"ERROR in ConvertTH2Bin()!"<<endl;
					return -1;
				}
				smallxy2->SetBinContent(binx,biny,(smallxy2->GetBinContent(binx,biny)+1));
			}
		}
					
		smallN1 = smallxy1->GetBinContent(smallxy1->GetMaximumBin());
		largeN1 = hxy1->GetBinContent(hxy1->GetMaximumBin());
		if(smallN1>=largeN1){
			cout<<"smallN1: "<<smallN1<<endl;
			hxy1->GetZaxis()->SetRangeUser(0,smallN1);
			smallxy1->GetZaxis()->SetRangeUser(0,smallN1);
		}
		else{
			cout<<"largeN1: "<<largeN1<<endl;
			hxy1->GetZaxis()->SetRangeUser(0,largeN1);
			smallxy1->GetZaxis()->SetRangeUser(0,largeN1);
		}
		cnt++;
                c1->cd(cnt);	
		hmass->Draw();
		hmass->Fit("g1","R+");
		sprintf(text,"gcorr = %.3f",ngcorr);
		TLatex ll(0.5,0.5,text);
		ll.SetNDC();
		ll.SetTextColor(2);
		ll.Draw("same");
		cnt++;
		c1->cd(cnt);
		hxy1->SetStats(0);
		hxy1->Draw("COLZ");
		TBox bx(-wbox,-wbox,wbox,wbox);
		bx.SetFillColor(18);
		bx.Draw("same");
		smallxy1->SetStats(0);
		smallxy1->Draw("zcolsame");
		cnt++;
		c1->cd(cnt);
		
		largeN2 = hxy2->GetBinContent(hxy2->GetMaximumBin());
		smallN2 = smallxy2->GetBinContent(smallxy2->GetMaximumBin());
		if(smallN2>=largeN2){
			cout<<"smallN2: "<<smallN2<<endl;
			hxy2->GetZaxis()->SetRangeUser(0,smallN2);
			smallxy2->GetZaxis()->SetRangeUser(0,smallN2);
		}
                else{
			cout<<"largeN2: "<<largeN2<<endl;
			hxy2->GetZaxis()->SetRangeUser(0,largeN2);
			smallxy2->GetZaxis()->SetRangeUser(0,largeN2);
		}
		hxy2->SetStats(0);
		hxy2->Draw("COLZ");
		bx.Draw("same");
		smallxy2->SetStats(0);
		smallxy2->Draw("zcolsame");
	}
	largeN1 = largeN2 = 0;
	smallN1 = smallN2 = 0;
	Trpi = 0;
	c1->Print(outfile);
	TString cmd = "ps2pdf " + outfile + "; rm -f " + outfile;
	system((const char*)cmd);
	mcell = 0;
	hmass->Reset();
        hxy1->Reset();
        hxy2->Reset();
        smallxy1->Reset();
        smallxy2->Reset();
	fcell->Close();
	fcell = 0;
  	
	return 1;
  }
