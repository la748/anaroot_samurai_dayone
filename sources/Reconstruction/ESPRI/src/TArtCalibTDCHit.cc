#include "TArtCalibTDCHit.hh" 
#include "TArtTDCHit.hh"
#include "TArtTDCHitPara.hh"
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
///TArtCalibTDCHit::TArtCalibTDCHit(const char *outdataname, const Int_t detid) : TArtReconstruction(outdataname), detectorid(detid) 
TArtCalibTDCHit::TArtCalibTDCHit()
  : TArtReconstruction("ESPRITDC")
{
  TArtCore::Info(__FILE__,"Creating the DC Hit calibration objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtESPRIParameters*)sman->FindParameters("ESPRIParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: ESPRIParameters");
    throw;
  }

  fTDCHitArray = new TClonesArray("TArtTDCHit",20);
  ///fTDCHitArray->SetName(outdataname);
  fTDCHitArray->SetName("ESPRITDC");
  fTDCHitArray->SetOwner();
  sman->AddDataContainer(fTDCHitArray);
}

//__________________________________________________________
TArtCalibTDCHit::~TArtCalibTDCHit()  {
  ClearData();

  delete fTDCHitArray;
}

//__________________________________________________________
void TArtCalibTDCHit::LoadData()   {
  TArtCore::Debug(__FILE__,"LoadData");

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t device   = seg->GetDevice();
    Int_t fpl      = seg->GetFP();
    Int_t detector = seg->GetDetector();
    
    //if((device == ESPRI) && (detectorid == detector)){
    //if((3 == detector)){//ESPRI2013
    //if((3 == detector || 2 == detector || 41 == detector)){//ESPRI2013//HIMAC2015
    //if((3 == detector || 2 == detector || 54 == detector || 56 == detector)){//ESPRI2016
    //if((3 == detector || 2 == detector || 54 == detector || 56 == detector || 19 == detector)){//ESPRI2016withESPRI03
    if(fpl==12&&(3 == detector || 2 == detector || 54 == detector || 56 == detector || 19 == detector)||(fpl==0&&(1<=detector<=6||detector==10))){//ESPRI2016withESPRI03withBLD
      LoadData(seg);
      ///Int_t nhit = fTDCHitArray->GetEntries();//iru??
      //TArtCore::Info(__FILE__,"Entries in TDCHitArray: %d, %d", nhit, detector);
    }
  }
  return;

}

void TArtCalibTDCHit::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();
  //Int_t geo, mhit, common = 0, det;
  Int_t mhit, common = 0;
  static Int_t geo, det;
  //if(detectorid != detector) return;
  //if(3 == detector || 2 == detector){//ESPRI2013
  //if(3 == detector || 2 == detector || 41 == detector ){//ESPRI2013//HIMAC2015
  //if((3 == detector || 2 == detector || 54 == detector || 56 == detector)){//ESPRI2016
  //if(fpl==12&&(3 == detector || 2 == detector || 54 == detector || 56 == detector || 19 == detector))){//ESPRI2016withESPRI03
  if(fpl==12&&(3 == detector || 2 == detector || 54 == detector || 56 == detector || 19 == detector)||(fpl==0&&(1<=detector<=6||detector==10))){//ESPRI2016withESPRI03withBLD
    for(Int_t j=0;j<seg->GetNumData();j++){
      TArtRawDataObject *d = seg->GetData(j);
      //if(geo!=d->GetGeo()){
      if((fpl==12&&geo!=d->GetGeo())||(fpl==0&&det!=seg->GetDetector())){
	common = 0;
	mhit = fTDCHitArray->GetEntries();
      }
      geo = d->GetGeo(); 
      det = seg->GetDetector(); 
      Int_t ch = d->GetCh(); 
      Int_t edge = d->GetEdge(); 
      Int_t val = (Int_t)d->GetVal();
      TArtRIDFMap mm(fpl,detector,geo,ch);
      TArtTDCHitPara *para = (TArtTDCHitPara*)setup->FindTDCHitPara(&mm);
      if(NULL == para){
	TArtCore::Debug(__FILE__,"Could not find TArtDCHitPara...: Dev:%d, %s", device, mm.GetMapInfo());
	//TArtCore::Info(__FILE__,"Could not find TArtDCHitPara...: Dev:%d, %s", device, mm.GetMapInfo());
	continue;
      }
      else {
	TArtCore::Debug(__FILE__,"Find TArtDCHitPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
	//TArtCore::Info(__FILE__,"Find TArtDCHitPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
      }

      Int_t tz = para->GetTzero();
      Int_t id = para->GetID();
      Int_t id_plane = para->GetPlaneID();
      Int_t nhit = fTDCHitArray->GetEntries();
      //TArtCore::Info(__FILE__,"ID: %d, nhit: %d, edge: %d plane:%d tzero:%d common:%d %s", id, nhit, edge, id_plane, tz, common, para->GetDetectorName()->Data());
      val -= tz;
      //cout<<nhit<<":"<<mhit<<":"<<tz<<endl;

      if(id_plane==0 && edge==0){
	common = val;
	//cout<<nhit<<":"<<mhit<<endl;
	if(nhit>mhit){
	  //cout<<"No Common"<<nhit<<":"<<mhit<<endl;
	  for(Int_t i=nhit-1;i>=mhit;i--){
	    TArtTDCHit *hit = (TArtTDCHit *)fTDCHitArray->At(i);
	    hit->SetTDC(hit->GetTDC()-common);
	    hit->SetTrailTDC(hit->GetTrailTDC()-common);
	    //hit->SetTDC(hit->GetTDC());
	    //hit->SetTrailTDC(hit->GetTrailTDC());
	  }
	}
      }
      val -= common;
      if(0 == edge){
	new ((*fTDCHitArray)[nhit]) TArtTDCHit();
	TArtTDCHit *hit = (TArtTDCHit *)fTDCHitArray->At(nhit);
	hit->SetID(id);
	hit->SetTDC(val);
	/*
	if(detector<4)
	  cout << detector << endl;
	*/
	fTDCHitParaArray.push_back(para);
      }
      else if(1 == edge){
	TArtTDCHit * hit = FindTDCHit(id);
	if(hit){
	  if(hit->GetTrailTDC()==-999999){
	    hit->SetTrailTDC(val);
	  }else{
	    //TArtCore::Debug(__FILE__,"trailtdc is already set tdc: %d -> %d",val,hit->GetTrailTDC());
	  }
	}
      }
    }
  }else{
    return;
  }

  fDataLoaded = true;
  return;
}


