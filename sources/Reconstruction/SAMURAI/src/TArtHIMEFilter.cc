
#include "TArtHIMEFilter.hh" 

#include "TArtCore.hh"
#include "TArtCalibHIME.hh"
#include "TArtHIMEPla.hh"
#include "TArtHIMEPlaPara.hh"

#include "TClonesArray.h"

#include <iostream>
#include <cmath>

//__________________________________________________________
TArtHIMEFilter::TArtHIMEFilter()
{;}

//__________________________________________________________
TArtHIMEFilter::~TArtHIMEFilter()
{;}

//__________________________________________________________
void TArtHIMEFilter::IHitMin(TArtCalibHIME *fCalibHIME, Int_t ihitmin_n, Int_t ihitmin_v)
{
  TArtHIMEPlaArray *fHIMEPlaArray = fCalibHIME->GetHIMEPlaArray();
  for(Int_t i=0; i<fCalibHIME->GetNumHIMEPla(); ++i){
    TArtHIMEPla* pla = fCalibHIME->GetHIMEPla(i);
    Int_t id = pla->GetID();
    const TArtHIMEPlaPara* para = fCalibHIME->FindHIMEPlaPara(id);
    if(!para){
      TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
      continue;
    }

    Int_t turaw = pla->GetTURaw();
    Int_t tdraw = pla->GetTDRaw();
    Int_t quraw = pla->GetQURaw();
    Int_t qdraw = pla->GetQDRaw();

    Int_t hit = 0;
    if(turaw>0 && turaw<=4096) ++hit;
    if(tdraw>0 && tdraw<=4096) ++hit;      
    if(quraw>0 && quraw<=4096) ++hit;
    if(qdraw>0 && qdraw<=4096) ++hit;

    if(para->GetSubLayer() != 0){ // NEUT
      if(hit < ihitmin_n){
	fHIMEPlaArray->RemoveAt(i);
	fHIMEPlaArray->Compress();
	--i;
      }
    }else{ // VETO
      if(hit < ihitmin_v){
	fHIMEPlaArray->RemoveAt(i);
	fHIMEPlaArray->Compress();
	--i;
      }
    }
  }
}

//__________________________________________________________
void TArtHIMEFilter::Threshold(TArtCalibHIME *fCalibHIME)
{
  TArtHIMEPlaArray *fHIMEPlaArray = fCalibHIME->GetHIMEPlaArray();
  for(Int_t i=0; i<fCalibHIME->GetNumHIMEPla(); ++i){
    TArtHIMEPla* pla = fCalibHIME->GetHIMEPla(i);
    Int_t id = pla->GetID();
    const TArtHIMEPlaPara* para = fCalibHIME->FindHIMEPlaPara(id);
    if(!para){
      TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
      continue;
    }
    double qavecal = pla->GetQAveCal();
    //    if(qavecal < para->GetThreshold()){ // get from parameter (not yet)
    if(qavecal < 0){ // get from parameter (not yet)
      fHIMEPlaArray->RemoveAt(i);
      fHIMEPlaArray->Compress();
      --i;
    }
  }
}

//__________________________________________________________
void TArtHIMEFilter::Threshold(TArtCalibHIME *fCalibHIME, Double_t threshold_n, Double_t threshold_v)
{
  TArtHIMEPlaArray *fHIMEPlaArray = fCalibHIME->GetHIMEPlaArray();
  for(Int_t i=0; i<fCalibHIME->GetNumHIMEPla(); ++i){
    TArtHIMEPla* pla = fCalibHIME->GetHIMEPla(i);
    Int_t id = pla->GetID();
    const TArtHIMEPlaPara* para = fCalibHIME->FindHIMEPlaPara(id);
    if(!para){
      TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
      continue;
    }
    double qavecal = pla->GetQAveCal();
    if(para->GetSubLayer() != 0){ // NEUT
      if(threshold_n > 0 && qavecal < threshold_n){
	fHIMEPlaArray->RemoveAt(i);
	fHIMEPlaArray->Compress();
	--i;
      }
    }else{ // VETO
      if(threshold_v > 0 && qavecal < threshold_v){
	//	std::cout << "never" << std::endl;
	fHIMEPlaArray->RemoveAt(i);
	fHIMEPlaArray->Compress();
	--i;
      }
    }
  }
}

//__________________________________________________________
void TArtHIMEFilter::TOF(TArtCalibHIME *fCalibHIME, Double_t min, Double_t max)
{
  TArtHIMEPlaArray *fHIMEPlaArray = fCalibHIME->GetHIMEPlaArray();
  for(Int_t i=0; i<fCalibHIME->GetNumHIMEPla(); ++i){
    TArtHIMEPla* pla = fCalibHIME->GetHIMEPla(i);
    Int_t id = pla->GetID();
    const TArtHIMEPlaPara* para = fCalibHIME->FindHIMEPlaPara(id);
    if(!para){
      TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
      continue;
    }

    if(para->GetSubLayer() != 0){ // only apply to NEUT
      double t = pla->GetTAveSlw();
      if(t < min ||
	 t > max){ 
	fHIMEPlaArray->RemoveAt(i);
	fHIMEPlaArray->Compress();
	--i;
      }
    }
  }
}

