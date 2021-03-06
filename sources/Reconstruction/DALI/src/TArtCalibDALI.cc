#include "TArtCalibDALI.hh"
#include "TArtCalibCoin.hh"
#include "TArtDALINaI.hh"
#include "TArtDALINaIPara.hh"
#include "TArtDALIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtTOF.hh"
#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"
#include "TArtMap.hh"
#include "TArtBeam.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>
#include <TVector3.h>

#include <string.h>
#include <stdio.h>

//__________________________________________________________
TArtCalibDALI::TArtCalibDALI(const TString outdataname)
  : TArtReconstruction(outdataname)
{
  TArtCore::Info(__FILE__,"Creating the DALI detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtDALIParameters*)sman->FindParameters("DALIParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: DALIParameters");
    throw;
  }
  //fCalibCoin = new TArtCalibCoin(); //removed. usually defined in global reco class 
  fNaIArray = new TClonesArray("TArtDALINaI",10);
  fNaIArray->SetName("DALINaI");
  fNaIArray->SetOwner();
  sman->AddDataContainer(fNaIArray);

  r = new TRandom();
  LoadFile((char*)"daliparameters.txt");

  fPlTOF = 0;
  fVertex = 0.0;


}

//__________________________________________________________
TArtCalibDALI::~TArtCalibDALI()  {
  ClearData();

  delete fNaIArray;
}

//__________________________________________________________
void TArtCalibDALI::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device   = seg->GetDevice();
    Int_t detector = seg->GetDetector();
    if(DALIA == detector || DALIT == detector) LoadData(seg);
  }
  return;

}
//__________________________________________________________
void TArtCalibDALI::LoadFile(char s[]) {

  FILE *fInputs = fopen(std::string(s).c_str(),"r");
  if(fInputs){
    fscanf(fInputs,"%lf %lf %lf %lf",&fBeta,&fTimeTrueCutLow,&fTimeTrueCutHigh,&fMultiplicityThreshold);

    TArtCore::Info(__FILE__,"Beta: %f",fBeta);
    TArtCore::Info(__FILE__,"TimeTrueCutLow: %f",fTimeTrueCutLow);
    TArtCore::Info(__FILE__,"TimeTrueCutHigh: %f",fTimeTrueCutHigh);
    TArtCore::Info(__FILE__,"MultiplicityThreshold: %f",fMultiplicityThreshold);
    fclose(fInputs);
  }
  else{
    TArtCore::Error(__FILE__,"fail to load DALI configuration file: %s",s);
  }
  return;
  }

