#include "TSystem.h"
#include "TFile.h"
#include "MyCell.h"
  Int_t main(){
  
  	TFile* f1 = new TFile("MyCells.root","read");
  	MyCell* mcell;
  	f1->GetObject("Cellr0_c0_n1",mcell);
  	mcell->Print();
	return 1;
  }
