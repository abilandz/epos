#include <TTree.h>
#include <TFile.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "tre.h"
//#include "fto.h"

using namespace std;
//==============================================================================
//void Eptree::InitTreeFile(char* fnamein, Eptree * eee)
void InitTreeFile(char* fnamein, Eptree * eee)
//==============================================================================
{
  TTree *hd;
  TTree *ep;
  //int fSho=Glob::Sho;
  //if(fSho)cout<<"---------------- Init Epos Trees ----------------------"<<endl; // AB commented
  //if(fSho)cout<<"Open tree file " << fnamein <<"  ";
  eee->fEposTreeFile = TFile::Open(fnamein);
  //cout<<"file "<<eee->fEposTreeFile<<endl;
  //if(fSho)cout <<endl; // AB commented
  if(eee->fEposTreeFile==NULL) {
    cout<<"Error: can't open file "<< fnamein <<endl;
    exit(-1);
  } 
  hd=(TTree*)eee->fEposTreeFile->Get("teposhead");
  eee->EposHeadTree=(TTree*)hd;
  hd->SetBranchAddress("iversn", &eee->iversn);
  hd->SetBranchAddress("laproj", &eee->laproj);
  hd->SetBranchAddress("maproj", &eee->maproj);
  hd->SetBranchAddress("latarg", &eee->latarg);
  hd->SetBranchAddress("matarg", &eee->matarg);
  hd->SetBranchAddress("engy",   &eee->engy  );         
  hd->SetBranchAddress("nfreeze",&eee->nfreeze);    
  hd->SetBranchAddress("nfull",  &eee->nfull ); 
  //if(fSho)cout<<"hd tree entries : "<<hd->GetEntries()<<endl; // AB commented
  hd->GetEntry(0);
  //if(fSho)cout<<"version                : "<< eee->iversn <<endl; // AB commented
  //if(fSho)cout<<"laproj                 : "<< eee->laproj <<endl; // AB commented
  //if(fSho)cout<<"maproj                 : "<< eee->maproj <<endl; // AB commented
  //if(fSho)cout<<"latarg                 : "<< eee->latarg <<endl; // AB commented
  //if(fSho)cout<<"matarg                 : "<< eee->matarg <<endl; // AB commented
  //if(fSho)cout<<"engy                   : "<< eee->engy   <<endl; // AB commented
  //if(fSho)cout<<"nfreeze                : "<< eee->nfreeze<<endl; // AB commented
  //if(fSho)cout<<"nfull                  : "<< eee->nfull  <<endl; // AB commented
  ep=(TTree*)eee->fEposTreeFile->Get("teposevent");
  eee->EposTree=(TTree*)ep;
  eee->treEve=ep->GetEntries();
  //if(fSho)cout<<"ep tree entries : "<<eee->treEve<<endl; // AB commented
  //cout << eee->px << endl;
  ep->SetBranchAddress("np",  &eee->np);
  ep->SetBranchAddress("bim", &eee->bim);
  ep->SetBranchAddress("zus", eee->zus);
  ep->SetBranchAddress("px",  eee->px);
  ep->SetBranchAddress("py",  eee->py);
  ep->SetBranchAddress("pz",  eee->pz);
  ep->SetBranchAddress("e",   eee->en);
  ep->SetBranchAddress("x",   eee->x);
  ep->SetBranchAddress("y",   eee->y);
  ep->SetBranchAddress("z",   eee->z);
  ep->SetBranchAddress("t",   eee->t);
  ep->SetBranchAddress("id",  eee->id);
  ep->SetBranchAddress("ist", eee->ist);
  ep->SetBranchAddress("ity", eee->ity);
  ep->SetBranchAddress("ior", eee->ior);
  ep->SetBranchAddress("jor", eee->jor);
  //cout << eee->px << endl;
  //ep->GetEntry(0);
  //cout << eee->px << "	" << eee->py << endl;
  //if(fSho)cout<<"---------------- End of Init Epos Trees -----------------"<<endl; // AB commented

}



//Eptree::~Eptree(){
  //for(Int_t i=0;i<Xdimension;i++){
  // delete zus[i];
  //}
//  cout<<"Eptree::~Eptree() do nothing "<<t[10]<<endl;
  //delete  *zus[0];
  //operator delete(zus);
//}
