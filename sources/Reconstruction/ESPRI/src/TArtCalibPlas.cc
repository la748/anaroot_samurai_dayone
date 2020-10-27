#include "TArtCalibPlas.hh" 
#include "TArtPlas.hh"
#include "TArtPlasPara.hh"
#include "TArtESPRIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"
#include "TArtMap.hh"
#include "TArtTDCHit.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

#include "Riostream.h"

//__________________________________________________________
TArtCalibPlas::TArtCalibPlas()
  : TArtReconstruction("ESPRIPLAS")
{
  TArtCore::Info(__FILE__,"Creating the Plas detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtESPRIParameters*)sman->FindParameters("ESPRIParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: ESPRIParameters");
    throw;
  }

  fPlasArray = new TClonesArray("TArtPlas",10);
  fPlasArray->SetName("ESPRIPLAS");
  fPlasArray->SetOwner();
  sman->AddDataContainer(fPlasArray);

}

//__________________________________________________________
TArtCalibPlas::~TArtCalibPlas()  {
  ClearData();

  delete fPlasArray;
}

//__________________________________________________________
void TArtCalibPlas::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device   = seg->GetDevice();
    Int_t detector = seg->GetDetector();
    //if(PLAQ2 == detector) LoadData(seg);//ESPRI2016
    if(PLAQ == detector) LoadData(seg);//SAMURAI2016
  }
  return;

}

