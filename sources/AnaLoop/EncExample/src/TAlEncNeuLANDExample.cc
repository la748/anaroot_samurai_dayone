
#include "TAlEncNeuLANDExample.hh"

#include <iostream>

#include "TArtCore.hh"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibNeuLAND.hh"
#include "TArtNeuLANDPla.hh"
#include "TArtNeuLANDPlaPara.hh"
//#include "TArtNeuLANDFilter.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

int TAlEncNeuLANDExample::fFilterFlag = 0x0000; // N/A

TAlEncNeuLANDExample::TAlEncNeuLANDExample()
  : TAlEncSub()
{;}

TAlEncNeuLANDExample::~TAlEncNeuLANDExample()
{
  Destruct();
}

void TAlEncNeuLANDExample::Construct()
{
  fStoreManager = TArtStoreManager::Instance();
  samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter((char*)"db/NEULAND.xml");
  samuraiparameters->LoadParameter((char*)"db/NEULANDVETO.xml");

  fCalibNeuLAND = new TArtCalibNeuLAND();
  fCalibNeuLANDVETO = new TArtCalibNeuLANDVETO();

//  //  fCalibNeuLAND->SetName("CalibNeuLAND");
//  //  fStoreManager->AddCalibContainer(fCalibNeuLAND);
//
//  std::string nebulafiltermessage= "\n";
//  if(fFilterFlag & 0x0001){
//    nebulafiltermessage += " TArtNeuLANDFilter::IHitMin: on\n";
//  }
//  if(fFilterFlag & 0x0002){
//    nebulafiltermessage += " TArtNeuLANDFilter::Threshold: on\n";
//  }
//  if(fFilterFlag & 0x0020){  
//    nebulafiltermessage += " TArtNeuLANDFilter::VETO(only front): on\n";
//  }
//  if(fFilterFlag & 0x0004){
//    nebulafiltermessage += " TArtNeuLANDFilter::Veto: on\n";
//  }
//  if(fFilterFlag & 0x0008){  
//    nebulafiltermessage += " TArtNeuLANDFilter::HitMinPos: on\n";
//  }
//  if(fFilterFlag & 0x0010){  
//    nebulafiltermessage += " TArtNeuLANDFilter::HitMinPos2: on\n";
//  }
//  if(fFilterFlag & 0x0040){  
//    nebulafiltermessage += " TArtNeuLANDFilter::HitMinTime: on\n";
//  }
//  if(nebulafiltermessage.size() > 1){
//    std::cout << nebulafiltermessage << std::endl;
// }
}

void TAlEncNeuLANDExample::PreCalculate()
{
  fCalibNeuLAND->ClearData();
  fCalibNeuLANDVETO->ClearData();
}

bool TAlEncNeuLANDExample::Calculate()
{
  fCalibNeuLAND->ReconstructData();
  fCalibNeuLANDVETO->ReconstructData();

//  // 1n: flag=0x0063
//  // 2n: flag=0x0017
//
//  if(fFilterFlag & 0x0001){
//    TArtNeuLANDFilter::IHitMin(fCalibNeuLAND, 4, 1); // neut>=4hit, veto>=1hit
//  }
//  if(fFilterFlag & 0x0002){
//    //    TArtNeuLANDFilter::Threshold(fCalibNeuLAND, 6, -1);
//    TArtNeuLANDFilter::Threshold(fCalibNeuLAND, 10, -1);
//  }
//  if(fFilterFlag & 0x0020){  
//    TArtNeuLANDFilter::Veto(fCalibNeuLAND, 1); // only front
//  }
//  if(fFilterFlag & 0x0004){  
//    TArtNeuLANDFilter::Veto(fCalibNeuLAND, 2); // two layer
//  }
//  if(fFilterFlag & 0x0008){  
//    TArtNeuLANDFilter::HitMinPos(fCalibNeuLAND); // get front hit 
//  }
//  if(fFilterFlag & 0x0010){  
//    TArtNeuLANDFilter::HitMinPos2(fCalibNeuLAND); // get front hit in each layer
//  }
//  if(fFilterFlag & 0x0040){  
//    TArtNeuLANDFilter::HitMinTime(fCalibNeuLAND); // get front hit in each layer
//  }

//  fCalibNeuLAND->CalMultiplicity();  

  Fill();

  return true;
}

void TAlEncNeuLANDExample::Destruct()
{
  delete fCalibNeuLAND;
  fCalibNeuLAND = 0;
  delete fCalibNeuLANDVETO;
  fCalibNeuLANDVETO = 0;
  if(samuraiparameters) samuraiparameters->Delete();
  samuraiparameters = 0;
}

