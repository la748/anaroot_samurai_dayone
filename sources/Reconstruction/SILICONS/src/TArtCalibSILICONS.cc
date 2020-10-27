#include "TArtCalibSILICONS.hh" 
#include "TArtSILICONS.hh"
#include "TArtSILICONSPara.hh"
#include "TArtSILICONSParameters.hh"
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
TArtCalibSILICONS::TArtCalibSILICONS(const TString outdataname) 
  : TArtReconstruction(outdataname)
{
  TArtCore::Info(__FILE__,"Creating SILICONS detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtSILICONSParameters*)sman->FindParameters("SILICONSParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: SILICONSParameters");
    //    cout << "Can not find parameter handler: SILICONSParameters" << endl;

    throw;
  }
  //  setup->PrintListOfPara(); exit(0);

  fSiArray = new TClonesArray("TArtSILICONS",140);
  fSiArray->SetName("SILICONS");
  fSiArray->SetOwner();
  sman->AddDataContainer(fSiArray);

}

//__________________________________________________________
TArtCalibSILICONS::~TArtCalibSILICONS()  {
  ClearData();
  fSiArray->Delete();
  fSiParaArray.clear();
  delete fSiArray;
}

//__________________________________________________________
void TArtCalibSILICONS::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t detector = seg->GetDetector();
    if(TEL == detector) LoadData(seg);
  }
  return;

}

//__________________________________________________________
void TArtCalibSILICONS::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();

  if(TEL != detector) return;

  Int_t module	 = seg->GetModule();

  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtSILICONSPara *para = (TArtSILICONSPara*)setup->GetSILICONSPara(&mm);
    if(NULL == para){
      TArtCore::Debug(__FILE__,"Could not find TArtSILICONSPara...: Dev:%d, %s", device, mm.GetMapInfo());
      //      cout << "Could not find TArtSILICONSPara...: " <<  device << " " <<  mm.GetMapInfo() << " " << fpl << " " << detector << " " << geo << " " << ch << endl;
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtSILICONSPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    Int_t nsi = fSiArray->GetEntries();
    TArtCore::Debug(__FILE__,"Entries in SILICONSArray: %d, Si-ID:%d", nsi, id);
    TArtSILICONS * si = FindSi(id);

    if(NULL==si){
      new ((*fSiArray)[nsi]) TArtSILICONS();
      si = (TArtSILICONS *)fSiArray->At(nsi);
      si->SetID(para->GetID());
      si->SetFpl(para->GetFpl());
      si->SetDetectorName(*(para->GetDetectorName()));
      //      std::cout << para->GetID()  << std::endl; 
      fSiParaArray.push_back(para);
    }

    // set raw data
    if(TEL == detector){
      if(module == TEL_ADC){
	si->SetRawADC(val);
      } else if(module == TEL_TDC) {
	si->SetRawTDC(val);
      }
      si->SetPosX(para->GetPosition(0));
      si->SetPosY(para->GetPosition(1));
      si->SetPosZ(para->GetPosition(2));
      //     si->SetDirection(para->GetDirection());
    }

  }
  //      exit(0);
  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibSILICONS::ClearData()   {
  fSiArray->Clear();
  fSiParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibSILICONS::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  TArtSILICONS *si = 0;

  for(Int_t i=0;i<GetNumSi();i++){
    
    si = (TArtSILICONS*)fSiArray->At(i);    
    TArtSILICONSPara *para = fSiParaArray[i];

    Int_t adc = si->GetRawADC();
    Int_t tdc = si->GetRawTDC();
    Double_t fEnergy = (Double_t)adc;
    Double_t fTime = (Double_t)tdc;

    si->SetEnergy((fEnergy - para->GetPedestal()) * para->GetCh2MeV());
    si->SetTime(para->GetTOffset() + fTime   * para->GetCh2Ns());

  }

  fReconstructed = true;

  return;
}

//__________________________________________________________
TArtSILICONS * TArtCalibSILICONS::GetSi(Int_t i) {
  return (TArtSILICONS *)fSiArray->At(i);
}
//__________________________________________________________
TArtSILICONSPara * TArtCalibSILICONS::GetSiPara(Int_t i) {
  return (TArtSILICONSPara *)fSiParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibSILICONS::GetNumSi() {
  return fSiArray->GetEntries();
}
//__________________________________________________________
TArtSILICONS * TArtCalibSILICONS::FindSi(Int_t id){
  for(Int_t i=0;i<GetNumSi();i++)
    if(id == ((TArtSILICONS*)fSiArray->At(i))->GetID())
      return (TArtSILICONS*)fSiArray->At(i);
  return NULL;
}
