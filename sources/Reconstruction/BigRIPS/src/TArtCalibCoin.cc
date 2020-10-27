#include "TArtCalibCoin.hh" 
#include "TArtRawEventObject.hh"
#include "TArtEventInfo.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"

#include "segidlist.hh"

#include <TROOT.h>

//__________________________________________________________
TArtCalibCoin::TArtCalibCoin(const TString outdataname) 
  : TArtReconstruction(outdataname) {
  TArtCore::Info(__FILE__,"Creating the Coincidence register calibration objects...");
  sman = TArtStoreManager::Instance();

  // event info class is not registered to StoreManager since it is supposed to be made in TArtEventStore

  myfpl=-1;
  myaddress=-1;

}

//__________________________________________________________
TArtCalibCoin::~TArtCalibCoin()  {
}

//__________________________________________________________
void TArtCalibCoin::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");
  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t detector = seg->GetDetector();
    if(COIN == detector)
      LoadData(seg);
  }
 
  return;

}

//__________________________________________________________
void TArtCalibCoin::ClearData()   {
  TClonesArray *info = (TClonesArray*)sman->FindDataContainer("EventInfo");
  ((TArtEventInfo *)info->At(0))->SetTriggerBit(0);
}

//__________________________________________________________
void TArtCalibCoin::LoadData(TArtRawSegmentObject *seg)   {

  Int_t detector = seg->GetDetector();
  Int_t fpl = seg->GetFP();
  Int_t address = seg->GetAddress();

  if(COIN != detector) return;
  if(myfpl !=-1 && myfpl != fpl) return;
  if(myaddress !=-1 && myaddress != address) return;

  if(seg->GetNumData()>1) 
    TArtCore::Error(__FILE__,"Can not handle more than two data.:%d",seg->GetNumData());
  TArtRawDataObject *d = seg->GetData(0);

  val = d->GetVal();
  TClonesArray *info = (TClonesArray*)sman->FindDataContainer("EventInfo");
  if(((TArtEventInfo *)info->At(0))->GetTriggerBit() == 0)
    ((TArtEventInfo *)info->At(0))->SetTriggerBit(val);
  else
    TArtCore::Error(__FILE__,"Two Coin is found. fpl: %d val: %d and %d",fpl,val,((TArtEventInfo *)info->At(0))->GetTriggerBit());

  fDataLoaded = true;
  return;

}

//__________________________________________________________
bool TArtCalibCoin::IsChTrue(Int_t id) {
  return ((1 << (id - 1)) & val) ? true : false;
}

//__________________________________________________________
void TArtCalibCoin::SetFpl(Int_t val){
  myfpl = val;
  TArtCore::Info(__FILE__,"Docode the Coin whose Fpl is %d", myfpl);
}

//__________________________________________________________
void TArtCalibCoin::SetAddress(Int_t val){
  myaddress = val;
  TArtCore::Info(__FILE__,"Docode the Coin whose Address is %d", myaddress);
}


