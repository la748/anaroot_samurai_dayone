#ifndef _TARTCATANACSIPARA_H_
#define _TARTCATANACSIPARA_H_

#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

class TArtCATANACsIPara : public TObject {

 public:
  TArtCATANACsIPara(Int_t id = -1, Int_t ch=-1,const TString& detname = "",
		    Int_t fpl = -1, Int_t frame=-1, Int_t layer=-1,const Double_t* detpos = 0,
		    Double_t tcal = -1, Double_t toff = -1,
		    Double_t qcal = -1, Double_t qped = -1
		    )
    : TObject(), fID(id),fChannel(ch), fDetName(detname), fFpl(fpl), fFrame(frame),fLayer(layer),
      fTCal(tcal), fTOff(toff), fQCal(qcal), fQPed(qped)
  {
    if(detpos) for(Int_t i=0;i<3;i++)fDetPos[i] = detpos[i];
    else for(Int_t i=0;i<3;i++)fDetPos[i] = -10000;
  }
  ~TArtCATANACsIPara(){;}

  void SetMap(Int_t tgeo, Int_t tch, Int_t qgeo, Int_t qch) {
    fMapADC.SetMap(fFpl,CATANAQ,qgeo,qch); 
    fMapTDC.SetMap(fFpl,CATANAT,tgeo,tch); 
  }

  void SetID(Int_t val){ fID = val;}
  void SetChannel(Int_t val){ fChannel = val;}
  void SetDetectorName(const TString& val){fDetName = val;}
  void SetFpl(Int_t val){fFpl = val;}
  void SetFrame(Int_t val){fFrame = val;}
  void SetDetPos(const Double_t* val){
    for(Int_t i=0;i<3;i++)fDetPos[i] = val[i];
  }
  void SetDetPos(Double_t val, Int_t i){fDetPos[i] = val;}
  void SetTCal(Double_t val){fTCal = val;}
  void SetTOff(Double_t val){fTOff = val;}
  void SetQCal(Double_t val){fQCal = val;}
  void SetQPed(Double_t val){fQPed = val;}

  Int_t GetID() const { return fID; }
  Int_t GetChannel() const { return fChannel; }
  const TString& GetDetectorName() const { return fDetName; }
  Int_t GetFpl() const { return fFpl; }
  Int_t GetFrame() const { return fFrame; }
  Int_t GetLayer() const { return fLayer; }
  Double_t GetTCal() const { return fTCal; } 
  Double_t GetTOffset() const { return fTOff; } 
  Double_t GetQCal() const { return fQCal; } 
  Double_t GetQPed() const { return fQPed; } 
  const Double_t* GetDetPos() const { return fDetPos; } 
  Double_t GetDetPos(Int_t i) const { return fDetPos[i]; } 

  TArtRIDFMap * GetTDCMap() { return &fMapTDC;  }
  TArtRIDFMap * GetADCMap() { return &fMapADC;  }

  friend std::ostream & operator << (std::ostream& out, const TArtCATANACsIPara& p) {
    out << "ID: " << p.fID << ", ";
    out << "Channel: " << p.fChannel << ", ";
    out << "DetectorName: " << p.fDetName << ", ";
    out << "Focal Plane: " << p.fFpl << ", ";
    out << "Frame: " << p.fFrame << ", ";
    out << "Layer: " << p.fLayer << ", ";
    out << "Time Calib: " << p.fTCal << ", ";
    out << "Time Offset: " << p.fTOff << ", ";
    out << "QDC Calib: " << p.fQCal << ", ";
    out << "QDC pedestal: " << p.fQPed << ", ";
    out << "Position: " <<p.fDetPos[0]<< " " 
	<<p.fDetPos[1]<< " " <<p.fDetPos[2]<< ", " << std::endl;
    out << "Map of ADC: "  << p.fMapADC;
    out << "Map of TDC: "  << p.fMapTDC;
    return out;
  }

 private:
  Int_t     fID;
  Int_t     fChannel;
  TString   fDetName;
  Int_t     fFpl;
  Int_t     fFrame;
  Int_t     fLayer;
  Double_t  fDetPos[3];
  Double_t  fTCal;
  Double_t  fTOff;
  Double_t  fQCal;
  Double_t  fQPed;

  TArtRIDFMap fMapTDC;
  TArtRIDFMap fMapADC;

  ClassDef(TArtCATANACsIPara,1);

};

#endif
