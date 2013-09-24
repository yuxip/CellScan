

  Float_t GetLEDfactor(Int_t nstb = 1, Int_t row = 0; Int_t col = 0; Int_t run = 12098007){
	
	gSystem->Load("/home/yuxip/FMS/CellScan/lib/libMyCellMgr.so");
	Int_t baserun = 12095006;
	
	TFile* RdepFile = new TFile("/home/yuxip/FMS/root12fms/fmstxt/RunDep.root","read");
	RunDepMgr* Mgr = (RunDepMgr*)RdepFile("Mgr");
	Mgr->SetRdep(run);
	Mgr->SetBase(baserun);
	
	cout<<"Finished updating LED factors array"<<endl;
