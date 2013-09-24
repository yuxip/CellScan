  
  Int_t CompareGainCor(){
	fstream infile("cmp.txt",ios::in);
	TH1F* gdiff = new TH1F("gdiff","gdiff",200,-1,1);
	Int_t ew1,nstb1,ch1;
	Float_t diff;
	while(infile>>ew1>>nstb1>>ch1>>diff){
		if(ew1==1)continue;
		gdiff->Fill(diff);
	}
	gdiff->Draw();
  	return 1;
  }
	