//__________________________________________________________
void TArtCalibDALI::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();
  //  if(DALIA != detector && DALIT != detector) LoadRawData(seg);
  if(DALIA != detector && DALIT != detector) return;

  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtDALINaIPara *para = (TArtDALINaIPara*)setup->GetDALINaIPara(&mm);
    if(NULL == para){
      TArtCore::Debug(__FILE__,"Could not find TArtDALINaIPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtDALINaIPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    Int_t nnai = fNaIArray->GetEntries();
    TArtCore::Debug(__FILE__,"Entries in DALINaIArray: %d, DALI-ID:%d", nnai, id);
    TArtDALINaI * nai = FindNaI(id);

    if(NULL==nai){
      new ((*fNaIArray)[nnai]) TArtDALINaI();
      nai = (TArtDALINaI *)fNaIArray->At(nnai);
      nai->SetID(para->GetID());
      nai->SetFpl(para->GetFpl());
      nai->SetDetectorName(*(para->GetDetectorName()));
      nai->SetLayer(para->GetLayer());
      nai->SetTheta(para->GetNaITheta());

      nai->SetXPos(para->GetXPos());
      nai->SetYPos(para->GetYPos());
      nai->SetZPos(para->GetZPos());
      //nai->SetRawTRef(tref);

      fNaIParaArray.push_back(para);
    }

    // set raw data
    if(DALIA == detector)
      //nai->SetRawADC(val);
      nai->SetRawADC(val<4000?val:-9999);
    if(DALIT == detector)
      nai->SetRawTDC(val);

  }

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibDALI::ClearData()   {
  fNaIArray->Clear();
  fNaIParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  fMultWithoutT = 0;                                        
  fMult = 0;                                                
  fTimeTrueMult = 0;                                        
  fMultThres = 0;                                           
  fTimeTrueMultThres = 0;   
  return;
}
//__________________________________________________________
void TArtCalibDALI::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  Int_t trefdummy[3] = {0}; // trefbuffer for each V1190 module  

  //Clear all reconstructed values
  for(Int_t i=0;i<GetNumNaI();i++){
    TArtDALINaI *nai = (TArtDALINaI*)fNaIArray->At(i);
    nai->SetEnergyWithoutT(-999.);
    nai->SetCosTheta(0);
    nai->SetEnergy(-999.);
    nai->SetTime(-9999.);
    nai->SetTimeOffseted(-10000.);
    nai->SetTimeTrueEnergy(-999.);
    nai->SetTimeTrueTime(-10000.);
    nai->SetTimeTrueTimeOffseted(-10000.);

    //ID depends on the definition in mapping file
    if(nai->GetID()==140) trefdummy[0] = nai->GetRawTDC();
    if(nai->GetID()==141) trefdummy[1] = nai->GetRawTDC();
    //if(nai->GetID()==228) trefdummy[2] = nai->GetRawTDC();
    }

  for(Int_t i=0;i<GetNumNaI();i++){

    TArtDALINaI *nai = (TArtDALINaI*)fNaIArray->At(i);    
    TArtDALINaIPara *para = fNaIParaArray[i];

    Int_t adc = nai->GetRawADC();
    Int_t tdc = nai->GetRawTDC();
    Double_t theta = nai->GetTheta();
    Double_t costheta = TMath::Cos(theta*TMath::Pi()/180.);
    Double_t fEnergy = (adc - para->GetQPed()) * para->GetQCal();
    Double_t random = (Double_t)r->Rndm();
    //Double_t fEnergy = (adc-0.5 + random) * para->GetQCal()  + para->GetQPed();
    //cout << adc << fEnergy <<endl;

    //	Double_t fEnergy = adc * para->GetQCal() + para->GetQPed();
    //nai->SetEnergy(fEnergy);
    nai->SetEnergyWithoutT(fEnergy);
    nai->SetCosTheta(costheta);

    if(fEnergy>0) fMultWithoutT++;
    nai->SetEnergy(fEnergy);
    //	if(!(tdc>0 && adc>0 && tdc<100000 && adc<4000)) continue;
    //if(tdc>0 && adc>0 && tdc<100000 && adc<4000 && (trefdummy[0]||trefdummy[1])>0){ }
    //else{ continue; }

    //nai->SetEnergy(fEnergy);

    if(fEnergy>fMultiplicityThreshold) fMultThres++;
    if(fEnergy>0)fMult++;

    Double_t tref;

    // set tfer for each NaI
    if(para->GetTDCMap()->GetGeo()==6)
      tref = (double)trefdummy[0];
    else if(para->GetTDCMap()->GetGeo()==8)
      tref = (double)trefdummy[1];
    else
      tref = (double)trefdummy[2];

    //if(!tref>0) continue; //This can be set to ensure the tref was present.

    Double_t  fTime = -9999.;
    fTime = ((double)tdc - tref )* para->GetTCal();
    //cout << "fTime" << fTime << endl;
    //fTime = (double)tdc * para->GetTCal();
    
    Double_t fTimeOffseted = -9999.;
    fTimeOffseted = fTime + para->GetTOffset() -fPlTOF ; //Subtract Plastic Time. This variable is set externally in the macro.

    //Double_t fTime = (double)tdc * para->GetTCal();
    //Double_t fTimeOffseted = fTime + para->GetTOffset();

    nai->SetTime(fTime);
    nai->SetTimeOffseted(fTimeOffseted);

    if(fTimeOffseted > fTimeTrueCutLow && fTimeOffseted < fTimeTrueCutHigh){
      nai->SetTimeTrueEnergy(fEnergy);
      nai->SetTimeTrueTime(fTime);
      nai->SetTimeTrueTimeOffseted(fTimeOffseted);
      if(fEnergy>fMultiplicityThreshold) fTimeTrueMultThres++;
      if(fEnergy>0) fTimeTrueMult++;
      nai->SetMultiplicity(fTimeTrueMult);
    }
    // copy some information from para to data container
    //	nai->SetID(para->GetID());
    //	nai->SetFpl(para->GetFpl());
    //	nai->SetDetectorName(*(para->GetDetectorName()));
    //	nai->SetLayer(para->GetLayer());
    //	nai->SetTheta(para->GetNaITheta());

    }
  fReconstructed = true;

  //DopplerCorrect(fBeta);
  //DopplerCorrectVertex(fBeta,fVertex);

  DopplerCorrect();
  DopplerCorrectVertex(fVertex);
    
  return;
}

