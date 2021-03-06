#include "TArtCalibNEBULA.hh" 
#include "TArtPlastic.hh"
#include "TArtCalibSAMURAITZero.hh"
#include "TArtTZero.hh"
#include "TArtMath.hh"

#include "TMath.h"
#include "TClonesArray.h"
#include "TRandom.h"

#include <cmath>
#include <limits>

using namespace std;

#include "TChain.h"

#include "TCutG.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

#include "TROOT.h"
#include "TRint.h"
#include "TVector3.h"

#include "TMath.h"

#include <iostream>
#include <strstream>
#include <stdlib.h>

#include <TFile.h>

#include <search.h>

//__________________________________________________________
TArtCalibNEBULA::TArtCalibNEBULA()
  : TArtReconstruction("NEBULAPla"), fSAMURAIParameters(0)
{
  TArtCore::Info(__FILE__,"Creating the NEBULA detector objects...");

  fStoreManager = TArtStoreManager::Instance();
  fNEBULAPlaArray = new TArtNEBULAPlaArray("TArtNEBULAPla", 300);
  fNEBULAPlaArray->SetOwner();
  fNEBULAPlaArray->SetName("NEBULAPla");
  fNEBULAPlaParaArray = new TArtNEBULAPlaParaArray("TArtNEBULAPlaPara", 300);
  fNEBULAPlaParaArray->SetOwner();
  fNEBULAPlaParaArray->SetName("NEBULAPlaPara");
  fStoreManager->AddDataContainer(fNEBULAPlaArray);
  fSAMURAIParameters = (const TArtSAMURAIParameters*)fStoreManager->FindParameters("SAMURAIParameters");
  if(!fSAMURAIParameters){
    TArtCore::Info(__FILE__,"Could not find SAMURAIParameters.");
    throw;
  }

  //  fCalibSAMURAIT0 = (TArtCalibSAMURAIT0*)fStoreManager->FindCalibContainer("CalibSAMURAIT0");
  //  if(!fCalibSAMURAIT0){
  //    TArtCore::Info(__FILE__, "t0 will be NaN during this analysis.");
  //  }

  //  fT0Array = (TClonesArray *)fStoreManager->FindDataContainer("SAMURAITZero");
  fT0Array = (TClonesArray *)fStoreManager->FindDataContainer("SAMURAIT0");
  if(!fT0Array) TArtCore::Info(__FILE__, "t0 will be NaN during this analysis.");
}

//__________________________________________________________
TArtCalibNEBULA::~TArtCalibNEBULA()
{
  delete fNEBULAPlaParaArray;
  delete fNEBULAPlaArray;
}

//__________________________________________________________
void TArtCalibNEBULA::LoadData()
{
  TArtRawEventObject* fEvent = (TArtRawEventObject*)fStoreManager->FindDataContainer("RawEvent");

  for(Int_t i=0; i<fEvent->GetNumSeg(); ++i){
    TArtRawSegmentObject* seg = fEvent->GetSegment(i);
    LoadData(seg);
  }
  fDataLoaded = true;
}

void TArtCalibNEBULA::LoadData(TArtRawSegmentObject* seg)
{
  Int_t device = seg->GetDevice();
  Int_t fpl = seg->GetFP();
  Int_t detector = seg->GetDetector();
  if(device != SAMURAI) return;
  if(detector != NEBULA1Q &&
     detector != NEBULA1T &&
     detector != NEBULA2Q &&
     detector != NEBULA2T &&
     detector != NEBULA3Q &&
     detector != NEBULA3T &&
     detector != NEBULA4Q &&
     detector != NEBULA4T){
     return; // analysis only tdc and qdc
  }
  
  for(Int_t j=0; j<seg->GetNumData(); ++j){
    TArtRawDataObject* d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    const TArtNEBULAPlaPara* parau = fSAMURAIParameters->FindNEBULAPlaUPara(mm);
    const TArtNEBULAPlaPara* parad = fSAMURAIParameters->FindNEBULAPlaDPara(mm);
    const TArtNEBULAPlaPara* para;
    if(parau && !parad){
      para = parau;
    }else if(!parau && parad){
      para = parad;
    }else if(parau && parad){
      TArtCore::Info(__FILE__,"TArtNEBULAPara...: Dev:%d, %s found in U and D",
		     device, mm.GetMapInfo());
      return;
    }else{
      TArtCore::Info(__FILE__,"Could not find TArtNEBULAPlaPara...: Dev:%d, %s",
		     device, mm.GetMapInfo());
      return;
    }
    if(para->GetIgnore()){
      TArtCore::Debug(__FILE__,"Ignore TArtNEBULAPlaPara...: Dev:%d, %s", device, mm.GetMapInfo());
      return;
    }

    if(!FindNEBULAPlaPara(para->GetID())){
      Int_t npara = fNEBULAPlaParaArray->GetEntries();
      new ((*fNEBULAPlaParaArray)[npara]) TArtNEBULAPlaPara(*para);

      fIDNPlaParaMap.insert(std::pair<int, int>(para->GetID(), npara));
    }
    TArtNEBULAPla* pla = FindNEBULAPla(para->GetID());
    if(!pla){
      Int_t npla = fNEBULAPlaArray->GetEntries();
      pla = new ((*fNEBULAPlaArray)[npla]) TArtNEBULAPla();
      pla->SetID(para->GetID());
      pla->SetFpl(para->GetFpl());
      pla->SetDetectorName(para->GetDetectorName());
      //pla->SetDetectorName(*(para->GetDetectorName()));
      pla->SetLayer(para->GetLayer());
      pla->SetSubLayer(para->GetSubLayer());
      pla->SetDetPos(para->GetDetPos());

      fIDNPlaMap.insert(std::pair<int, int>(para->GetID(), npla));
    }

    if(detector == NEBULA1T ||
       detector == NEBULA2T ||
       detector == NEBULA3T ||
       detector == NEBULA4T){
      if(parau){
	pla->SetTURaw(val);
      }else{
	pla->SetTDRaw(val);
      }
    }
    if(detector == NEBULA1Q ||
       detector == NEBULA2Q ||
       detector == NEBULA3Q ||
       detector == NEBULA4Q){
      if(parau){
	pla->SetQURaw(val);
      }else{
	pla->SetQDRaw(val);
      }
    }
  }
}

