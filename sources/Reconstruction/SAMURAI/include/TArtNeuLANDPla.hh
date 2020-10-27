#ifndef _TARTNEULANDPLA_H_
#define _TARTNEULANDPLA_H_

#include "TString.h"

#include <iostream>

#include "TArtDataObject.hh"

class TArtNeuLANDPla : public TArtDataObject
{
public:
  TArtNeuLANDPla()
    : TArtDataObject(),
      fLayer(-1), fBarID(-1), fPos(-1), fZPos(-1)
  {
    for(int i=0;i<2;i++){
      fQRaw[i] = -1;
      fQCal[i] = -1;
      fTac[i] = -1;
      fTCycle[i] = -1;
      fTRaw[i] = -1;
      fTCal[i] = -1;
      isFired[i] = kFALSE;
    }
  }
  virtual ~TArtNeuLANDPla(){}

  virtual void SetLayer(Int_t val){fLayer = val;}
  virtual void SetBarID(Int_t val){fBarID = val;}

  virtual void SetQRaw(Int_t i, Int_t val){fQRaw[i] = val;}
  virtual void SetQCal(Int_t i, Double_t val){fQCal[i] = val;}
  virtual void SetTCycle(Int_t i, Int_t val){fTCycle[i] = val;}
  virtual void SetTac(Int_t i, Int_t val){fTac[i] = val;}
  virtual void SetTacRef(Int_t i, Int_t val){fTacRef[i] = val;}
  virtual void SetTRaw(Int_t i, Double_t val){fTRaw[i] = val;} 
  virtual void SetTCal(Int_t i, Double_t val){fTCal[i] = val;}
  virtual void SetTacquilaID(Int_t sam, Int_t gtb, Int_t mod, Int_t ch) {
    fsam = sam;
    fgtb = gtb;
    fmod = mod;
    fch = ch;
  }

  virtual void SetFired(int i) {isFired[i] = kTRUE;}
  virtual void SetPos(Double_t val){fPos = val;}
  virtual void SetZPos(Double_t val){fZPos = val;}

  virtual Int_t    GetLayer()         const {return fLayer;}
  virtual Int_t    GetBarID()         const {return fBarID;}
  virtual Int_t    GetQRaw  (Int_t i) const {return fQRaw[i];}
  virtual Double_t GetQCal  (Int_t i) const {return fQCal[i];}
  virtual Int_t    GetTCycle(Int_t i) const {return fTCycle[i];}
  virtual Int_t    GetTac   (Int_t i) const {return fTac[i];}
  virtual Int_t    GetTacRef(Int_t i) const {return fTacRef[i];}
  virtual Double_t GetTRaw  (Int_t i) const {return fTRaw[i];}
  virtual Double_t GetTCal  (Int_t i) const {return fTCal[i];}
  virtual void     GetTacquilaID(Int_t &sam, Int_t &gtb, Int_t &mod, Int_t &ch) {
    sam = fsam;
    gtb = fgtb;
    mod = fmod;
    ch = fch;
  }

  virtual Bool_t   GetFired(int i) const {return isFired[i];}
  virtual Bool_t   GetBothFired() const {return isFired[0] && isFired[1];}
  virtual Double_t GetPos() const {return fPos;}
  // position in 2d, size of bar is 5x5x250 cm3
  virtual Double_t GetX() const {return fLayer%2 == 0 ? fPos : (fBarID-24.5)*50;}
  virtual Double_t GetY() const {return fLayer%2 == 1 ? fPos : (fBarID-24.5)*50;}
  virtual Double_t GetZ() const {return fZPos;}

  virtual void PrintData(){
    std::cout << "Layer: "       << fLayer   << std::endl;
    std::cout << "Bar ID: "      << fBarID   << std::endl;

    for(int i=0;i<2;i++){
      std::cout << "QRaw"<<i<<": " << fQRaw[i]          << std::endl;
      std::cout << "QCal"<<i<<": " << fQCal[i]          << std::endl;
      std::cout << "Tac"<<i<<": " << fTac[i]        << std::endl;
      std::cout << "TacRef"<<i<<": " << fTacRef[i]       << std::endl;
      std::cout << "TCycle"<<i<<": " << fTCycle[i]    << std::endl;
      std::cout << "TRaw"<<i<<": " << fTRaw[i]   << std::endl;
      std::cout << "TCal"<<i<<": " << fTCal[i]   << std::endl;
    }

    std::cout << "Pos: "   << fPos << std::endl;
    std::cout << std::endl;
    return;
  }

private:
  Int_t    fLayer;  
  Int_t    fBarID;  

  Int_t    fQRaw[2];
  Double_t fQCal[2];

  Int_t    fTac[2];
  Int_t    fTacRef[2]; // reference tac, Tac 16
  Int_t    fTCycle[2];
  Double_t fTRaw[2]; // traw = tac1 (calibrated in ns) + 25*cnt - t17 (calibrated in ns)
  Double_t fTCal[2];

  Bool_t   isFired[2];
  Double_t fPos;

  Double_t fZPos;

  Int_t fsam;
  Int_t fgtb;
  Int_t fmod;
  Int_t fch;

  ClassDef(TArtNeuLANDPla,1);
};
#endif

