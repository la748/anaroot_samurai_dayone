#include "TArtCalibPlastic.hh" 
#include "TArtPlastic.hh"
#include "TArtPlasticPara.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtTOF.hh" 
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>
#include <stdio.h>

//__________________________________________________________
TArtCalibPlastic::TArtCalibPlastic(const TString outdataname)
  : TArtReconstruction(outdataname)
{
  TArtCore::Info(__FILE__,"Creating the Plastic detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtBigRIPSParameters*)sman->FindParameters("BigRIPSParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: BigRIPSParameters");
    throw;
  }

  fPlasticArray = new TClonesArray("TArtPlastic",10);
  fPlasticArray->SetName(outdataname);
  fPlasticArray->SetOwner();
  sman->AddDataContainer(fPlasticArray);

  TIter next(setup->GetListOfPlasticPara());
  for(int np=0;TArtPlasticPara *p = (TArtPlasticPara *)next();np++){
    TArtPlastic *pla = (TArtPlastic *) fPlasticArray->ConstructedAt(np);

    pla->SetID(p->GetID());
    pla->SetDetectorName(*(p->GetDetectorName()));
    pla->SetFpl(p->GetFpl());
    pla->SetZoffset(p->GetZoffset());

    fPlasticParaArray.push_back(p);
  }

}

//__________________________________________________________
TArtCalibPlastic::~TArtCalibPlastic()  {
  ClearData();
  fPlasticArray->Delete();
  fPlasticParaArray.clear();   
  delete fPlasticArray;
}

//__________________________________________________________
void TArtCalibPlastic::LoadData()   {

  TArtRawEventObject *fEvent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  for(Int_t i=0;i<fEvent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = fEvent->GetSegment(i);
    Int_t detector = seg->GetDetector();

    if(PLAT == detector || RF == detector || PLAQ == detector || PLA2T == detector || PLA2Q == detector || STOPPLA == detector || SHBLD == detector || PILEUP == detector ) LoadData(seg); // PILEUP is added at 2017/10/30
  }
  return;

}

