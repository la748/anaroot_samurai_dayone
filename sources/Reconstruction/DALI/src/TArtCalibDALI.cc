#include "TArtCalibDALI.hh"
//#include "TArtCalibCoin.hh"
#include "TArtDALINaI.hh"
#include "TArtDALINaIPara.hh"
#include "TArtDALIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtTOF.hh"
#include "TArtCore.hh"
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"
#include "TArtMap.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>
#include <TVector3.h>

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
      nai->SetRawADC(val);
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
    nai->SetTimeOffseted(-10000.);
    nai->SetTimeTrueEnergy(-999.);
    nai->SetTimeTrueTime(-10000.);
    nai->SetTimeTrueTimeOffseted(-10000.);

    //ID depends on the definition in mapping file
    if(nai->GetID()==226) trefdummy[0] = nai->GetRawTDC();
    if(nai->GetID()==227) trefdummy[1] = nai->GetRawTDC();
    if(nai->GetID()==228) trefdummy[2] = nai->GetRawTDC();
  }

  for(Int_t i=0;i<GetNumNaI();i++){

    TArtDALINaI *nai = (TArtDALINaI*)fNaIArray->At(i);    
    TArtDALINaIPara *para = fNaIParaArray[i];

    Int_t adc = nai->GetRawADC();
    Int_t tdc = nai->GetRawTDC();
    Double_t theta = nai->GetTheta();
    Double_t costheta = TMath::Cos(theta*TMath::Pi()/180.);
    //Double_t fEnergy = (adc - para->GetQPed()) * para->GetQCal();
    Double_t random = (Double_t)r->Rndm();
    Double_t fEnergy = (adc-0.5 + random) * para->GetQCal()  + para->GetQPed();

    //	Double_t fEnergy = adc * para->GetQCal() + para->GetQPed();
    nai->SetEnergyWithoutT(fEnergy);
    nai->SetCosTheta(costheta);

    if(fEnergy>0) fMultWithoutT++;

    //	if(!(tdc>0 && adc>0 && tdc<100000 && adc<4000)) continue;
    if(tdc>0 && adc>0 && tdc<100000 && adc<4000 && (trefdummy[0]||trefdummy[1]||trefdummy[2])>0){ }
    else{ continue; }

    nai->SetEnergy(fEnergy);

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
      if(fEnergy>0)fTimeTrueMult++;
    }

    // copy some information from para to data container
    //	nai->SetID(para->GetID());
    //	nai->SetFpl(para->GetFpl());
    //	nai->SetDetectorName(*(para->GetDetectorName()));
    //	nai->SetLayer(para->GetLayer());
    //	nai->SetTheta(para->GetNaITheta());

  }

  fReconstructed = true;

  DopplerCorrect(fBeta);
  DopplerCorrectVertex(fBeta,fVertex);

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
      if( tof->GetUpstreamPlaFpl()<=fpl && tof->GetDownstreamPlaFpl()>=fpl ){
	DopplerCorrect(tof->GetBeta());
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
    if(nai->GetTimeTrueEnergy() > 0.){
      nai->SetTimeTrueDoppCorEnergy(fDoppCorEnergy);
    }else{
      nai->SetTimeTrueDoppCorEnergy(-1.);
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