//__________________________________________________________
void TArtCalibDALI::DopplerCorrect()   {

  if(!fReconstructed){
    TArtCore::Info(__FILE__,"Data is not reconstructed.");
    return;
  }

  // here all DALI NaI is assumed to be at same fpl.
  if(fNaIArray->GetEntries()==0) return;
  Int_t fpl = ((TArtDALINaI*)fNaIArray->At(0))->GetFpl();
  TClonesArray *tofarray = (TClonesArray *)sman->FindDataContainer("BigRIPSTOF");
  if(NULL != tofarray){
    for(Int_t i=0;i<tofarray->GetEntries();i++){
      TArtTOF *tof = (TArtTOF *)tofarray->At(i);
      //if( tof->GetUpstreamPlaFpl()==3 && tof->GetDownstreamPlaFpl()==13 ){
      if( tof->GetUpstreamPlaFpl()==3 && tof->GetDownstreamPlaFpl1()==13 ){ 
	DopplerCorrect(tof->GetBeta());
	//cout<<"DALI check incoming beta:"<<tof->GetBeta()<<endl;
	break;
      }
    }
  }
  else{
    TArtCore::Info(__FILE__,"Could not find tof data.");
  }

  return;
}

//__________________________________________________________
void TArtCalibDALI::DopplerCorrect(Double_t beta)   {

  if(!fReconstructed){
    TArtCore::Info(__FILE__,"Data is not reconstructed.");
    return;
  }

  for(Int_t i=0;i<GetNumNaI();i++){
    TArtDALINaI *nai = (TArtDALINaI*)fNaIArray->At(i);
    // reconstructed position and angle based on PPAC information is too fine and not used since the most of the resolution is determined by DALI-NaI angle resolution.
    Double_t fDoppCorEnergy = nai->GetEnergy() * (1-beta*nai->GetCosTheta())/TMath::Sqrt((1.0-beta*beta));
    nai->SetDoppCorEnergy(fDoppCorEnergy);
    nai->SetBeta(beta);
    if(nai->GetTimeTrueEnergy() > 0.){
      nai->SetTimeTrueDoppCorEnergy(fDoppCorEnergy);
      //cout<<"Dopp cor check, Detector ID:"<<nai->GetID()<<", Raw energy: "<<nai->GetRawADC()<<", Raw time:"<<nai->GetRawTDC()<<", Energy:"<<nai->GetTimeTrueEnergy()<<", Time:"<<nai->GetTimeTrueTime()<<", Time Offseted:"<<nai->GetTimeOffseted()<<", Beta:"<<nai->GetBeta()<<", Original theta:"<<nai->GetTheta()<<", Dopp Cor Energy: "<<nai->GetTimeTrueDoppCorEnergy()<<", Extra dopp cor Energy check: "<<fDoppCorEnergy<<", multiplicity ="<<nai->GetMultiplicity()<<endl;
      TClonesArray *beams = (TClonesArray *)sman->FindDataContainer("BigRIPSBeam");
      for(Int_t l=0;l<beams->GetEntries();l++){
	TArtBeam *beam = (TArtBeam *)beams->At(0);
	//cout<<"Dopp Cor incoming info, Z:"<<beam->GetZet()<<", AoQ:"<<beam->GetAoQ()<<", Beta:"<<beam->GetBeta()<<endl;
      }
    }
    else{
      nai->SetTimeTrueDoppCorEnergy(-9999.);
      //cout<<"Underflow check, multiplicity ="<<nai->GetMultiplicity()<<", Detector ID:"<<nai->GetID()<<", Raw energy: "<<nai->GetRawADC()<<", Raw time:"<<nai->GetRawTDC()<<", Energy:"<<nai->GetTimeTrueEnergy()<<", Time:"<<nai->GetTimeTrueTime()<<", Beta:"<<nai->GetBeta()<<", Original theta:"<<nai->GetTheta()<<", Dopp Cor Energy: "<<nai->GetTimeTrueDoppCorEnergy()<<", Extra dopp cor Energy check: "<<fDoppCorEnergy<<endl; 
    }
  }

  return;
}