//__________________________________________________________
void TArtCalibPlastic::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();

  if(!(PLAT == detector || RF == detector || PLAQ == detector || PLA2T == detector || PLA2Q == detector || STOPPLA == detector || SHBLD == detector || PILEUP == detector )) return; // PILEUP is added at 2017/10/30
  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    Int_t edge = (Int_t)d->GetEdge();

    TArtRIDFMap mm(device,fpl,detector,geo,ch);
    TArtPlasticPara *para = (TArtPlasticPara*)setup->FindPlasticPara(&mm);

    if(NULL == para){
      TArtCore::Warning(__FILE__,"Could not find TArtPlasticPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtPlasticPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    TArtPlastic * pla = (TArtPlastic *)fPlasticArray->At(id-1);

    if(id != pla->GetID())
      TArtCore::Error(__FILE__,"Plastic ID is different: %d, %d", id, pla->GetID());

    if(NULL==pla)
      TArtCore::Error(__FILE__,"Could not find Plastic...: ID:%d, Dev:%d, %s", id, device, mm.GetMapInfo());

    // set raw data
    if(PLAQ == detector || PLA2Q == detector || SHBLD == detector){
      if(mm==*((TArtRIDFMap *)para->GetQLMap())){
	if(pla->GetQLRaw()<=0)pla->SetQLRaw(val);
      }
      if(mm==*((TArtRIDFMap *)para->GetQRMap())){
	if(pla->GetQRRaw()<=0)pla->SetQRRaw(val);
      }
    }

    if(PLAT == detector || RF == detector  || PLA2T == detector || STOPPLA == detector || SHBLD == detector || PILEUP == detector){ // PILEUP is added at 2017/10/30
      if(edge != 1){ // leading edge

	// high resolution TDC
	if(mm==*((TArtRIDFMap *)para->GetTLMap())){
	  if(pla->GetTLRaw()<=0 &&
	     val>para->GetTDCUnderflow() && val<para->GetTDCOverflow() ){ 
	    pla->SetTLRaw(val);
	    pla->SetTCalLeft(para->GetTCalLeft());
	  }
	  else{
	    TArtCore::Warning(__FILE__,"set multiple tdc for one left pla data cont.");
	  }
	}
	if(mm==*((TArtRIDFMap *)para->GetTRMap())){
	  if(pla->GetTRRaw()<=0 &&
	     val>para->GetTDCUnderflow() && val<para->GetTDCOverflow() ){ 
	    pla->SetTRRaw(val);
	    pla->SetTCalRight(para->GetTCalRight());
	  }
	  else{
	    TArtCore::Warning(__FILE__,"set multiple tdc for one right pla data cont.");
	  }
	}

	// TDC with QTC measurement
	if(mm==*((TArtRIDFMap *)para->GetQTCLMap())){
	  if(pla->GetQTCLRawStart()<1 && 
	     val>para->GetQTCUnderflow() && val<para->GetQTCOverflow() ){ 
	    pla->SetQTCLRawStart(val);
	  }
	}
	if(mm==*((TArtRIDFMap *)para->GetQTCRMap())){
	  if(pla->GetQTCRRawStart()<1 && 
	     val>para->GetQTCUnderflow() && val<para->GetQTCOverflow() ){ 
	    pla->SetQTCRRawStart(val);
	  }
	}

	// TDC for multihit information, deadtime less is preferable
	if(mm==*((TArtRIDFMap *)para->GetMulHitTDCLMap())){
	  if(val>para->GetMulHitTDCUnderflow() && val<para->GetMulHitTDCOverflow() ){ 
	    pla->AddMulHitTDCL(val);
	  }
	}
	if(mm==*((TArtRIDFMap *)para->GetMulHitTDCRMap())){
	  if(val>para->GetMulHitTDCUnderflow() && val<para->GetMulHitTDCOverflow() ){ 
	    pla->AddMulHitTDCR(val);
	  }
	}

      } // if(edge != 1)
      else if(edge == 1){ // trailing edge
	if(mm==*((TArtRIDFMap *)para->GetQTCLMap())){
	    if(pla->GetQTCLRawWidth()<1 && pla->GetQTCLRawStart()>0 && 
	       val>para->GetQTCUnderflow() && val<para->GetQTCOverflow() ){ 
	      pla->SetQTCLRawWidth(val - pla->GetQTCLRawStart() );
	    }
	  }
	if(mm==*((TArtRIDFMap *)para->GetQTCRMap())){
	    if(pla->GetQTCRRawWidth()<1 && pla->GetQTCRRawStart()>0 &&
	       val>para->GetQTCUnderflow() && val<para->GetQTCOverflow() ){ 
	      pla->SetQTCRRawWidth(val - pla->GetQTCRRawStart() );
	  }
	}
      }
    }

    pla->SetDataState(1);

  }

  fDataLoaded = true;
  return;
}

