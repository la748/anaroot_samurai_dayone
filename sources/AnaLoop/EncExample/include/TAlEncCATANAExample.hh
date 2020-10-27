#ifndef TALENCCATANAEXAMPLE_HH
#define TALENCCATANAEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TROOT.h"
#include "TH2.h"

#include "TArtCATANAParameters.hh"
#include "TArtCalibCATANA.hh"

class TAlEncCATANAExample : public TAlEncSub
{
public:
  TAlEncCATANAExample();
  virtual ~TAlEncCATANAExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  TH2 *hcsi_adc; // adc distribution
  TH2 *hcsi_tdc; // adc distribution

  TArtCATANAParameters *catanaparameters;
  TArtCalibCATANA *calibcatana;

private:
  Double_t BetaValueForDopplerCorrect ;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncCATANAExample,1);
};

class TAlEncCATANAExampleFactory : public TAlEncFactory
{
public:
  TAlEncCATANAExampleFactory();
  virtual ~TAlEncCATANAExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncCATANAExampleFactory,1);
};

#endif