//__________________________________________________________
void TArtCalibTDCHit::ClearData()   {
  fTDCHitArray->Clear();
  fTDCHitParaArray.clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibTDCHit::ReconstructData()  {

  if(!fDataLoaded) LoadData();

  TArtCore::Debug(__FILE__,"ReconstructData");

  for(Int_t i=0;i<GetNumTDCHit();i++){
    TArtTDCHit *hit = (TArtTDCHit*)fTDCHitArray->At(i);
    TArtTDCHitPara *para = fTDCHitParaArray[i];

    Int_t myTDC = (Double_t)hit->GetTDC();

    // copy some information from para to data container
    hit->SetHitID(i); // to let tracker know which hit is used
    hit->SetID(para->GetID());
    hit->SetDetectorName(*(para->GetDetectorName()));
    hit->SetFpl(para->GetFpl());
    hit->SetWireID(para->GetWireID());
    hit->SetWireAngle(para->GetWireAngle());
    hit->SetWireZPosition(para->GetWireZPosition());
    hit->SetTzero(para->GetTzero());
    hit->SetLayer(para->GetLayer());
    hit->SetPlaneID(para->GetPlaneID());
    //if(para->GetPlaneID()>30)
    //cout<<para->GetLayer()<<":"<<para->GetPlaneID()<<endl;
    /*
    Int_t id_plane=hit->GetPlaneID();
    if(id_plane==0||id_plane>30)
      cout<<"Rec: "<< hit->GetDetectorName()->Data() <<endl;
    */
  }

  //cout<<"Total Size of Array::"<<fTDCHitArray->GetEntries()<<endl;
  //cout<<"NextEvent"<<endl<<endl;

  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtTDCHit * TArtCalibTDCHit::GetTDCHit(Int_t i) {
  return (TArtTDCHit *)fTDCHitArray->At(i);
}
//__________________________________________________________
TArtTDCHitPara * TArtCalibTDCHit::GetTDCHitPara(Int_t i) {
  return (TArtTDCHitPara *)fTDCHitParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibTDCHit::GetNumTDCHit() {
  return fTDCHitArray->GetEntries();
}
//__________________________________________________________
TArtTDCHit * TArtCalibTDCHit::FindTDCHit(Int_t id){
  for(Int_t i=GetNumTDCHit()-1;i>=0;i--) // looking from recent entry
    if(id == ((TArtTDCHit*)fTDCHitArray->At(i))->GetID())
      return (TArtTDCHit*)fTDCHitArray->At(i);
  return NULL;
}
//__________________________________________________________
TArtTDCHit * TArtCalibTDCHit::FindTDCHit(TString *n){
  for(Int_t i=0;i<GetNumTDCHit();i++)
    if(*n == *(((TArtTDCHit*)fTDCHitArray->At(i))->GetDetectorName()))
      return (TArtTDCHit*)fTDCHitArray->At(i);
  TArtCore::Info(__FILE__,"Fail to find TArtTDCHit: %s", n->Data());
  return NULL;
}
//__________________________________________________________
TArtTDCHit * TArtCalibTDCHit::FindTDCHit(char *n){
  for(Int_t i=0;i<GetNumTDCHit();i++)
    if(strcmp(((TArtTDCHit*)fTDCHitArray->At(i))->GetDetectorName()->Data(),n) == 0)
      return (TArtTDCHit*)fTDCHitArray->At(i);
  TArtCore::Info(__FILE__,"Fail to find TArtTDCHit: %s", n);
  return NULL;
}
