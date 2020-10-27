
#include "TAlEncHIMEExample.hh"

#include <iostream>

#include "TArtCore.hh"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibHIME.hh"
#include "TArtHIMEPla.hh"
#include "TArtHIMEPlaPara.hh"
#include "TArtHIMEFilter.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

int TAlEncHIMEExample::fFilterFlag = 0x0000;

TAlEncHIMEExample::TAlEncHIMEExample()
  : TAlEncSub(), samuraiparameters(0), calibHIME(0)
{;}

TAlEncHIMEExample::~TAlEncHIMEExample()
{
  Destruct();
}

void TAlEncHIMEExample::Construct()
{
  fStoreManager = TArtStoreManager::Instance();
  samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter((char*)"db/HIME.xml");
  calibHIME = new TArtCalibHIME;
  //  calibHIME->SetName("CalibHIME");
  //  fStoreManager->AddCalibContainer(calibHIME);

  std::string HIMEfiltermessage= "\n";
  if(fFilterFlag & 0x0001){
    HIMEfiltermessage += " TArtHIMEFilter::IHitMin: on\n";
  }
  if(fFilterFlag & 0x0002){
    HIMEfiltermessage += " TArtHIMEFilter::Threshold: on\n";
  }
  if(fFilterFlag & 0x0020){  
    HIMEfiltermessage += " TArtHIMEFilter::VETO(only front): on\n";
  }
  if(fFilterFlag & 0x0004){
    HIMEfiltermessage += " TArtHIMEFilter::Veto: on\n";
  }
  if(fFilterFlag & 0x0008){  
    HIMEfiltermessage += " TArtHIMEFilter::HitMinPos: on\n";
  }
  if(fFilterFlag & 0x0010){  
    HIMEfiltermessage += " TArtHIMEFilter::HitMinPos2: on\n";
  }
  if(fFilterFlag & 0x0040){  
    HIMEfiltermessage += " TArtHIMEFilter::HitMinTime: on\n";
  }
  if(HIMEfiltermessage.size() > 1){
    std::cout << HIMEfiltermessage << std::endl;
  }
}

void TAlEncHIMEExample::PreCalculate()
{
  calibHIME->ClearData();
}

bool TAlEncHIMEExample::Calculate()
{
  calibHIME->ReconstructData();

  // 1n: flag=0x0063
  // 2n: flag=0x0017

  if(fFilterFlag & 0x0001){
    TArtHIMEFilter::IHitMin(calibHIME, 4, 1); // neut>=4hit, veto>=1hit
  }
  if(fFilterFlag & 0x0002){
    //    TArtHIMEFilter::Threshold(calibHIME, 6, -1);
    TArtHIMEFilter::Threshold(calibHIME, 10, -1);
  }
  if(fFilterFlag & 0x0020){  
    TArtHIMEFilter::Veto(calibHIME, 1); // only front
  }
  if(fFilterFlag & 0x0004){  
    TArtHIMEFilter::Veto(calibHIME, 2); // two layer
  }
  if(fFilterFlag & 0x0008){  
    TArtHIMEFilter::HitMinPos(calibHIME); // get front hit 
  }
  if(fFilterFlag & 0x0010){  
    TArtHIMEFilter::HitMinPos2(calibHIME); // get front hit in each layer
  }
  if(fFilterFlag & 0x0040){  
    TArtHIMEFilter::HitMinTime(calibHIME); // get front hit in each layer
  }

  calibHIME->CalMultiplicity();  

  Fill();

  return true;
}

void TAlEncHIMEExample::Destruct()
{
  delete calibHIME;
  calibHIME = 0;
  if(samuraiparameters) samuraiparameters->Delete();
  samuraiparameters = 0;
}

