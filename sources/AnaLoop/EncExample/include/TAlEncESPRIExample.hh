#ifndef TALENCESPRIEXAMPLE_HH
#define TALENCESPRIEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TH2.h"
#include "TNtuple.h"

#include <iostream>

#include "TClonesArray.h"
#include "TMath.h"
#include "TFile.h"

#include "TArtAnaLoop.hh"
#include "TArtAnaLoopManager.hh"
#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "TArtBDC.hh"
#include "TArtBLD.hh"
#include "TArtCalibBDC.hh"
#include "TArtCalibBLD.hh"
#include "TArtCalibDia.hh"
#include "TArtCalibFADC.hh"
#include "TArtCalibPlas.hh"
#include "TArtCalibRDC.hh"
#include "TArtCalibRNaI.hh"
#include "TArtCalibTDCHit.hh"
#include "TArtDia.hh"
#include "TArtESPRIParameters.hh"
#include "TArtFADC.hh"
#include "TArtFADCPara.hh"
#include "TArtPlas.hh"
#include "TArtPlasPara.hh"
#include "TArtRDC.hh"
#include "TArtRNaI.hh"
#include "TArtRNaIPara.hh"
#include "TArtTDCHit.hh"
#include "TArtTDCHitPara.hh"
#include "TArtUserParameters.hh"


#include "TArtEventInfo.hh"

#include "EAnalyser.hh"
#include "EWNum.hh"

#include "TArtStoreManager.hh"


class TAlEncESPRIExample : public TAlEncSub
{
public:
  TAlEncESPRIExample();
  virtual ~TAlEncESPRIExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  virtual void LoadDCTDCDistribution(char * file);

private:
  TArtESPRIParameters* ESPRIparameters;
  TArtUserParameters *ESPRIUserPara ;
  TArtCalibRNaI *fCalibESPRI_NaI ;
  TArtCalibTDCHit *fCalibESPRI_tdc ;
  TArtCalibRDC *fCalibESPRI_rdc ;
  TArtCalibBDC *fCalibESPRI_bdc ;
  TArtCalibFADC *fCalibESPRI_fadc ;
  TArtCalibPlas *fCalibESPRI_plas ;
  Double_t NaIMergeID[30];
  Double_t PlaMergeID[4];

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncESPRIExample,1);
};

class TAlEncESPRIExampleFactory : public TAlEncFactory
{
public:
  TAlEncESPRIExampleFactory();
  virtual ~TAlEncESPRIExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncESPRIExampleFactory,1);
};

#endif