//__________________________________________________________
void TArtCalibNEBULA::ReconstructData()
{
  if(!fDataLoaded) LoadData();

  //共通のoffset。どう値を取ってくるかは後で考える(とりあえず手打ち？)。
  Double_t posxoff = 0;
  Double_t posyoff = 0;
  Double_t poszoff = 0;

  for(Int_t i=0;i<GetNumNEBULAPla();++i){
    TArtNEBULAPla* pla = GetNEBULAPla(i);
    Int_t id = pla->GetID();
    const TArtNEBULAPlaPara* para = FindNEBULAPlaPara(id);
    if(!para){
      TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
      continue;
    }
    Double_t turaw = pla->GetTURaw();
    Double_t tdraw = pla->GetTDRaw();
    Double_t quraw = pla->GetQURaw();
    Double_t qdraw = pla->GetQDRaw();

    if(!(turaw>=0 && turaw<4095)) turaw = TArtMath::InvalidNum();
    if(!(tdraw>=0 && tdraw<4095)) tdraw = TArtMath::InvalidNum();
    if(!(quraw>=0 && quraw<4095)) quraw = TArtMath::InvalidNum();
    if(!(qdraw>=0 && qdraw<4095)) qdraw = TArtMath::InvalidNum();
    
    double t0 = TArtMath::InvalidNum();
    if(fT0Array){
      if(0 == fT0Array->GetEntries()){
	TArtCore::Error(__FILE__,"CalibSAMURAIT0 seems not to be reconstructed.\n Cannot reconstruct CalibSAMURAITZero.");
      }else{
	//	t0 = ((TArtTZero*)fT0Array->At(0))->GetTZeroCal();
	t0 = ((TArtTZero*)fT0Array->At(0))->GetTZeroSlw();
      }
    }

    Int_t hit = 0;
    if(TMath::Finite(quraw)) hit += 1;
    if(TMath::Finite(qdraw)) hit += 2;
    if(TMath::Finite(turaw)) hit += 4;
    if(TMath::Finite(tdraw)) hit += 8;      

    Double_t quped = quraw - para->GetQUPed();
    Double_t qdped = qdraw - para->GetQDPed();
    Double_t qucal = quped * para->GetQUCal();
    Double_t qdcal = qdped * para->GetQDCal();
    Double_t qaveped = sqrt(quped*qdped);
    Double_t qavecal = para->GetQAveCal()*sqrt(qucal*qdcal);
    Double_t logqped = log(quped/qdped);
    Double_t logqcal = log(qucal/qdcal);
    Double_t ivsqrtquped = 1/sqrt(quped);
    Double_t ivsqrtqdped = 1/sqrt(qdped);
    Double_t ivsqrtqaveped = 1/sqrt(qaveped);

    Double_t tucal = turaw * para->GetTUCal() + para->GetTUOff();
    Double_t tdcal = tdraw * para->GetTDCal() + para->GetTDOff();
    //Double_t tuslw = tucal;
    Double_t tuslw = tucal - (para->GetTUSlwB()/sqrt(qavecal) + para->GetTUSlwA());
    //Double_t tdslw = tdcal;
    Double_t tdslw = tdcal - (para->GetTDSlwB()/sqrt(qavecal) + para->GetTDSlwA());
    if(para->GetTUSlwLog(0) != 0 && para->GetTDSlwLog(0) != 0){
      double logquped = log(quped);
      double logqdped = log(qdped);
      double logquped2 = logquped*logquped;
      double logqdped2 = logqdped*logqdped;
      double logquped4 = logquped2*logquped2;
      double logqdped4 = logqdped2*logqdped2;
      tuslw -= (para->GetTUSlwLog(0)*logquped +
		para->GetTUSlwLog(1)*logquped2 +
		para->GetTUSlwLog(2)*logquped2*logquped +
		para->GetTUSlwLog(3)*logquped4 +
		para->GetTUSlwLog(4)*logquped4*logquped);
      tdslw -= (para->GetTDSlwLog(0)*logqdped +
		para->GetTDSlwLog(1)*logqdped2 +
		para->GetTDSlwLog(2)*logqdped2*logqdped +
		para->GetTDSlwLog(3)*logqdped4 +
		para->GetTDSlwLog(4)*logqdped4*logqdped);
    }else{
      tuslw -= para->GetTUSlwB()/sqrt(quped);
      tdslw -= para->GetTDSlwB()/sqrt(qdped);
    }
    Double_t dtraw = tdraw - turaw;
    Double_t dtcal = tdcal - tucal;
    Double_t dtslw = tdslw - tuslw;
    Double_t taveraw = (turaw + tdraw)/2.;
    Double_t tavecal = (tucal + tdcal)/2 + para->GetTAveOff();
    Double_t taveslw = (tuslw + tdslw)/2 + para->GetTAveOff(); //original
    Double_t poscal = dtcal * para->GetDTCal() + para->GetDTOff(); //removed to see if source of crash 19/03/21
    Double_t posslw = dtslw * para->GetDTCal() + para->GetDTOff(); //removed to see if source of crash 19/03/21  
    //Double_t y_pos = dtcal * para->GetPosY();
    
    Double_t tucalt0 = tucal - t0;
    Double_t tdcalt0 = tdcal - t0;
    Double_t tuslwt0 = tuslw - t0;
    Double_t tdslwt0 = tdslw - t0;
    Double_t tavecalt0 = tavecal - t0;
    Double_t taveslwt0 = taveslw - t0;

    Double_t pos[3];
    if(para->GetSubLayer() != 0){ // NEUT
      //      pos[0] = para->GetDetPos(0) + posxoff + gRandom->Uniform(-6,6);
      pos[0] = para->GetDetPos(0) + posxoff;
    }else{ // VETO
      //      pos[0] = para->GetDetPos(0) + posxoff + gRandom->Uniform(-16,16);
      pos[0] = para->GetDetPos(0) + posxoff;
    }
    pos[1] = posslw + para->GetDetPos(1) + posyoff; 
    pos[2] = para->GetDetPos(2) + poszoff;

    qavecal /= 1+pos[1]*pos[1]*para->GetQAveCalAtt();

    Double_t flightlength = sqrt(pos[0]*pos[0]+pos[1]*pos[1]+pos[2]*pos[2]);
    Double_t flightangle = sqrt(pos[0]*pos[0]+pos[1]*pos[1])/pos[2];
    Double_t ttofgamma = taveslw - flightlength/29.979; //beta=1
    Double_t ttofneutron = taveslw - flightlength/20.; //とりあえずbeta=2/3. 上流でのbetaを本当は取ってきたい.
    
    pla->SetHit(hit);
    pla->SetQUPed(quped);
    pla->SetQDPed(qdped);
    pla->SetQUCal(qucal);
    pla->SetQDCal(qdcal);
    pla->SetQAvePed(qaveped);
    pla->SetQAveCal(qavecal);
    pla->SetLogQPed(logqped);
    pla->SetLogQCal(logqcal);
    pla->SetIvSqrtQUPed(ivsqrtquped);
    pla->SetIvSqrtQDPed(ivsqrtqdped);
    pla->SetIvSqrtQAvePed(ivsqrtqaveped);

    pla->SetTUCal(tucal);
    pla->SetTDCal(tdcal);
    pla->SetTUSlw(tuslw);
    pla->SetTDSlw(tdslw);
    pla->SetDTRaw(dtraw);
    pla->SetDTCal(dtcal);
    pla->SetDTSlw(dtslw);    
    pla->SetTAveRaw(taveraw);
    pla->SetTAveCal(tavecal);
    pla->SetTAveSlw(taveslw);    

    pla->SetTUCalT0(tucalt0);
    pla->SetTDCalT0(tdcalt0);
    pla->SetTUSlwT0(tuslwt0);
    pla->SetTDSlwT0(tdslwt0);
    pla->SetTAveCalT0(tavecalt0);
    pla->SetTAveSlwT0(taveslwt0);    

    pla->SetTTOFGamma(ttofgamma);
    pla->SetTTOFNeutron(ttofneutron);

    pla->SetYPos(poscal);
    pla->SetPosCal(poscal);
    pla->SetPosSlw(posslw);
    pla->SetPos(pos);
    pla->SetFlightLength(flightlength);
    pla->SetFlightAngle(flightangle);

    pla->SetDataState(1);
  }
  
  fReconstructed = true;
}
//__________________________________________________________
const int NUMBEROFNEBULADETECTORS = 144;
bool DetUsedForDT[NUMBEROFNEBULADETECTORS] = {false};
struct nebula{
  int id;
  int layer;
  int sublayer;
  float qur; //QURaw
  float qdr; //QDRaw
  float tur; //TURaw
  float tdr; //TDRaw
  float tuc; //TUCal
  float tdc; //TDCal
  float tus; //TUSlew
  float tds; //TDSlew
  int h; //hit
  bool ttrue; //time true
  float at; //ave time
  float tt[4]; //test time
  float idwa;
  }
  fNebula[NUMBEROFNEBULADETECTORS];
