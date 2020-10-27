
#include "TAlEncNINJAExample.hh"

#include <iostream>

#include "TArtCore.hh"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibNINJA.hh"
#include "TArtNINJAPla.hh"
#include "TArtNINJAPlaPara.hh"
//#include "TArtNINJAFilter.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

int TAlEncNINJAExample::fFilterFlag = 0x0000;

TAlEncNINJAExample::TAlEncNINJAExample()
  : TAlEncSub(), samuraiparameters(0), calibNINJA(0)
{;}

TAlEncNINJAExample::~TAlEncNINJAExample()
{
  Destruct();
}

void TAlEncNINJAExample::Construct()
{
  fStoreManager = TArtStoreManager::Instance();
  samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter((char*)"db/NINJA.xml");
  calibNINJA = new TArtCalibNINJA;
  //  calibNINJA->SetName("CalibNINJA");
  //  fStoreManager->AddCalibContainer(calibNINJA);

/*  std::string NINJAfiltermessage= "\n";
  if(fFilterFlag & 0x0001){
    NINJAfiltermessage += " TArtNINJAFilter::IHitMin: on\n";
  }
  if(fFilterFlag & 0x0002){
    NINJAfiltermessage += " TArtNINJAFilter::Threshold: on\n";
  }
  if(fFilterFlag & 0x0020){  
    NINJAfiltermessage += " TArtNINJAFilter::VETO(only front): on\n";
  }
  if(fFilterFlag & 0x0004){
    NINJAfiltermessage += " TArtNINJAFilter::Veto: on\n";
  }
  if(fFilterFlag & 0x0008){  
    NINJAfiltermessage += " TArtNINJAFilter::HitMinPos: on\n";
  }
  if(fFilterFlag & 0x0010){  
    NINJAfiltermessage += " TArtNINJAFilter::HitMinPos2: on\n";
  }
  if(fFilterFlag & 0x0040){  
    NINJAfiltermessage += " TArtNINJAFilter::HitMinTime: on\n";
  }
  if(NINJAfiltermessage.size() > 1){
    std::cout << NINJAfiltermessage << std::endl;
  }*/
}

void TAlEncNINJAExample::PreCalculate()
{
  calibNINJA->ClearData();
}

bool TAlEncNINJAExample::Calculate()
{
  calibNINJA->ReconstructData();

  // 1n: flag=0x0063
  // 2n: flag=0x0017
/*
  if(fFilterFlag & 0x0001){
    TArtNINJAFilter::IHitMin(calibNINJA, 4, 1); // neut>=4hit, veto>=1hit
  }
  if(fFilterFlag & 0x0002){
    //    TArtNINJAFilter::Threshold(calibNINJA, 6, -1);
    TArtNINJAFilter::Threshold(calibNINJA, 10, -1);
  }
  if(fFilterFlag & 0x0020){  
    TArtNINJAFilter::Veto(calibNINJA, 1); // only front
  }
  if(fFilterFlag & 0x0004){  
    TArtNINJAFilter::Veto(calibNINJA, 2); // two layer
  }
  if(fFilterFlag & 0x0008){  
    TArtNINJAFilter::HitMinPos(calibNINJA); // get front hit 
  }
  if(fFilterFlag & 0x0010){  
    TArtNINJAFilter::HitMinPos2(calibNINJA); // get front hit in each layer
  }
  if(fFilterFlag & 0x0040){  
    TArtNINJAFilter::HitMinTime(calibNINJA); // get front hit in each layer
  }
*/
//  calibNINJA->CalMultiplicity();  

  Fill();

  return true;
}

void TAlEncNINJAExample::Destruct()
{
  delete calibNINJA;
  calibNINJA = 0;
  if(samuraiparameters) samuraiparameters->Delete();
  samuraiparameters = 0;
}

