// 'master' version: v2.0.0 + fix for memory corruption on 20250828
//

// To compile and run this macro, follow these steps:
//  0. enable ROOT 6
//  1. make sure that this macro, and files "fil.cpp", "fil.h", "tre.cpp" and "tre.h" which define EPOS TTree structure, are in the same directory
//  2. adapt the 5 lines below which define variables: "inputData", "maxNoFiles", "maxNoEventsPerFile", "checkForCorruptedFiles" and "verbose"
//  3. implement your declarations (histograms, etc.) below at placeholder '// YOUR DECLARATIONS HERE'
//  4. implement your analysis code below at placeholder '// YOUR ANALYSIS CODE HERE' 
//  5. compile and run with: root fileReader.C++

#include "Riostream.h"
#include "stdio.h"
#include "string.h"
#include "TSystem.h"
#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include "TStopwatch.h"
#include "fil.h"
#include "tre.h"
#include "tre.cpp"

TString inputData = "/home/ga45mof/epos/exampleList"; // abs-path to some ASCII file containing list of ROOT files which this macro will process (generate that list e.g. with 'find' command)
const Int_t maxNoFiles = -1; // set here how many files at maximum you want to process, set to -1 to process 'em all
const Int_t maxNoEventsPerFile = -1; // set here how many events per file at maximum you want to process, set to -1 to process 'em all
Bool_t checkForCorruptedFiles = kTRUE; // if kTRUE, checks for corrupted ROOT files will be performed (and code crash avoided), but then this macro will run significantly slower 
Bool_t verbose = kTRUE; // if kTRUE, additional printouts (e.g. on the overall progress) will appear, but then this macro will run a tiny bit slower

// Declarations of simple utility functions:
Int_t NumberOfNonEmptyLines(const char *externalFile); 
void Red(const char* text);
void Green(const char* text);
void Yellow(const char* text);
void Blue(const char* text);


// YOUR DECLARATIONS HERE


// Main:
Int_t fileReader()
{
 // Simple file reader...

 // Welcome message:
 Green("=> Welcome to fileReader.C, v2.0.0 !");
 TStopwatch timer; timer.Start(); // this stopwatch measures the total execution time for all files
 TStopwatch timerFile; // this stopwatch measures the execution time per file

 // Calculate the total number of files:
 Int_t nFiles = NumberOfNonEmptyLines(inputData.Data()); 
 if (-1 != maxNoFiles){ Yellow(Form("=> Processing %d files out of maximum of %d from input file list \"%s\" ....",maxNoFiles,nFiles,inputData.Data())); }
 else { Yellow(Form("=> Processing all %d files from input file list \"%s\" .... ",nFiles,inputData.Data())); }

 // Loop over list of files:
 string line;
 ifstream myfile;
 myfile.open(inputData.Data());
 Int_t fileCounter = 0;
 Int_t fileCounterHealthy = 0;
 while(getline(myfile,line))
 { 
  // Bail out if maximum number of files is processed:
  if(fileCounterHealthy == maxNoFiles)
  {
   Yellow(Form("\n=> Per request, bailing out after %d healthy files have beed processed.",maxNoFiles));
   break;
  }

  // Skip the empty lines:
  if(TString(line).EqualTo("")){continue;}

  cout<<Form("\nProcessing file %s .... (%d/%d)",line.data(),fileCounter+1,nFiles)<<endl;
  timerFile.Reset(); timerFile.Start();

  // Optional additional checks for corrupted files:
  if(checkForCorruptedFiles)
  { 
   // Check if file exists:
   if(gSystem->AccessPathName(line.data(),kFileExists))
   {
    Red("!!!! This file doesn't exist !!!!"); 
    fileCounter++;
    continue; 
   } 

   // Check if file is corrupted:
   // Taken from https://root.cern.ch/doc/master/classTFile.html
   TFile f1(line.data());  
   if(f1.IsZombie())
   {
    Red("!!!! This file is corrupted, check #1 !!!!"); 
    fileCounter++;
    f1.Close();
    continue; 
   } 
 
   TFile *f2 = TFile::Open(line.data());
   if(!f2) 
   {
    Red("!!!! This file is corrupted, check #2 !!!!"); 
    fileCounter++;
    f2->Close();
    delete f2; 
    continue; 
   }
   delete f2; 

   cout<<"All implemented checks for corrupted files went fine."<<endl; 
  } // if(checkForCorruptedFiles)

  // Okay, we have a healthy file to process:
  fileCounter++;
  fileCounterHealthy++;     
 
  // EPOS TTree structure:
  Eptree *eposttree = new Eptree(2000000);
  // Increase this argument even further, if you get a 'glibc' error:
  //   corrupted size vs. prev_size
  //   Aborted
  
  // Accessing EPOS TTree from the example output file:
  InitTreeFile(strdup(line.data()),eposttree);
  
  // Number of EPOS events dumped in EPOS TTree:
  Int_t nEvts = (Int_t)eposttree->EposTree->GetEntries();
  cout<<Form("There are %d events in it." ,nEvts)<<endl; 

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
  Int_t eventCounter=0;
  for(Int_t e=0;e<nEvts;e++)
  {
   // Bail out if maximum number of events per file is reached:
   if(eventCounter++ == maxNoEventsPerFile)
   {
    cout<<Form("Per request, moving to next file after %d events have beed processed from this file.",maxNoEventsPerFile)<<endl;
    break;
   }

   // For large number of events per file (10000, let's say), provide optionally some printout on the progress:
   if(verbose){ if(nEvts > 10000 && 0==e%1000){ cout<<Form("%.2f%%\r",100.*e/nEvts)<<flush; } }

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

  cout<<"Processing time only of the last file ... "; timerFile.Print();
  cout<<"Total processing time for all files ..... "; timer.Print(); timer.Continue();

  eposttree->fEposTreeFile->Close();
  delete eposttree;

 } // while(getline(myfile,line))

 Green("=> Hasta la vista!");
 cout<<"Total processing time for all files ..... "; timer.Print();

 return 0;

} // Int_t fileReader() 

//=====================================================================================

// Implementation of simple utility functions:

Int_t NumberOfNonEmptyLines(const char *externalFile)
{
 // Count number of non-empty lines in some external file.

 if(gSystem->AccessPathName(externalFile,kFileExists))
 {
  cout<<Form("if(gSystem->AccessPathName(externalFile,kFileExists)), externalFile = %s",externalFile)<<endl; 
  cout<<__LINE__<<endl;
  exit(1);
 }

 string line;
 ifstream myfile;
 myfile.open(externalFile);
 Int_t nLines = 0;
 while (getline(myfile,line))
 { 
  if(TString(line).EqualTo("")){continue;}
  nLines++;
 }

 myfile.close();

 return nLines;

} // Int_t AliAnalysisTaskMuPa::NumberOfNonEmptyLines(const char *externalFile)

//=====================================================================================

void Red(const char* text)
{ 
 cout<<"\n\033[1;31m"<<text<<"\033[0m\n"<<endl;
} 

//=======================================================================================

void Green(const char* text)
{ 
 cout<<"\n\033[1;32m"<<text<<"\033[0m\n"<<endl;
}
//=======================================================================================

void Yellow(const char* text)
{ 
 cout<<"\n\033[1;33m"<<text<<"\033[0m\n"<<endl;
} 

//=======================================================================================

void Blue(const char* text)
{ 
 cout<<"\n\033[1;34m"<<text<<"\033[0m\n"<<endl;
} 

//=======================================================================================