void TAlEncNeuLANDExample::Fill()
{
  //  using namespace Analyser;
  //  using namespace WNum::NEULAND;  

  Int_t Multiplicity[8+1] = {0,0,0,0,0,0,0,0,0}; // multiplicity neutron (1 to 8) and veto (0)
  Int_t MultiplicityTot = 0;
  
  TClonesArray *neuland_array = fCalibNeuLAND->GetNeuLANDPlaArray();
  Int_t NumNeuLANDPla = neuland_array->GetEntries();

  for(int i=0; i<NumNeuLANDPla; ++i){
    TArtNeuLANDPla* pla = (TArtNeuLANDPla*)neuland_array->At(i);
    if(pla->GetDataState() < 0) continue;

    Int_t id    = pla->GetID();

    if(!fAnaLoop->UsingAnaFile() ){
      /*nothing*/;
    }else{

      MultiplicityTot++;
      Multiplicity[int(id/50)+1]++;
      //      std::cout << id << " " << int(id/50) << std::endl;

      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: ID,       id, id);
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: Layer,    id, int(pla->GetLayer()/2)+1);
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: SubLayer, id, pla->GetLayer()+1);
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: DetPosX,  id, pla->GetX());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: DetPosY,  id, pla->GetY());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: DetPosZ,  id, pla->GetZ());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: Hit,      id, pla->GetBothFired());
      //
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: QURaw,        id, pla->GetQRaw(0));
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: QDRaw,        id, pla->GetQRaw(1));
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: QUPed,        id, pla->GetQUPed());
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: QDPed,        id, pla->GetQDPed());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: QUCal,        id, pla->GetQCal(0));
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: QDCal,        id, pla->GetQCal(1));
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: QAvePed,      id, pla->GetQAvePed());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: QAveCal,      id, 
		    (Double_t)TMath::Sqrt((Double_t)(pla->GetQCal(0)*pla->GetQCal(1))));
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: LogQPed,      id, pla->GetLogQPed());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: LogQCal,      id,
		    100*log(pla->GetQCal(0)/pla->GetQCal(1))); 
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: IvSqrtQUPed,  id, pla->GetIvSqrtQUPed());
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: IvSqrtQDPed,  id, pla->GetIvSqrtQDPed());
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: IvSqrtQAvePed,id, pla->GetIvSqrtQAvePed());
      //
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TURaw,       id, pla->GetTRaw(0));
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TDRaw,       id, pla->GetTRaw(1));
//      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TUCal,       id, pla->GetTCal(0));
//      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TDCal,       id, pla->GetTCal(1));
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TUSlw,       id, pla->GetTCal(0));
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TDSlw,       id, pla->GetTCal(1));
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: DTRaw,       id,
		    pla->GetTRaw(0)-pla->GetTRaw(1));
