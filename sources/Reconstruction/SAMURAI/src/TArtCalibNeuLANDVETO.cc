
#include "TArtCalibNeuLANDVETO.hh" 
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
TArtCalibNeuLANDVETO::TArtCalibNeuLANDVETO()
  : TArtReconstruction("NeuLANDVETOPla"), fSAMURAIParameters(0)
{
  TArtCore::Info(__FILE__,"Creating the NeuLANDVETO detector objects...");

  fStoreManager = TArtStoreManager::Instance();
  fNeuLANDVETOPlaArray = new TClonesArray("TArtNEBULAPla", 8);
  fNeuLANDVETOPlaArray->SetOwner();
  fNeuLANDVETOPlaArray->SetName("NeuLANDVETOPla");
  fNeuLANDVETOPlaParaArray = new TClonesArray("TArtNEBULAPlaPara", 16);
  fNeuLANDVETOPlaParaArray->SetOwner();
  fNeuLANDVETOPlaParaArray->SetName("NeuLANDVETOPlaPara");
  fStoreManager->AddDataContainer(fNeuLANDVETOPlaArray);
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
TArtCalibNeuLANDVETO::~TArtCalibNeuLANDVETO()
{
  delete fNeuLANDVETOPlaParaArray;
  delete fNeuLANDVETOPlaArray;
}

//__________________________________________________________
void TArtCalibNeuLANDVETO::LoadData()
{
  TArtRawEventObject* fEvent = (TArtRawEventObject*)fStoreManager->FindDataContainer("RawEvent");

  for(Int_t i=0; i<fEvent->GetNumSeg(); ++i){
    TArtRawSegmentObject* seg = fEvent->GetSegment(i);
    LoadData(seg);
  }
  fDataLoaded = true;
}

void TArtCalibNeuLANDVETO::LoadData(TArtRawSegmentObject* seg)
{
  Int_t device = seg->GetDevice();
  Int_t fpl = seg->GetFP();
  Int_t detector = seg->GetDetector();
  if(device != SAMURAI) return;
  if(detector != NEULANDVETOQ &&
     detector != NEULANDVETOT    ) return; // analysis only tdc and qdc

  for(Int_t j=0; j<seg->GetNumData(); ++j){
    TArtRawDataObject* d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t val = (Int_t)d->GetVal();

    TArtRIDFMap mm(fpl,detector,geo,ch);
    const TArtNEBULAPlaPara* parau = fSAMURAIParameters->FindNeuLANDVETOPlaUPara(mm);
    const TArtNEBULAPlaPara* parad = fSAMURAIParameters->FindNeuLANDVETOPlaDPara(mm);

    const TArtNEBULAPlaPara* para;
    if(parau && !parad){
      para = parau;
    }else if(!parau && parad){
      para = parad;
    }else if(parau && parad){
      TArtCore::Warning(__FILE__,"TArtNEBULAPara...: Dev:%d, %s found in U and D",
			device, mm.GetMapInfo());
      continue;
    }else{
      TArtCore::Warning(__FILE__,"Could not find TArtNEBULAPlaPara...: Dev:%d, %s",
		     device, mm.GetMapInfo());
      continue;
    }

    if(para->GetIgnore()){
      TArtCore::Debug(__FILE__,"Ignore TArtNEBULAPlaPara...: Dev:%d, %s", device, mm.GetMapInfo());
      continue;
    }

    if(!FindNeuLANDVETOPlaPara(para->GetID())){
      Int_t npara = fNeuLANDVETOPlaParaArray->GetEntries();
      new ((*fNeuLANDVETOPlaParaArray)[npara]) TArtNEBULAPlaPara(*para);

      fIDNPlaParaMap.insert(std::pair<int, int>(para->GetID(), npara));
    }
    TArtNEBULAPla* pla = FindNeuLANDVETOPla(para->GetID());
    if(!pla){
      Int_t npla = fNeuLANDVETOPlaArray->GetEntries();
      pla = new ((*fNeuLANDVETOPlaArray)[npla]) TArtNEBULAPla();
      pla->SetID(para->GetID());
      pla->SetFpl(para->GetFpl());
      pla->SetDetectorName(para->GetDetectorName());
      //pla->SetDetectorName(*(para->GetDetectorName()));
      pla->SetLayer(para->GetLayer());
      pla->SetSubLayer(para->GetSubLayer());
      pla->SetDetPos(para->GetDetPos());

      fIDNPlaMap.insert(std::pair<int, int>(para->GetID(), npla));
    }
    
    if (9 == pla->GetID()) {
      fNeuLANDVETOPlaMasterStart = pla;
    }

    if(detector == NEULANDVETOT){
      if(parau){
	pla->SetTURaw(val);
      }else{
	pla->SetTDRaw(val);
      }
    }
    if(detector == NEULANDVETOQ){
      if(parau){
	pla->SetQURaw(val);
      }else{
	pla->SetQDRaw(val);
      }
    }
  }//for(Int_t j=0; j<seg->GetNumData(); ++j){
}

//__________________________________________________________
void TArtCalibNeuLANDVETO::ReconstructData()
{
  if(!fDataLoaded) LoadData();

  //共通のoffset。どう値を取ってくるかは後で考える(とりあえず手打ち？)。
  Double_t posxoff = 0;
  Double_t posyoff = 0;
  Double_t poszoff = 0;

//if (NULL == fNeuLANDVETOPlaMasterStart) puts("!!");
//else puts("##");

  for(Int_t i=0;i<GetNumNeuLANDVETOPla();++i){
    TArtNEBULAPla* pla = GetNeuLANDVETOPla(i);
    Int_t id = pla->GetID();
    const TArtNEBULAPlaPara* para = FindNeuLANDVETOPlaPara(id);
    if(!para){
      TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
      continue;
    }

    Double_t time_ms = 0.0;
    if (NULL != fNeuLANDVETOPlaMasterStart) {
      if (9 != id) {
        time_ms = fNeuLANDVETOPlaMasterStart->GetTDRaw();
      }
    } else {
      TArtCore::Info(__FILE__, "No MasterStart=Master start?!");
    }

    Double_t turaw = pla->GetTURaw() - time_ms;
    Double_t tdraw = pla->GetTDRaw() - time_ms;
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
Int_t TArtCalibNeuLANDVETO::GetNumNeuLANDVETOPla() const
{
  return fNeuLANDVETOPlaArray->GetEntries();
}

//__________________________________________________________
TArtNEBULAPla* TArtCalibNeuLANDVETO::GetNeuLANDVETOPla(Int_t i) const
{
  return (TArtNEBULAPla*)fNeuLANDVETOPlaArray->At(i);
}

//__________________________________________________________
TArtNEBULAPla* TArtCalibNeuLANDVETO::FindNeuLANDVETOPla(Int_t id) const
{
  std::map<int,int>::const_iterator it = fIDNPlaMap.find(id);
  if(it != fIDNPlaMap.end()) return GetNeuLANDVETOPla(it->second);
  else return 0;
}

//__________________________________________________________
TClonesArray* TArtCalibNeuLANDVETO::GetNeuLANDVETOPlaArray() const
{
  return fNeuLANDVETOPlaArray;
}

//__________________________________________________________
Int_t TArtCalibNeuLANDVETO::GetNumNeuLANDVETOPlaPara() const
{
  return fNeuLANDVETOPlaParaArray->GetEntries();
}

//__________________________________________________________
TArtNEBULAPlaPara* TArtCalibNeuLANDVETO::GetNeuLANDVETOPlaPara(Int_t i) const
{
  return (TArtNEBULAPlaPara*)fNeuLANDVETOPlaParaArray->At(i);
}

//__________________________________________________________
TArtNEBULAPlaPara* TArtCalibNeuLANDVETO::FindNeuLANDVETOPlaPara(Int_t id) const
{
  std::map<int,int>::const_iterator it = fIDNPlaParaMap.find(id);
  if(it != fIDNPlaParaMap.end()) return GetNeuLANDVETOPlaPara(it->second);
  else return 0;
}

//__________________________________________________________
TClonesArray* TArtCalibNeuLANDVETO::GetNeuLANDVETOPlaParaArray() const
{
  return fNeuLANDVETOPlaParaArray;
}

//__________________________________________________________
void TArtCalibNeuLANDVETO::ClearData()
{
  fNeuLANDVETOPlaArray->Delete();
  fNeuLANDVETOPlaParaArray->Delete();
  fIDNPlaMap.clear();
  fIDNPlaParaMap.clear();
  fDataLoaded = false;
  fReconstructed = false;
  fNeuLANDVETOPlaMasterStart = NULL;
}

//__________________________________________________________
