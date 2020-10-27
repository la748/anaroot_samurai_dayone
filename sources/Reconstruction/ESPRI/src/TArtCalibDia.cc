#include "TArtCalibDia.hh" 
#include "TArtDia.hh"
#include "TArtESPRIParameters.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtReconstruction.hh"
#include "TArtMap.hh"
#include "TArtTDCHit.hh"

#include <TROOT.h>
#include <TMath.h>
#include <TClonesArray.h>

#include "Riostream.h"

//__________________________________________________________
TArtCalibDia::TArtCalibDia()
  : TArtReconstruction("ESPRIDIA")
{
  TArtCore::Info(__FILE__,"Creating the Dia detector objects...");
  sman = TArtStoreManager::Instance();
  setup = (TArtESPRIParameters*)sman->FindParameters("ESPRIParameters");
  if(!setup){
    TArtCore::Info(__FILE__,"Can not find parameter handler: ESPRIParameters");
    throw;
  }

  fDiaArray = new TClonesArray("TArtDia",10);
  fDiaArray->SetName("ESPRIDIA");
  fDiaArray->SetOwner();
  sman->AddDataContainer(fDiaArray);

}

//__________________________________________________________
TArtCalibDia::~TArtCalibDia()  {
  ClearData();

  delete fDiaArray;
}

//__________________________________________________________
void TArtCalibDia::ClearData()   {
  fDiaArray->Clear();
  fDataLoaded = false;
  fReconstructed = false;
  return;
}

//__________________________________________________________
void TArtCalibDia::ReconstructData()   { // call after the raw data are loaded

  TClonesArray *tdctrk =(TClonesArray *)sman->FindDataContainer("ESPRITDC");
  if(!tdctrk) return;
  Int_t nrt_tdc = tdctrk->GetEntriesFast();

  Int_t narray=0;
  for(Int_t j=0;j<nrt_tdc;j++){//Scan for TDC
    TArtTDCHit *hit = (TArtTDCHit*)tdctrk->At(j);
    Int_t id_plane = (Int_t)hit->GetPlaneID();
    Int_t layer = (Int_t)hit->GetLayer();
    Int_t ch  = (Int_t)hit->GetWireID();
    Int_t tdc1  = (Int_t)hit->GetTDC();
    Int_t tdc2  = (Int_t)hit->GetTrailTDC();
    if(id_plane>=97&&tdc1>-1000&&tdc1<15000){
      //if(id_plane>=97){
      //cout<<j<<":"<<id_plane<<":"<<ch<<":"<<layer<<" "<<tdc1<<":"<<tdc2<<endl;
      new ((*fDiaArray)[narray]) TArtDia();
      TArtDia *dia = (TArtDia*)fDiaArray->At(narray);    
      narray++;
      if(layer>10){//QTC
	dia->SetTimeW(tdc1);
	dia->SetWidth(tdc2-tdc1);
      }else if(ch>0){//TDC
	dia->SetTime(tdc1);
      }
      dia->SetLayer(layer);
      dia->SetPlaneID(id_plane);
      dia->SetCh(ch);
    }
  }
  //cout<<"Total Size of Array::"<<fDiaArray->GetEntries()<<endl;
  //cout<<"NextEvent"<<endl<<endl;

  fReconstructed = true;
  return ;
}

//__________________________________________________________
TArtDia * TArtCalibDia::GetDia(Int_t i) {
  return (TArtDia *)fDiaArray->At(i);
}
//__________________________________________________________
Int_t TArtCalibDia::GetNumDia() {
  return fDiaArray->GetEntries();
}
//__________________________________________________________
TArtDia * TArtCalibDia::FindDia(Int_t id){
  for(Int_t i=0;i<GetNumDia();i++)
    if(id == ((TArtDia*)fDiaArray->At(i))->GetID())
      return (TArtDia*)fDiaArray->At(i);
  return NULL;
}
