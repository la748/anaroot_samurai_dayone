#ifndef TALENCNEULANDLEXAMPLE_HH
#define TALENCNEULANDLEXAMPLE_HH

#include "TAlEncSub.hh"
#include "TAlEncFactory.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtCalibNeuLAND.hh"
#include "TArtCalibNeuLANDVETO.hh"

#include "TNamed.h"
#include "TMath.h"



class TArtStoreManager;

class TAlEncNeuLANDExample : public TAlEncSub
{
public:
  TAlEncNeuLANDExample();
  virtual ~TAlEncNeuLANDExample();

protected:
  virtual void Construct();
  virtual void PreCalculate();
  virtual bool Calculate();
  virtual void Destruct();

  virtual void Fill();

private:
  TArtSAMURAIParameters* samuraiparameters;
  TArtCalibNeuLAND *fCalibNeuLAND;
  TArtCalibNeuLANDVETO *fCalibNeuLANDVETO;
  TArtStoreManager* fStoreManager;

public:
  static void SetFilterFlag(int filterflag);
  static int fFilterFlag;

public:
  virtual const char* ClassName() const;

  ClassDef(TAlEncNeuLANDExample,1);
};

class TAlEncNeuLANDExampleFactory : public TAlEncFactory
{
public:
  TAlEncNeuLANDExampleFactory();
  virtual ~TAlEncNeuLANDExampleFactory();

  TAlEncSub* CreateEnc();

  ClassDef(TAlEncNeuLANDExampleFactory,1);
};

#endif
