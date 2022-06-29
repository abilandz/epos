// v1.0.0

// To compile and run this macro, follow these steps:
// 0. enable ROOT 5
// 1. make sure that this macro, and files "fil.cpp", "fil.h", "tre.cpp" and "tre.h" which define EPOS TTree structure, are in the same directory;
// 2. adapt the 3 lines below which define variables: "productionDir", "outputROOTFileName" and "maxNoFiles"
// 3. implement your analysis code at placeholder '// YOUR ANALYSIS CODE HERE ' below 
// 4. compile and run with: root -l fileReader.C++

#include "Riostream.h"
#include "stdio.h"
#include "string.h"
#include "TH1D.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TSystem.h"
#include "TList.h"
#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include "fil.h"
#include "tre.h"
#include "tre.cpp"

const Char_t *productionDir = "/scratch6/abilandz/sim/EPOS_LBF_pp80"; // path to top-level directory which contains all ROOT files with EPOS events you want to analyze
const Char_t *outputROOTFileName = "z-p13000_EPOS_LBF_pp80.root"; // generic name of ROOT file with EPOS events
const Int_t maxNoFiles = -1; // set here how many outputROOTFileName files you want to process, set to -1 to process 'em all

Int_t fileReader()
{
 // Simple file reader...

 TSystemDirectory *baseDir = new TSystemDirectory(".",productionDir);
 TList *listOfFilesInBaseDir = baseDir->GetListOfFiles();
 
 Int_t fileCounter=0;
 for(Int_t iFile=0;iFile<listOfFilesInBaseDir->GetEntries();iFile++)
 {
  TSystemFile *currentFile = (TSystemFile*)listOfFilesInBaseDir->At(iFile);
  // Consider only subdirectories: 
  if(!currentFile || 
     //!currentFile->IsDirectory() || 
     strcmp(currentFile->GetName(), ".") == 0 || 
     strcmp(currentFile->GetName(), "..") == 0){continue;} 

     TString rootFile = TString(Form("%s/%s/%s",productionDir,currentFile->GetName(),outputROOTFileName));
     if(gSystem->AccessPathName(rootFile.Data(),kFileExists))
     {
      cout<<Form("File doesn't exist: %s",rootFile.Data())<<endl;
      continue;
     } // if(gSystem->AccessPathName(rootFile.Data(),kFileExists))
 
   if(fileCounter++ == maxNoFiles){break;}
   cout<<Form("Accessing file %s",rootFile.Data())<<endl;

   // EPOS TTree structure:
   Eptree *eposttree = new Eptree(200000);
  
   // Accessing EPOS TTree from the example output file:
   InitTreeFile(strdup(rootFile.Data()),eposttree);
  
   // Number of EPOS events dumped in EPOS TTree:
   Int_t nEvts = (Int_t)eposttree->EposTree->GetEntries();
   //cout<<Form("There are %d events in it." ,nEvts)<<endl; 

   // 0) Header variables (common to all events):
   /*
   cout<<"====== HEADER ======"<<endl;
   cout<<Form("iversn:%d",eposttree->iversn)<<endl;   
   cout<<Form("laproj:%d",eposttree->laproj)<<endl;   
   cout<<Form("maproj:%d",eposttree->maproj)<<endl;   
   cout<<Form("latarg:%d",eposttree->latarg)<<endl;   
   cout<<Form("matarg:%d",eposttree->matarg)<<endl;   
   cout<<Form("engy:%f",eposttree->engy)<<endl;   
   cout<<Form("nfreeze:%d",eposttree->nfreeze)<<endl;   
   cout<<Form("nfull:%d",eposttree->nfull)<<endl;   
   cout<<"====== HEADER ======"<<endl;
   */

   // Loop over events, and for each event say something:
   for(Int_t e=0;e<nEvts;e++)
   {
    // Switch to EPOS event #e:
    eposttree->EposTree->GetEntry(e);

    // 1) Event variables (common to all particles):
    Int_t nParticles = eposttree->np; // number of particles in EPOS event #e:
    Float_t nAbsIP = eposttree->bim; // centrality variable, most likely the absolute value of impact parameter       
    //cout<<Form("eventNo:%d; #particles:%d; nAbsIP:%f",e,nParticles,nAbsIP)<<endl;   

    // Loop over particles, and for each particle say something:
    for(Int_t p=0;p<nParticles;p++)
    {

     // 2) Particle variables:
     Float_t fzus = eposttree->zus[p]; // different meaning depending on ptl type:
                                       // partons: presently unused
                                       // hadrons:  decay information :
                                       // -999 : hadron is decay product from decay 
                                       //        in cascade part (mother unknown)
                                       //   -1 : hadron is decay product, mother not stored
                                       //   >0 : hadron is decay product, mother index = zus
                                       //   -2 : no mother  

     // Particle four momentum:
     Float_t fpx = eposttree->px[p];  
     Float_t fpy = eposttree->py[p];  
     Float_t fpz = eposttree->pz[p];  
     Float_t fen = eposttree->en[p];  

     // Particle four position:
     Float_t fx = eposttree->x[p];  
     Float_t fy = eposttree->y[p];  
     Float_t fz = eposttree->z[p];  
     Float_t ft = eposttree->t[p];  

     // Particle ID, ISAJET/VENUS convention:
     Int_t nid = eposttree->id[p]; // particle id (epos code)

     // Particle status and type:
     Int_t nist = eposttree->ist[p]; //  0 and 1 ... hadrons (0 = last generation)
                                     //  21 ........ partons
                                     //  25 ........ intermediate out-Born partons
                                     //  29 ........ string

     // Particle type of origin:
     Int_t nity = eposttree->ity[p];

     // Particle origin (particle index):
     Int_t nior = eposttree->ior[p];
     Int_t njor = eposttree->jor[p]; // ior>0, jor=0 : ior is origin
                                     // ior>0, jor>0 : origins from  ior to jor
                                     // ior=0, jor=0 : no origin

     // 3) Analysis:

     // Example: primary protons (+- 1120) which ceased to exist once created:
     if(1120==abs(nid) && -2==fzus && -2==nist ) // 
     {
      //cout<<Form("%d %f (%f %f %f %f) (%f %f %f %f) %d %d %d %d %d",p,fzus,fpx,fpy,fpz,fen,fx,fy,fz,ft,nid,nist,nity,nior,njor)<<endl;


      // YOUR ANALYSIS CODE HERE 


     }

    } // for(Int_t p=0;p<nParticles;p++)
   } // for(Int_t e=0;e<nEvts;e++)

  delete eposttree;
  delete currentFile;

 } // for(Int_t iFile=0;iFile<listOfFilesInBaseDir->GetEntries();iFile++)

 return 0;

} // Int_t fileReader() 