//__________________________________________________________
void TArtCalibDALI::DopplerCorrectVertex(Double_t beta, Double_t vertex) {

  if(!fReconstructed){
    TArtCore::Info(__FILE__,"Data is not reconstructed.");
    return;
  }

  for(Int_t i=0;i<GetNumNaI();i++){
    TArtDALINaI *nai = (TArtDALINaI*)fNaIArray->At(i);

    TVector3 gamma(nai->GetXPos(),nai->GetYPos(),nai->GetZPos()-vertex);

    Double_t fDoppCorEnergy = nai->GetEnergy() * (1-beta*gamma.CosTheta())/TMath::Sqrt((1.0-beta*beta));

    if(nai->GetTimeTrueEnergy() > 0.){
      nai->SetTimeTrueDoppCorVertexEnergy(fDoppCorEnergy);
    }else{
      nai->SetTimeTrueDoppCorVertexEnergy(-1.);
    }
  }

  return;
  }

//__________________________________________________________
//added

#include "TChain.h"

#include "TCutG.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

#include "TROOT.h"
#include "TRint.h"
#include "TVector3.h"

#include "TMath.h"

#include <iostream>
#include <strstream>
#include <stdlib.h> 

#include <TFile.h>

#include <search.h>

using namespace std;

const int NUMBEROFDALICRYSTALS = 140;
const int NUMBEROFDALIADDBACKCRYSTALS = 30;
//float ADDBACKDISTANCE = 10;

//For addback
int fAddbackTable[NUMBEROFDALICRYSTALS][NUMBEROFDALIADDBACKCRYSTALS];
int fNumberOfAddbackPartners[NUMBEROFDALICRYSTALS];
bool crystalUsedForAddback[NUMBEROFDALICRYSTALS] = {false};

float fTheta[NUMBEROFDALICRYSTALS];
float fPosX[NUMBEROFDALICRYSTALS];
float fPosY[NUMBEROFDALICRYSTALS];
float fPosZ[NUMBEROFDALICRYSTALS];

int fDaliFold   = 0;//Fold
int fDaliFoldTa = 0;//Fold at Target time
int fDaliFoldNoTa = 0;
int fDaliMult   = 0;//multiplicity
int fDaliMultTa = 0;//multiplicity at target time

//This is only for Add-back based analysis
struct dali{
  int id;
  int layer;
  float theta;   //Angle for Doppler Correction
  float x;
  float y;
  float z;
  float re;      //raw energy
  float e;       //Energy
  float rt;      //Raw time
  float t;       //Time
  float ode;     //non add back doppler corrected energy
  float beta;    //beta
  float othe;    //original theta
  float mult;    //multiplicity
  bool  ttrue;   //Bool if time is true
  float dopp[1]; //Doppler energy. Three doppler corrections for the three betas [1] 
  float doppwa[1]; //Doppler energy with true multiplicity and addback.
  float idwa;
  //float twa;
  //float ttruewa;
}
fDali[NUMBEROFDALICRYSTALS];

