#ifndef _TARTTKE_H_
#define _TARTTKE_H_

#include "TArtDataObject.hh"

//#define NUM_TKE_CHANNEL 4 // for Photo diode
#define NUM_TKE_CHANNEL 1

class TArtTKE: public TArtDataObject {

 public:
  TArtTKE(){Clear();}
  virtual ~TArtTKE(){}
  virtual void Clear(){
    for(Int_t i=0;i<NUM_TKE_CHANNEL;i++){fADCArray[i]=-9999;fEnergyArray[i]=-9999;fTDCArray[i]=-9999;fTimingArray[i]=-9999;}
    fADC=-9999;fEnergy=-9999;fTDC=-9999;fTiming=-9999;numadc = 0;
  }


  //
  virtual void SetNumADC(Int_t val){numadc = val;}
  virtual Int_t GetNumADC(){return numadc;}

  // function to set raw data
  virtual void  SetRawADC(Int_t val){fADC = val;}
  virtual void  SetRawTDC(Int_t val){fTDC = val;}
  virtual void  SetRawADC(Int_t i, Int_t val){fADCArray[i] = val;}
  virtual void  SetRawTDC(Int_t i, Int_t val){fTDCArray[i] = val;}
  // function to get raw data
  virtual Int_t GetRawADC(){return fADC;}
  virtual Int_t GetRawTDC(){return fTDC;}
  virtual Int_t GetRawADC(Int_t i){return fADCArray[i];}
  virtual Int_t GetRawTDC(Int_t i){return fTDCArray[i];}

  // function to set calibrated data
  virtual void  SetEnergy(Double_t val){fEnergy = val;}
  virtual void  SetTiming(Double_t val){fTiming = val;}
  virtual void  SetEnergy(Int_t i, Double_t val){fEnergyArray[i] = val;}
  virtual void  SetTiming(Int_t i, Double_t val){fTimingArray[i] = val;}
  // function to get calibrated data
  virtual Double_t GetEnergy(){return fEnergy;}
  virtual Double_t GetTiming(){return fTiming;}
  virtual Double_t GetEnergy(Int_t i){return fEnergyArray[i];}
  virtual Double_t GetTiming(Int_t i){return fTimingArray[i];}

 private:

  // number of adc
  Int_t numadc;

  // raw value
  Int_t fADC;  
  Int_t fTDC;  
  // calibrated value
  Double_t fEnergy;
  Double_t fTiming;

  // raw value for array of adc and tdc
  Int_t fADCArray[NUM_TKE_CHANNEL];  
  Int_t fTDCArray[NUM_TKE_CHANNEL];  
  // calibrated value for array of energy and timing
  Double_t fEnergyArray[NUM_TKE_CHANNEL];
  Double_t fTimingArray[NUM_TKE_CHANNEL];

  ClassDef(TArtTKE,1);

};
#endif