//      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: DTCal,       id, 
//		    pla->GetTCal(0)-pla->GetTCal(1));
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: DTSlw,       id, 
		    pla->GetTCal(0)-pla->GetTCal(1));

      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TAveRaw,     id, 
		    (pla->GetTRaw(0)+pla->GetTRaw(1))/2.);
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TAveCal,     id, pla->GetTAveCal());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TAveSlw,     id, 
		    (pla->GetTCal(0)+pla->GetTCal(1))/2.);
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TUCalT0,     id, pla->GetTUCalT0());
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TDCalT0,     id, pla->GetTDCalT0());
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TUSlwT0,     id, pla->GetTUSlwT0());
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TDSlwT0,     id, pla->GetTDSlwT0());
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TAveCalT0,   id, pla->GetTAveCalT0());
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TAveSlwT0,   id, pla->GetTAveSlwT0());
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TTOFGamma,   id, pla->GetTTOFGamma());
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: TTOFNeutron, id, pla->GetTTOFNeutron());
      
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: PosCal,       id, pla->GetPos());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: PosSlw,       id, pla->GetPos());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: PosX,         id, pla->GetX());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: PosY,         id, pla->GetY());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: PosZ,         id, pla->GetZ());
      //      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND:: FlightLength, id, pla->GetFlightLength());
      //   if(id==25) std::cout << (Double_t)TMath::Log((Double_t)pla->GetQCal(0)/(Double_t)pla->GetQCal(1)) << std::endl;
    }
  }

 TClonesArray *neulandveto_array = fCalibNeuLANDVETO->GetNeuLANDVETOPlaArray();
  Int_t NumNeuLANDVETOPla = neulandveto_array->GetEntries();
  for(int i=0; i<NumNeuLANDVETOPla; ++i){
    TArtNEBULAPla* pla = (TArtNEBULAPla*)neulandveto_array->At(i);
    if(pla->GetDataState() < 0) continue;

    Int_t id    = pla->GetID() + 400;
    //if(id != 409)     cout << id << endl;
    if(!fAnaLoop->UsingAnaFile() ){
      /*nothing*/;
    }else{

      Multiplicity[0]++;

      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::ID,       id, id);
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::Layer,    id, pla->GetLayer());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::SubLayer, id, pla->GetSubLayer());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::DetPosX,  id, pla->GetDetPos(0));
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::DetPosY,  id, pla->GetDetPos(1));
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::DetPosZ,  id, pla->GetDetPos(2));
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::Hit,      id, pla->GetHit());

      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::QURaw,        id, pla->GetQURaw());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::QDRaw,        id, pla->GetQDRaw());
      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::QUPed,        id, pla->GetQUPed());
      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::QDPed,        id, pla->GetQDPed());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::QUCal,        id, pla->GetQUCal());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::QDCal,        id, pla->GetQDCal());
      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::QAvePed,      id, pla->GetQAvePed());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::QAveCal,      id, pla->GetQAveCal());
      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::LogQPed,      id, pla->GetLogQPed());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::LogQCal,      id, pla->GetLogQCal());
      //fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::IvSqrtQUPed,  id, pla->GetIvSqrtQUPed());
      //fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::IvSqrtQDPed,  id, pla->GetIvSqrtQDPed());
      //fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::IvSqrtQAvePed,id, pla->GetIvSqrtQAvePed());

      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TURaw,       id, pla->GetTURaw());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TDRaw,       id, pla->GetTDRaw());
      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TUCal,       id, pla->GetTUCal());
      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TDCal,       id, pla->GetTDCal());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TUSlw,       id, pla->GetTUSlw());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TDSlw,       id, pla->GetTDSlw());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::DTRaw,       id, pla->GetDTRaw());
      //fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::DTCal,       id, pla->GetDTCal());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::DTSlw,       id, pla->GetDTSlw());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TAveRaw,     id, pla->GetTAveRaw());
      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TAveCal,     id, pla->GetTAveCal());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TAveSlw,     id, pla->GetTAveSlw());
      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TUCalT0,     id, pla->GetTUCalT0());
      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TDCalT0,     id, pla->GetTDCalT0());
      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TUSlwT0,     id, pla->GetTUSlwT0());
      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TDSlwT0,     id, pla->GetTDSlwT0());
      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TAveCalT0,   id, pla->GetTAveCalT0());
      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TAveSlwT0,   id, pla->GetTAveSlwT0());
      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TTOFGamma,   id, pla->GetTTOFGamma());
      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::TTOFNeutron, id, pla->GetTTOFNeutron());

      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::PosCal,       id, pla->GetPosCal());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::PosSlw,       id, pla->GetPosSlw());
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::PosX,         id, pla->GetPos(0));
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::PosY,         id, pla->GetPos(1));
      fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::PosZ,         id, pla->GetPos(2));
      // fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::FlightLength, id, pla->GetFlightLength());
    }
  }

  if(!fAnaLoop->UsingAnaFile()){
    /*nothing*/;
  }else{
    fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::Multiplicity_N  , 0, MultiplicityTot);
    fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::Multiplicity_N11, 0, Multiplicity[1]);
    fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::Multiplicity_N12, 0, Multiplicity[2]);
    fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::Multiplicity_N21, 0, Multiplicity[3]);
    fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::Multiplicity_N22, 0, Multiplicity[4]);
    fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::Multiplicity_N31, 0, Multiplicity[5]);
    fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::Multiplicity_N32, 0, Multiplicity[6]);
    fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::Multiplicity_N41, 0, Multiplicity[7]);
    fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::Multiplicity_N42, 0, Multiplicity[8]);
    fAnaLoop->Add(Analyser::NEULAND, WNum::NEULAND::Multiplicity_V  , 0, Multiplicity[0]);
  }


}

void TAlEncNeuLANDExample::SetFilterFlag(int filterflag)
{
  fFilterFlag = filterflag;
}

const char* TAlEncNeuLANDExample::ClassName() const
{
  return "TAlEncNeuLANDExample";
}

TAlEncNeuLANDExampleFactory::TAlEncNeuLANDExampleFactory()
 : TAlEncFactory()
{;}

TAlEncNeuLANDExampleFactory::~TAlEncNeuLANDExampleFactory()
{;}

TAlEncSub* TAlEncNeuLANDExampleFactory::CreateEnc()
{
  return new TAlEncNeuLANDExample;
}