//__________________________________________________________
int CompareByTimeUp(const void *element1, const void *element2) {
  return((nebula*)element1)->tuc < ((nebula*)element2)->tuc? 1: -1;
}

//__________________________________________________________
void SortDataUp(int count, nebula fNebula[]) {
  qsort(fNebula,count,sizeof(nebula),CompareByTimeUp);
}
//__________________________________________________________
int CompareByTimeDown(const void *element1, const void *element2) {
  return((nebula*)element1)->tdc < ((nebula*)element2)->tdc? 1: -1;
}

//__________________________________________________________
void SortDataDown(int count, nebula fNebula[]) {
  qsort(fNebula,count,sizeof(nebula),CompareByTimeDown);
}
//__________________________________________________________  
void TArtCalibNEBULA::CalMultiplicity(){
  Int_t fMultiplicity_N = 0;
  Int_t fMultiplicity_N1 = 0;
  Int_t fMultiplicity_N2 = 0;
  Int_t fMultiplicity_V1 = 0;
  Int_t fMultiplicity_V2 = 0;
  Int_t fMultiplicity_N11 = 0;
  Int_t fMultiplicity_N12 = 0;
  Int_t fMultiplicity_N21 = 0;
  Int_t fMultiplicity_N22 = 0;

  int fNEBULAFold = 0;
  int fNEBULAFoldTa = 0;
  
  for(int j=0;j<NUMBEROFNEBULADETECTORS;j++) {
    DetUsedForDT[j] = false;
  }

  for(Int_t i=0;i<GetNumNEBULAPla();++i){
    TArtNEBULAPla *pla = (TArtNEBULAPla*)fNEBULAPlaArray->At(i);
    fNebula[i].id          = pla->GetID();
    fNebula[i].layer       = pla->GetLayer();
    fNebula[i].sublayer    = pla->GetSubLayer();
    fNebula[i].qur         = pla->GetQURaw();
    fNebula[i].qdr         = pla->GetQDRaw();
    fNebula[i].tur         = pla->GetTURaw();
    fNebula[i].tdr         = pla->GetTDRaw();
    fNebula[i].tuc         = pla->GetTUCal();
    fNebula[i].tdc         = pla->GetTDCal();
    fNebula[i].tus         = pla->GetTUSlw();
    fNebula[i].tds         = pla->GetTDSlw();
    fNebula[i].h           = pla->GetHit();
    //fNebula[i].at          = pla->GetTAveCal();
    //fNebula[i].at          = pla->GetTAveSlw();
    
    fNEBULAFold++;
    if(fNebula[i].h == 15 && !fNebula[i].sublayer == 0){
      ++fMultiplicity_N;
      //fNEBULAFoldTa++; 
      //cout<<"N ID test: "<<fNebula[i].id<<endl; 
    }
  }

  for(Int_t i=0;i<GetNumNEBULAPla();++i){
    TArtNEBULAPla *pla = (TArtNEBULAPla*)fNEBULAPlaArray->At(i);
    if(fNebula[i].h == 15 && fNebula[i].layer == 1 && !fNebula[i].sublayer == 0){
      ++fMultiplicity_N1;
      fNEBULAFoldTa++;
      //cout<<"N1 ID test: "<<fNebula[i].id<<endl; 
    }
  }
  
  for(Int_t i=0;i<GetNumNEBULAPla();++i){
    TArtNEBULAPla *pla = (TArtNEBULAPla*)fNEBULAPlaArray->At(i);
    if(fNebula[i].h == 15 && fNebula[i].layer == 1 && fNebula[i].sublayer == 1){
      ++fMultiplicity_N11;
      //cout<<"N11 ID test: "<<fNebula[i].id<<endl;
    }
  }
  
  for(Int_t i=0;i<GetNumNEBULAPla();++i){
    TArtNEBULAPla *pla = (TArtNEBULAPla*)fNEBULAPlaArray->At(i);
    if(fNebula[i].h == 15 && fNebula[i].layer == 1 && fNebula[i].sublayer == 2){
      ++fMultiplicity_N12;
      //cout<<"N12 ID test: "<<fNebula[i].id<<endl; 
    }
  }
  
  for(Int_t i=0;i<GetNumNEBULAPla();++i){
    TArtNEBULAPla *pla = (TArtNEBULAPla*)fNEBULAPlaArray->At(i);
    if(fNebula[i].h == 15 && fNebula[i].layer == 1 && fNebula[i].sublayer == 0){
      ++fMultiplicity_V1;
      //cout<<"V1 ID test: "<<fNebula[i].id<<endl; 
    }
  }
  
  for(Int_t i=0;i<GetNumNEBULAPla();++i){
    TArtNEBULAPla *pla = (TArtNEBULAPla*)fNEBULAPlaArray->At(i);
    if(fNebula[i].h == 15 && fNebula[i].layer == 2 && !fNebula[i].sublayer == 0){
      ++fMultiplicity_N2;
      //cout<<"N2 ID test: "<<fNebula[i].id<<endl; 
    }
  }
  
  for(Int_t i=0;i<GetNumNEBULAPla();++i){
    TArtNEBULAPla *pla = (TArtNEBULAPla*)fNEBULAPlaArray->At(i);
    if(fNebula[i].h == 15 && fNebula[i].layer == 2 && fNebula[i].sublayer == 1){
      ++fMultiplicity_N21;
      //cout<<"N21 ID test: "<<fNebula[i].id<<endl; 
    }
  }
  
  for(Int_t i=0;i<GetNumNEBULAPla();++i){
    TArtNEBULAPla *pla = (TArtNEBULAPla*)fNEBULAPlaArray->At(i);
    if(fNebula[i].h == 15 && fNebula[i].layer == 2 && fNebula[i].sublayer == 2){
      ++fMultiplicity_N22;
      //cout<<"N22 ID test: "<<fNebula[i].id<<endl; 
    }
  }
  
  for(Int_t i=0;i<GetNumNEBULAPla();++i){
    TArtNEBULAPla *pla = (TArtNEBULAPla*)fNEBULAPlaArray->At(i);
    if(fNebula[i].h == 15 && fNebula[i].layer == 2 && fNebula[i].sublayer == 0){
      ++fMultiplicity_V2;
      //cout<<"V2 ID test: "<<fNebula[i].id<<endl; 
    }
  }
  
  for(Int_t i=0;i<GetNumNEBULAPla();++i){
    TArtNEBULAPla *pla = (TArtNEBULAPla*)fNEBULAPlaArray->At(i);
    pla->SetMultiplicity_N(fMultiplicity_N);
    pla->SetMultiplicity_N1(fMultiplicity_N1);
    pla->SetMultiplicity_N11(fMultiplicity_N11);
    pla->SetMultiplicity_N12(fMultiplicity_N12);
    pla->SetMultiplicity_V1(fMultiplicity_V1);
    pla->SetMultiplicity_N2(fMultiplicity_N2);
    pla->SetMultiplicity_N21(fMultiplicity_N21);
    pla->SetMultiplicity_N22(fMultiplicity_N22);
    pla->SetMultiplicity_V2(fMultiplicity_V2); 
    //cout<<"Multiplicity: "<<pla->GetMultiplicity_N12()<<endl;
  }

  float test_time_UP_N11[NUMBEROFNEBULADETECTORS][2] = {{0}};
  float test_time_DWN_N11[NUMBEROFNEBULADETECTORS][2] = {{0}};
  float test_time_SLW_UP_N11[NUMBEROFNEBULADETECTORS][2] = {{0}};
  float test_time_SLW_DWN_N11[NUMBEROFNEBULADETECTORS][2] = {{0}};
  float test_time_UP_N12[NUMBEROFNEBULADETECTORS][2] = {{0}};
  float test_time_DWN_N12[NUMBEROFNEBULADETECTORS][2] = {{0}};
  float test_time_UP_N21[NUMBEROFNEBULADETECTORS][2] = {{0}};
  float test_time_DWN_N21[NUMBEROFNEBULADETECTORS][2] = {{0}};
  float test_time_UP_N22[NUMBEROFNEBULADETECTORS][2] = {{0}};
  float test_time_DWN_N22[NUMBEROFNEBULADETECTORS][2] = {{0}};

  for(Int_t i=0;i<fNEBULAFold;++i){
    TArtNEBULAPla* pla = GetNEBULAPla(i);
    if(pla->GetMultiplicity_N11() == 2 && fNebula[i].layer == 1 && fNebula[i].sublayer == 1){
      SortDataUp(fMultiplicity_N11,fNebula);

      if(DetUsedForDT[fNebula[i].id] == true) continue;
      test_time_UP_N11[fMultiplicity_N11][0] = fNebula[i].tuc;
      test_time_SLW_UP_N11[fMultiplicity_N11][0] = fNebula[i].tus;
      cout<<"ID_U_11_1: "<<fNebula[i].id<<"measured UP time: "<<fNebula[i].tuc<<"test UP time: "<<test_time_UP_N11[fMultiplicity_N11][0]<<endl;
      DetUsedForDT[fNebula[i].id]=true;
      for(int j = i+1; j<pla->GetMultiplicity_N11(); j++){
	if(fNebula[j].id == fNebula[i].id+1 || fNebula[j].id == fNebula[i].id-1){
	  test_time_UP_N11[fMultiplicity_N11][0] -= fNebula[j].tuc;
	  test_time_SLW_UP_N11[fMultiplicity_N11][0] -= fNebula[j].tuc;
	  cout<<"ID_U_11_2: "<<fNebula[j].id<<"measured UP time: "<<fNebula[j].tuc<<"test UP time: "<<test_time_UP_N11[fMultiplicity_N11][0]<<endl;
	  DetUsedForDT[fNebula[j].id]=true;
	}
	else{
	  test_time_UP_N11[fMultiplicity_N11][0] = -9999;
	  test_time_SLW_UP_N11[fMultiplicity_N11][0] = -9999;
	}
      }
      pla->SetTestTime_UP_N11(test_time_UP_N11[fMultiplicity_N11][0]);
      pla->SetTestTime_SLW_UP_N11(test_time_SLW_UP_N11[fMultiplicity_N11][0]);
      cout<<"test time up 11: "<<pla->GetTestTime_UP_N11()<<endl;
      
    }
    else{
      pla->SetTestTime_UP_N11(-9999);
      pla->SetTestTime_SLW_UP_N11(-9999);
    }

    for(int j=0;j<NUMBEROFNEBULADETECTORS;j++) {
      DetUsedForDT[j] = false;
    }

    if(pla->GetMultiplicity_N11() == 2 && fNebula[i].layer == 1 && fNebula[i].sublayer == 1){
      SortDataDown(fMultiplicity_N11,fNebula);
      
      if(DetUsedForDT[fNebula[i].id] == true) continue;
      test_time_DWN_N11[fMultiplicity_N11][0] = fNebula[i].tdc;
      test_time_SLW_DWN_N11[fMultiplicity_N11][0] = fNebula[i].tds;
      cout<<"ID_D_11_1: "<<fNebula[i].id<<"measured DWN time: "<<fNebula[i].tdc<<"test DWN time: "<<test_time_DWN_N11[fMultiplicity_N11][0]<<endl;
      DetUsedForDT[fNebula[i].id]=true;
      for(int j = i+1; j<pla->GetMultiplicity_N11(); j++){
	if(fNebula[j].id == fNebula[i].id+1 || fNebula[j].id == fNebula[i].id-1){
	  test_time_DWN_N11[fMultiplicity_N11][0] -= fNebula[j].tdc;
	  test_time_SLW_DWN_N11[fMultiplicity_N11][0] -= fNebula[j].tdc;
	  cout<<"ID_D_11_2: "<<fNebula[j].id<<"measured DWN time: "<<fNebula[j].tdc<<"test DWN time: "<<test_time_DWN_N11[fMultiplicity_N11][0]<<endl;
	  DetUsedForDT[fNebula[j].id]=true;
	}
	else{
	  test_time_DWN_N11[fMultiplicity_N11][0] = -9999;
	  test_time_SLW_DWN_N11[fMultiplicity_N11][0] = -9999;
	}
      }
      pla->SetTestTime_DWN_N11(test_time_DWN_N11[fMultiplicity_N11][0]);
      pla->SetTestTime_SLW_DWN_N11(test_time_SLW_DWN_N11[fMultiplicity_N11][0]);
      cout<<"test time dwn 11: "<<pla->GetTestTime_DWN_N11()<<endl;
    }
    else{
      pla->SetTestTime_DWN_N11(-9999);
      pla->SetTestTime_SLW_DWN_N11(-9999);
    }
  }

  for(Int_t i=0;i<fNEBULAFold;++i){
    TArtNEBULAPla* pla = GetNEBULAPla(i);
    if(pla->GetMultiplicity_N12() == 2 && fNebula[i].layer == 1 && fNebula[i].sublayer == 2){
      SortDataUp(fMultiplicity_N12,fNebula);

      if(DetUsedForDT[fNebula[i].id] == true) continue;
      test_time_UP_N12[fMultiplicity_N12][0] = fNebula[i].tuc;
      cout<<"ID_U_12_1: "<<fNebula[i].id<<"measured UP time: "<<fNebula[i].tuc<<"test UP time: "<<test_time_UP_N12[fMultiplicity_N12][0]<<endl;
      DetUsedForDT[fNebula[i].id]=true;
      for(int j = i+1; j<pla->GetMultiplicity_N12(); j++){
	if(fNebula[j].id == fNebula[i].id+1 || fNebula[j].id == fNebula[i].id-1){
	  test_time_UP_N12[fMultiplicity_N12][0] -= fNebula[j].tuc;
	  cout<<"ID_U_12_2: "<<fNebula[j].id<<"measured UP time: "<<fNebula[j].tuc<<"test UP time: "<<test_time_UP_N12[fMultiplicity_N12][0]<<endl;
	  DetUsedForDT[fNebula[j].id]=true;
	}
	else{
	  test_time_UP_N12[fMultiplicity_N12][0] = -9999;
	}
      }
      pla->SetTestTime_UP_N12(test_time_UP_N12[fMultiplicity_N12][0]);
      cout<<"test time up 12: "<<pla->GetTestTime_UP_N12()<<endl;

    }
    else{
      pla->SetTestTime_UP_N12(-9999);
    }

    for(int j=0;j<NUMBEROFNEBULADETECTORS;j++) {
      DetUsedForDT[j] = false;
    }

    if(pla->GetMultiplicity_N12() == 2 && fNebula[i].layer == 1 && fNebula[i].sublayer == 2){
      SortDataDown(fMultiplicity_N12,fNebula);

      if(DetUsedForDT[fNebula[i].id] == true) continue;
      test_time_DWN_N12[fMultiplicity_N12][0] = fNebula[i].tdc;
      cout<<"ID_D_12_1: "<<fNebula[i].id<<"measured DWN time: "<<fNebula[i].tdc<<"test DWN time: "<<test_time_DWN_N12[fMultiplicity_N12][0]<<endl;
      DetUsedForDT[fNebula[i].id]=true;
      for(int j = i+1; j<pla->GetMultiplicity_N12(); j++){
	if(fNebula[j].id == fNebula[i].id+1 || fNebula[j].id == fNebula[i].id-1){
	  test_time_DWN_N12[fMultiplicity_N12][0] -= fNebula[j].tdc;
	  cout<<"ID_D_12_2: "<<fNebula[j].id<<"measured DWN time: "<<fNebula[j].tdc<<"test DWN time: "<<test_time_DWN_N12[fMultiplicity_N12][0]<<endl;
	  DetUsedForDT[fNebula[j].id]=true;
	}
	else{
	  test_time_DWN_N12[fMultiplicity_N12][0] = -9999;
	}
      }
      pla->SetTestTime_DWN_N12(test_time_DWN_N12[fMultiplicity_N12][0]);
      cout<<"test time dwn 12: "<<pla->GetTestTime_DWN_N12()<<endl;
    }
    else{
      pla->SetTestTime_DWN_N12(-9999);
    }
  }
  
  for(Int_t i=0;i<fNEBULAFold;++i){
    TArtNEBULAPla* pla = GetNEBULAPla(i);
    if(pla->GetMultiplicity_N21() == 2 && fNebula[i].layer == 2 && fNebula[i].sublayer == 1){
      SortDataUp(fMultiplicity_N21,fNebula);

      if(DetUsedForDT[fNebula[i].id] == true) continue;
      test_time_UP_N21[fMultiplicity_N21][0] = fNebula[i].tuc;
      cout<<"ID_U_21_1: "<<fNebula[i].id<<"measured UP time: "<<fNebula[i].tuc<<"test UP time: "<<test_time_UP_N21[fMultiplicity_N21][0]<<endl;
      DetUsedForDT[fNebula[i].id]=true;
      for(int j = i+1; j<pla->GetMultiplicity_N21(); j++){
	if(fNebula[j].id == fNebula[i].id+1 || fNebula[j].id == fNebula[i].id-1){
	  test_time_UP_N21[fMultiplicity_N21][0] -= fNebula[j].tuc;
	  cout<<"ID_U_21_2: "<<fNebula[j].id<<"measured UP time: "<<fNebula[j].tuc<<"test UP time: "<<test_time_UP_N21[fMultiplicity_N21][0]<<endl;
	  DetUsedForDT[fNebula[j].id]=true;
	}
	else{
	  test_time_UP_N21[fMultiplicity_N21][0] = -9999;
	}
      }
      pla->SetTestTime_UP_N21(test_time_UP_N21[fMultiplicity_N21][0]);
      cout<<"test time up 21: "<<pla->GetTestTime_UP_N21()<<endl;
      
    }
    else{
      pla->SetTestTime_UP_N21(-9999);
    }

    for(int j=0;j<NUMBEROFNEBULADETECTORS;j++) {
      DetUsedForDT[j] = false;
    }

    if(pla->GetMultiplicity_N21() == 2 && fNebula[i].layer == 2 && fNebula[i].sublayer == 1){
      SortDataDown(fMultiplicity_N21,fNebula);
      
      if(DetUsedForDT[fNebula[i].id] == true) continue;
      test_time_DWN_N21[fMultiplicity_N21][0] = fNebula[i].tdc;
      cout<<"ID_D_21_1: "<<fNebula[i].id<<"measured DWN time: "<<fNebula[i].tdc<<"test DWN time: "<<test_time_DWN_N21[fMultiplicity_N21][0]<<endl;
      DetUsedForDT[fNebula[i].id]=true;
      for(int j = i+1; j<pla->GetMultiplicity_N21(); j++){
	if(fNebula[j].id == fNebula[i].id+1 || fNebula[j].id == fNebula[i].id-1){
	  test_time_DWN_N21[fMultiplicity_N21][0] -= fNebula[j].tdc;
	  cout<<"ID_D_21_2: "<<fNebula[j].id<<"measured DWN time: "<<fNebula[j].tdc<<"test DWN time: "<<test_time_DWN_N21[fMultiplicity_N21][0]<<endl;
	  DetUsedForDT[fNebula[j].id]=true;
	}
	else{
	  test_time_DWN_N21[fMultiplicity_N21][0] = -9999;
	}
      }
      pla->SetTestTime_DWN_N21(test_time_DWN_N21[fMultiplicity_N21][0]);
      cout<<"test time dwn 21: "<<pla->GetTestTime_DWN_N21()<<endl;
    }
    else{
      pla->SetTestTime_DWN_N21(-9999);
    }
  }

  for(Int_t i=0;i<fNEBULAFold;++i){
    TArtNEBULAPla* pla = GetNEBULAPla(i);
    if(pla->GetMultiplicity_N22() == 2 && fNebula[i].layer == 2 && fNebula[i].sublayer == 2){
      SortDataUp(fMultiplicity_N22,fNebula);

      if(DetUsedForDT[fNebula[i].id] == true) continue;
      test_time_UP_N22[fMultiplicity_N22][0] = fNebula[i].tuc;
      cout<<"ID_U_22_1: "<<fNebula[i].id<<"measured UP time: "<<fNebula[i].tuc<<"test UP time: "<<test_time_UP_N22[fMultiplicity_N22][0]<<endl;
      DetUsedForDT[fNebula[i].id]=true;
      for(int j = i+1; j<pla->GetMultiplicity_N22(); j++){
	if(fNebula[j].id == fNebula[i].id+1 || fNebula[j].id == fNebula[i].id-1){
	  test_time_UP_N22[fMultiplicity_N22][0] -= fNebula[j].tuc;
	  cout<<"ID_U_22_2: "<<fNebula[j].id<<"measured UP time: "<<fNebula[j].tuc<<"test UP time: "<<test_time_UP_N22[fMultiplicity_N22][0]<<endl;
	  DetUsedForDT[fNebula[j].id]=true;
	}
	else{
	  test_time_UP_N22[fMultiplicity_N22][0] = -9999;
	}
      }
      pla->SetTestTime_UP_N22(test_time_UP_N22[fMultiplicity_N22][0]);
      cout<<"test time up 22: "<<pla->GetTestTime_UP_N22()<<endl;

    }
    else{
      pla->SetTestTime_UP_N22(-9999);
    }

    for(int j=0;j<NUMBEROFNEBULADETECTORS;j++) {
      DetUsedForDT[j] = false;
    }

    if(pla->GetMultiplicity_N22() == 2 && fNebula[i].layer == 2 && fNebula[i].sublayer == 2){
      SortDataDown(fMultiplicity_N22,fNebula);

      if(DetUsedForDT[fNebula[i].id] == true) continue;
      test_time_DWN_N22[fMultiplicity_N22][0] = fNebula[i].tdc;
      cout<<"ID_D_22_1: "<<fNebula[i].id<<"measured DWN time: "<<fNebula[i].tdc<<"test DWN time: "<<test_time_DWN_N22[fMultiplicity_N22][0]<<endl;
      DetUsedForDT[fNebula[i].id]=true;
      for(int j = i+1; j<pla->GetMultiplicity_N22(); j++){
	if(fNebula[j].id == fNebula[i].id+1 || fNebula[j].id == fNebula[i].id-1){
	  test_time_DWN_N22[fMultiplicity_N22][0] -= fNebula[j].tdc;
	  cout<<"ID_D_22_2: "<<fNebula[j].id<<"measured DWN time: "<<fNebula[j].tdc<<"test DWN time: "<<test_time_DWN_N22[fMultiplicity_N22][0]<<endl;
	  DetUsedForDT[fNebula[j].id]=true;
	}
	else{
	  test_time_DWN_N22[fMultiplicity_N22][0] = -9999;
	}
      }
      pla->SetTestTime_DWN_N22(test_time_DWN_N22[fMultiplicity_N22][0]);
      cout<<"test time dwn 22: "<<pla->GetTestTime_DWN_N22()<<endl;
    }
    else{
      pla->SetTestTime_DWN_N22(-9999);
    }
  }
}

