#include "TAlEncSAMURAIDCExample.hh"

//___________________________________________________________________________

TAlEncSAMURAIDCExample::TAlEncSAMURAIDCExample()
  : TAlEncSub(), samuraiparameters(0), fCalibBPCHit(0), fCalibBDC1Hit(0), fCalibBDC2Hit(0), fCalibFDC0Hit(0), fCalibFDC2Hit(0), fCalibBDC1Track(0), fCalibBDC2Track(0), fCalibFDC0Track(0), fCalibFDC2Track(0) ,  fCalibFDC1Hit(0), fCalibFDC1Track(0),  fCalibPDCHit(0), fCalibPDCTrack(0)
{;}

//___________________________________________________________________________

TAlEncSAMURAIDCExample::~TAlEncSAMURAIDCExample()
{
  Destruct();
}
//___________________________________________________________________________

void TAlEncSAMURAIDCExample::Construct()
{
  samuraiparameters			  = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter("db/SAMURAIBPC.xml");
  samuraiparameters->LoadParameter("db/SAMURAIBDC1.xml");
  samuraiparameters->LoadParameter("db/SAMURAIBDC2.xml");
  samuraiparameters->LoadParameter("db/SAMURAIFDC0.xml");
  samuraiparameters->LoadParameter("db/SAMURAIFDC1.xml");
  samuraiparameters->LoadParameter("db/SAMURAIFDC2.xml");
  samuraiparameters->LoadParameter("db/SAMURAIPDC.xml");

  fCalibBPCHit	  = new TArtCalibBPCHit();
  fCalibBDC1Hit	  = new TArtCalibBDC1Hit();
  fCalibBDC2Hit	  = new TArtCalibBDC2Hit();
  fCalibFDC0Hit	  = new TArtCalibFDC0Hit();
  fCalibFDC1Hit	  = new TArtCalibFDC1Hit();
  fCalibFDC2Hit	  = new TArtCalibFDC2Hit();
  fCalibPDCHit	  = new TArtCalibPDCHit();

  fCalibBDC1Track = new TArtCalibBDC1Track();
  fCalibBDC2Track = new TArtCalibBDC2Track();
  fCalibFDC0Track = new TArtCalibFDC0Track();
  fCalibFDC1Track = new TArtCalibFDC1Track();
  fCalibFDC2Track = new TArtCalibFDC2Track();
  fCalibPDCTrack  = new TArtCalibPDCTrack();


}

//___________________________________________________________________________
void TAlEncSAMURAIDCExample::PreCalculate()
{
  fCalibBPCHit->ClearData();
  fCalibBDC1Hit->ClearData();
  fCalibBDC1Track->ClearData();
  fCalibBDC2Hit->ClearData();
  fCalibBDC2Track->ClearData();
  fCalibFDC0Hit->ClearData();
  fCalibFDC0Track->ClearData();
  fCalibFDC1Hit->ClearData();
  fCalibFDC1Track->ClearData();
  fCalibFDC2Hit->ClearData();
  fCalibFDC2Track->ClearData();
  fCalibPDCHit->ClearData();
  fCalibPDCTrack->ClearData();
}

void TAlEncSAMURAIDCExample::LoadDCTDCDistribution(char * file)
{
  char	 myname[128];
  TFile	*fdcin = new TFile(file,"READ"); 
  gROOT->cd();
  TH1F	*hist  = NULL;

  for(int i=0;i<4;i++){

    sprintf(myname,"bdc1_ftdc_corr_%d",i);
    if(fdcin->Get(myname)){
      hist	      = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
      if(hist) fCalibBDC1Track->SetTDCDistribution(hist,i*2);
      delete hist; hist = NULL;
      hist	      = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
      if(hist) fCalibBDC1Track->SetTDCDistribution(hist,i*2+1);
      delete hist; hist = NULL;
    }

    sprintf(myname,"bdc2_ftdc_corr_%d",i);
    if(fdcin->Get(myname)){
      hist	      = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
      if(hist) fCalibBDC2Track->SetTDCDistribution(hist,i*2);
      delete hist; hist = NULL;
      hist	      = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
      if(hist) fCalibBDC2Track->SetTDCDistribution(hist,i*2+1);
      delete hist; hist = NULL;
    }

    sprintf(myname,"fdc0_ftdc_corr_%d",i);
    if(fdcin->Get(myname)){
      hist	      = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
      if(hist) fCalibFDC0Track->SetTDCDistribution(hist,i*2);
      delete hist; hist = NULL;
      hist	      = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
      if(hist) fCalibFDC0Track->SetTDCDistribution(hist,i*2+1);
      delete hist; hist = NULL;
    }
  }

//  for(int i=0;i<5;i++){		//?
//    sprintf(myname,"fdc0_ftdc_corr_%d",i);
//    if(fdcin->Get(myname)){
//      hist				  = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
//      if(hist) fCalibPDCTrack->SetTDCDistribution(hist,i*2);
//      delete hist; hist			  = NULL;
//      hist				  = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
//      if(hist)  fCalibPDCTrack->SetTDCDistribution(hist,i*2+1);
//      delete hist; hist			  = NULL;
//    }
//  }

  for(int i=0;i<7;i++){
    sprintf(myname,"fdc1_ftdc_corr_%d",i);
    if(fdcin->Get(myname)){
      hist				  = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
      if(hist) fCalibFDC1Track->SetTDCDistribution(hist,i*2);
      delete hist; hist			  = NULL;
      hist				  = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
      if(hist) fCalibFDC1Track->SetTDCDistribution(hist,i*2+1);
      delete hist; hist			  = NULL;
    }

    sprintf(myname,"fdc2_ftdc_corr_%d",i);
    if(fdcin->Get(myname)){
      hist				  = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionX();
      if(hist) fCalibFDC2Track->SetTDCDistribution(hist,i*2);
      delete hist; hist			  = NULL;
      hist				  = (TH1F*)((TH2F*)fdcin->Get(myname))->ProjectionY();
      if(hist) fCalibFDC2Track->SetTDCDistribution(hist,i*2+1);
      delete hist; hist			  = NULL;
    }
  }


}

