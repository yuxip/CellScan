{
  gROOT->Reset();
  gSystem->Load("libMyCellMgr.so");
  const char* fmsgain = gSystem->Getenv("FMSGAIN");
  const char* gaincor = gSystem->Getenv("FMSCORR");
  CalibStr* mGainCor = new CalibStr(12000000, gaincor);
  CalibStr* mGain = new CalibStr(12000000, fmsgain);
  Geom* mGeom = new Geom("./","geom.txt");
  
}