//__________________________________________________________
void TArtCalibPlastic::ClearData()   {
  for(int i=0;i<fPlasticArray->GetEntries();i++)
    ((TArtPlastic*)fPlasticArray->At(i))->Clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibPlastic::ReconstructData()   { // call after the raw data are loaded

  if(!fDataLoaded) LoadData();

  for(Int_t i=0;i<GetNumPlastic();i++){
    TArtPlastic *pla = (TArtPlastic*)fPlasticArray->At(i);
    if(!pla->IsDataValid()) continue;

    TArtPlasticPara *para = fPlasticParaArray[i];

    Double_t fTimeL = -9999., fTimeR = -9999.;
    Bool_t fLFired = false, fRFired = false, fFired = false;
    Double_t fTLRaw = pla->GetTLRaw();
    Double_t fTRRaw = pla->GetTRRaw();
    Double_t fQLRaw = pla->GetQLRaw();
    Double_t fQRRaw = pla->GetQRRaw();

    Double_t fTime = -9999; 
    Double_t fTimeLSlew = -9999; 
    Double_t fTimeRSlew = -9999; 
    Double_t fTimeSlew = -9999; 

    //    if(fTLRaw>para->GetTDCUnderflow() && fTLRaw<para->GetTDCOverflow()) { // move to LoadData(), Dec. 02 2016 TI 
    if(fTLRaw>0) {
      fLFired = true;
      fTimeL = fTLRaw * para->GetTCalLeft();
      if(fQLRaw>0){
	fTimeLSlew = fTLRaw + para->GetTLSlewA()/(TMath::Sqrt(fQLRaw)) + para->GetTLSlewB();
	fTimeLSlew = fTimeLSlew * para->GetTCalLeft();
      }
      else{
	fTimeLSlew = fTimeL;
      }
    }

    //    if(fTRRaw>para->GetTDCUnderflow() && fTRRaw<para->GetTDCOverflow()) { // move to LoadData(), Dec. 02 2016 TI 
    if(fTRRaw>0) {
      fRFired = true;
      fTimeR = fTRRaw * para->GetTCalRight();
      if(fQRRaw>0){
	fTimeRSlew = fTRRaw + para->GetTRSlewA()/(TMath::Sqrt(fQRRaw)) + para->GetTRSlewB();
	fTimeRSlew = fTimeRSlew * para->GetTCalRight();
      }
      else{
	fTimeRSlew = fTimeR;
      }
    }
    if(fLFired && fRFired) fFired = true;

    if(fFired) {
      fTime = (fTimeL+fTimeR)/2.;
      fTimeSlew = (fTimeLSlew+fTimeRSlew)/2.;
      //TArtCore::Info(__FILE__,"ch %d: fpl: %d, tlraw: %d, trraw: %d, tl: %f, tr: %f, t:%f", i , fFocalPlane, fTLRaw,fTRRaw,(float)fTL,(float)fTR,(float)fTOF);
      //fTDiff = fTL-fTR;
      //fX     = fDT2X[0] * pow(fTDiff,2) + fDT2X[1] * fTDiff + fDT2X[2];
    }

    pla->SetTime(fTime);
    pla->SetTimeL(fTimeL);
    pla->SetTimeR(fTimeR);
    pla->SetTimeLSlew(fTimeLSlew);
    pla->SetTimeRSlew(fTimeRSlew);
    pla->SetTimeSlew(fTimeSlew);

  }

  fReconstructed = true;
  return;
}

//__________________________________________________________
TArtPlastic * TArtCalibPlastic::GetPlastic(Int_t i) {
  return (TArtPlastic *)fPlasticArray->At(i);
}
//__________________________________________________________
TArtPlasticPara * TArtCalibPlastic::GetPlasticPara(Int_t i) {
  return (TArtPlasticPara *)fPlasticParaArray[i];
}
//__________________________________________________________
Int_t TArtCalibPlastic::GetNumPlastic() {
  return fPlasticArray->GetEntries();
}
//__________________________________________________________
TArtPlastic * TArtCalibPlastic::FindPlastic(Int_t id){
  for(Int_t i=0;i<GetNumPlastic();i++)
    if(id == ((TArtPlastic*)fPlasticArray->At(i))->GetID())
      return (TArtPlastic*)fPlasticArray->At(i);
  return NULL;
}
//__________________________________________________________
TArtPlastic * TArtCalibPlastic::FindPlastic(TString *n){
  for(Int_t i=0;i<GetNumPlastic();i++)
    if(*n == *(((TArtPlastic*)fPlasticArray->At(i))->GetDetectorName()))
      return (TArtPlastic*)fPlasticArray->At(i);
  return NULL;
}
//__________________________________________________________
TArtPlastic * TArtCalibPlastic::FindPlastic(char *n){
  for(Int_t i=0;i<GetNumPlastic();i++)
    if(strcmp(((TArtPlastic*)fPlasticArray->At(i))->GetDetectorName()->Data(),n) == 0)
      return (TArtPlastic*)fPlasticArray->At(i);
  return NULL;
}