void TAlEncHIMEExample::Fill()
{
  using namespace Analyser;
  using namespace WNum::HIME;  

  Int_t NumHIMEPla = calibHIME->GetNumHIMEPla();
  for(int i=0; i<NumHIMEPla; ++i){
    TArtHIMEPla* pla = calibHIME->GetHIMEPla(i);
    if(pla->GetDataState() < 0) continue;

    Int_t id    = pla->GetID();

    if(!fAnaLoop->UsingAnaFile()){
      /*nothing*/;
    }else{
      fAnaLoop->Add(HIME, ID,             id, id);
      fAnaLoop->Add(HIME, Layer,          id, pla->GetLayer());
      fAnaLoop->Add(HIME, SubLayer,       id, pla->GetSubLayer());
      fAnaLoop->Add(HIME, DetPosX,        id, pla->GetDetPos(0));
      fAnaLoop->Add(HIME, DetPosY,        id, pla->GetDetPos(1));
      fAnaLoop->Add(HIME, DetPosZ,        id, pla->GetDetPos(2));
      fAnaLoop->Add(HIME, Hit,            id, pla->GetHit());
      fAnaLoop->Add(HIME, DetDirect,      id, pla->DirectionIs(pla->Horiz));
					  
      fAnaLoop->Add(HIME, Q0Raw,          id, pla->GetQRaw(0));
      fAnaLoop->Add(HIME, Q1Raw,          id, pla->GetQRaw(1));
      fAnaLoop->Add(HIME, Q0Ped,          id, pla->GetQPed(0));
      fAnaLoop->Add(HIME, Q1Ped,          id, pla->GetQPed(1));
      fAnaLoop->Add(HIME, Q0Cal,          id, pla->GetQCal(0));
      fAnaLoop->Add(HIME, Q1Cal,          id, pla->GetQCal(1));
      fAnaLoop->Add(HIME, QAvePed,        id, pla->GetQAvePed());
      fAnaLoop->Add(HIME, QAveCal,        id, pla->GetQAveCal());
      fAnaLoop->Add(HIME, LogQPed,        id, pla->GetLogQPed());
      fAnaLoop->Add(HIME, LogQCal,        id, pla->GetLogQCal());
      fAnaLoop->Add(HIME, IvSqrtQ0Ped,    id, pla->GetIvSqrtQPed(0));
      fAnaLoop->Add(HIME, IvSqrtQ1Ped,    id, pla->GetIvSqrtQPed(1));
      fAnaLoop->Add(HIME, IvSqrtQAvePed,  id, pla->GetIvSqrtQAvePed());
					  
      fAnaLoop->Add(HIME, T0Raw,          id, pla->GetTRaw(0));
      fAnaLoop->Add(HIME, T1Raw,          id, pla->GetTRaw(1));
      fAnaLoop->Add(HIME, T0Cal,          id, pla->GetTCal(0));
      fAnaLoop->Add(HIME, T1Cal,          id, pla->GetTCal(1));
      fAnaLoop->Add(HIME, T0Slw,          id, pla->GetTSlw(0));
      fAnaLoop->Add(HIME, T1Slw,          id, pla->GetTSlw(1));
      fAnaLoop->Add(HIME, DTRaw,          id, pla->GetDTRaw());
      fAnaLoop->Add(HIME, DTCal,          id, pla->GetDTCal());
      fAnaLoop->Add(HIME, DTSlw,          id, pla->GetDTSlw());
      fAnaLoop->Add(HIME, TAveRaw,        id, pla->GetTAveRaw());
      fAnaLoop->Add(HIME, TAveCal,        id, pla->GetTAveCal());
      fAnaLoop->Add(HIME, TAveSlw,        id, pla->GetTAveSlw());
      fAnaLoop->Add(HIME, T0CalT0,        id, pla->GetTCalT0(0));
      fAnaLoop->Add(HIME, T1CalT0,        id, pla->GetTCalT0(1));
      fAnaLoop->Add(HIME, T0SlwT0,        id, pla->GetTSlwT0(0));
      fAnaLoop->Add(HIME, T1SlwT0,        id, pla->GetTSlwT0(1));
      fAnaLoop->Add(HIME, TAveCalT0,      id, pla->GetTAveCalT0());
      fAnaLoop->Add(HIME, TAveSlwT0,      id, pla->GetTAveSlwT0());
      fAnaLoop->Add(HIME, TTOFGamma,      id, pla->GetTTOFGamma());
      fAnaLoop->Add(HIME, TTOFNeutron,    id, pla->GetTTOFNeutron());
					  
      fAnaLoop->Add(HIME, PosCal,         id, pla->GetPosCal());
      fAnaLoop->Add(HIME, PosSlw,         id, pla->GetPosSlw());
      fAnaLoop->Add(HIME, PosX,           id, pla->GetPos(0));
      fAnaLoop->Add(HIME, PosY,           id, pla->GetPos(1));
      fAnaLoop->Add(HIME, PosZ,           id, pla->GetPos(2));
      fAnaLoop->Add(HIME, FlightLength,   id, pla->GetFlightLength());
    }
  }
  fAnaLoop->Add(HIME, Multiplicity_N,  0, calibHIME->GetMultiplicity_N());
  fAnaLoop->Add(HIME, Multiplicity_N1, 0, calibHIME->GetMultiplicity_N1());
  fAnaLoop->Add(HIME, Multiplicity_N2, 0, calibHIME->GetMultiplicity_N2());
  fAnaLoop->Add(HIME, Multiplicity_N3, 0, calibHIME->GetMultiplicity_N3());
  fAnaLoop->Add(HIME, Multiplicity_N4, 0, calibHIME->GetMultiplicity_N4());
  fAnaLoop->Add(HIME, Multiplicity_N5, 0, calibHIME->GetMultiplicity_N5());
  fAnaLoop->Add(HIME, Multiplicity_V,  0, calibHIME->GetMultiplicity_V());
}

void TAlEncHIMEExample::SetFilterFlag(int filterflag)
{
  fFilterFlag = filterflag;
}

const char* TAlEncHIMEExample::ClassName() const
{
  return "TAlEncHIMEExample";
}

TAlEncHIMEExampleFactory::TAlEncHIMEExampleFactory()
 : TAlEncFactory()
{;}

TAlEncHIMEExampleFactory::~TAlEncHIMEExampleFactory()
{;}

TAlEncSub* TAlEncHIMEExampleFactory::CreateEnc()
{
  return new TAlEncHIMEExample;
}