//////////////////////////////////////////////////////////////////////////////////

Double_t DopplerCorrectAddBack(Double_t beta1,Double_t theta,Double_t energy){
  if(energy <=0.) return -1;
  else return energy * (1-beta1*TMath::Cos(theta))/TMath::Sqrt((1.0-beta1*beta1));
}

//////////////////////////////////////////////////////////////////////////////////

bool IncludeAddbackTable(int detid[2],Double_t maxDistance, TVector3 fPos[]) {

  float distance = TMath::Sqrt(TMath::Power(fPos[detid[0]].X()-fPos[detid[1]].X(),2) +
                               TMath::Power(fPos[detid[0]].Y()-fPos[detid[1]].Y(),2) + 
                               TMath::Power(fPos[detid[0]].Z()-fPos[detid[1]].Z(),2));

  //cout<<"Distance: "<<distance<<endl;
  if( distance > maxDistance ) return false;
  else return true;
}
//////////////////////////////////////////////////////////////////////////////////

void TArtCalibDALI::CreateAddBackTable(Double_t maxDistance){

  //cout<<"Creating the add-back table"<<endl;
  
  TVector3 detPos[NUMBEROFDALICRYSTALS];
  float x,y,z;
  int number;
  float angle;
  
  FILE *fdetPos  = fopen("/shared/storage/physnp/rt1091/share/SAMURAI-Day1/users/la748/Analysis/input/AverageInteractionPoint.out","r");
  char dummy[100];//100
  for(int i=0;i<NUMBEROFDALICRYSTALS;i++){ 
    fgets(dummy,100,fdetPos);//100
    sscanf(dummy,"%f %f %f %i %i %f",&x,&y,&z,&number,&number,&angle);
    detPos[i].SetX(x);
    detPos[i].SetY(y);
    detPos[i].SetZ(z);
    fTheta[i] = angle*3.14159/180.;
    fPosX[i] = x;
    fPosY[i] = y;
    fPosZ[i] = z;
    //cout<<detPos[i].X()<<" "<<detPos[i].Y()<<" "<<detPos[i].Z()<<endl;
    //cout<<"angle:"<<angle<<", theta:"<<fTheta[i]<<endl;
  }
  FILE *fAddbackTableOut = fopen("/shared/storage/physnp/rt1091/share/SAMURAI-Day1/users/la748/Analysis/input/AddbackTable.out","w");

  int detid[2];
  bool inTable;
  int counter = 0;
  for(int i=0;i<NUMBEROFDALICRYSTALS;i++) {
    fNumberOfAddbackPartners[i] = 0;   
    fprintf(fAddbackTableOut," %i",i);

    for(int j=0;j<NUMBEROFDALICRYSTALS;j++) {
      if(j!=i){
	detid[0] = i;
	detid[1] = j;
       
	inTable = IncludeAddbackTable(detid,maxDistance,detPos);

	if(inTable && counter< NUMBEROFDALIADDBACKCRYSTALS) {
	  fprintf(fAddbackTableOut," %i",j);
	  fAddbackTable[i][counter] = j;
	  fNumberOfAddbackPartners[i]++;
	  counter++;
	}
	if(counter == NUMBEROFDALIADDBACKCRYSTALS)  { //Too many detectors 
	  cout<<"You have to increase the variable NUMBEROFDALI2ADDBACKCRYSTALS!!!"<<endl;
	  //STD::abort();
	}
      }
    }
    counter = 0;
    fprintf(fAddbackTableOut," \n");
  }
  fclose(fAddbackTableOut);
}

//__________________________________________________________
int CompareByEnergy(const void *element1, const void *element2) {
  return((dali*)element1)->e < ((dali*)element2)->e? 1: -1;
}

