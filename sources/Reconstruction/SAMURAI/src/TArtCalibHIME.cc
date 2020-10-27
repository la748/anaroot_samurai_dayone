
#include "TArtCalibHIME.hh" 
#include "TArtPlastic.hh"
#include "TArtCalibSAMURAITZero.hh"
#include "TArtTZero.hh"
#include "TArtMath.hh"

#include "TMath.h"
#include "TClonesArray.h"
#include "TRandom.h"

#include <cmath>
#include <limits>

//__________________________________________________________
TArtCalibHIME::TArtCalibHIME()
  : TArtReconstruction("HIMEPla"), fSAMURAIParameters(0)
{
  TArtCore::Info(__FILE__,"Creating the HIME detector objects...");

  fStoreManager = TArtStoreManager::Instance();
  fHIMEPlaArray = new TArtHIMEPlaArray("TArtHIMEPla", 300);
  fHIMEPlaArray->SetOwner();
  fHIMEPlaArray->SetName("HIMEPla");
  fHIMEPlaParaArray = new TArtHIMEPlaParaArray("TArtHIMEPlaPara", 300);
  fHIMEPlaParaArray->SetOwner();
  fHIMEPlaParaArray->SetName("HIMEPlaPara");
  fStoreManager->AddDataContainer(fHIMEPlaArray);
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
TArtCalibHIME::~TArtCalibHIME()
{
  delete fHIMEPlaParaArray;
  delete fHIMEPlaArray;
}

//__________________________________________________________
void TArtCalibHIME::LoadData()
{
  TArtRawEventObject* fEvent = (TArtRawEventObject*)fStoreManager->FindDataContainer("RawEvent");

  for(Int_t i=0; i<fEvent->GetNumSeg(); ++i){
    TArtRawSegmentObject* seg = fEvent->GetSegment(i);
    LoadData(seg);
  }
  fDataLoaded = true;
}

void TArtCalibHIME::LoadData(TArtRawSegmentObject* seg)
{
  Int_t device = seg->GetDevice();
  Int_t fpl = seg->GetFP();
  Int_t detector = seg->GetDetector();
  if(device != SAMURAI) return;
  if(detector != HIMEQ &&
     detector != HIMET ) return; // analysis only tdc and qdc

  for(Int_t j=0; j<seg->GetNumData(); ++j){
    TArtRawDataObject* d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();
    TArtRIDFMap mm(fpl,detector,geo,ch);
    const TArtHIMEPlaPara* parau = fSAMURAIParameters->FindHIMEPlaUPara(mm);
    const TArtHIMEPlaPara* parad = fSAMURAIParameters->FindHIMEPlaDPara(mm);
    const TArtHIMEPlaPara* para;
    if(parau && !parad){
      para = parau;
    }else if(!parau && parad){
      para = parad;
    }else if(parau && parad){
      TArtCore::Info(__FILE__,"TArtHIMEPara...: Dev:%d, %s found in U and D",
		     device, mm.GetMapInfo());
      return;
    }else{
      TArtCore::Info(__FILE__,"Could not find TArtHIMEPlaPara...: Dev:%d, %s",
		     device, mm.GetMapInfo());
      return;
    }
    if(para->GetIgnore()){
      TArtCore::Debug(__FILE__,"Ignore TArtHIMEPlaPara...: Dev:%d, %s", device, mm.GetMapInfo());
      return;
    }

    if(!FindHIMEPlaPara(para->GetID())){
      Int_t npara = fHIMEPlaParaArray->GetEntries();
      new ((*fHIMEPlaParaArray)[npara]) TArtHIMEPlaPara(*para);

      fIDNPlaParaMap.insert(std::pair<int, int>(para->GetID(), npara));
    }
    TArtHIMEPla* pla = FindHIMEPla(para->GetID());
    if(!pla){
      Int_t npla = fHIMEPlaArray->GetEntries();
      pla = new ((*fHIMEPlaArray)[npla]) TArtHIMEPla();
      pla->SetID(para->GetID());
      pla->SetFpl(para->GetFpl());
      pla->SetDetectorName(para->GetDetectorName());
      //pla->SetDetectorName(*(para->GetDetectorName()));
      pla->SetLayer(para->GetLayer());
      pla->SetSubLayer(para->GetSubLayer());
      pla->SetDetPos(para->GetDetPos());

      fIDNPlaMap.insert(std::pair<int, int>(para->GetID(), npla));
    }

    if(detector == HIMET){
      if(parau){
	pla->SetTURaw(val);
      }else{
	pla->SetTDRaw(val);
      }
    }
    if(detector == HIMEQ){
      if(parau){
	pla->SetQURaw(val);
      }else{
	pla->SetQDRaw(val);
      }
    }
  }
}

//__________________________________________________________
void TArtCalibHIME::ReconstructData()
{
  if(!fDataLoaded) LoadData();

  //共通のoffset。どう値を取ってくるかは後で考える(とりあえず手打ち？)。
  Double_t posoff[3] = { 0.};

  for(Int_t i=0;i<GetNumHIMEPla();++i){
    TArtHIMEPla* pla = GetHIMEPla(i);
    Int_t id = pla->GetID();
    const TArtHIMEPlaPara* para = FindHIMEPlaPara(id);
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
    Double_t tuslw = tucal;
    Double_t tdslw = tdcal;
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
      tuslw -= para->GetTUSlw()/sqrt(quped);
      tdslw -= para->GetTDSlw()/sqrt(qdped);
    }
    Double_t dtraw = tdraw - turaw;
    Double_t dtcal = tdcal - tucal;
    Double_t dtslw = tdslw - tuslw;
    Double_t taveraw = (turaw + tdraw)/2.;
    Double_t tavecal = (tucal + tdcal)/2 + para->GetTAveOff();
    Double_t taveslw = (tuslw + tdslw)/2 + para->GetTAveOff();
    Double_t poscal = dtcal * para->GetDTCal() + para->GetDTOff();
    Double_t posslw = dtslw * para->GetDTCal() + para->GetDTOff();
    
    Double_t tucalt0 = tucal - t0;
    Double_t tdcalt0 = tdcal - t0;
    Double_t tuslwt0 = tuslw - t0;
    Double_t tdslwt0 = tdslw - t0;
    Double_t tavecalt0 = tavecal - t0;
    Double_t taveslwt0 = taveslw - t0;

    Double_t pos[3];

      //      pos[0] = para->GetDetPos(0) + posoff[0] + gRandom->Uniform(-6,6);
    if(para->DirectionIs(para->Vert)){
      pos[0] = para->GetDetPos(0) + posoff[0];
      pos[1] = posslw + para->GetDetPos(1) + posoff[1]; 
    } else if(para->DirectionIs(para->Horiz) ){
      pos[0] = posslw + para->GetDetPos(0) + posoff[0]; 
      pos[1] = para->GetDetPos(1) + posoff[1];
    }
    pos[2] = para->GetDetPos(2) + posoff[2];

    qavecal /= 1+pos[1]*pos[1]*para->GetQAveCalAtt();

    Double_t flightlength = sqrt(pos[0]*pos[0]+pos[1]*pos[1]+pos[2]*pos[2]);
    Double_t flightangle = sqrt(pos[0]*pos[0]+pos[1]*pos[1])/pos[2];
    Double_t ttofgamma = taveslw - flightlength/29.979; //beta=1
    Double_t ttofneutron = taveslw - flightlength/20.; //とりあえずbeta=2/3. 上流でのbetaを本当は取ってきたい。

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
void TArtCalibHIME::CalMultiplicity()
{
  for(Int_t i=0;i<GetNumHIMEPla();++i){
    TArtHIMEPla* pla = GetHIMEPla(i);
    Int_t id = pla->GetID();
    const TArtHIMEPlaPara* para = FindHIMEPlaPara(id);
    if(!para){
      TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
      continue;
    }

    if(!(pla->GetTURaw() > 0 &&  pla->GetTURaw() < 4095 &&  
	 pla->GetTDRaw() > 0 &&  pla->GetTDRaw() < 4095)){
      continue; // consider threshold
    }

    if(!para->GetSubLayer() == 0) {
      ++fMult.fMultiplicity_N[0]; // mul. of all NEUT
      ++fMult.fMultiplicity_N[para->GetSubLayer()];
    }
    else if(para->GetSubLayer() == 0)
      ++fMult.fMultiplicity_V;
  }
}

//__________________________________________________________
Int_t TArtCalibHIME::GetNumHIMEPla() const
{
  return fHIMEPlaArray->GetEntries();
}

//__________________________________________________________
TArtHIMEPla* TArtCalibHIME::GetHIMEPla(Int_t i) const
{
  return (TArtHIMEPla*)fHIMEPlaArray->At(i);
}

//__________________________________________________________
TArtHIMEPla* TArtCalibHIME::FindHIMEPla(Int_t id) const
{
  std::map<int,int>::const_iterator it = fIDNPlaMap.find(id);
  if(it != fIDNPlaMap.end()) return GetHIMEPla(it->second);
  else return 0;
}

//__________________________________________________________
TArtHIMEPlaArray* TArtCalibHIME::GetHIMEPlaArray() const
{
  return fHIMEPlaArray;
}

//__________________________________________________________
Int_t TArtCalibHIME::GetNumHIMEPlaPara() const
{
  return fHIMEPlaParaArray->GetEntries();
}

//__________________________________________________________
TArtHIMEPlaPara* TArtCalibHIME::GetHIMEPlaPara(Int_t i) const
{
  return (TArtHIMEPlaPara*)fHIMEPlaParaArray->At(i);
}

//__________________________________________________________
TArtHIMEPlaPara* TArtCalibHIME::FindHIMEPlaPara(Int_t id) const
{
  std::map<int,int>::const_iterator it = fIDNPlaParaMap.find(id);
  if(it != fIDNPlaParaMap.end()) return GetHIMEPlaPara(it->second);
  else return 0;
}

//__________________________________________________________
TArtHIMEPlaParaArray* TArtCalibHIME::GetHIMEPlaParaArray() const
{
  return fHIMEPlaParaArray;
}

//__________________________________________________________
void TArtCalibHIME::ClearData()
{
  fMult.ClearData();
  //  fNeutron.ClearData();

  fHIMEPlaArray->Delete();
  fHIMEPlaParaArray->Delete();
  fIDNPlaMap.clear();
  fIDNPlaParaMap.clear();
  fDataLoaded = false;
  fReconstructed = false;
}

// //__________________________________________________________
// void TArtCalibHIMENeutron::ClearData()
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
void TArtCalibHIMEMult::ClearData()
{
  for (Int_t imult=0;imult<=NSubLayers;imult++)
    fMultiplicity_N[imult] = 0;

  fMultiplicity_V = 0;
}

// //__________________________________________________________
// TArtCalibHIMENeutron::TArtCalibHIMENeutron()
// {
//   ClearData();
// }
// 
// //__________________________________________________________
// TArtCalibHIMENeutron::~TArtCalibHIMENeutron()
// {;}

//__________________________________________________________
TArtCalibHIMEMult::TArtCalibHIMEMult()
{
  ClearData();
}

//__________________________________________________________
TArtCalibHIMEMult::~TArtCalibHIMEMult()
{;}

//
// ClassImp(TArtCalibHIMENeutron)
ClassImp(TArtCalibHIMEMult)
