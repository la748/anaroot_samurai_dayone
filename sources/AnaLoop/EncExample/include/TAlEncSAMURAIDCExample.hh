#ifndef TALENCSAMURAIDCEXAMPLE_HH
#define TALENCSAMURAIDCEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TH2.h"
#include "TNtuple.h"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibSAMURAI.hh"
#include <iostream>

#include "TClonesArray.h"
#include "TMath.h"
#include "TFile.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtCalibBPCHit.hh"
#include "TArtCalibBDC1Hit.hh"
#include "TArtCalibBDC2Hit.hh"
#include "TArtCalibFDC0Hit.hh"
#include "TArtCalibFDC1Hit.hh"
#include "TArtCalibFDC2Hit.hh"
#include "TArtCalibPDCHit.hh"
#include "TArtCalibBDC1Track.hh"
#include "TArtCalibBDC2Track.hh"
#include "TArtCalibFDC0Track.hh"
#include "TArtCalibFDC1Track.hh"
#include "TArtCalibFDC2Track.hh"
#include "TArtCalibPDCTrack.hh"
#include "TArtEventInfo.hh"

#include "TArtDCHit.hh"
#include "TArtDCTrack.hh"
#include "TArtBigRIPSParameters.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtStoreManager.hh"

class TAlEncSAMURAIDCExample : public TAlEncSub
{
public:
  TAlEncSAMURAIDCExample();
  virtual ~TAlEncSAMURAIDCExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  virtual void LoadDCTDCDistribution(char * file);

private:
  TArtSAMURAIParameters* samuraiparameters;
  TArtCalibBPCHit *	fCalibBPCHit;
  TArtCalibBDC1Hit *	fCalibBDC1Hit;
  TArtCalibBDC2Hit *	fCalibBDC2Hit;
  TArtCalibFDC0Hit *	fCalibFDC0Hit;
  TArtCalibFDC1Hit *	fCalibFDC1Hit;
  TArtCalibFDC2Hit *	fCalibFDC2Hit;
  TArtCalibPDCHit *	fCalibPDCHit;

  TArtCalibBDC1Track *	fCalibBDC1Track;
  TArtCalibBDC2Track *	fCalibBDC2Track;
  TArtCalibFDC0Track *	fCalibFDC0Track;
  TArtCalibFDC1Track *	fCalibFDC1Track;
  TArtCalibFDC2Track *	fCalibFDC2Track;
  TArtCalibPDCTrack *	fCalibPDCTrack;

  Double_t		dist_BDCs;
  Double_t		dist_BDC1_SBV;
  Double_t		dist_BDC1_TGT;
  
  enum {
    BPC	  = 1,
    BDC1  = 2,
    BDC2  = 3,
    FDC0  = 6,
    FDC1  = 4,
    FDC2  = 5,
    PDC	  = 7,
    Image = 10,
  };

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncSAMURAIDCExample,1);
};

class TAlEncSAMURAIDCExampleFactory : public TAlEncFactory
{
public:
  TAlEncSAMURAIDCExampleFactory();
  virtual ~TAlEncSAMURAIDCExampleFactory();

  TAlEncSub*		CreateEnc();

  ClassDef(TAlEncSAMURAIDCExampleFactory,1);
};

#endif
