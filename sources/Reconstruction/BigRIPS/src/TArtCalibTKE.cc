#include "TArtCalibTKE.hh" 
#include "TArtTKE.hh"
#include "TArtTKEPara.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"
#include "TArtMap.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

#include <stdio.h>

//__________________________________________________________
TArtCalibTKE::TArtCalibTKE(const TString outdataname) 
  : TArtReconstruction(outdataname)
{
  TArtCore::Info(__FILE__,"Creating the BigRIPS TKE detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtBigRIPSParameters*)sman->FindParameters("BigRIPSParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: BigRIPSParameters");
    throw;
  }

  fTKEArray = new TClonesArray("TArtTKE",4);
  fTKEArray->SetName("BigRIPSTKE");
  fTKEArray->SetOwner();
  sman->AddDataContainer(fTKEArray);
  
}

//__________________________________________________________
TArtCalibTKE::~TArtCalibTKE()  {
  ClearData();
}

//__________________________________________________________
void TArtCalibTKE::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t detector = seg->GetDetector();
    if(NAIE == detector || LABRA == detector || TOTQ == detector) LoadData(seg);
  }
  return;

}

//__________________________________________________________
void TArtCalibTKE::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();

  if(NAIE != detector && LABRA != detector || TOTQ == detector) return;

  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtTKEPara *para = (TArtTKEPara*)setup->FindTKEPara(&mm);
    if(NULL == para){
      TArtCore::Debug(__FILE__,"Could not find TArtTKEPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtTKEPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    Int_t ntke = fTKEArray->GetEntries();
    TArtCore::Debug(__FILE__,"Entries in TKEArray: %d, TKE-ID:%d", ntke, id);
    TArtTKE * tke = FindTKE(id);

    if(NULL==tke){
      new ((*fTKEArray)[ntke]) TArtTKE();
      tke = (TArtTKE *)fTKEArray->At(ntke);
      tke->SetID(para->GetID());
      tke->SetFpl(para->GetFpl());
      tke->SetDetectorName(*(para->GetDetectorName()));
      tke->SetNumADC(para->GetNumADC());

      fTKEParaArray.push_back(para);
    }

    // set raw data
    tke->SetRawADC(val);
    if(1 == para->GetNumADC()){
      tke->SetRawADC(val);
      //tke->SetRawTDC(val);// which channel?
    }
    else{
      Int_t tke_ch = para->FindCh(&mm);
      tke->SetRawADC(tke_ch,val);
    }

  }

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibTKE::ClearData()   {
  fTKEArray->Clear();
  fTKEParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibTKE::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  TArtTKE *tke = 0;

  for(Int_t i=0;i<GetNumTKE();i++){
    
    TArtTKE *tke = (TArtTKE*)fTKEArray->At(i);    
    TArtTKEPara *para = fTKEParaArray[i];

    if(1 == para->GetNumADC()){
      Int_t adc = tke->GetRawADC();
      Int_t tdc = tke->GetRawTDC();
      Double_t fEnergy = (Double_t)adc - para->GetEPedestal();
      Double_t fTiming = (Double_t)tdc;
      tke->SetEnergy(fEnergy * para->GetECh2MeV());
      tke->SetTiming((para->GetTOffset()) + (fTiming * para->GetTCh2nsec()));
    }
    else{
      int numadc = para->GetNumADC();
      for(int j=0;j<numadc;j++){
	Int_t adc = tke->GetRawADC(j);
	Int_t tdc = tke->GetRawTDC(j);
	Double_t fEnergy = (Double_t)adc - para->GetEPedestal(j);
	Double_t fTiming = (Double_t)tdc;
	tke->SetEnergy(j,fEnergy * para->GetECh2MeV(j));
	tke->SetTiming(j,para->GetTOffset(j) + fTiming * para->GetTCh2nsec(j));
      }
    }

  }

  fReconstructed = true;

  return;
}

//__________________________________________________________
TArtTKE * TArtCalibTKE::GetTKE(Int_t i) {
  return (TArtTKE *)fTKEArray->At(i);
}
//__________________________________________________________
TArtTKEPara * TArtCalibTKE::GetTKEPara(Int_t i) {
  return (TArtTKEPara *)fTKEParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibTKE::GetNumTKE() {
  return fTKEArray->GetEntries();
}
//__________________________________________________________
TArtTKE * TArtCalibTKE::FindTKE(Int_t id){
  for(Int_t i=0;i<GetNumTKE();i++)
    if(id == ((TArtTKE*)fTKEArray->At(i))->GetID())
      return (TArtTKE*)fTKEArray->At(i);
  return NULL;
}
//__________________________________________________________                           
TArtTKE * TArtCalibTKE::FindTKE(TString *n){
  for(Int_t i=0;i<GetNumTKE();i++)
    if(*n == *(((TArtTKE*)fTKEArray->At(i))->GetDetectorName()))
      return (TArtTKE*)fTKEArray->At(i);
  return NULL;
}
//__________________________________________________________                           
TArtTKE * TArtCalibTKE::FindTKE(char *n){
  for(Int_t i=0;i<GetNumTKE();i++)
    if(strcmp(((TArtTKE*)fTKEArray->At(i))->GetDetectorName()->Data(),n) == 0)
      return (TArtTKE*)fTKEArray->At(i);
  return NULL;
}
