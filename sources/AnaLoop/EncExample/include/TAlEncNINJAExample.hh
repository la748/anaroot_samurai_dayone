#ifndef TALENCNINJALEXAMPLE_HH
#define TALENCNINJALEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibNINJA.hh"

#include "TNamed.h"

class TArtStoreManager;

class TAlEncNINJAExample : public TAlEncSub
{
public:
  TAlEncNINJAExample();
  virtual ~TAlEncNINJAExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  virtual void Fill();

private:
  TArtSAMURAIParameters* samuraiparameters;
  TArtCalibNINJA *calibNINJA;

  TArtStoreManager* fStoreManager;

public:
  static void SetFilterFlag(int filterflag);
  static int fFilterFlag;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncNINJAExample,1);
};

class TAlEncNINJAExampleFactory : public TAlEncFactory
{
public:
  TAlEncNINJAExampleFactory();
  virtual ~TAlEncNINJAExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncNINJAExampleFactory,1);
};

#endif
