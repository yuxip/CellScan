//Plot 2-photon mass from pion samples of the specified cells
//check respose uniformity
//Yuxi Pan 02/26/2012
  Int_t Check3Cells(){
	
	gSystem->Load("/home/yuxip/FMS/CellScan/lib/libMyCellMgr.so");
	TCanvas* c1 = new TCanvas("c1","c1",900,400);
	c1->Divide(3,1);
	TString outfile("horizontalGroup.ps");
	Int_t nstb = 4;
	Int_t row0 = 3;
	Int_t col0[3] = {1,2,3};
	TH1F* m[3] = {0,0,0};
	MyCell* mcell[3] = {0,0,0};
	Char_t hname[3][50];
	Char_t filename[100];
	Char_t cellname[3][50];
	Char_t fname[3][50];
	TFile* f1 = 0;
	TTree* t1[3] = {0,0,0};
	TF1* fgaus[3] = {0,0,0};
	const char* celldir = "/home/yuxip/FMS/CellScan/cells";
	cout<<"celldir: "<<celldir<<endl;

	for(Int_t i = 0; i < 3; i++){
		
		sprintf(cellname[i],"Cellr%d_c%d_n%d",row0,col0[i],nstb);
		cout<<"processing "<<cellname[i]<<endl;
		sprintf(filename,"%s/%s.root",celldir,cellname[i]);
		f1 = new TFile(filename,"read");
		if(!f1){
			cout<<filename<<" does not exist!!"<<endl;
			return 0;
		}
		mcell[i] = (MyCell*)f1->Get(cellname[i]);
		if(!mcell[i]){
			cout<<"couldn't get "<<cellname<<" object"<<endl;
			return 0;
		}
		t1[i] = (TTree*)mcell[i]->GetPionSample("all","/home/yuxip/FMS/CellScan/AllMgrRun.list");
		t1[i]->Print();
		sprintf(hname[i],"hCellr%d_c%d_n%d",row0,col0[i],nstb);
		sprintf(fname[i],"fCellr%d_c%d_n%d",row0,col0[i],nstb);
		m[i] = new TH1F(hname[i],hname[i],40,0,0.8);
		t1[i]->Project(hname[i],"abs(M)","");
		cout<<"m["<<i<<"] entries: "<<m[i]->GetEntries()<<endl;
		Float_t xmax = m[i]->GetBinCenter(m[i]->GetMaximumBin());
		fgaus[i] = new TF1(fname[i],"gaus",0.06,0.2);
		fgaus[i]->SetParameter(1,xmax);
		fgaus[i]->SetParLimits(1,0.06,0.2);
		c1->cd(i+1);
		m[i]->Fit(fname[i],"R+");
		m[i]->Draw();
	}
	c1->Print(outfile);
	
  return 1;
  }

