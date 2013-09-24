
  Int_t ConvertTH2Bin(Int_t nstb = 1, Int_t row0 = 0, Int_t col0 = 0, Int_t& binx, Int_t& biny,TH2F* h2){

	//convert (nstb,row0,col0) into TH2 bin number for event display	
	//example TH2:
	//TH2F* large	= new TH2F("large","large",34,-98.6,98.6,34,-98.6,98.6);
	//TH2F* small	= new TH2F("small","small",52,-98.6,98.6,52,-98.6,98.6);
	Float_t x = 0;
	Float_t y = 0;
	gROOT->LoadMacro("/home/yuxip/FMS/CellScan/macros/Legal.C");
	if(!Legal(2,nstb,row0,col0)){
		cout<<"illegal (nstb,row0,col0) = ("<<nstb<<", "<<row0<<", "<<col0<<") combination!!"<<endl;
		return 0;
	}

	if(nstb<3){		//large cells
		if(nstb==1){
			x = -(col0+0.5)*5.8; 
			y = (17-row0-0.5)*5.8;
		}
		else if(nstb==2){
			x = (col0+0.5)*5.8; 
			y = (17-row0-0.5)*5.8;
		}
		binx = h2->GetXaxis()->FindBin(x);
		biny = h2->GetYaxis()->FindBin(y);

	}
	else{	//small cells
		if(nstb==3){
			x = -(col0+0.5)*3.8; 
			y = (12-row0-0.5)*3.8;
		}
		else if(nstb==4){
			x = (col0+0.5)*3.8; 
			y = (12-row0-0.5)*3.8;
		}
		binx=h2->GetXaxis()->FindBin(x);
          	biny=h2->GetYaxis()->FindBin(y);
	}
	
	return 1;

  }				
