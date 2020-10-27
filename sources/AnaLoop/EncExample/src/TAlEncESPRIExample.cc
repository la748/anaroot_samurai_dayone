#include "TAlEncESPRIExample.hh"

//___________________________________________________________________________

TAlEncESPRIExample::TAlEncESPRIExample()
  : TAlEncSub(), ESPRIUserPara(0), ESPRIparameters(0),   fCalibESPRI_NaI(0),  fCalibESPRI_tdc(0),  fCalibESPRI_rdc(0),  fCalibESPRI_bdc(0),  fCalibESPRI_fadc(0),  fCalibESPRI_plas(0),
    NaIMergeID({3,3,4,4,5,5,6,6,7,7,8,8,9,9,
	  13,13,14,14,15,15,16,16,17,17,18,18,19,19}),
    PlaMergeID({2,2,12,12})
{;}
//___________________________________________________________________________

TAlEncESPRIExample::~TAlEncESPRIExample()
{
  Destruct();
}
//___________________________________________________________________________

void TAlEncESPRIExample::Construct()
{
  ESPRIparameters = TArtESPRIParameters::Instance();
  ESPRIparameters->LoadParameter("db/ESPRIRNAI.xml");
  //ESPRIparameters->LoadParameter("db/ESPRIRNAI2.xml");
  ESPRIparameters->LoadParameter("db/ESPRITDC.xml");
  // ESPRIparameters->LoadParameter("db/ESPRIFADC.xml");
  ESPRIparameters->LoadParameter("db/ESPRIPLAS.xml");
  //  
  ESPRIUserPara = TArtUserParameters::Instance();
  ESPRIUserPara->LoadParameter("db/USERPARAMETER.xml");
  fCalibESPRI_NaI = new TArtCalibRNaI();
  fCalibESPRI_tdc = new TArtCalibTDCHit();
  fCalibESPRI_rdc = new TArtCalibRDC();
  fCalibESPRI_bdc = new TArtCalibBDC();
  fCalibESPRI_fadc = new TArtCalibFADC();
  fCalibESPRI_plas = new TArtCalibPlas();



}

//___________________________________________________________________________
void TAlEncESPRIExample::PreCalculate()
{
  fCalibESPRI_NaI->ClearData();
  fCalibESPRI_tdc->ClearData();
  fCalibESPRI_rdc->ClearData();
  fCalibESPRI_bdc->ClearData();
  fCalibESPRI_fadc->ClearData();
  fCalibESPRI_plas->ClearData();
}

void TAlEncESPRIExample::LoadDCTDCDistribution(char * file)
{

  // this code copied from SAMURAI DCs !

  //  char myname[128];
  //  TFile *fdcin = new TFile(file,"READ"); 
  //  gROOT->cd();
  //  TH1F *hist = NULL;
  //
  //  for(int i=0;i<4;i++){
  //    sprintf(myname,"bdc1_ftdc_corr_%d",i);
  //
  //    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
  //    fCalibBDC1Track->SetTDCDistribution(hist,i*2);
  //    delete hist; hist = NULL;
  //    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
  //    fCalibBDC1Track->SetTDCDistribution(hist,i*2+1);
  //    delete hist; hist = NULL;
  //
  //    sprintf(myname,"bdc2_ftdc_corr_%d",i);
  //
  //    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
  //    fCalibBDC2Track->SetTDCDistribution(hist,i*2);
  //    delete hist; hist = NULL;
  //    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
  //    fCalibBDC2Track->SetTDCDistribution(hist,i*2+1);
  //    delete hist; hist = NULL;
  //  }
  //
  //  for(int i=0;i<7;i++){
  //    sprintf(myname,"fdc1_ftdc_corr_%d",i);
  //
  //    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
  //    fCalibFDC1Track->SetTDCDistribution(hist,i*2);
  //    delete hist; hist = NULL;
  //    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
  //    fCalibFDC1Track->SetTDCDistribution(hist,i*2+1);
  //    delete hist; hist = NULL;
  //
  //    sprintf(myname,"fdc2_ftdc_corr_%d",i);
  //
  //    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
  //    fCalibFDC2Track->SetTDCDistribution(hist,i*2);
  //    delete hist; hist = NULL;
  //    hist = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
  //    fCalibFDC2Track->SetTDCDistribution(hist,i*2+1);
  //    delete hist; hist = NULL;
  //  }
  //
}