//__________________________________________________________
Int_t TArtCalibNEBULA::GetNumNEBULAPla() const
{
  return fNEBULAPlaArray->GetEntries();
}

//__________________________________________________________
TArtNEBULAPla* TArtCalibNEBULA::GetNEBULAPla(Int_t i) const
{
  return (TArtNEBULAPla*)fNEBULAPlaArray->At(i);
}

//__________________________________________________________
TArtNEBULAPla* TArtCalibNEBULA::FindNEBULAPla(Int_t id) const
{
  std::map<int,int>::const_iterator it = fIDNPlaMap.find(id);
  if(it != fIDNPlaMap.end()) return GetNEBULAPla(it->second);
  else return 0;
}

//__________________________________________________________
TArtNEBULAPlaArray* TArtCalibNEBULA::GetNEBULAPlaArray() const
{
  return fNEBULAPlaArray;
}

//__________________________________________________________
Int_t TArtCalibNEBULA::GetNumNEBULAPlaPara() const
{
  return fNEBULAPlaParaArray->GetEntries();
}

//__________________________________________________________
TArtNEBULAPlaPara* TArtCalibNEBULA::GetNEBULAPlaPara(Int_t i) const
{
  return (TArtNEBULAPlaPara*)fNEBULAPlaParaArray->At(i);
}