void TAlEncNINJAExample::Fill()
{
  using namespace Analyser;
  using namespace WNum::NINJA;  

  // Multiplicities
  fAnaLoop->Add(NINJA, Mult,  0, calibNINJA->GetMultGlobal() );
  fAnaLoop->Add(NINJA, Mult1, 0, calibNINJA->GetMult(0) );
  fAnaLoop->Add(NINJA, Mult2, 0, calibNINJA->GetMult(1) );

  Int_t NumNINJAPla = calibNINJA->GetNumNINJAPla();
  for(int i=0; i<NumNINJAPla; ++i){
    TArtNINJAPla* pla = calibNINJA->GetNINJAPla(i);
    if(pla->GetDataState() < 0) continue;

    Int_t id    = pla->fID;
    if(id<0){
       printf("You're doomed \n");
       exit(1);
    }
    if(!fAnaLoop->UsingAnaFile()){
      /*nothing*/;
    }else{
      fAnaLoop->Add(NINJA, ID,         id, id);
      fAnaLoop->Add(NINJA, Layer,      id, pla->fLayer);
      fAnaLoop->Add(NINJA, SubLayer,   id, pla->fSubLayer);
      fAnaLoop->Add(NINJA, DetPosX,    id, pla->fDetPos[0]);
      fAnaLoop->Add(NINJA, DetPosY,    id, pla->fDetPos[1]);
      fAnaLoop->Add(NINJA, DetPosZ,    id, pla->fDetPos[2]);
      fAnaLoop->Add(NINJA, Hit,        id, pla->fHit);
      fAnaLoop->Add(NINJA, DetDirect,  id, pla->fDetDirect);
      // Raw Time
      fAnaLoop->Add(NINJA, T0_LERaw,   id, pla->fT_LERaw[0]);
      fAnaLoop->Add(NINJA, T1_LERaw,   id, pla->fT_LERaw[1]);
      fAnaLoop->Add(NINJA, T0_TERaw,   id, pla->fT_TERaw[0]);
      fAnaLoop->Add(NINJA, T1_TERaw,   id, pla->fT_TERaw[1]);
      // Calibrated Time
      fAnaLoop->Add(NINJA, T0_LECal,   id, pla->fT_LECal[0]);
      fAnaLoop->Add(NINJA, T1_LECal,   id, pla->fT_LECal[1]);
      fAnaLoop->Add(NINJA, T0_TECal,   id, pla->fT_TECal[0]);
      fAnaLoop->Add(NINJA, T1_TECal,   id, pla->fT_TECal[1]);
      // Time over Threshold -> TU_LECal-TU_TECal 
      fAnaLoop->Add(NINJA, T0_TOTCal,  id, pla->fTOTCal[0]);
      fAnaLoop->Add(NINJA, T1_TOTCal,  id, pla->fTOTCal[1]);
      // Calibrated charge, based on TOT
      fAnaLoop->Add(NINJA, Q0Cal,      id, pla->fQCal[0]);
      fAnaLoop->Add(NINJA, Q1Cal,      id, pla->fQCal[1]);
      // Average charge sqrt(Q0 * Q1)
      fAnaLoop->Add(NINJA, QAveCal,       id, pla->fQAveCal);
      // Log of QAve should be constant vs position? 
      fAnaLoop->Add(NINJA, LogQAveCal,    id, pla->fLogQAveCal);
      // T Average, - > 0.5*(T1+T0) effectively, time of flight of the particle
      fAnaLoop->Add(NINJA, T_LEAve,       id, pla->fT_LEAve);
      fAnaLoop->Add(NINJA, T_TEAve,       id, pla->fT_TEAve);
      // TOF with beam reference
      fAnaLoop->Add(NINJA, T_LEAveT0,     id, pla->fT_LEAveT0);
      fAnaLoop->Add(NINJA, T_TEAveT0,     id, pla->fT_TEAveT0);
      //      fAnaLoop->Add(NINJA, PosY,     id, pla->fPos[1]);
      //      fAnaLoop->Add(NINJA, PosZ,     id, pla->fPos[2]);
    }
  }

}

void TAlEncNINJAExample::SetFilterFlag(int filterflag)
{
  fFilterFlag = filterflag;
}

const char* TAlEncNINJAExample::ClassName() const
{
  return "TAlEncNINJAExample";
}

TAlEncNINJAExampleFactory::TAlEncNINJAExampleFactory()
 : TAlEncFactory()
{;}

TAlEncNINJAExampleFactory::~TAlEncNINJAExampleFactory()
{;}

TAlEncSub* TAlEncNINJAExampleFactory::CreateEnc()
{
  return new TAlEncNINJAExample;
}

