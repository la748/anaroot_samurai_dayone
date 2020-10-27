#include "TArtCalibDCHit.hh" 
#include "TArtCalibPDCHit.hh" 
#include "TArtDCHit.hh"
#include "TArtDCHitPara.hh"
#include "TArtSAMURAIParameters.hh"
#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "segidlist.hh" 

#include <TROOT.h>

//__________________________________________________________
TArtCalibPDCHit::TArtCalibPDCHit(const TString outdataname) 
  : TArtCalibDCHit(outdataname, PDC)
{
  TArtCore::Info(__FILE__,"Creating the PDC detector hit objects...");
}

//__________________________________________________________
TArtCalibPDCHit::~TArtCalibPDCHit()  {
}

//__________________________________________________________
void TArtCalibPDCHit::LoadData(TArtRawSegmentObject *seg)   {

  Int_t device   = seg->GetDevice();
  Int_t fpl      = seg->GetFP();
  Int_t detector = seg->GetDetector();

  if(detectorid != detector) return;
  for(Int_t j=0;j<seg->GetNumData();j++){
    TArtRawDataObject *d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t edge = d->GetEdge(); // << Time of threshold  

    if(edge>1) continue; 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch); // << XML !! 
    TArtDCHitPara *para = (TArtDCHitPara*)setup->FindPDCHitPara(&mm);
    if(NULL == para){
      TArtCore::Error(__FILE__,"Could not find TArtDCHitPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }
    else {
      TArtCore::Debug(__FILE__,"Find TArtDCHitPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
    }

    Int_t id = para->GetID();
    Int_t nhit = fDCHitArray->GetEntries();
    TArtCore::Debug(__FILE__,"Entries in DCHitArray: %d", nhit);
    TArtDCHit * hit = FindDCHit(id);

    if(NULL==hit){
      new ((*fDCHitArray)[nhit]) TArtDCHit();
      hit = (TArtDCHit *)fDCHitArray->At(nhit);
      hit->SetID(id);
      fDCHitParaArray.push_back(para);
    }

    // set raw data
    if(detectorid == detector){
      if(mm==*((TArtRIDFMap *)para->GetTDCMap()))
	if(edge==0){
	  hit->SetTDC(val);
	} else {
	  hit->SetTrailTDC(val);
	}
    }

  }

  fDataLoaded = true;
  return;
}