//__________________________________________________________
void TArtHIMEFilter::Veto(TArtCalibHIME *fCalibHIME, Int_t VetoNum)
{
  TArtHIMEPlaArray *fHIMEPlaArray = fCalibHIME->GetHIMEPlaArray();

  std::vector<int> IsVeto;
  for(Int_t i=0; i<VetoNum; ++i){
    IsVeto.push_back(0); // false
  }

  for(Int_t i=0; i<fCalibHIME->GetNumHIMEPla(); ++i){
    TArtHIMEPla* pla = fCalibHIME->GetHIMEPla(i);
    Int_t id = pla->GetID();
    const TArtHIMEPlaPara* para = fCalibHIME->FindHIMEPlaPara(id);
    if(!para){
      TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
      continue;
    }

    if(para->GetSubLayer() == 0){ // VETO
      IsVeto[para->GetLayer()-1] = 1; // true

      // delete VETO data
      fHIMEPlaArray->RemoveAt(i);
      fHIMEPlaArray->Compress();
      --i;
    }
  }

  // search Veto hit of lower layer number
  Int_t VetoHitMin = VetoNum;
  for(Int_t i=0; i<VetoNum; ++i){
    if(IsVeto[i] && VetoHitMin > i) VetoHitMin = i;
  }

  if(VetoHitMin == VetoNum){
    return; // no veto hit
  }else if(VetoHitMin == 0){
    fHIMEPlaArray->Delete(); // veto hit at the front row, delete all pla
    return;
  }else{
    for(Int_t i=0; i<fCalibHIME->GetNumHIMEPla(); ++i){
      TArtHIMEPla* pla = fCalibHIME->GetHIMEPla(i);
      Int_t id = pla->GetID();
      const TArtHIMEPlaPara* para = fCalibHIME->FindHIMEPlaPara(id);
      if(!para){
	TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
	continue;
      }

      if(para->GetLayer() > VetoHitMin){
	fHIMEPlaArray->RemoveAt(i);
	fHIMEPlaArray->Compress();
	--i;
      }
    }
  }
}


//__________________________________________________________
void TArtHIMEFilter::HitMinPos(TArtCalibHIME *fCalibHIME)
{
  Int_t layermin = 100000;
  Int_t sublayermin = 100000;
  Double_t timingmin = 100000;

//  //debug
//  std::cout << "a" << std::endl;
//  for(Int_t i=0; i<fCalibHIME->GetNumHIMEPla(); ++i){
//    TArtHIMEPla* pla = fCalibHIME->GetHIMEPla(i);
//    Int_t id = pla->GetID();
//    const TArtHIMEPlaPara* para = fCalibHIME->FindHIMEPlaPara(id);
//    if(!para){
//      TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
//      continue;
//    }
//    if(para->GetSubLayer() == 0) std::cout << "veto" << std::endl;
//    std::cout << pla->GetPos(2) << std::endl;
//    std::cout << pla->GetTAveSlw() << std::endl;
//  }

  TArtHIMEPlaArray *fHIMEPlaArray = fCalibHIME->GetHIMEPlaArray();
  for(Int_t i=0; i<fCalibHIME->GetNumHIMEPla(); ++i){
    TArtHIMEPla* pla = fCalibHIME->GetHIMEPla(i);
    Int_t id = pla->GetID();
    const TArtHIMEPlaPara* para = fCalibHIME->FindHIMEPlaPara(id);
    if(!para){
      TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
      continue;
    }

    bool hitmin = false;
    if(para->GetSubLayer() == 0){ // VETO
      /* nothing */
    }else{ //NEUT
      if(i == 0){
	layermin = para->GetLayer();      
	sublayermin = para->GetSubLayer();
	timingmin = pla->GetTAveSlw();
	continue;
      }

      if(layermin > para->GetLayer()){
	layermin = para->GetLayer();
	hitmin = true;
      }else if(layermin == para->GetLayer()){
	if(sublayermin > para->GetSubLayer()){
	  sublayermin =  para->GetSubLayer();
	  hitmin = true;
	}else if(sublayermin == para->GetSubLayer()){	
	  if(timingmin > pla->GetTAveSlw()){
	    timingmin =  pla->GetTAveSlw();
	    hitmin = true;
	  }
	}
      }	
    }
    if(hitmin){
      fHIMEPlaArray->RemoveAt(i-1);
      fHIMEPlaArray->Compress();
    }else{
      fHIMEPlaArray->RemoveAt(i);
      fHIMEPlaArray->Compress();
    }
    --i;
  }

//  std::cout << "b" << std::endl;
//  for(Int_t i=0; i<fCalibHIME->GetNumHIMEPla(); ++i){
//    TArtHIMEPla* pla = fCalibHIME->GetHIMEPla(i);
//    Int_t id = pla->GetID();
//    const TArtHIMEPlaPara* para = fCalibHIME->FindHIMEPlaPara(id);
//    if(!para){
//      TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
//      continue;
//    }
//    if(para->GetSubLayer() == 0) std::cout << "veto" << std::endl;
//    std::cout << pla->GetPos(2) << std::endl;
//    std::cout << pla->GetTAveSlw() << std::endl;
//  }
}

