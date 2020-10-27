#ifndef TALENCHIMELEXAMPLE_HH
#define TALENCHIMELEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibHIME.hh"

#include "TNamed.h"

class TArtStoreManager;

class TAlEncHIMEExample : public TAlEncSub
{
public:
  TAlEncHIMEExample();
  virtual ~TAlEncHIMEExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  virtual void Fill();

private:
  TArtSAMURAIParameters* samuraiparameters;
  TArtCalibHIME *calibHIME;

  TArtStoreManager* fStoreManager;

public:
  static void SetFilterFlag(int filterflag);
  static int fFilterFlag;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncHIMEExample,1);
};

class TAlEncHIMEExampleFactory : public TAlEncFactory
{
public:
  TAlEncHIMEExampleFactory();
  virtual ~TAlEncHIMEExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncHIMEExampleFactory,1);
};

#endif
