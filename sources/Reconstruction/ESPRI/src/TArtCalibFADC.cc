#include "TArtCalibFADC.hh" 
#include "TArtFADC.hh"
#include "TArtFADCPara.hh"
#include "TArtESPRIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"
#include "TArtMap.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

//__________________________________________________________
TArtCalibFADC::TArtCalibFADC()
  : TArtReconstruction("ESPRIFADC")
{
  TArtCore::Info(__FILE__,"Creating the FADC calibration objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtESPRIParameters*)sman->FindParameters("ESPRIParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: ESPRIParameters");
    throw;
  }

  fFADCArray = new TClonesArray("TArtFADC",20);
  fFADCArray->SetName("ESPRIFADC");
  fFADCArray->SetOwner();
  sman->AddDataContainer(fFADCArray);
}

//__________________________________________________________
TArtCalibFADC::~TArtCalibFADC()  {
  ClearData();

  delete fFADCArray;
}

//__________________________________________________________
void TArtCalibFADC::LoadData()   {
  TArtCore::Debug(__FILE__,"LoadData");

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device   = seg->GetDevice();
    Int_t detector = seg->GetDetector();
    
    //if((device == ESPRI) && (detectorid == detector)){
    if((18 == detector)){//ESPRI201
      LoadData(seg);
      //Int_t nhit = fFADCArray->GetEntries();//iru??
      //Int_t jmax = seg->GetNumFADCData();
      //TArtCore::Info(__FILE__,"Entries in FADCArray: %d, %d", jmax, detector);
    }
  }
  return;

}

void TArtCalibFADC::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();
  //if(detectorid != detector) return;
  TArtFADC *hit;
  if(18 == detector){//ESPRI2016
    Int_t ich=0;
    for(Int_t j=0;j<seg->GetNumFADCData();j++){
      TArtRawFADCDataObject *df = seg->GetFADCData(j);
      Int_t ch=df->GetCh();
      Int_t kmax=df->GetNData();
      Int_t tb=df->GetTimeBacket(0);
      Int_t va=df->GetVal(0);
      Int_t geo=0;
      //if(tb<5||tb>2045)
      //cout << ch << " " << tb << " " << va << endl;

      TArtRIDFMap mm(fpl,detector,geo,ch*2048+tb);
      TArtFADCPara *para = (TArtFADCPara*)setup->FindFADCPara(&mm);
      if(NULL == para){
	TArtCore::Debug(__FILE__,"Could not find TArtDCHitPara...: Dev:%d, %s", device, mm.GetMapInfo());
	//TArtCore::Info(__FILE__,"Could not find TArtDCHitPara...: Dev:%d, %s", device, mm.GetMapInfo());
	continue;
      }
      else {
	TArtCore::Debug(__FILE__,"Find TArtFADC: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
	//TArtCore::Info(__FILE__,"Find TArtFADC: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
      }
      
      Int_t id = para->GetID();
      Int_t nhit = fFADCArray->GetEntries();
      //TArtCore::Info(__FILE__,"ID: %d, nhit: %d, %s", id, nhit, para->GetDetectorName()->Data());
      if(j==0){
	new ((*fFADCArray)[0]) TArtFADC();
	//TArtFADC *hit = (TArtFADC *)fFADCArray->At(0);
	hit = (TArtFADC *)fFADCArray->At(0);
	//cout<<"First Array"<<":"<<j<<":"<<nhit<<endl;
      }
      if(ch<4 && ich != ch){
	new ((*fFADCArray)[ch]) TArtFADC();
	//TArtFADC *hit = (TArtFADC *)fFADCArray->At(ch);
	hit = (TArtFADC *)fFADCArray->At(ch);
	ich++;
	//cout<<"Next Array"<<":"<<ich<<":"<<ch<<endl;
	//cout<<"Entry "<< fFADCArray->GetEntries()<<endl;
      }
      //hit->SetID(id); 
      //hit->SetTb(tb);
      hit->SetCh(ch);
      hit->SetLength(2048);
      //cout<<"TEST:"<<2048*ch+tb<<":"<<va<<"::"<<endl;
      //hit->SetFADC(2048*ch+tb, va);
      hit->SetFADC(tb, va);
      //cout<<"TEST:OK"<<ch<<endl;
      //cout<<nhit<<" "<<id<<" "<<ch<<" "<<tb<<" "<<endl;
      //fFADCParaArray.push_back(para);
    }
  }else{
    return;
  }
  
  fDataLoaded = true;
  return;
}


//__________________________________________________________
void TArtCalibFADC::ClearData()   {
  fFADCArray->Clear();
  fFADCParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibFADC::ReconstructData()  {

  if(!fDataLoaded) LoadData();

  TArtCore::Debug(__FILE__,"ReconstructData");

  /* CHECK LATER 20160328 SaTERA
  for(Int_t i=0;i<GetNumFADC();i++){
    TArtFADC *hit = (TArtFADC*)fFADCArray->At(i);
    TArtFADCPara *para = fFADCParaArray[i];
  }
  */

  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtFADC * TArtCalibFADC::GetFADC(Int_t i) {
  return (TArtFADC *)fFADCArray->At(i);
}
//__________________________________________________________
TArtFADCPara * TArtCalibFADC::GetFADCPara(Int_t i) {
  return (TArtFADCPara *)fFADCParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibFADC::GetNumFADC() {
  return fFADCArray->GetEntries();
}
//__________________________________________________________
TArtFADC * TArtCalibFADC::FindFADC(Int_t id){
  for(Int_t i=GetNumFADC()-1;i>=0;i--) // looking from recent entry
    if(id == ((TArtFADC*)fFADCArray->At(i))->GetID())
      return (TArtFADC*)fFADCArray->At(i);
  return NULL;
}
//__________________________________________________________
TArtFADC * TArtCalibFADC::FindFADC(TString *n){
  for(Int_t i=0;i<GetNumFADC();i++)
    if(*n == *(((TArtFADC*)fFADCArray->At(i))->GetDetectorName()))
      return (TArtFADC*)fFADCArray->At(i);
  TArtCore::Info(__FILE__,"Fail to find TArtFADC: %s", n->Data());
  return NULL;
}
//__________________________________________________________
TArtFADC * TArtCalibFADC::FindFADC(char *n){
  for(Int_t i=0;i<GetNumFADC();i++)
    if(strcmp(((TArtFADC*)fFADCArray->At(i))->GetDetectorName()->Data(),n) == 0)
      return (TArtFADC*)fFADCArray->At(i);
  TArtCore::Info(__FILE__,"Fail to find TArtFADC: %s", n);
  return NULL;
}