//___________________________________________________________________________
bool TAlEncESPRIExample::Calculate()
{

  TArtStoreManager * sman = TArtStoreManager::Instance();

  TClonesArray *info = (TClonesArray*)sman->FindDataContainer("EventInfo");
  //  static int runnumber = -1;
  //  if(runnumber!=((TArtEventInfo *)info->At(0))->GetRunNumber()){
  //    runnumber = ((TArtEventInfo *)info->At(0))->GetRunNumber();
  //    TArtCore::Info(__FILE__,"runnumber: %d",runnumber);
  //    LoadDCTDCDistribution((char*)"db/dc/s018_daq0046.root");
  //  }
  if(fAnaLoop->UsingAnaFile()){


    fCalibESPRI_NaI->ReconstructData();
    fCalibESPRI_tdc->ReconstructData();
    fCalibESPRI_rdc->ReconstructData();
    fCalibESPRI_bdc->ReconstructData();
    fCalibESPRI_fadc->ReconstructData();
    fCalibESPRI_plas->ReconstructData();

    Int_t tmpID ;
    Bool_t NaIFilledID[30] = {0};
    Bool_t PlasFilledID[30] = {0};
    Double_t ChargeProduct[30] = {0};
    //ADC for NaI (TDC is not included yet)
    TClonesArray *ESPRI_NaIs = (TClonesArray *)sman->FindDataContainer("ESPRINaI");
    //    cout << ESPRI_NaIs->GetEntries() << endl;
    for(int i=0;i<ESPRI_NaIs->GetEntries();i++){
      TArtRNaI *ESPRINaI = (TArtRNaI*)ESPRI_NaIs->At(i);
      if(ESPRINaI){
	tmpID = NaIMergeID[ESPRINaI->GetID()-1];
	if(!NaIFilledID[tmpID]){
	  // First Side found
	  fAnaLoop->Add(Analyser::ESPRI,  WNum::ESPRI::ID,    tmpID, tmpID); 
	  fAnaLoop->Add(Analyser::ESPRI,  WNum::ESPRI::Layer, tmpID, ESPRINaI->GetLayer()); 
	  NaIFilledID[tmpID] = kTRUE ;
	  ChargeProduct[tmpID] = ESPRINaI->GetEnergy();
	} else {
	  // Second Side found
	  ChargeProduct[tmpID] = TMath::Sqrt(ESPRINaI->GetEnergy()*ChargeProduct[tmpID]);
	  fAnaLoop->Add(Analyser::ESPRI,  WNum::ESPRI::AveEnergy, tmpID, ChargeProduct[tmpID]); 
	}
	if(ESPRINaI->GetID()%2==1){
	  fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::RawADC0,  tmpID, ESPRINaI->GetRawADC()); 
	  fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::RawTDC0,  tmpID, ESPRINaI->GetRawTDC());
	  fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::Energy0,  tmpID, ESPRINaI->GetEnergy());  
	  fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::Time0,    tmpID, ESPRINaI->GetTime()); 
 	} else {			 
	  fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::RawADC1,  tmpID, ESPRINaI->GetRawADC()); 
	  fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::RawTDC1,  tmpID, ESPRINaI->GetRawTDC());
	  fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::Energy1,  tmpID, ESPRINaI->GetEnergy());  
	  fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::Time1,    tmpID, ESPRINaI->GetTime()); 
	}

      }
    }
	
    //All of multihitTDC
    //    TClonesArray *ESPRI_TDC = (TClonesArray *)sman->FindDataContainer("ESPRITDC");	

    //    for(int i=0;i<ESPRI_TDC->GetEntries();i++){
    //      TArtTDCHit *TDC = (TArtTDCHit*)ESPRI_TDC->At(i);
    //      {
    //	    
    //      }
    //    }
    
    //RDCup&dw
    TClonesArray *ESPRI_RDC = (TClonesArray *)sman->FindDataContainer("ESPRIRDC");
    for(int i=0;i<ESPRI_RDC->GetEntries();i++){
      TArtRDC *rdc = (TArtRDC*)ESPRI_RDC->At(i);
      if(rdc){
	tmpID = (rdc->GetLayer()-1)*10+1 ;
	fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::ID,    tmpID, tmpID); 
	fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::Layer, tmpID, rdc->GetLayer()); 
	fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::XPos,  tmpID, rdc->GetRDCX()); 
	fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::XAng,  tmpID, rdc->GetRDCA()); 
	fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::YPos,  tmpID, rdc->GetRDCY()); 
	fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::YAng,  tmpID, rdc->GetRDCB()); 
      }
    }
	
    //QDC
    //    ESPRI_Plas_Mult=0;
    TClonesArray *ESPRI_Plas = (TClonesArray *)sman->FindDataContainer("ESPRIPLAS");
    if(ESPRI_Plas){
      for(int i=0;i<ESPRI_Plas->GetEntries();i++){
	TArtPlas *ESPRIPlas = (TArtPlas*)ESPRI_Plas->At(i);
	if(ESPRIPlas){
	  tmpID = PlaMergeID[ESPRIPlas->GetID()-1];
	  if(!PlasFilledID[tmpID]){
	    fAnaLoop->Add(Analyser::ESPRI,  WNum::ESPRI::ID,    tmpID, tmpID); 
	    fAnaLoop->Add(Analyser::ESPRI,  WNum::ESPRI::Layer, tmpID, ESPRIPlas->GetLayer()); 
	    PlasFilledID[tmpID] = kTRUE;
	    ChargeProduct[tmpID] = ESPRIPlas->GetEnergy();
	  } else {
	    // Second Side found
	    ChargeProduct[tmpID] = TMath::Sqrt(ESPRIPlas->GetEnergy()*ChargeProduct[tmpID]);
	    fAnaLoop->Add(Analyser::ESPRI,  WNum::ESPRI::AveEnergy, tmpID, ChargeProduct[tmpID]); 
	  }

	  if(ESPRIPlas->GetID()%2==1){
	    fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::RawADC0,  tmpID, ESPRIPlas->GetRawADC()); 
	    fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::RawTDC0,  tmpID, ESPRIPlas->GetRawTDC());
	    fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::Energy0,  tmpID, ESPRIPlas->GetEnergy());  
	    fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::Time0,    tmpID, ESPRIPlas->GetTime()); 
	  } else {			 
	    fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::RawADC1,  tmpID, ESPRIPlas->GetRawADC()); 
	    fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::RawTDC1,  tmpID, ESPRIPlas->GetRawTDC());
	    fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::Energy1,  tmpID, ESPRIPlas->GetEnergy());  
	    fAnaLoop->Add(Analyser::ESPRI, WNum::ESPRI::Time1,    tmpID, ESPRIPlas->GetTime()); 
	  }
	}
      }
    }

  }
  return true;
}
//___________________________________________________________________________

void TAlEncESPRIExample::Destruct()
{
  delete fCalibESPRI_NaI ; fCalibESPRI_NaI =0;
  delete fCalibESPRI_tdc ; fCalibESPRI_tdc =0;
  delete fCalibESPRI_rdc ; fCalibESPRI_rdc =0;
  delete fCalibESPRI_bdc ; fCalibESPRI_bdc =0;
  delete fCalibESPRI_fadc; fCalibESPRI_fadc=0;
  delete fCalibESPRI_plas; fCalibESPRI_plas=0;

  if(ESPRIparameters) ESPRIparameters->Delete();
  ESPRIparameters = 0;
}
//___________________________________________________________________________

const char* TAlEncESPRIExample::ClassName() const
{
  return "TAlEncESPRIExample";
}

TAlEncESPRIExampleFactory::TAlEncESPRIExampleFactory()
  : TAlEncFactory()
{;}

TAlEncESPRIExampleFactory::~TAlEncESPRIExampleFactory()
{;}

TAlEncSub* TAlEncESPRIExampleFactory::CreateEnc()
{
  return new TAlEncESPRIExample;
}