//__________________________________________________________
void TArtHIMEFilter::HitMinTime(TArtCalibHIME *fCalibHIME)
{
  Int_t layermin = 100000;
  Int_t sublayermin = 100000;
  Double_t timingmin = 100000;

  TArtHIMEPlaArray *fHIMEPlaArray = fCalibHIME->GetHIMEPlaArray();
  for(Int_t i=0; i<fCalibHIME->GetNumHIMEPla(); ++i){
    TArtHIMEPla* pla = fCalibHIME->GetHIMEPla(i);
    Int_t id = pla->GetID();
    const TArtHIMEPlaPara* para = fCalibHIME->FindHIMEPlaPara(id);
    if(!para){
      TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
      continue;
    }

    bool hitmin = false;
    if(para->GetSubLayer() == 0){ // VETO
      /* nothing */
    }else{ //NEUT
      if(i == 0){
	layermin = para->GetLayer();      
	sublayermin = para->GetSubLayer();
	timingmin = pla->GetTAveSlw();
	continue;
      }

      if(timingmin > pla->GetTAveSlw()){
	timingmin =  pla->GetTAveSlw();
	hitmin = true;
      }else if(timingmin == pla->GetTAveSlw()){
	if(layermin > para->GetLayer()){
	  layermin = para->GetLayer();
	  hitmin = true;
	}else if(layermin == para->GetLayer()){
	  if(sublayermin > para->GetSubLayer()){
	    sublayermin =  para->GetSubLayer();
	    hitmin = true;
	  }
	}	
      }
    }
    if(hitmin){
      fHIMEPlaArray->RemoveAt(i-1);
      fHIMEPlaArray->Compress();
    }else{
      fHIMEPlaArray->RemoveAt(i);
      fHIMEPlaArray->Compress();
    }
    --i;
  }
}

//__________________________________________________________
void TArtHIMEFilter::HitMinPos2(TArtCalibHIME *fCalibHIME)
{
  bool init1 = true; 
  //  Int_t layermin1 = 100000;
  Int_t sublayermin1 = 100000;
  Double_t timingmin1 = 100000;

  bool init2 = true; 
  //  Int_t layermin2 = 100000;
  Int_t sublayermin2 = 100000;
  Double_t timingmin2 = 100000;

  TArtHIMEPlaArray *fHIMEPlaArray = fCalibHIME->GetHIMEPlaArray();
  for(Int_t i=0; i<fCalibHIME->GetNumHIMEPla(); ++i){
    TArtHIMEPla* pla = fCalibHIME->GetHIMEPla(i);
    Int_t id = pla->GetID();
    const TArtHIMEPlaPara* para = fCalibHIME->FindHIMEPlaPara(id);
    if(!para){
      TArtCore::Info(__FILE__,"cannot find para %d", id); // programming problem
      continue;
    }

    bool hitmin = false;
    if(para->GetSubLayer() == 0){ // VETO
      /* nothing */
    }else if(para->GetLayer() == 1){ // first
      if(init1){
	//	layermin1 = para->GetLayer();      
	sublayermin1 = para->GetSubLayer();
	timingmin1 = pla->GetTAveSlw();
	init1 =false;
	continue;
      }
      if(sublayermin1 > para->GetSubLayer()){
	sublayermin1 =  para->GetSubLayer();
	hitmin = true;
      }else if(sublayermin1 == para->GetSubLayer()){	
	if(timingmin1 > pla->GetTAveSlw()){
	  timingmin1 =  pla->GetTAveSlw();
	  hitmin = true;
	}
      }
    }else if(para->GetLayer() == 2){ // second
      if(init2){
	//	layermin2 = para->GetLayer();      
	sublayermin2 = para->GetSubLayer();
	timingmin2 = pla->GetTAveSlw();
	init2 =false;
	continue;
      }
      if(sublayermin2 > para->GetSubLayer()){
	sublayermin2 =  para->GetSubLayer();
	hitmin = true;
      }else if(sublayermin2 == para->GetSubLayer()){	
	if(timingmin2 > pla->GetTAveSlw()){
	  timingmin2 =  pla->GetTAveSlw();
	  hitmin = true;
	}
      }
    }
    if(hitmin){
      fHIMEPlaArray->RemoveAt(i-1);
      fHIMEPlaArray->Compress();
    }else{
      fHIMEPlaArray->RemoveAt(i);
      fHIMEPlaArray->Compress();
    }
    --i;
  }
}
