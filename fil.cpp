#include <TRandom3.h>
#include <TTree.h>
#include <TFile.h>
#include <iostream>
#include <cstdlib> 

#include "fil.h"

int *id, *ist, *ity, *ior, *jor, *np, *iversn, *laproj, *maproj, *latarg, *matarg, *nfull, *nfreeze ;
float *bim, *engy ;
float *zus, *px, *py, *pz, *e, *x, *y, *z, *t;
const int Mxpom = 40 ;
TTree* thead[Mxpom];         
TTree *tree[Mxpom] ;
TFile *file[Mxpom] ;
const int nmax = 250000 ;

void opentree_(int *_npom, char* filename)
{
        if(*_npom>Mxpom-1)
        {
           std::cout<<"Error: npom = "<<*_npom<<" is too big"<<std::endl;
           exit(-1);
        } 

        int npom=*_npom-1;

        char name [200] ; 
        strcpy(name,filename);
        printf(" opentree *************  %s *************\n",name);
        std::cout<<" *****fil****  *_npom = "<<npom+1<<std::endl;
	file[npom] = new TFile(filename, "RECREATE"); //output file
        std::cout << " *****fil**** open tree; file: " << filename << std::endl ;

	iversn = new int ;    //EPOS version number
        laproj = new int ;    //atomic number projectile
        maproj = new int ;    //mass number projectile
        latarg = new int ;    //atomic number target
        matarg = new int ;    //mass number target
        engy   = new float ;  //energy in the cms in GeV
        nfull  = new int ;    //number of full events
        nfreeze= new int ;    //number of freeze outs per full event (to increase stat)

        sprintf(name,"teposhead%i",npom);
        //std::cout<<" *****fil**** thead name  "<<name<<std::endl;
        thead[npom] = new TTree(name,"header"); 
        thead[npom]->Branch("iversn",iversn,"iversn/I");
        thead[npom]->Branch("laproj",laproj,"laproj/I");
        thead[npom]->Branch("maproj",maproj,"maproj/I");
        thead[npom]->Branch("latarg",latarg,"latarg/I");
        thead[npom]->Branch("matarg",matarg,"matarg/I");
        thead[npom]->Branch("engy",engy,"engy/F");
        thead[npom]->Branch("nfull",nfull,"nfull/I");
        thead[npom]->Branch("nfreeze",nfreeze,"nfreeze/I");
      
	zus = new float [nmax]; //private use
	px = new float [nmax];  //  p_x of particle
	py = new float [nmax];  //  p_y of particle
	pz = new float [nmax];  //  p_z of particle
	e = new float [nmax];   //  energy of particle
	x = new float [nmax];   //  x component of formation point
	y = new float [nmax];   //  y component of formation point
	z = new float [nmax];   //  z component of formation point
	t = new float [nmax];   //  formation time
	id = new int [nmax];    //  particle id  (see array "idt" in function "idtrafo" in file "ids.f": first column "epos id" second one PDG id
	ist = new int [nmax];   //  particle status (hadron last generation (0) or not (1); other numbers refer to partons, Pomerons, etc)  
	ity = new int [nmax];   //  type of particle origin (20-29 from soft strings, 30-39 from hard strings, 40-59 from remnants, 60 from fluid)
	ior = new int [nmax];   //  index of father  (resonance decay products have only a father)
	jor = new int [nmax];   //  index of mother  (mothers are needed for exemple for strings: the partons between ior and jor constitute the string)
	np = new int ;          //  number of particles
	bim = new float ;       //  impact parameter (usually; other choices are possible)

        sprintf(name,"teposevent%i",npom);
	tree[npom] = new TTree(name,"particles"); 
	tree[npom]->Branch("np",np,"np/I");
	tree[npom]->Branch("bim",bim,"bim/F");
	tree[npom]->Branch("zus",zus,"zus[np]/F"); 
	tree[npom]->Branch("px",px,"px[np]/F"); 
	tree[npom]->Branch("py",py,"py[np]/F"); 
	tree[npom]->Branch("pz",pz,"pz[np]/F"); 
	tree[npom]->Branch("e",e,"e[np]/F"); 
	tree[npom]->Branch("x",x,"x[np]/F"); 
	tree[npom]->Branch("y",y,"y[np]/F"); 
	tree[npom]->Branch("z",z,"z[np]/F"); 
	tree[npom]->Branch("t",t,"t[np]/F"); 
	tree[npom]->Branch("id",  id, "id[np]/I"); 
	tree[npom]->Branch("ist",ist,"ist[np]/I"); 
	tree[npom]->Branch("ity",ity,"ity[np]/I"); 
	tree[npom]->Branch("ior",ior,"ior[np]/I"); 
	tree[npom]->Branch("jor",jor,"jor[np]/I"); 
}

