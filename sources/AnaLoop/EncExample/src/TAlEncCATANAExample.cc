#include "TAlEncCATANAExample.hh"

#include <iostream>

#include "TClonesArray.h"
#include "TMath.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibCATANA.hh"
#include "TArtCATANACsI.hh"
#include "TArtCATANAParameters.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtStoreManager.hh"

TAlEncCATANAExample::TAlEncCATANAExample()
  : TAlEncSub(), catanaparameters(0), calibcatana(0)
{;}

TAlEncCATANAExample::~TAlEncCATANAExample()
{
  Destruct();
}

void TAlEncCATANAExample::Construct()
{
  catanaparameters = TArtCATANAParameters::Instance();
  catanaparameters->LoadParameter((char*)"db/CATANA.xml");
  //  catanaparameters->PrintListOfCsIPara();
  calibcatana = new TArtCalibCATANA; //must be called after parameter is constructed
  //  BetaValueForDopplerCorrect = atof(std::getenv("SAMURAI_CATANA_BETA")); 
  
  TArtCore::Info(__FILE__,"\e[33m\e[1mBeta for Doppler correction in CATANA: %f\e[0m",BetaValueForDopplerCorrect);
  TArtCore::Info(__FILE__,"\e[33m\e[1mexport SAMURAI_CATANA_BETA=0.XX to change\e[0m");
  if(!fAnaLoop->UsingAnaFile()){
    hcsi_adc = new TH2F("csi_adc","csi_adc",1000,0,4000,140,0,140);
    hcsi_tdc = new TH2F("csi_tdc","csi_tdc",1000,0,4000,140,0,140);
  }else{
    /* nothing */;
  }
}

void TAlEncCATANAExample::PreCalculate()
{
   calibcatana->ClearData();
}

bool TAlEncCATANAExample::Calculate()
{
  calibcatana->ReconstructData();
  calibcatana->DopplerCorrect(0.616);
  //  calibcatana->DopplerCorrect(BetaValueForDopplerCorrect);
  //Done in the reconstructor;

  using namespace Analyser;
  using namespace WNum::CATANA;

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TClonesArray *csis = (TClonesArray *)sman->FindDataContainer("CATANACsI");

  int mult=0;
  Double_t abs =0, costheta, radius2;

  for(int i=0;i<csis->GetEntries();i++){
    TArtCATANACsI *csi = (TArtCATANACsI *)csis->At(i);

    Int_t id = csi->GetID();
    if(!fAnaLoop->UsingAnaFile()){
      hcsi_adc->Fill(csi->GetRawADC(), id);
      hcsi_tdc->Fill(csi->GetRawTDC(), id);
    }else{
      fAnaLoop->Add(CATANA, ID,            id, id);
      fAnaLoop->Add(CATANA, Layer,         id, (id-1)/20+1);

      radius2 = csi->GetPositionX()*csi->GetPositionX()+
	csi->GetPositionY()*csi->GetPositionY();
      
      abs = TMath::Sqrt(radius2+
			csi->GetPositionZ()*csi->GetPositionZ());
      
      costheta = csi->GetPositionZ()/abs ;
      fAnaLoop->Add(CATANA, Theta,         id, TMath::ATan2(TMath::Sqrt(radius2),
							    csi->GetPositionZ()));
      fAnaLoop->Add(CATANA, CosTheta,      id, costheta);
      fAnaLoop->Add(CATANA, DetPosX,      id,  csi->GetPositionX());
      fAnaLoop->Add(CATANA, DetPosY,      id,  csi->GetPositionY());
      fAnaLoop->Add(CATANA, DetPosZ,      id,  csi->GetPositionZ());

//      fAnaLoop->Add(CATANA, RawADC,        id, 1500);
      fAnaLoop->Add(CATANA, RawADC,        id, csi->GetRawADC());
      fAnaLoop->Add(CATANA, Energy,        id, csi->GetEnergy());
      fAnaLoop->Add(CATANA, DoppCorEnergy, id, csi->GetDoppCorEnergy());
      // fAnaLoop->Add(CATANA, EnergyWithoutT, id, csi->GetEnergyWithoutT());
      
      fAnaLoop->Add(CATANA, RawTDC,        id, csi->GetRawTDC());
      fAnaLoop->Add(CATANA, Time,          id, csi->GetTime());
      fAnaLoop->Add(CATANA, TimeOffseted,  id, csi->GetTimeOffseted());
      
      // fAnaLoop->Add(CATANA, TimeTrueEnergy,        id, csi->GetTimeTrueEnergy());
      // fAnaLoop->Add(CATANA, TimeTrueDoppCorEnergy, id, csi->GetTimeTrueDoppCorEnergy());
      // fAnaLoop->Add(CATANA, TimeTrueTime,          id, csi->GetTimeTrueTime());
      // fAnaLoop->Add(CATANA, TimeTrueTimeOffseted,  id, csi->GetTimeTrueTimeOffseted());
      
      if(csi->GetTime()>0) ++mult;
    }      
  }
  fAnaLoop->Add(CATANA, Multiplicity, 0, mult);

  return true;
}

void TAlEncCATANAExample::Destruct()
{
  delete calibcatana;
  calibcatana = 0;
  if(catanaparameters) catanaparameters->Delete();
  catanaparameters = 0;
}

const char* TAlEncCATANAExample::ClassName() const
{
  return "TAlEncCATANAExample";
}

TAlEncCATANAExampleFactory::TAlEncCATANAExampleFactory()
 : TAlEncFactory()
{;}

TAlEncCATANAExampleFactory::~TAlEncCATANAExampleFactory()
{;}

TAlEncSub* TAlEncCATANAExampleFactory::CreateEnc()
{
  return new TAlEncCATANAExample;
}