//___________________________________________________________________________
bool TAlEncSAMURAIDCExample::Calculate()
{
  TArtStoreManager *	 sman		  = TArtStoreManager::Instance();

  TClonesArray		*info		  = (TClonesArray*)sman->FindDataContainer("EventInfo");
  static int		 runnumber	  = -1;
  if(runnumber!=((TArtEventInfo *)info->At(0))->GetRunNumber()){
    runnumber				  = ((TArtEventInfo *)info->At(0))->GetRunNumber();
    TArtCore::Info(__FILE__,"runnumber: %d",runnumber);
    //    LoadDCTDCDistribution((char*)"db/dc/s018_daq0046.root");
    //LoadDCTDCDistribution((char*)"db/dc/time_distribution_ANALOOP.root");
    //LoadDCTDCDistribution((char*)"db/dc/s12_20180519_run367.root");
    LoadDCTDCDistribution((char*)"db/dc/S012DCTDCDistribution509_10Be.root");

  }

  fCalibBDC1Hit->ReconstructData();
  fCalibBDC1Track->ReconstructData();
  fCalibBDC2Hit->ReconstructData();
  fCalibBDC2Track->ReconstructData();
  fCalibBPCHit->ReconstructData();
  fCalibFDC0Hit->ReconstructData();
  fCalibFDC0Track->ReconstructData();
  fCalibFDC1Hit->ReconstructData();
  fCalibFDC1Track->ReconstructData();
  fCalibFDC2Hit->ReconstructData();
  fCalibFDC2Track->ReconstructData();
  fCalibPDCHit->ReconstructData(); // << reads data
  fCalibPDCTrack->ReconstructData(); // << rebuild tracks

  //  sman				  = TArtStoreManager::Instance();

  TClonesArray		*bdc1trks	  = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC1Track");
  int			 bdc1ntr	  = 0;
  Double_t		 chi2x		  = 100000;
  Double_t		 chi2y		  = 100000;
  //  Double_t chi2x			  = InvalidNum();
  //  Double_t chi2y			  = InvalidNum();
  Double_t bdc1tr[6]			  = {NAN,NAN,NAN,NAN,NAN,NAN} ;	// x, a, y, b, chi2x, chi2y 

  if(bdc1trks){
    bdc1ntr				  = bdc1trks->GetEntries();
    for(int i=0;i<bdc1ntr&&(isnan(bdc1tr[0])||isnan(bdc1tr[2]));i++){
      TArtDCTrack	*trk		  = (TArtDCTrack *)bdc1trks->At(i);
      Double_t		 chi2		  = trk->GetChi2();
      Int_t		 ndf		  = trk->GetNDF();
      Double_t		 posx		  = trk->GetPosition(0);
      Double_t		 posy		  = trk->GetPosition(1);
      Double_t		 angx		  = trk->GetAngle(0);
      Double_t		 angy		  = trk->GetAngle(1);
      if(posx<-5000){
	//      if(!IsValidNum(posx)){
	if(chi2/(Double_t)ndf < chi2y){
	  //	if(!IsValidNum(chi2y) || chi2/(Double_t)ndf < chi2y){
	  chi2y				  = chi2/(Double_t)ndf;
	  bdc1tr[2]			  = posy; bdc1tr[3] = angy; bdc1tr[5] = chi2y;  
	}
      }
      else if(posy<-5000){
	//      else if(!IsValidNum(posy)){
	if(chi2/(Double_t)ndf < chi2x){
	  //	if(!IsValidNum(chi2x) || chi2/(Double_t)ndf < chi2x){
	  chi2x				  = chi2/(Double_t)ndf;
	  bdc1tr[0]			  = posx; bdc1tr[1] = angx; bdc1tr[4] = chi2x; 
	}
      }
    }
  }
  Double_t	 bdc1chi2 = sqrt(chi2x*chi2x+chi2y*chi2y);

  TClonesArray	*bdc2trks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC2Track");
  int		 bdc2ntr  = 0;
  chi2x			  = 100000;
  chi2y			  = 100000;
  //  chi2x		  = InvalidNum();
  //  chi2y		  = InvalidNum();
  Double_t bdc2tr[6]	  = {NAN,NAN,NAN,NAN,NAN,NAN};	// x, a, y, b, chi2x, chi2y 

  if(bdc2trks){
    bdc2ntr				  = bdc2trks->GetEntries();
    // tracks are already sorted w.r.t. chi2
    for(int i=0;i<bdc2ntr&&(isnan(bdc2tr[0])||isnan(bdc2tr[2]));i++){
      TArtDCTrack	*trk		  = (TArtDCTrack *)bdc2trks->At(i);
      Double_t		 chi2		  = trk->GetChi2();
      Int_t		 ndf		  = trk->GetNDF();
      Double_t		 posx		  = trk->GetPosition(0);
      Double_t		 posy		  = trk->GetPosition(1);
      Double_t		 angx		  = trk->GetAngle(0);
      Double_t		 angy		  = trk->GetAngle(1);
      if(posx<-5000){
	//      if(!IsValidNum(posx)){
	if(chi2/(Double_t)ndf < chi2y){
	  //	if(!IsValidNum(chi2y) || chi2/(Double_t)ndf < chi2y){
	  chi2y				  = chi2/(Double_t)ndf;
	  bdc2tr[2]			  = posy; bdc2tr[3] = angy; bdc2tr[5] = chi2y;  
	}
      }
      else if(posy<-5000){
	//      else if(!IsValidNum(posy)){
	if(chi2/(Double_t)ndf < chi2x){
	  //	if(!IsValidNum(chi2x) || chi2/(Double_t)ndf < chi2x){
	  chi2x				  = chi2/(Double_t)ndf;
	  bdc2tr[0]			  = posx; bdc2tr[1] = angx; bdc2tr[4] = chi2x;  
	}
      }
    }
  }
  Double_t		 bdc2chi2	  = sqrt(chi2x*chi2x+chi2y*chi2y);

  //===================================================== <<
  
  TClonesArray	*fdc0trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC0Track");
  int		 fdc0ntr  = 0;
  chi2x			  = 100000;
  chi2y			  = 100000;
  //  Double_t chi2x	  = InvalidNum();
  //  Double_t chi2y	  = InvalidNum();
  Double_t fdc0tr[6]	  = {NAN,NAN,NAN,NAN,NAN,NAN};	// x, a, y, b, chi2x, chi2y 
  if(fdc0trks){
    fdc0ntr				  = fdc0trks->GetEntries();
    for(int i=0;i<fdc0ntr&&(isnan(fdc0tr[0])||isnan(fdc0tr[2]));i++){
      TArtDCTrack	*trk  = (TArtDCTrack *)fdc0trks->At(i);
      Double_t		 chi2 = trk->GetChi2();
      Int_t		 ndf  = trk->GetNDF();
      Double_t		 posx = trk->GetPosition(0);
      Double_t		 posy = trk->GetPosition(1);
      Double_t		 angx = trk->GetAngle(0);
      Double_t		 angy = trk->GetAngle(1);
      if(posx<-5000){
	//      if(!IsValidNum(posx)){
	if(chi2/(Double_t)ndf < chi2y){
	  //	if(!IsValidNum(chi2y) || chi2/(Double_t)ndf < chi2y){
	  chi2y	    = chi2/(Double_t)ndf;
	  fdc0tr[2] = posy; fdc0tr[3] = angy; fdc0tr[5] = chi2y;  
	}
      }
      else if(posy<-5000){
	//      else if(!IsValidNum(posy)){
	if(chi2/(Double_t)ndf < chi2x){
	  //	if(!IsValidNum(chi2x) || chi2/(Double_t)ndf < chi2x){
	  chi2x	    = chi2/(Double_t)ndf;
	  fdc0tr[0] = posx; fdc0tr[1] = angx; fdc0tr[4] = chi2x; 
	}
      }
    }
  }
  Double_t		 fdc0chi2	  = sqrt(chi2x*chi2x+chi2y*chi2y);


  //===================================================== <<

  TClonesArray	*fdc1trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Track");
  int		 fdc1ntr  = 0;
  Double_t fdc1tr[6]	  = {NAN,NAN,NAN,NAN,NAN,NAN};	// x, a, y, b, chi2x, chi2y 
  Double_t	 fdc1chi2 = 100000;

  if(fdc1trks){
    fdc1ntr				  = fdc1trks->GetEntries();
    if(fdc1ntr>0){
      // tracks are sorted w.r.t. chi2
      TArtDCTrack	*trk  = (TArtDCTrack *)fdc1trks->At(0);
      Double_t		 chi2 = trk->GetChi2();
      Int_t		 ndf  = trk->GetNDF();
      Double_t		 posx = trk->GetPosition(0);
      Double_t		 posy = trk->GetPosition(1);
      Double_t		 angx = trk->GetAngle(0);
      Double_t		 angy = trk->GetAngle(1);
      fdc1tr[0]		      = posx; fdc1tr[1] = angx; fdc1tr[4] = chi2;   
      fdc1tr[2]		      = posy; fdc1tr[3] = angy;
      fdc1chi2		      = chi2;
    }
  }

  TClonesArray	*fdc2trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC2Track");
  int		 fdc2ntr  = 0;
  Double_t fdc2tr[6]	  = {NAN,NAN,NAN,NAN,NAN,NAN};	// x, a, y, b, chi2x, chi2y 
  Double_t	 fdc2chi2 = 100000;
  if(fdc2trks){
    fdc2ntr				  = fdc2trks->GetEntries();
    if(fdc2ntr>0){
      // tracks are sorted w.r.t. chi2
      TArtDCTrack	*trk  = (TArtDCTrack *)fdc2trks->At(0);
      Double_t		 chi2 = trk->GetChi2();
      Int_t		 ndf  = trk->GetNDF();
      Double_t		 posx = trk->GetPosition(0);
      Double_t		 posy = trk->GetPosition(1);
      Double_t		 angx = trk->GetAngle(0);
      Double_t		 angy = trk->GetAngle(1);
      fdc2tr[0]		      = posx; fdc2tr[1] = angx; fdc2tr[4] = chi2;   
      fdc2tr[2]		      = posy; fdc2tr[3] = angy;
      fdc2chi2		      = chi2;
    }
  }

  // PDC
  TClonesArray	*pdctrks = (TClonesArray *)sman->FindDataContainer("SAMURAIPDCTrack"); // << look for PDC data, calib
  int		 pdcntr	 = 0;
  Double_t pdctr[6]	 = {NAN,NAN,NAN,NAN,NAN,NAN};	// x, a, y, b, chi2x, chi2y 
  Double_t	 pdcchi2 = 100000;
  if(pdctrks){
    pdcntr				  = pdctrks->GetEntries();
    if(pdcntr>0){
      // tracks are sorted w.r.t. chi2
      TArtDCTrack	*trk  = (TArtDCTrack *)pdctrks->At(0); // << basic !
      Double_t		 chi2 = trk->GetChi2();
      Int_t		 ndf  = trk->GetNDF();
      Double_t		 posx = trk->GetPosition(0);
      Double_t		 posy = trk->GetPosition(1);
      Double_t		 angx = trk->GetAngle(0);
      Double_t		 angy = trk->GetAngle(1);
      pdctr[0]		      = posx; pdctr[1] = angx; pdctr[4] = chi2;   
      pdctr[2]		      = posy; pdctr[3] = angy;
      pdcchi2		      = chi2;
    }
  }


  //  TArtStoreManager * sman		  = TArtStoreManager::Instance();
  TClonesArray	*bpchits  = (TClonesArray *)sman->FindDataContainer("SAMURAIBPCHit");
  TClonesArray	*bdc1hits = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC1Hit");
  TClonesArray	*bdc2hits = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC2Hit");
  TClonesArray	*fdc0hits = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC0Hit");
  TClonesArray	*fdc1hits = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Hit");
  TClonesArray	*fdc2hits = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC2Hit");
  TClonesArray	*pdchits  = (TClonesArray *)sman->FindDataContainer("SAMURAIPDCHit");

  // for SBV/TGT/FOCUS image
  Double_t	sbvx = NAN;
  Double_t	sbvy = NAN;

  Double_t	tgtx = NAN;
  Double_t	tgty = NAN;
  Double_t	tgta = NAN;
  Double_t	tgtb = NAN;

  Double_t	focx = NAN;
  Double_t	focy = NAN;
  Double_t	foca = NAN;
  Double_t	focb = NAN;


  //  Double_t dist_BDCs			  = 1000;//mm
  //  Double_t dist_BDC1_SBV		  = 1444;//mm
  //  Double_t dist_BDC1_TGT		  = 1994;//mm
  //  Double_t dist_BDCs			  = 1000;//mm
  //  Double_t dist_BDC1_SBV		  = 120+880+250+75;//mm
  //  Double_t dist_BDC1_TGT		  = 120+880+250-12+189+525;//mm
  //  Double_t		 dist_BDC1_FOC	  = dist_BDC1_TGT-1000;//mm

  // SAMURAI SUMMER 2017 CAMPAIGN
  //  Double_t		 dist_BDCs	  = 1000;//mm
  //  Double_t		 dist_BDC1_SBV	  = dist_BDCs+558;//mm
  //  Double_t		 dist_BDC1_FOC	  = dist_BDCs+1173;//mm
  //  Double_t		 dist_BDC1_TGT	  = dist_BDC1_FOC+916;//mm
  // SAMURAI SUMMER 2018 CAMPAIGN

  Double_t	dist_BDCs     = 1000;	//mm
  Double_t	dist_BDC1_SBV = 2248.4;	//mm
  Double_t	dist_BDC1_FOC = 2443.4;	//mm
  Double_t	dist_BDC1_TGT = 2443.4;	//mm


  if( bdc1tr[0]>-1000 && bdc1tr[2]>-1000 && bdc2tr[0]>-1000 && bdc2tr[2]>-1000){
    sbvx = ( bdc2tr[0]-bdc1tr[0] )/dist_BDCs*dist_BDC1_SBV + bdc1tr[0] ;
    sbvy = ( bdc2tr[2]-bdc1tr[2] )/dist_BDCs*dist_BDC1_SBV + bdc1tr[2] ;

    tgtx = ( bdc2tr[0]-bdc1tr[0] )/dist_BDCs*dist_BDC1_TGT + bdc1tr[0] ;
    tgty = ( bdc2tr[2]-bdc1tr[2] )/dist_BDCs*dist_BDC1_TGT + bdc1tr[2] ;
    tgta = ( bdc2tr[0]-bdc1tr[0] )/dist_BDCs;	//rad
    tgtb = ( bdc2tr[2]-bdc1tr[2] )/dist_BDCs;	//rad

    focx = ( bdc2tr[0]-bdc1tr[0] )/dist_BDCs*dist_BDC1_FOC + bdc1tr[0] ;
    focy = ( bdc2tr[2]-bdc1tr[2] )/dist_BDCs*dist_BDC1_FOC + bdc1tr[2] ;
    foca = ( bdc2tr[0]-bdc1tr[0] )/dist_BDCs;	//rad
    focb = ( bdc2tr[2]-bdc1tr[2] )/dist_BDCs;	//rad
  }

  if(fAnaLoop->UsingAnaFile()){
    using namespace	 Analyser;
    using namespace	 WNum::SAMURAIDC;  

    // BPC
    fAnaLoop->Add(SAMURAIDC, ID,    BPC, BPC); 
    if(bpchits){
      int		 minentry[2];
      int		 mintraw[2];
      for(int i=0;i<2;++i){
	minentry[i]	       = -1;
	mintraw[i]	       = 100000;
      }
      for(int i=0;i<bpchits->GetEntries();i++){
	TArtDCHit	*hit   = (TArtDCHit *)bpchits->At(i);
	int		 layer = hit->GetLayer();
	int		 val   = hit->GetTDC();
	if(val<mintraw[layer]){
	  minentry[layer]      = i;
	  mintraw[layer]       = val;
	}
      }
      for(int i=0;i<2;++i){
	int		 j     = minentry[i];
	if(j<0) continue;
	TArtDCHit	*hit   = (TArtDCHit *)bpchits->At(j);	
	int		 layer = hit->GetLayer();
	int		 id    = layer + BPC*10+1;
	fAnaLoop->Add(SAMURAIDC, ID,      id, id); 
	fAnaLoop->Add(SAMURAIDC, Layer,   id, hit->GetLayer());
	fAnaLoop->Add(SAMURAIDC, WireID,  id, hit->GetWireID());
	fAnaLoop->Add(SAMURAIDC, TRaw,    id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TCal,    id, hit->GetTDC());
	//	fAnaLoop->Add(SAMURAIDC, TPos,   id, hit->GetPosition());
	fAnaLoop->Add(SAMURAIDC, WirePos, id, hit->GetWirePosition());
	fAnaLoop->Add(SAMURAIDC, Delta,   id, hit->GetWirePosition()/33.);	// 33 mm/% BigRIPS dispersion.
      }
    }

    // BDC1
    fAnaLoop->Add(SAMURAIDC, ID,    BDC1, BDC1); 
    fAnaLoop->Add(SAMURAIDC, XPos,  BDC1, bdc1tr[0]); 
    fAnaLoop->Add(SAMURAIDC, XAng,  BDC1, bdc1tr[1]); 
    fAnaLoop->Add(SAMURAIDC, YPos,  BDC1, bdc1tr[2]); 
    fAnaLoop->Add(SAMURAIDC, YAng,  BDC1, bdc1tr[3]); 
    fAnaLoop->Add(SAMURAIDC, Chi2,  BDC1, bdc1chi2);
    fAnaLoop->Add(SAMURAIDC, NTr,   BDC1, bdc1ntr); 
    if(bdc1hits){
      int		 minentry[8];
      int		 mintraw[8];
      for(int i=0;i<8;++i){
	minentry[i] = -1;
	mintraw[i]  = 100000;
      }
      for(int i=0;i<bdc1hits->GetEntries();i++){
	TArtDCHit	*hit		  = (TArtDCHit *)bdc1hits->At(i);
	int		 layer		  = hit->GetLayer();
	int		 val		  = hit->GetTDC();
	if(val<mintraw[layer]){
	  minentry[layer]		  = i;
	  mintraw[layer]		  = val;
	}
      }
      for(int i=0;i<8;++i){
	int		 j		  = minentry[i];
	if(j<0) continue;
	TArtDCHit	*hit		  = (TArtDCHit *)bdc1hits->At(j);	
	int		 layer		  = hit->GetLayer();
	int		 id		  = layer + BDC1*10+1;
	fAnaLoop->Add(SAMURAIDC, ID,     id, id); 
	fAnaLoop->Add(SAMURAIDC, Layer,  id, hit->GetLayer());
	fAnaLoop->Add(SAMURAIDC, WireID, id, hit->GetWireID());
	fAnaLoop->Add(SAMURAIDC, TRaw,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TCal,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TPos,   id, hit->GetPosition());
      }
    }

    // BDC2
    fAnaLoop->Add(SAMURAIDC, ID,    BDC2, BDC2); 
    fAnaLoop->Add(SAMURAIDC, XPos,  BDC2, bdc2tr[0]); 
    fAnaLoop->Add(SAMURAIDC, XAng,  BDC2, bdc2tr[1]); 
    fAnaLoop->Add(SAMURAIDC, YPos,  BDC2, bdc2tr[2]); 
    fAnaLoop->Add(SAMURAIDC, YAng,  BDC2, bdc2tr[3]); 
    fAnaLoop->Add(SAMURAIDC, Chi2,  BDC2, bdc2chi2);
    fAnaLoop->Add(SAMURAIDC, NTr,   BDC2, bdc2ntr); 
    if(bdc2hits){
      int		 minentry[8];
      int		 mintraw[8];
      for(int i=0;i<8;++i){
	minentry[i]			  = -1;
	mintraw[i]			  = 100000;
      }
      for(int i=0;i<bdc2hits->GetEntries();i++){
	TArtDCHit	*hit		  = (TArtDCHit *)bdc2hits->At(i);
	int		 layer		  = hit->GetLayer();
	int		 val		  = hit->GetTDC();
	if(val<mintraw[layer]){
	  minentry[layer]		  = i;
	  mintraw[layer]		  = val;
	}
      }
      for(int i=0;i<8;++i){
	int		 j		  = minentry[i];
	if(j<0) continue;
	TArtDCHit	*hit		  = (TArtDCHit *)bdc2hits->At(j);	
	int		 layer		  = hit->GetLayer();
	int		 id		  = layer + BDC2*10+1;
	fAnaLoop->Add(SAMURAIDC, ID,     id, id); 
	fAnaLoop->Add(SAMURAIDC, Layer,  id, hit->GetLayer());
	fAnaLoop->Add(SAMURAIDC, WireID, id, hit->GetWireID());
	fAnaLoop->Add(SAMURAIDC, TRaw,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TCal,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TPos,   id, hit->GetPosition());
      }
    }


    // FDC1
    fAnaLoop->Add(SAMURAIDC, ID,    FDC1, FDC1); 
    fAnaLoop->Add(SAMURAIDC, XPos,  FDC1, fdc1tr[0]); 
    fAnaLoop->Add(SAMURAIDC, XAng,  FDC1, fdc1tr[1]); 
    fAnaLoop->Add(SAMURAIDC, YPos,  FDC1, fdc1tr[2]); 
    fAnaLoop->Add(SAMURAIDC, YAng,  FDC1, fdc1tr[3]); 
    fAnaLoop->Add(SAMURAIDC, Chi2,  FDC1, fdc1chi2);
    fAnaLoop->Add(SAMURAIDC, NTr,   FDC1, fdc1ntr);
    if(fdc1hits){
      int		 minentry[14];
      int		 mintraw[14];
      for(int i=0;i<14;++i){
	minentry[i]			  = -1;
	mintraw[i]			  = 100000;
      }
      for(int i=0;i<fdc1hits->GetEntries();i++){
	TArtDCHit	*hit		  = (TArtDCHit *)fdc1hits->At(i);
	int		 layer		  = hit->GetLayer();
	int		 val		  = hit->GetTDC();
	if(val<mintraw[layer]){
	  minentry[layer]		  = i;
	  mintraw[layer]		  = val;
	}
      }
      for(int i=0;i<14;++i){
	int		 j		  = minentry[i];
	if(j<0) continue;
	TArtDCHit	*hit		  = (TArtDCHit *)fdc1hits->At(j);	
	int		 layer		  = hit->GetLayer();
	int		 id		  = layer + FDC1*10+1;
	fAnaLoop->Add(SAMURAIDC, ID,     id, id); 
	fAnaLoop->Add(SAMURAIDC, Layer,  id, hit->GetLayer());
	fAnaLoop->Add(SAMURAIDC, WireID, id, hit->GetWireID());
	fAnaLoop->Add(SAMURAIDC, TRaw,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TCal,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TPos,   id, hit->GetPosition());
      }
    }

    // FDC2
    fAnaLoop->Add(SAMURAIDC, ID,    FDC2, FDC2); 
    fAnaLoop->Add(SAMURAIDC, XPos,  FDC2, fdc2tr[0]); 
    fAnaLoop->Add(SAMURAIDC, XAng,  FDC2, fdc2tr[1]); 
    fAnaLoop->Add(SAMURAIDC, YPos,  FDC2, fdc2tr[2]); 
    fAnaLoop->Add(SAMURAIDC, YAng,  FDC2, fdc2tr[3]); 
    fAnaLoop->Add(SAMURAIDC, Chi2,  FDC2, fdc2chi2);
    fAnaLoop->Add(SAMURAIDC, NTr,   FDC2, fdc2ntr);
    if(fdc2hits){
      int		 minentry[14];
      int		 mintraw[14];
      for(int i=0;i<14;++i){
	minentry[i]			  = -1;
	mintraw[i]			  = 100000;
      }
      for(int i=0;i<fdc2hits->GetEntries();i++){
	TArtDCHit	*hit		  = (TArtDCHit *)fdc2hits->At(i);
	int		 layer		  = hit->GetLayer();
	int		 val		  = hit->GetTDC();
	if(val<mintraw[layer]){
	  minentry[layer]		  = i;
	  mintraw[layer]		  = val;
	}
      }
      for(int i=0;i<14;++i){
	int		 j		  = minentry[i];
	if(j<0) continue;
	TArtDCHit	*hit		  = (TArtDCHit *)fdc2hits->At(j);	
	int		 layer		  = hit->GetLayer();
	int		 id		  = layer + FDC2*10+11;
	fAnaLoop->Add(SAMURAIDC, ID,     id, id); 
	fAnaLoop->Add(SAMURAIDC, Layer,  id, hit->GetLayer());
	fAnaLoop->Add(SAMURAIDC, WireID, id, hit->GetWireID());
	fAnaLoop->Add(SAMURAIDC, TRaw,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TCal,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TPos,   id, hit->GetPosition());
      }
    }

    //FDC0    
    fAnaLoop->Add(SAMURAIDC, ID,    FDC0, FDC0); 
    fAnaLoop->Add(SAMURAIDC, XPos,  FDC0, fdc0tr[0]); 
    fAnaLoop->Add(SAMURAIDC, XAng,  FDC0, fdc0tr[1]); 
    fAnaLoop->Add(SAMURAIDC, YPos,  FDC0, fdc0tr[2]); 
    fAnaLoop->Add(SAMURAIDC, YAng,  FDC0, fdc0tr[3]); 
    fAnaLoop->Add(SAMURAIDC, Chi2,  FDC0, fdc0chi2);
    fAnaLoop->Add(SAMURAIDC, NTr,   FDC0, fdc0ntr);

    if(fdc0hits){
      int			 minentry[8];
      int			 mintraw[8];
      for(int i=0;i<8;++i){
	minentry[i]			  = -1;
	mintraw[i]			  = 100000;
      }
      for(int i=0;i<fdc0hits->GetEntries();i++){
	TArtDCHit	*hit		  = (TArtDCHit *)fdc0hits->At(i);
	int		 layer		  = hit->GetLayer();
	int		 val		  = hit->GetTDC();
	if(val<mintraw[layer]){
	  minentry[layer]		  = i;
	  mintraw[layer]		  = val;
	}
      }
      for(int i=0;i<8;++i){
	int		 j		  = minentry[i];
	if(j<0) continue;
	TArtDCHit	*hit		  = (TArtDCHit *)fdc0hits->At(j);	
	int		 layer		  = hit->GetLayer();
	int		 id		  = layer + FDC0*10+21;
	fAnaLoop->Add(SAMURAIDC, ID,     id, id); 
	fAnaLoop->Add(SAMURAIDC, Layer,  id, hit->GetLayer());
	fAnaLoop->Add(SAMURAIDC, WireID, id, hit->GetWireID());
	fAnaLoop->Add(SAMURAIDC, TRaw,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TCal,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TPos,   id, hit->GetPosition());
      }
    }


    // PDC
    fAnaLoop->Add(SAMURAIDC, ID,    PDC, PDC); 
    fAnaLoop->Add(SAMURAIDC, XPos,  PDC, pdctr[0]); 
    fAnaLoop->Add(SAMURAIDC, XAng,  PDC, pdctr[1]); 
    fAnaLoop->Add(SAMURAIDC, YPos,  PDC, pdctr[2]); 
    fAnaLoop->Add(SAMURAIDC, YAng,  PDC, pdctr[3]); 
    fAnaLoop->Add(SAMURAIDC, Chi2,  PDC, pdcchi2);
    fAnaLoop->Add(SAMURAIDC, NTr,   PDC, pdcntr);
    if(0){//pdchits){
      int		 minentry[14];
      int		 mintraw[14];
      for(int i=0;i<14;++i){
	minentry[i]			  = -1;
	mintraw[i]			  = 100000;
      }
      for(int i=0;i<pdchits->GetEntries();i++){
	TArtDCHit	*hit		  = (TArtDCHit *)pdchits->At(i);
	int		 layer		  = hit->GetLayer();
	int		 val		  = hit->GetTDC();
	if(val<mintraw[layer]){
	  minentry[layer]		  = i;
	  mintraw[layer]		  = val;
	}
      }
      for(int i=0;i<14;++i){
	int		 j		  = minentry[i];
	if(j<0) continue;
	TArtDCHit	*hit		  = (TArtDCHit *)pdchits->At(j);	
	int		 layer		  = hit->GetLayer();
	int		 id		  = layer + PDC*10+11;
	fAnaLoop->Add(SAMURAIDC, ID,     id, id); 
	fAnaLoop->Add(SAMURAIDC, Layer,  id, hit->GetLayer());
	fAnaLoop->Add(SAMURAIDC, WireID, id, hit->GetWireID());
	fAnaLoop->Add(SAMURAIDC, TRaw,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TCal,   id, hit->GetTDC());
	fAnaLoop->Add(SAMURAIDC, TPos,   id, hit->GetPosition());
      }
    }


    // SBV, TGT image
    fAnaLoop->Add(SAMURAIDC, ID,    Image, Image); 
    fAnaLoop->Add(SAMURAIDC, SBVX,  Image, sbvx); 
    fAnaLoop->Add(SAMURAIDC, SBVY,  Image, sbvy); 
    fAnaLoop->Add(SAMURAIDC, TGTX,  Image, tgtx); 
    fAnaLoop->Add(SAMURAIDC, TGTY,  Image, tgty); 
    fAnaLoop->Add(SAMURAIDC, TGTA,  Image, tgta); 
    fAnaLoop->Add(SAMURAIDC, TGTB,  Image, tgtb); 
    // commented 18.05.18
    //fAnaLoop->Add(SAMURAIDC, FOCX,  Image, focx); 
    //fAnaLoop->Add(SAMURAIDC, FOCY,  Image, focy); 
    //fAnaLoop->Add(SAMURAIDC, FOCA,  Image, foca); 
    //fAnaLoop->Add(SAMURAIDC, FOCB,  Image, focb); 
  }

  return true;
}
//___________________________________________________________________________

