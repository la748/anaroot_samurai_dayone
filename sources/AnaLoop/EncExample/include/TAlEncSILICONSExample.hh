#ifndef TALENCSILICONSEXAMPLE_HH
#define TALENCSILICONSEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TH2.h"

#include "TArtSILICONSParameters.hh"
#include "TArtCalibSILICONS.hh"

class TAlEncSILICONSExample : public TAlEncSub
{
public:
  TAlEncSILICONSExample();
  virtual ~TAlEncSILICONSExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  TH2 *hSi_adc; // adc distribution
  TH2 *hSi_tdc; // tdc distribution
  Double_t silicon_thr ;
  
  TArtSILICONSParameters *siliconsparameters;
  TArtCalibSILICONS *calibsilicons;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncSILICONSExample,1);
};

class TAlEncSILICONSExampleFactory : public TAlEncFactory
{
public:
  TAlEncSILICONSExampleFactory();
  virtual ~TAlEncSILICONSExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncSILICONSExampleFactory,1);
};

#endif