//__________________________________________________________
void TArtCalibPlas::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();

  //if(PLAQ2 != detector) return;
  if(PLAQ != detector) return;//SAMURAI2016
  //TArtCore::Info(__FILE__,"Run PLAS Load: det:%d Num:%d", detector,seg->GetNumData());

  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtPlasPara *para = (TArtPlasPara*)setup->FindPlasPara(&mm);
    if(NULL == para){
      TArtCore::Debug(__FILE__,"Could not find TArtPlasPara...: Dev:%d, %s", device, mm.GetMapInfo());
      //TArtCore::Info(__FILE__,"Could not find TArtPlasPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtPlasPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
      //TArtCore::Info(__FILE__,"Find TArtPlasPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    Int_t layer = para->GetLayer();
    Int_t channel = para->GetCh();
    Int_t npla = fPlasArray->GetEntries();
    TArtCore::Debug(__FILE__,"Entries in PlasArray: %d, Plas-ID:%d", npla, id);
    TArtPlas * pla = FindPlas(id);

    if(NULL==pla){
      new ((*fPlasArray)[npla]) TArtPlas();
      pla = (TArtPlas *)fPlasArray->At(npla);
      pla->SetID(para->GetID());
      pla->SetFpl(para->GetFpl());
      pla->SetDetectorName(*(para->GetDetectorName()));
      pla->SetLayer(para->GetLayer());

      fPlasParaArray.push_back(para);
    }

    // set raw data
    pla->SetRawADC(val);
  }

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibPlas::ClearData()   {
  fPlasArray->Clear();
  fPlasParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibPlas::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  TClonesArray *tdctrk =(TClonesArray *)sman->FindDataContainer("ESPRITDC");
  if(!tdctrk) return;
  Int_t nrt_tdc = tdctrk->GetEntriesFast();

  TArtPlas *pla = 0;

  //for mean calculation
  Int_t ioffset = GetNumPlas();

    Double_t fEnergy,fLEnergy, fTime, fLTime;
    Double_t fWidth,fLWidth, fTimeW, fLTimeW;

  for(Int_t i=0;i<ioffset;i++){
    TArtPlas *pla = (TArtPlas*)fPlasArray->At(i);    
    TArtPlasPara *para = fPlasParaArray[i];

    fEnergy=0; fTime=0; fWidth=0; fTimeW=0;
    
    Int_t ch=para->GetCh();
    Int_t layer=para->GetLayer();
    Int_t id_plane=para->GetPlaneID();

    Int_t adc = pla->GetRawADC();
    fEnergy = (adc - para->GetQPed()) * para->GetQCal();
    pla->SetEnergy(fEnergy);
    
    for(Int_t j=0;j<nrt_tdc;j++){//Scan for TDC
      TArtTDCHit *hit = (TArtTDCHit*)tdctrk->At(j);
      Int_t id_plane_t = (Double_t)hit->GetPlaneID();
      Int_t layer_t = (Double_t)hit->GetLayer();
      Int_t ch_t  = (Double_t)hit->GetWireID();
      if(id_plane_t==id_plane&&ch_t==ch){
	/*
	cout<<"Same element: "<< para->GetDetectorName()->Data()<<":"<<endl;
	cout<<"&Same element: "<< hit->GetDetectorName()->Data()<<":"<<layer_t<<endl;
	cout<<hit->GetTrailTDC()<<":"<<hit->GetTDC()<<endl;
	*/
	if(layer_t>10){//QTC
	  fTimeW=hit->GetTDC();	  fWidth=hit->GetTrailTDC()-hit->GetTDC();
	  pla->SetTimeW(fTimeW);  pla->SetWidth(fWidth);
	}else{//TDC
	  fTime=hit->GetTDC();
	  pla->SetTime(fTime);
	}
      }
    }
    
    //cout<<pla->GetEnergy()<<":"<<pla->GetTime()<<":"<<pla->GetTimeW()<<":"<<pla->GetWidth()<<":"<<endl;
    // copy some information from para to data container
    pla->SetID(para->GetID());
    pla->SetFpl(para->GetFpl());
    pla->SetDetectorName(*(para->GetDetectorName()));
    pla->SetLayer(para->GetLayer());
    pla->SetPlaneID(para->GetPlaneID());
    pla->SetCh(para->GetCh());

    //if(!(tdc>0 && adc>0 && tdc<100000 && adc<4000)) continue;
    //if(!( adc>0  && adc<4000)) continue;

  /*
    if(ch%2==0){//Mean
      Int_t addr =ioffset+ch/2+layer+layer/3-2;
      new ((*fPlasArray)[addr]) TArtPlas();
      TArtPlas *pla = (TArtPlas*)fPlasArray->At(addr);
      Double_t fMean=0.5*(fEnergy+fLEnergy);
      //cout<<addr<<":"<<fLEnergy<<":"<<fEnergy<<":"<<fMean<<endl;
      pla->SetEnergy(fMean);
      pla->SetID(addr);
      pla->SetFpl(para->GetFpl());
      pla->SetPlaneID(para->GetPlaneID());
      pla->SetLayer(5);
      pla->SetCh(addr-ioffset-1);
      pla->SetTime(0.5*(fTime+fLTime));
      pla->SetTimeW(0.5*(fTimeW+fLTimeW));
      pla->SetWidth(0.5*(fWidth+fLWidth));
    }
    if(layer!=2){
      fLEnergy = fEnergy; fLTime = fTime;
      fLWidth = fWidth; fLTimeW = fTimeW;
    }
  */
  }
  //cout<<"Total Size of Array::"<<fPlasArray->GetEntries()<<endl;
  //cout<<"NextEvent"<<endl<<endl;

  fReconstructed = true;
  return ;
}

//__________________________________________________________
TArtPlas * TArtCalibPlas::GetPlas(Int_t i) {
  return (TArtPlas *)fPlasArray->At(i);
}
//__________________________________________________________
TArtPlasPara * TArtCalibPlas::GetPlasPara(Int_t i) {
  return (TArtPlasPara *)fPlasParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibPlas::GetNumPlas() {
  return fPlasArray->GetEntries();
}
//__________________________________________________________
TArtPlas * TArtCalibPlas::FindPlas(Int_t id){
  for(Int_t i=0;i<GetNumPlas();i++)
    if(id == ((TArtPlas*)fPlasArray->At(i))->GetID())
      return (TArtPlas*)fPlasArray->At(i);
  return NULL;
}