void TAlEncSAMURAIDCExample::Destruct()
{
  delete fCalibBPCHit; fCalibBPCHit	  = 0;
  delete fCalibBDC1Hit; fCalibBDC1Hit	  = 0;
  delete fCalibBDC2Hit; fCalibBDC2Hit	  = 0;
  delete fCalibFDC0Hit; fCalibFDC0Hit	  = 0;
  delete fCalibFDC1Hit; fCalibFDC1Hit	  = 0;
  delete fCalibFDC2Hit; fCalibFDC2Hit	  = 0;
  delete fCalibBDC1Track; fCalibBDC1Track = 0;
  delete fCalibBDC2Track; fCalibBDC2Track = 0;
  delete fCalibFDC0Track; fCalibFDC0Track = 0;
  delete fCalibFDC1Track; fCalibFDC1Track = 0;
  delete fCalibFDC2Track; fCalibFDC2Track = 0;
  delete fCalibPDCTrack; fCalibPDCTrack	  = 0;

  if(samuraiparameters) samuraiparameters->Delete();
  samuraiparameters			  = 0;
}
//___________________________________________________________________________

const char* TAlEncSAMURAIDCExample::ClassName() const
{
  return "TAlEncSAMURAIDCExample";
}

TAlEncSAMURAIDCExampleFactory::TAlEncSAMURAIDCExampleFactory()
  : TAlEncFactory()
{;}

TAlEncSAMURAIDCExampleFactory::~TAlEncSAMURAIDCExampleFactory()
{;}

TAlEncSub* TAlEncSAMURAIDCExampleFactory::CreateEnc()
{
  return new TAlEncSAMURAIDCExample;
}