//__________________________________________________________
TArtNEBULAPlaPara* TArtCalibNEBULA::FindNEBULAPlaPara(Int_t id) const
{
  std::map<int,int>::const_iterator it = fIDNPlaParaMap.find(id);
  if(it != fIDNPlaParaMap.end()) return GetNEBULAPlaPara(it->second);
  else return 0;
}

//__________________________________________________________
TArtNEBULAPlaParaArray* TArtCalibNEBULA::GetNEBULAPlaParaArray() const
{
  return fNEBULAPlaParaArray;
}

//__________________________________________________________
void TArtCalibNEBULA::ClearData()
{
  fMult.ClearData();
  //  fNeutron.ClearData();

  fNEBULAPlaArray->Delete();
  fNEBULAPlaParaArray->Delete();
  fIDNPlaMap.clear();
  fIDNPlaParaMap.clear();
  fDataLoaded = false;
  fReconstructed = false;
}

// //__________________________________________________________
// void TArtCalibNEBULANeutron::ClearData()
// {
//   fQ = -10000;
//   fT = -10000;
//   fP[0] = -10000;
//   fP[1] = -10000;
//   fP[2] = -10000;
//   fPabs = -10000;
//   fE = -10000;
//   fTheta = -10000;
//   fPhi = -10000;
// }

//__________________________________________________________
void TArtCalibNEBULAMult::ClearData()
{
  fMultiplicity_N = 0;
  fMultiplicity_N1 = 0;
  fMultiplicity_N2 = 0;
  fMultiplicity_V1 = 0;
  fMultiplicity_V2 = 0;
  fMultiplicity_N11 = 0;
  fMultiplicity_N12 = 0;
  fMultiplicity_N21 = 0;
  fMultiplicity_N22 = 0;
}

// //__________________________________________________________
// TArtCalibNEBULANeutron::TArtCalibNEBULANeutron()
// {
//   ClearData();
// }
// 
// //__________________________________________________________
// TArtCalibNEBULANeutron::~TArtCalibNEBULANeutron()
// {;}

//__________________________________________________________
TArtCalibNEBULAMult::TArtCalibNEBULAMult()
{
  ClearData();
}

//__________________________________________________________
TArtCalibNEBULAMult::~TArtCalibNEBULAMult()
{;}

//
// ClassImp(TArtCalibNEBULANeutron)
ClassImp(TArtCalibNEBULAMult)
