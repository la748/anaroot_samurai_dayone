#include "TArtCalibCATANA.hh" 
#include "TArtCATANACsI.hh"
#include "TArtCATANACsIPara.hh"
#include "TArtCATANAParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"
//#include "TArtMapper.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

#include <stdio.h>

//__________________________________________________________
TArtCalibCATANA::TArtCalibCATANA()
  : TArtReconstruction("CATANACsI")
{
  TArtCore::Info(__FILE__,"Creating the CATANA detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtCATANAParameters*)sman->FindParameters("CATANAParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: CATANAParameters");
    throw;
  }

  fCsIArray = new TClonesArray("TArtCATANACsI",10);
  fCsIArray->SetName("CATANACsI");
  fCsIArray->SetOwner();
  sman->AddDataContainer(fCsIArray);

}

//__________________________________________________________
TArtCalibCATANA::~TArtCalibCATANA()  {
  ClearData();

  delete fCsIArray;
}

//__________________________________________________________
void TArtCalibCATANA::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device   = seg->GetDevice();
    Int_t detector = seg->GetDetector();
    if(CATANAQ == detector || CATANAT == detector) {
      LoadData(seg);
    }
  }
  return;

}

//__________________________________________________________
void TArtCalibCATANA::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();
  if(CATANAQ != detector && CATANAT != detector) return;
  // look for tref first
  Int_t tref1=0;


  for(Int_t i=0;i<seg->GetNumData();i++){
    TArtRawDataObject *d = seg->GetData(i);
    Int_t geo = d->GetGeo();
    Int_t ch = d->GetCh();
    Int_t val = (Int_t)d->GetVal();
    if(geo==7 && ch==127) tref1 = val;
  }

  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    TArtCATANACsIPara *para = (TArtCATANACsIPara*)setup->GetCATANACsIPara(&mm);
    if(NULL == para){
      TArtCore::Debug(__FILE__,"Could not find TArtCATANACsIPara...: Dev:%d, %s", device, mm.GetMapInfo());
      //printf("Could not find TArtCATANACsIPara...: Dev:%d, %s\n", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtCATANACsIPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName().Data());
      //      printf("Find TArtCATANACsIPara...: Dev:%d, %s, : %s\n", device, mm.GetMapInfo(), para->GetDetectorName().Data());

    }


    Int_t id = para->GetID();
    Int_t ncsi = fCsIArray->GetEntries();
    TArtCore::Debug(__FILE__,"Entries in CATANACsIArray: %d, CATANA-ID:%d", ncsi, id);
    TArtCATANACsI* csi = FindCsI(id);

    if(NULL==csi){
      new ((*fCsIArray)[ncsi]) TArtCATANACsI();
      csi = (TArtCATANACsI *)fCsIArray->At(ncsi);
      csi->SetID(para->GetID());
      csi->SetFpl(para->GetFpl());
      csi->SetDetectorName(*(para->GetDetectorName()));
      csi->SetPositionX(para->GetDetPos(0));
      csi->SetPositionY(para->GetDetPos(1));
      csi->SetPositionZ(para->GetDetPos(2));
      csi->SetRawTRef(tref1);

      fCsIParaArray.push_back(para);
    }

    // set raw data
    if(CATANAQ == detector)csi->SetRawADC(val);
    if(CATANAT == detector)csi->SetRawTDC(val);

  }

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibCATANA::ClearData()   {
  fCsIArray->Clear();
  fCsIParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibCATANA::ReconstructData(){ // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  TArtCATANACsI *csi = 0;
    
  for(Int_t i=0;i<GetCsIEntries();i++){

    TArtCATANACsI *csi = (TArtCATANACsI*)fCsIArray->At(i);    
    TArtCATANACsIPara *para = fCsIParaArray[i];

    Int_t adc = csi->GetRawADC();
    Int_t tdc = csi->GetRawTDC();
    Int_t tref = csi->GetRawTRef();
    Int_t tdcsub = tdc - tref;
    
    csi->SetRawTDC_TRefSubtracted(tdcsub);

    //Double_t fEnergy = (adc - para->GetQPed()) * para->GetQCal();
    Double_t fEnergy = adc * para->GetQCal() + para->GetQPed();
    csi->SetEnergy(fEnergy);

    if(!(tdc>0 && adc>0 && tdc<100000 && adc<4000)) continue;
    
    Double_t fTime = (double)tdcsub* para->GetTCal();
    Double_t fTimeOffseted = fTime + para->GetTOffset();

    csi->SetTime(fTime);
    csi->SetTimeOffseted(fTimeOffseted);
    
    // copy some information from para to data container
    csi->SetID(para->GetID());
    csi->SetFpl(para->GetFpl());
    csi->SetDetectorName(*(para->GetDetectorName()));
    csi->SetPositionX(para->GetDetPos(0));
    csi->SetPositionY(para->GetDetPos(1));
    csi->SetPositionZ(para->GetDetPos(2));
  }

  fReconstructed = true;
  return;
}

//__________________________________________________________
void TArtCalibCATANA::DopplerCorrect(Double_t beta)   {

  if(!fReconstructed){
    TArtCore::Info(__FILE__,"Data is not reconstructed.");
    return;
  }

  for(Int_t i=0;i<GetCsIEntries();i++){
    TArtCATANACsI *csi = (TArtCATANACsI*)fCsIArray->At(i);
    Double_t x = csi->GetPositionX();
    Double_t y = csi->GetPositionY();
    Double_t z = csi->GetPositionZ();
    Double_t abs = TMath::Sqrt(TMath::Power(x,2)+TMath::Power(y,2)+
			       TMath::Power(z,2));
    Double_t cost = z/abs;
    Double_t fDoppCorEnergy = csi->GetEnergy() * (1-beta*cost)/TMath::Sqrt((1.0-beta*beta));
    csi->SetDoppCorEnergy(fDoppCorEnergy);
  }

  return;
}

//__________________________________________________________
TArtCATANACsI * TArtCalibCATANA::GetCsI(Int_t i) {
  return (TArtCATANACsI *)fCsIArray->At(i);
}
//__________________________________________________________
TArtCATANACsIPara * TArtCalibCATANA::GetCsIPara(Int_t i) {
  return (TArtCATANACsIPara *)fCsIParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibCATANA::GetCsIEntries() {
  return fCsIArray->GetEntries();
}
//__________________________________________________________
TArtCATANACsI * TArtCalibCATANA::FindCsI(Int_t id){
  for(Int_t i=0;i<GetCsIEntries();i++)
    if(id == ((TArtCATANACsI*)fCsIArray->At(i))->GetID())
      return (TArtCATANACsI*)fCsIArray->At(i);
  return NULL;
}
