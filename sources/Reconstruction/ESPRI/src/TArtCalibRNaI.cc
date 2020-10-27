#include "TArtCalibRNaI.hh" 
#include "TArtRNaI.hh"
#include "TArtRNaIPara.hh"
#include "TArtESPRIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"
#include "TArtMap.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

//#include <stdio.h>
#include "Riostream.h"

//__________________________________________________________
TArtCalibRNaI::TArtCalibRNaI()
  : TArtReconstruction("ESPRINaI")
{
  TArtCore::Info(__FILE__,"Creating the RNaI detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtESPRIParameters*)sman->FindParameters("ESPRIParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: ESPRIParameters");
    throw;
  }

  fRNaIArray = new TClonesArray("TArtRNaI",10);
  fRNaIArray->SetName("ESPRINaI");
  fRNaIArray->SetOwner();
  sman->AddDataContainer(fRNaIArray);

}

//__________________________________________________________
TArtCalibRNaI::~TArtCalibRNaI()  {
  ClearData();

  delete fRNaIArray;
}

//__________________________________________________________
void TArtCalibRNaI::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device   = seg->GetDevice();
    Int_t detector = seg->GetDetector();
    if(NAIE == detector) LoadData(seg);//ESPRI2016
    //if(5 == detector) LoadData(seg);//ESPRI2013
  }
  return;

}

//__________________________________________________________
void TArtCalibRNaI::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();

  if(NAIE != detector) return;
  //if(5 != detector) return;
  ///if(5 == detector)   TArtCore::Info(__FILE__,"NaI ADC data %d",seg->GetNumData());

  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtRNaIPara *para = (TArtRNaIPara*)setup->FindRNaIPara(&mm);
    if(NULL == para){
      TArtCore::Debug(__FILE__,"Could not find TArtRNaIPara...: Dev:%d, %s", device, mm.GetMapInfo());
      //TArtCore::Info(__FILE__,"Could not find TArtRNaIPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtRNaIPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
      //TArtCore::Info(__FILE__,"Find TArtRNaIPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    Int_t nnai = fRNaIArray->GetEntries();
    TArtCore::Debug(__FILE__,"Entries in RNaIArray: %d, RNaI-ID:%d", nnai, id);
    TArtRNaI * nai = FindRNaI(id);

    if(NULL==nai){
      new ((*fRNaIArray)[nnai]) TArtRNaI();
      nai = (TArtRNaI *)fRNaIArray->At(nnai);
      nai->SetID(para->GetID());
      nai->SetFpl(para->GetFpl());
      nai->SetDetectorName(*(para->GetDetectorName()));
      nai->SetLayer(para->GetLayer());

      fRNaIParaArray.push_back(para);
    }

    // set raw data
    if(NAIE == detector)//ESPRI2016
      //if(5 == detector)//ESPRI2013
      nai->SetRawADC(val);
    if(NAIT == detector)//ESPRI2016
      //if(17 == detector)//ESPRI2013
      nai->SetRawTDC(val);

  }

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibRNaI::ClearData()   {
  fRNaIArray->Clear();
  fRNaIParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibRNaI::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  TArtRNaI *nai = 0;

  //for mean calculation
  Int_t ioffset = GetNumRNaI();
  Double_t fEnergy=0.,fLEnergy=0.;
  for(Int_t i=0;i<ioffset;i++){
    TArtRNaI *nai = (TArtRNaI*)fRNaIArray->At(i);    
    TArtRNaIPara *para = fRNaIParaArray[i];

    Int_t adc = nai->GetRawADC();
    Int_t tdc = nai->GetRawTDC();
    Double_t fEnergy = (adc - para->GetQPed()) * para->GetQCal();
    nai->SetEnergy(fEnergy);

    // copy some information from para to data container
    nai->SetID(para->GetID());
    nai->SetFpl(para->GetFpl());
    nai->SetDetectorName(*(para->GetDetectorName()));
    nai->SetLayer(para->GetLayer());

    //if(!(tdc>0 && adc>0 && tdc<100000 && adc<4000)) continue;
    if(!( adc>0  && adc<4000)) continue;

    /*
    Double_t fTime = (double)tdc * para->GetTCal();
    nai->SetTime(fTime);
    */
    /*
    if(i%2==1){
      //Int_t nnum = fRNaIArray->GetEntries();
      //new ((*fRNaIArray)[nnum+i/2]) TArtRNaI();
      new ((*fRNaIArray)[i/2+ioffset]) TArtRNaI();
      TArtRNaI *nai = (TArtRNaI*)fRNaIArray->At(i/2+ioffset);
      Double_t fMean=0.5*(fEnergy+fLEnergy);
      //cout<<i/2<<":"<<i/2+ioffset<<":"<<fLEnergy<<":"<<fEnergy<<":"<<fMean<<endl;
      nai->SetEnergy(fMean);
      nai->SetID(para->GetID()/2+ioffset);
      nai->SetFpl(para->GetFpl());
      nai->SetLayer(para->GetLayer()+2);
    }
    fLEnergy = fEnergy;
    //cout<<i<<":Number"<<endl;
    */
  }
  
  //cout<<"Total Size of Array::"<<fRNaIArray->GetEntries()<<endl;
  //cout<<"NextEvent"<<endl<<endl;
  fReconstructed = true;
  return ;
}

//__________________________________________________________
TArtRNaI * TArtCalibRNaI::GetRNaI(Int_t i) {
  return (TArtRNaI *)fRNaIArray->At(i);
}
//__________________________________________________________
TArtRNaIPara * TArtCalibRNaI::GetRNaIPara(Int_t i) {
  return (TArtRNaIPara *)fRNaIParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibRNaI::GetNumRNaI() {
  return fRNaIArray->GetEntries();
}
//__________________________________________________________
TArtRNaI * TArtCalibRNaI::FindRNaI(Int_t id){
  for(Int_t i=0;i<GetNumRNaI();i++)
    if(id == ((TArtRNaI*)fRNaIArray->At(i))->GetID())
      return (TArtRNaI*)fRNaIArray->At(i);
  return NULL;
}
