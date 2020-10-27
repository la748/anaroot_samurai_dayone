
#include "TArtCalibNINJA.hh" 
#include "TArtPlastic.hh"
#include "TArtCalibSAMURAITZero.hh"
#include "TArtTZero.hh"
#include "TArtMath.hh"

#include "TMath.h"
#include "TClonesArray.h"
#include "TRandom.h"

#include <cmath>
#include <limits>
#include <bitset>
//__________________________________________________________
TArtCalibNINJA::TArtCalibNINJA()
  : TArtReconstruction("NINJAPla"), fSAMURAIParameters(0), fNLayers(2)
{
  fMult.resize(fNLayers,0);

  TArtCore::Info(__FILE__,"Creating the NINJA detector objects...");

  fStoreManager = TArtStoreManager::Instance();
  fNINJAPlaArray = new TArtNINJAPlaArray("TArtNINJAPla", 300);
  fNINJAPlaArray->SetOwner();
  fNINJAPlaArray->SetName("NINJAPla");
  fNINJAPlaParaArray = new TArtNINJAPlaParaArray("TArtNINJAPlaPara", 300);
  fNINJAPlaParaArray->SetOwner();
  fNINJAPlaParaArray->SetName("NINJAPlaPara");
  fStoreManager->AddDataContainer(fNINJAPlaArray);
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
TArtCalibNINJA::~TArtCalibNINJA()
{
  delete fNINJAPlaParaArray;
  delete fNINJAPlaArray;
}

//__________________________________________________________
void TArtCalibNINJA::LoadData()
{
  TArtRawEventObject* fEvent = (TArtRawEventObject*)fStoreManager->FindDataContainer("RawEvent");

  for(Int_t i=0; i<fEvent->GetNumSeg(); ++i){
    TArtRawSegmentObject* seg = fEvent->GetSegment(i);
    LoadData(seg);
  }
  fDataLoaded = true;
}

void TArtCalibNINJA::LoadData(TArtRawSegmentObject* seg)
{
  Int_t device = seg->GetDevice();
  Int_t fpl = seg->GetFP();
  Int_t detector = seg->GetDetector();

  if(device != SAMURAI) return;
  if(detector != NINJAT ) return; // 

  for(Int_t j=0; j<seg->GetNumData(); ++j){
    TArtRawDataObject* d = seg->GetData(j);
    Int_t geo = d->GetGeo(); 
    Int_t ch = d->GetCh(); 
    Int_t edge = d->GetEdge(); 
    Int_t val = (Int_t)d->GetVal();

    TArtRIDFMap mm(fpl,detector,geo,ch);
    const TArtNINJAPlaPara* para0 = fSAMURAIParameters->FindNINJAPla0Para(mm);
    const TArtNINJAPlaPara* para1 = fSAMURAIParameters->FindNINJAPla1Para(mm);
    const TArtNINJAPlaPara* para;
    unsigned int index = 1000;
    if(para0 && !para1){
      para = para0;
      index = 0;
    }else if(!para0 && para1){
      para = para1;
      index = 1 ;
    }else if(para0 && para1){
      TArtCore::Info(__FILE__,"TArtNINJAPara...: Dev:%d, %s found in U and D",
          device, mm.GetMapInfo());
      return;
    }else{
      TArtCore::Info(__FILE__,"Could not find TArtNINJAPlaPara...: Dev:%d, %s",
          device, mm.GetMapInfo());
      return;
    }
    if(para->fIgnore){
      TArtCore::Debug(__FILE__,"Ignore TArtNINJAPlaPara...: Dev:%d, %s", device, mm.GetMapInfo());
      return;
    }
    if(edge<0){
      TArtCore::Debug(__FILE__,"No edge in TArtNINJAPlaPara...: Dev:%d, %s", device, mm.GetMapInfo());
      return;
    }


    if(!FindNINJAPlaPara(para->fID)){
      Int_t npara = fNINJAPlaParaArray->GetEntries();
      new ((*fNINJAPlaParaArray)[npara]) TArtNINJAPlaPara(*para);

      fIDNPlaParaMap.insert(std::pair<int, int>(para->fID, npara));
    }
    TArtNINJAPla* pla = FindNINJAPla(para->fID);
    if(!pla){
      if(!FindNINJAPlaPara(para->fID)){
        Int_t npara = fNINJAPlaParaArray->GetEntries();
        new ((*fNINJAPlaParaArray)[npara]) TArtNINJAPlaPara(*para);

        fIDNPlaParaMap.insert(std::pair<int, int>(para->fID, npara));
      }
      TArtNINJAPla* pla = FindNINJAPla(para->fID);
      if(!pla){
        Int_t npla = fNINJAPlaArray->GetEntries();
        pla = new ((*fNINJAPlaArray)[npla]) TArtNINJAPla();
        pla->fID=para->fID;
        pla->fFpl=para->fFpl;
        pla->fDetName=para->fDetName;
        pla->fLayer=para->fLayer;
        pla->fSubLayer=para->fSubLayer;
        for(unsigned int pp = 0 ; pp < 3 ; pp++)
          pla->fDetPos[pp]=para->fDetPos[pp];

        fIDNPlaMap.insert(std::pair<int, int>(para->fID, npla));
      }

      if(detector == NINJAT){
        if(edge == 0){
          pla->fT_LERaw[index] = val;
        } else if(edge == 1) {
          pla->fT_TERaw[index] = val;
        } else{
          printf("The Ninja Calibration Pie is doomed!");
          exit(1);
        } 
      }
    }
  }
}
//__________________________________________________________
void TArtCalibNINJA::ReconstructData()
{
  if(!fDataLoaded) LoadData();

  fMultGlobal=0;
  for(Int_t i=0; i<fNLayers; i++)
    fMult[i]=0;
  
  Double_t posoff[3] = {0,0,0};

  for(Int_t i=0;i<GetNumNINJAPla();++i){
    TArtNINJAPla* pla = GetNINJAPla(i);
    Int_t id = pla->fID;
    const TArtNINJAPlaPara* para = FindNINJAPlaPara(id);
    if(!para){
      TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
      continue;
    }
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
    // Create a 4 bit set
    std::bitset<4> hit_pattern(0) ;
    // Each bit correspond to one of the Leading Edge or Trailing Edge
    // Time of either SIPM direction
    if( pla->fT_LERaw[0]>0 ) hit_pattern[0]=1;
    if( pla->fT_TERaw[0]>0 ) hit_pattern[1]=1;
    if( pla->fT_LERaw[1]>0 ) hit_pattern[2]=1;
    if( pla->fT_TERaw[1]>0 ) hit_pattern[3]=1;

    if ( hit_pattern.all() ) {
      fMult[pla->fLayer-1]++;
      fMultGlobal++;
    }
    // Convert the bit set to integer for storage and display
    hit = hit_pattern.to_ulong();
    pla->fHit=hit;

    // Calculate the Calibration Time
    pla->fT_LECal[0] = pla->fT_LERaw[0] * para->fT_LECal[0] + para->fT_LEOffset[0]; 
    pla->fT_LECal[1] = pla->fT_LERaw[1] * para->fT_LECal[1] + para->fT_LEOffset[1]; 
    pla->fT_TECal[0] = pla->fT_TERaw[0] * para->fT_TECal[0] + para->fT_TEOffset[0]; 
    pla->fT_TECal[1] = pla->fT_TERaw[1] * para->fT_TECal[1] + para->fT_TEOffset[1]; 

    // Calculate Time Over Threshold (TOT)
    pla->fTOTCal[0] = pla->fT_TECal[0] - pla->fT_LECal[0];
    pla->fTOTCal[1] = pla->fT_TECal[1] - pla->fT_LECal[1];

    // Calculate Q Cal
    pla->fQCal[0] =  pla->fTOTCal[0] * para->fQCal[0] + para->fQOffset[0];
    pla->fQCal[1] =  pla->fTOTCal[1] * para->fQCal[1] + para->fQOffset[1];

    // Calculate Average value
    pla->fQAveCal = sqrt(pla->fQCal[0] * pla->fQCal[1]);
    pla->fLogQAveCal = log(pla->fQAveCal);

    // Calulate the position
    double linear_pos_le =  pla->fT_LECal[0] - pla->fT_LECal[1]; 
    double linear_pos_te =  pla->fT_TECal[0] - pla->fT_TECal[1]; 
    
    pla->SetDataState(1);
  }

  fReconstructed = true;
}


//__________________________________________________________
Int_t TArtCalibNINJA::GetNumNINJAPla() const
{
  return fNINJAPlaArray->GetEntries();
}

//__________________________________________________________
TArtNINJAPla* TArtCalibNINJA::GetNINJAPla(const Int_t& i) const
{
  return (TArtNINJAPla*)fNINJAPlaArray->At(i);
}

//__________________________________________________________
TArtNINJAPla* TArtCalibNINJA::FindNINJAPla(const Int_t& id) const
{
  std::map<int,int>::const_iterator it = fIDNPlaMap.find(id);
  if(it != fIDNPlaMap.end()) return GetNINJAPla(it->second);
  else return 0;
}

//__________________________________________________________
TArtNINJAPlaArray* TArtCalibNINJA::GetNINJAPlaArray() const
{
  return fNINJAPlaArray;
}

//__________________________________________________________
Int_t TArtCalibNINJA::GetNumNINJAPlaPara() const
{
  return fNINJAPlaParaArray->GetEntries();
}

//__________________________________________________________
TArtNINJAPlaPara* TArtCalibNINJA::GetNINJAPlaPara(const Int_t& i) const
{
  return (TArtNINJAPlaPara*)fNINJAPlaParaArray->At(i);
}

//__________________________________________________________
TArtNINJAPlaPara* TArtCalibNINJA::FindNINJAPlaPara(const Int_t& id) const
{
  std::map<int,int>::const_iterator it = fIDNPlaParaMap.find(id);
  if(it != fIDNPlaParaMap.end()) return GetNINJAPlaPara(it->second);
  else return 0;
}

//__________________________________________________________
TArtNINJAPlaParaArray* TArtCalibNINJA::GetNINJAPlaParaArray() const
{
  return fNINJAPlaParaArray;
}

//__________________________________________________________
void TArtCalibNINJA::ClearData()
{
  //fMult.ClearData();
  //  fNeutron.ClearData();

  fNINJAPlaArray->Delete();
  fNINJAPlaParaArray->Delete();
  fIDNPlaMap.clear();
  fIDNPlaParaMap.clear();
  fDataLoaded = false;
  fReconstructed = false;
}
/*
//__________________________________________________________
void TArtCalibNINJAMult::ClearData()
{

}

// //__________________________________________________________
// 

//__________________________________________________________
TArtCalibNINJAMult::TArtCalibNINJAMult()
{
ClearData();
}

//__________________________________________________________
TArtCalibNINJAMult::~TArtCalibNINJAMult()
{;}

ClassImp(TArtCalibNINJAMult)
*/
