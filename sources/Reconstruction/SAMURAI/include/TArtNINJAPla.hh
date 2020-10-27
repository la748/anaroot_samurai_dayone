#ifndef _TARTNINJAPLA_H_
#define _TARTNINJAPLA_H_

#include "TString.h"

#include <iostream>

#include "TArtDataObject.hh"

class TArtNINJAPla : public TArtDataObject
{
public:
  TArtNINJAPla()
    : TArtDataObject(){
    fID=-1;
    fFpl=-1;
    fDetName="";
    fLayer=-1;
    fSubLayer=-1; 
    fHit=0;
    for(unsigned int i = 0 ; i < 2 ; i++){
      fT_LERaw[i]=-1;
      fT_TERaw[i]=-1;
      fT_LECal[i]=-1;
      fT_TECal[i]=-1;
      fTOTCal[i]=-1;
      fQCal[i]=-1;
    }
    
    fQAveCal=-1;
    fLogQAveCal=-1;
    fT_LEAve=-1;
    fT_TEAve=-1;
    fT_LEAveT0=-1;
    fT_TEAveT0=-1;
    for(Int_t i=0; i<3; ++i) {
      fDetPos[i] = -90000;
      fPos[i] = -90000;
    }
  }
  virtual ~TArtNINJAPla(){;}

  enum DetDirectionDef { Vert, Horiz};

public:
  Int_t    fID;
  Int_t    fFpl;
  TString  fDetName;
  Int_t    fLayer;
  Int_t    fSubLayer;  
  Double_t fDetPos[3];
  Int_t    fHit;
  // Leading edge and Trailing edge time Raw
  Int_t fT_LERaw[2];
  Int_t fT_TERaw[2];
  // Leading edge and Trailing edge time Calibrated
  Double_t fT_LECal[2];
  Double_t fT_TECal[2];
  // Calibrated Time over Threshold
  Double_t fTOTCal[2];
  // Charge calculated based on the TOT
  Double_t fQCal[2];
  // Average Charge
  Double_t fQAveCal;
  // Log
  Double_t fLogQAveCal;
  // Average time on Leading Edge
  Double_t fT_LEAve;
   // Average time on Trailing Edge
  Double_t fT_TEAve;
  // Average time on Leading Edge 
  Double_t fT_LEAveT0;
   // Average time on Trailing Edge
  Double_t fT_TEAveT0;
  // Position of the hit
  Double_t fPos[3];

  DetDirectionDef fDetDirect;

  ClassDef(TArtNINJAPla,1);
};
#endif