void treehead_(int *_npom, int *_iversn, int *_laproj, int *_maproj, int *_latarg, int *_matarg, float *_engy, int *_nfull, int *_nfreeze)
{
        int npom=*_npom-1;
	*iversn = *_iversn ;
        *laproj = *_laproj ;
        *maproj = *_maproj ;
        *latarg = *_latarg ;
        *matarg = *_matarg ;
        *engy   = *_engy ;
        *nfull  = *_nfull ;
        *nfreeze= *_nfreeze ;
	thead[npom]->Fill() ;
}
 
void filltree_(int *_npom, int *_np, float *_bim, int *_id, int *_ist, int *_ity, int *_ior, int *_jor, float *_zus, float *_px, float *_py, float *_pz,  float *_e, float *_x, float *_y, float *_z, float *_t, int *_iret) 
{
        int npom=*_npom-1;
	*_iret = 0;
        *np = *_np ;
        if(*np>nmax){
          *_iret=1;
          return;
        }
        //if(npom == 0){ std::cout<<" *****fil****  "<<  npom <<"  NP "<<  *np <<std::endl; }
	*bim = *_bim ;
	for(int i=0; i<*np; i++){
	id[i] = _id[i] ;
	ist[i] = _ist[i] ;
	ity[i] = _ity[i] ;
        ior[i] = _ior[i] ;
        jor[i] = _jor[i] ;
	zus[i] = _zus[i] ;
	px[i] = _px[i] ;
	py[i] = _py[i] ;
	pz[i] = _pz[i] ;
	e[i] = _e[i] ;
	x[i] = _x[i] ;
	y[i] = _y[i] ;
	z[i] = _z[i] ;
	t[i] = _t[i] ;
	}
         std::cout << " fill treeAB with " << *np << " particles" << std::endl ;
        if(npom == 0){ std::cout<<" *****fil****   Fill " <<std::endl; }
	tree[npom]->Fill() ;
}

void closetree_(int *_npom, char* filename)
{


       // std::cout << " close tree " << std::endl ;
       int npom=*_npom-1;

       file[npom]->cd() ;       
       //thead[npom]->Write() ;
       //tree[npom]->Write() ;
       file[npom]->Write() ;   //writes all objects into file
       file[npom]->Close() ;
       
       char name [200] ; 

       strcpy(name,filename);
       //std::cout << " closetree ************* " << name << " *************" <<std::endl ;
       TFile *f = new TFile(name,"UPDATE");
       
       sprintf(name,"teposhead%i",npom);
       TTree *th; 
       f->GetObject(name,th); 
       th->SetName("teposhead");
       
       sprintf(name,"teposevent%i",npom);
       TTree *te; 
       f->GetObject(name,te); 
       te->SetName("teposevent");

       f->Write() ;  
       f->Close() ;


// f = new TFile("data.root","UPDATE");
// TTree *t; f->GetObject("h11",t);
// if (t) {
//    t->SetName("newname");
//    f->Write();
// }        
        
}