//__________________________________________________________
void SortData(int count, dali fDali[]) {
  qsort(fDali,count,sizeof(dali),CompareByEnergy);
}

/////////////////////////////////////////////////////////////////////////////////
void TArtCalibDALI::AddBackAnalysis(){

  
  /*TClonesArray *beams = (TClonesArray *)sman->FindDataContainer("BigRIPSBeam");
  for(Int_t i=0;i<beams->GetEntries();i++){
     TArtBeam *beam = (TArtBeam *)beams->At(i);
     cout<<"incoming info, Z:"<<beam->GetZet()<<", AoQ:"<<beam->GetAoQ()<<", Beta:"<<beam->GetBeta()<<endl;
  }*/

  double beta1 = 0.6273;
  //CreateAddBackTable(20);
  double betaDiffLow = 0.0123;
  double betaDiffHigh = 0.0177;
  //cout<<"test1"<<endl;
  //TFile *rootfile = new TFile("analysis_development/DALI_outputfile.root","RECREATE");
  //rootfile->cd();

  int minBin = 0;
  int maxBin = 6000;
  int binning = 25;
  int numBin = (maxBin-minBin)/binning;

  //Specific variables;
  int daliMult;
  int daliTimeTrueMult;
  int daliFold;
  int daliTimeTrueFold;
  int daliFoldNoTa;

  Double_t DopplerAngle;

  int countingFilling = 0;

  fDaliFold   = 0;//Fold
  fDaliFoldTa = 0;//Fold
  fDaliFoldNoTa = 0;
  fDaliMultTa = 0;//multiplicity
  for(int j=0;j<NUMBEROFDALICRYSTALS;j++) {
    crystalUsedForAddback[j] = false;
  }
  //cout<<"test2"<<endl;
  for(Int_t i=0;i<GetNumNaI();i++){
    TArtDALINaI *nai = (TArtDALINaI*)fNaIArray->At(i);
    //fDali[i].id         = nai->GetID()-1;
    fDali[i].id         = nai->GetID();
    fDali[i].layer      = nai->GetLayer();
    //fDali[i].theta      = fTheta[fDali[i].id]; //uses averageinteractionpoint theta
    fDali[i].theta      = nai->GetTheta(); //try using xml theta
    fDali[i].x          = fPosX[fDali[i].id];
    fDali[i].y          = fPosY[fDali[i].id];
    fDali[i].z          = fPosZ[fDali[i].id];
    fDali[i].re         = nai->GetRawADC();
    fDali[i].e          = nai->GetTimeTrueEnergy();
    fDali[i].rt         = nai->GetRawTDC();
    fDali[i].t          = nai->GetTimeOffseted(); 
    fDali[i].ode        = nai->GetDoppCorEnergy();
    fDali[i].beta       = nai->GetBeta();
    fDali[i].othe       = nai->GetTheta();
    fDali[i].mult       = nai->GetMultiplicity();
    
    //cout<<"test3"<<endl;
    fDaliFold++; // In principle fDaliFold should be equal to GetNumNai(); 
    if(fDali[i].e>0){
      fDali[i].dopp[0] = fDali[i].e * (1-fDali[i].beta*TMath::Cos(fDali[i].theta*TMath::Pi()/180.))/TMath::Sqrt((1.0-fDali[i].beta*fDali[i].beta));
      //if(fDali[i].t>fTimeTrueCutLow-500&&fDali[i].t<fTimeTrueCutHigh+500)fDaliFold++; // This line is strange. Why it trys to counts the hits outside of the time condition...
      if(fDali[i].t>fTimeTrueCutLow && fDali[i].t<fTimeTrueCutHigh){
	  //cout<<"test6"<<endl;
	fDaliFoldTa++;
	fDali[i].ttrue = true;
      }
      else{
	fDali[i].ttrue = false;
	fDaliFoldNoTa++;
      }
    }
    else{
      fDali[i].dopp[0] = -999.;
      fDali[i].ttrue   = false;
    }
    //cout<<"test7"<<endl;
  }
  
  for(int i=GetNumNaI();i<NUMBEROFDALICRYSTALS;i++){
    fDali[i].id         = -1;
    fDali[i].layer      = -1;
    fDali[i].theta      = -1;
    fDali[i].x          = -999;
    fDali[i].y          = -999;
    fDali[i].z          = -999;
    fDali[i].e          = -999;
    fDali[i].t          = -999;
    fDali[i].ttrue      = false;
    fDali[i].dopp[0]    = -999;
    //cout<<"test8"<<endl;
  }

  for(int i=0;i<fDaliFold;i++){ 
    TArtDALINaI *nai = (TArtDALINaI*)fNaIArray->At(i);
    if(fDaliFoldTa>0){
      nai->SetTrueMult(fDaliFoldTa);
    }
    else{
      nai->SetTrueMult(-1);
    }
  }
  
  //cout<<"test9"<<endl;    
  if(fDali[0].e>0)
    SortData(fDaliFold,fDali); //old
  //SortData(fDaliFoldTa,fDali); //test according to time cut
  // SortData(GetNumNaI(),fDali);
  // fDali should have number of hits of GetNumNaI, so you have to tell sort process to check all of the energies. Now the definition of fDaliFold has been updated. So keep as it was.
  //Going to the add-back:
  float dummyEnergy[NUMBEROFDALICRYSTALS][6] = {{0.}}; //[6]
  int cluster_mult_counter = 0; //added
  int addback_mult_counter = 0; //added
  //Making add-back and true multiplicity:
  //The Energy must be sorted already according to the highest detected one.
  //cout<<"Starting addback"<<endl;
  for(int i=0;i<fDaliFold;i++){ //old
    TArtDALINaI *nai = (TArtDALINaI*)fNaIArray->At(i);
    //Check the values if properly stored in fDali.
    if(crystalUsedForAddback[fDali[i].id] == true || fDali[i].ttrue == false) continue;
      if(fDali[i].e>0 && fDali[i].id<140){
	cluster_mult_counter++;
	if(cluster_mult_counter<=nai->GetTrueMult()){
	  dummyEnergy[fDaliMultTa][0] = fDali[i].e * (1-fDali[i].beta*TMath::Cos(fDali[i].theta*TMath::Pi()/180.))/TMath::Sqrt((1.0-fDali[i].beta*fDali[i].beta));
	  crystalUsedForAddback[fDali[i].id]=true;
	  fDali[fDaliMultTa].idwa = fDali[i].id; //fDaliMultTa =fDali[i].id;
	  for(int j = i+1; j<fDaliFold;j++){
	    if(fDali[j].e>0 && fDali[j].id<140){
	      if(crystalUsedForAddback[fDali[j].id]==false && fDali[j].ttrue==true)  {
		for(int k = 0;k<fNumberOfAddbackPartners[fDali[i].id] ;k++) {
		  if(fDali[j].id == fAddbackTable[fDali[i].id][k+1])  {
		    crystalUsedForAddback[fDali[j].id]=true;
		    fDali[j].ttrue=false;
		    dummyEnergy[fDaliMultTa][0] += fDali[j].e * (1-fDali[i].beta*TMath::Cos(fDali[i].theta*TMath::Pi()/180.))/TMath::Sqrt((1.0-fDali[i].beta*fDali[i].beta));
		    cluster_mult_counter++;
		  }
		}
	      }
	    }
	  }
	}
      }
    fDali[fDaliMultTa].idwa = fDali[i].id;
    fDali[fDaliMultTa].doppwa[0] = dummyEnergy[fDaliMultTa][0];
    if(fDali[fDaliMultTa].doppwa[0]>0){
      nai->SetClusterMult(cluster_mult_counter);
      cluster_mult_counter=0;
      nai->SetAddBackEnergy(fDali[fDaliMultTa].doppwa[0]);
      //cout<<"Inside Set addback Energy loop:"<<", Detector ID:"<<fDali[i].id<<", Orig dopp cor energy:"<<fDali[i].ode<<", Orig addback dopp cor energy:"<<fDali[i].dopp[0]<<", Set Addback Energy:"<<nai->GetAddBackEnergy()<<", True mult counter:"<<nai->GetTrueMult()<<", Cluster mult counter:"<<nai->GetClusterMult()<<endl;
      addback_mult_counter++;
    }
    else{
      nai->SetAddBackEnergy(-9999.);
      nai->SetClusterMult(-1);
      nai->SetAddBackMult(-1);
    }
    fDaliMultTa++;
  }
      
  for(int i=0;i<fDaliFold;i++){
    TArtDALINaI *nai = (TArtDALINaI*)fNaIArray->At(i);
    if(addback_mult_counter>0){
      nai->SetAddBackMult(addback_mult_counter);
    }
    else{
      nai->SetAddBackMult(-1);
    }
    if(nai->GetTrueMult()>=1 && nai->GetClusterMult()>nai->GetTrueMult()){
      nai->SetClusterMult(-1);
      nai->SetAddBackEnergy(-9999.);
    }
    if(fDali[i].id>=140){
      nai->SetClusterMult(-1);
      nai->SetAddBackEnergy(-9999.);
      nai->SetTrueMult(-1);
    }
    if(nai->GetAddBackEnergy()==-9999){
      nai->SetTrueMult(-1);
      nai->SetClusterMult(-1);
    }
    if(fDali[i].e == -999){
      nai->SetClusterMult(-1);
      nai->SetAddBackEnergy(-9999.);
      nai->SetTrueMult(-1);
    }
    if(fDali[i].dopp[0]<0){
      nai->SetClusterMult(-1);
      nai->SetAddBackEnergy(-9999.);
      nai->SetTrueMult(-1);
    }
    if(fDali[i].ttrue==false){
      nai->SetClusterMult(-1);
      nai->SetAddBackEnergy(-9999.);
      nai->SetTrueMult(-1);
      nai->SetAddBackMult(-1);
    }
    if(nai->GetTrueMult()>=1){
      cout<<""<<endl;
      //cout<<"Check loop:"<<", Detector ID:"<<fDali[i].id<<", Orig dopp cor energy:"<<fDali[i].ode<<", Orig addback dopp cor energy:"<<fDali[i].dopp[0]<<", TTrue:"<<(fDali[i].ttrue?"true":"false")<<", Used for addback:"<<(crystalUsedForAddback[fDali[i].id]?"true":"false")<<", Set Addback Energy:"<<nai->GetAddBackEnergy()<<", True mult counter:"<<nai->GetTrueMult()<<", AddBack Mult:"<<nai->GetAddBackMult()<<", Cluster mult counter:"<<nai->GetClusterMult()<<endl;
      }
  }

  //cout<<"test20"<<endl;
  for(int i = fDaliMultTa;i<NUMBEROFDALICRYSTALS;i++) {
    fDali[i].doppwa[0] = -999;
    fDali[i].idwa      = -999;
  }
  //cout<<"test21"<<endl;
}

//__________________________________________________________
TArtDALINaI * TArtCalibDALI::GetNaI(Int_t i) {
  return (TArtDALINaI *)fNaIArray->At(i);
}
//__________________________________________________________
TArtDALINaIPara * TArtCalibDALI::GetNaIPara(Int_t i) {
  return (TArtDALINaIPara *)fNaIParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibDALI::GetNumNaI() {
  return fNaIArray->GetEntries();
}
//__________________________________________________________
TArtDALINaI * TArtCalibDALI::FindNaI(Int_t id){
  for(Int_t i=0;i<GetNumNaI();i++)
    if(id == ((TArtDALINaI*)fNaIArray->At(i))->GetID())
      return (TArtDALINaI*)fNaIArray->At(i);
  return NULL;
}
