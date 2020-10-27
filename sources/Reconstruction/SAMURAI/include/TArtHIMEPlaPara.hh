
#ifndef _TARTHIMEPLAPARA_H_
#define _TARTHIMEPLAPARA_H_

#include "TArtMap.hh"

class TArtSAMURAIParameters;

class TArtHIMEPlaPara : public TObject
{
  friend class TArtSAMURAIParameters;

public:
  TArtHIMEPlaPara(Int_t id = -1, const TString& detname = "",
		    Int_t fpl = -1, Int_t layer = -1, Int_t sublayer = -1, const Double_t* detpos = 0, 
		    Double_t tucal = 1, Double_t tuoff = 0,
		    Double_t tuslw = 0, const Double_t* tuslwlog = 0,
		    Double_t qucal = 1, Double_t quped = 0,
		    Double_t tdcal = 1, Double_t tdoff = 0,
		    Double_t tdslw = 0, const Double_t* tdslwlog = 0,
		    Double_t qdcal = 1, Double_t qdped = 0,
		    Double_t dtcal = 1, Double_t dtoff = 0, Double_t taveoff = 0,
		    Double_t qavecal = 1, Double_t qavecalatt = 0, Double_t qavecalatt2 = 0,
		    Int_t ignore = 0)
    : TObject(),
      fID(id), fDetName(detname),
      fFpl(fpl), fLayer(layer), fSubLayer(sublayer),
      fDTCal(dtcal), fDTOff(dtoff), fTAveOff(taveoff),
      fQAveCal(qavecal), fQAveCalAtt(qavecalatt), fQAveCalAtt2(qavecalatt2),
      fIgnore(ignore)
  {
      fTCal[0] = tucal; fTCal[1] = tdcal; 
      fTOff[0] = tuoff; fTOff[1] = tdoff;
      fTSlw[0] = tuslw; fTSlw[1] = tdslw;
      fQCal[0] = qucal; fQCal[1] = qdcal; 
      fQPed[0] = quped; fQPed[1] = qdped;
      
      if(detpos) for(Int_t i=0; i<3; ++i) fDetPos[i] = detpos[i];
      else for(Int_t i=0; i<3; ++i) fDetPos[i] = -90000;
      if(tuslwlog) for(Int_t i=0; i<5; ++i) fTSlwLog[0][i] = tuslwlog[i];
      else  for(Int_t i=0; i<5; ++i) fTSlwLog[0][i] = 0;
      if(tdslwlog) for(Int_t i=0; i<5; ++i) fTSlwLog[1][i] = tdslwlog[i];
      else  for(Int_t i=0; i<5; ++i) fTSlwLog[1][i] = 0;
  }
  ~TArtHIMEPlaPara(){;}

  enum DetDirectionDef { Vert, Horiz};

  void SetID(Int_t val){fID = val;}
  void SetDetectorName(const TString& val){fDetName = val;}
  void SetFpl(Int_t val){fFpl = val;}
  void SetLayer(Int_t val){fLayer = val;}
  void SetSubLayer(Int_t val){fSubLayer = val;}
  void SetDetDirect(DetDirectionDef val){fDetDirect = val;}

  void SetDetPos(const Double_t* val){for(Int_t i=0; i<3; ++i) fDetPos[i] = val[i];}
  void SetDetPos(Double_t val, Int_t i){fDetPos[i] = val;} 

  // Setters : computer and human friendly
  void SetTCal   (UInt_t i, Double_t val){fTCal[i] = val;} 
  void SetTOff   (UInt_t i, Double_t val){fTOff[i] = val;} 
  void SetTSlw   (UInt_t i, Double_t val){fTSlw[i] = val;} 
  void SetTSlwLog(UInt_t i, const Double_t* val){for(Int_t j=0; j<5; ++j) fTSlwLog[i][j] = val[j];}
  void SetTSlwLog(UInt_t i, Double_t val, Int_t j){fTSlwLog[i][j] = val;} 
  void SetQCal   (UInt_t i, Double_t val){fQCal[i] = val;} 
  void SetQPed   (UInt_t i, Double_t val){fQPed[i] = val;} 

  // Up <-> 0 and Down <-> 1
  void SetTUCal   (Double_t val){fTCal[0] = val;} 
  void SetTUOff   (Double_t val){fTOff[0] = val;} 
  void SetTUSlw   (Double_t val){fTSlw[0] = val;} 
  void SetTUSlwLog(const Double_t* val){for(Int_t i=0; i<5; ++i) fTSlwLog[0][i] = val[i];}
  void SetTUSlwLog(Double_t val, Int_t i){fTSlwLog[0][i] = val;} 
  void SetQUCal   (Double_t val){fQCal[0] = val;} 
  void SetQUPed   (Double_t val){fQPed[0] = val;} 
  void SetTDCal   (Double_t val){fTCal[1] = val;} 
  void SetTDOff   (Double_t val){fTOff[1] = val;} 
  void SetTDSlw   (Double_t val){fTSlw[1] = val;} 
  void SetTDSlwLog(const Double_t* val){for(Int_t i=0; i<5; ++i) fTSlwLog[1][i] = val[i];}
  void SetTDSlwLog(Double_t val, Int_t i){fTSlwLog[1][i] = val;} 
  void SetQDCal   (Double_t val){fQCal[1] = val;} 
  void SetQDPed   (Double_t val){fQPed[1] = val;} 

  // Left <-> 0 and Right <-> 1
  void SetTLCal   (Double_t val){fTCal[0] = val;} 
  void SetTLOff   (Double_t val){fTOff[0] = val;} 
  void SetTLSlw   (Double_t val){fTSlw[0] = val;} 
  void SetTLSlwLog(const Double_t* val){for(Int_t i=0; i<5; ++i) fTSlwLog[0][i] = val[i];}
  void SetTLSlwLog(Double_t val, Int_t i){fTSlwLog[0][i] = val;} 
  void SetQLCal   (Double_t val){fQCal[0] = val;} 
  void SetQLPed   (Double_t val){fQPed[0] = val;} 
  void SetTRCal   (Double_t val){fTCal[1] = val;} 
  void SetTROff   (Double_t val){fTOff[1] = val;} 
  void SetTRSlw   (Double_t val){fTSlw[1] = val;} 
  void SetTRSlwLog(const Double_t* val){for(Int_t i=0; i<5; ++i) fTSlwLog[1][i] = val[i];}
  void SetTRSlwLog(Double_t val, Int_t i){fTSlwLog[1][i] = val;} 
  void SetQRCal   (Double_t val){fQCal[1] = val;} 
  void SetQRPed   (Double_t val){fQPed[1] = val;}
 
  void SetDTCal(Double_t val){fDTCal = val;} 
  void SetDTOff(Double_t val){fDTOff = val;} 
  void SetTAveOff(Double_t val){fTAveOff = val;} 
  void SetQAveCal(Double_t val){fQAveCal = val;} 
  void SetQAveCalAtt(Double_t val){fQAveCalAtt = val;} 
  void SetQAveCalAtt2(Double_t val){fQAveCalAtt2 = val;} 
  void SetIgnore(Int_t val){fIgnore = val;}

  // Getters : computer and human friendly
  Double_t GetTCal   (UInt_t i) const {return fTCal[i];} 
  Double_t GetTOff   (UInt_t i) const {return fTOff[i];} 
  Double_t GetTSlw   (UInt_t i) const {return fTSlw[i];} 
  const Double_t* GetTSlwLog(UInt_t i){return fTSlwLog[i];}
  Double_t GetTSlwLog(UInt_t i, Int_t j) const {return fTSlwLog[i][j];} 
  Double_t GetQCal   (UInt_t i) const {return fQCal[i];} 
  Double_t GetQPed   (UInt_t i) const {return fQPed[i];} 

  // Up <-> 0 and Down <-> 1
  Double_t GetTUCal   () const {return fTCal[0];} 
  Double_t GetTUOff   () const {return fTOff[0];} 
  Double_t GetTUSlw   () const {return fTSlw[0];} 
  const Double_t* GetTUSlwLog(){return fTSlwLog[0];}
  Double_t GetTUSlwLog(Int_t i) const {return fTSlwLog[0][i];} 
  Double_t GetQUCal   () const {return fQCal[0];} 
  Double_t GetQUPed   () const {return fQPed[0];} 
  Double_t GetTDCal   () const {return fTCal[1];} 
  Double_t GetTDOff   () const {return fTOff[1];} 
  Double_t GetTDSlw   () const {return fTSlw[1];} 
  const Double_t* GetTDSlwLog(){return fTSlwLog[1];}
  Double_t GetTDSlwLog(Int_t i) const {return fTSlwLog[1][i];}
  Double_t GetQDCal   () const {return fQCal[1];} 
  Double_t GetQDPed   () const {return fQPed[1];} 

  // Left <-> 0 and Right <-> 1
  Double_t GetTLCal   () const {return fTCal[0];} 
  Double_t GetTLOff   () const {return fTOff[0];} 
  Double_t GetTLSlw   () const {return fTSlw[0];} 
  const Double_t* GetTLSlwLog(){return fTSlwLog[0];}
  Double_t GetTLSlwLog(Int_t i) const {return fTSlwLog[0][i];} 
  Double_t GetQLCal   () const {return fQCal[0];} 
  Double_t GetQLPed   () const {return fQPed[0];} 
  Double_t GetTRCal   () const {return fTCal[1];} 
  Double_t GetTROff   () const {return fTOff[1];} 
  Double_t GetTRSlw   () const {return fTSlw[1];} 
  const Double_t* GetTRSlwLog(){return fTSlwLog[1];}
  Double_t GetTRSlwLog(Int_t i) const {return fTSlwLog[1][i];} 
  Double_t GetQRCal   () const {return fQCal[1];} 
  Double_t GetQRPed   () const {return fQPed[1];}

  Int_t GetID() const {return fID;}
  const TString& GetDetectorName() const {return fDetName;}
  Int_t GetFpl() const {return fFpl;}
  Int_t GetLayer() const {return fLayer;}
  Int_t GetSubLayer() const {return fSubLayer;}
  DetDirectionDef GetDetDirect() const {return fDetDirect;}
  Bool_t DirectionIs(DetDirectionDef val) const {return (fDetDirect==val);}

  const Double_t* GetDetPos() const {return fDetPos;} 
  Double_t GetDetPos(Int_t i) const {return fDetPos[i];} 
  Double_t GetDTCal() const {return fDTCal;} 
  Double_t GetDTOff() const {return fDTOff;} 
  Double_t GetTAveOff() const {return fTAveOff;} 
  Double_t GetQAveCal() const {return fQAveCal;} 
  Double_t GetQAveCalAtt() const {return fQAveCalAtt;} 
  Double_t GetQAveCalAtt2() const {return fQAveCalAtt2;} 
  Int_t GetIgnore() const {return fIgnore;}

  friend std::ostream& operator<<(std::ostream& out, const TArtHIMEPlaPara& p){
    out << "ID: "          << p.fID      << std::endl;
    out << "Det. Name: "   << p.fDetName << std::endl;
    out << "Focal Plane: " << p.fFpl     << std::endl;
    out << "Layer: "       << p.fLayer   << std::endl;
    out << "SubLayer: "    << p.fSubLayer<< std::endl;
    out << "Det. Pos.: "   << p.fDetPos[0]  << " " << p.fDetPos[1]<< " " << p.fDetPos[2]<< std::endl;
    out << "TU Calib: "    << p.fTCal[0]   << std::endl;
    out << "TU Offset: "   << p.fTOff[0]   << std::endl;
    out << "TU Slew: "     << p.fTSlw[0]   << std::endl;
    out << "TU Slew Log:";
    for(int i=0; i<5; ++i) out << " " << p.fTSlwLog[0][i];
    out << std::endl;
    out << "QU Calib: "    << p.fQCal[0]   << std::endl;
    out << "QU Pedestal: " << p.fQPed[0]   << std::endl;
    out << "TD Calib: "    << p.fTCal[1]   << std::endl;
    out << "TD Offset: "   << p.fTOff[1]   << std::endl;
    out << "TD Slew: "     << p.fTSlw[1]   << std::endl;
    out << "TD Slew Log:";
    for(int i=0; i<5; ++i) out << " " << p.fTSlwLog[1][i];
    out << std::endl;
    out << "QD Calib: "    << p.fQCal[1]   << std::endl;
    out << "QD Pedestal: " << p.fQPed[1]   << std::endl;
    out << "dT Calib: "    << p.fDTCal   << std::endl;
    out << "dT Offset: "   << p.fDTOff   << std::endl;
    out << "TAve Offset: " << p.fTAveOff << std::endl;
    out << "QAve Calib: "  << p.fQAveCal << std::endl;
    out << "QAve Calib Attenuation: "  << p.fQAveCalAtt << std::endl;
    out << "QAve Calib Attenuation2: "  << p.fQAveCalAtt2 << std::endl;
    out << "Ignore: "      << p.fIgnore  << std::endl;
    out << "Map of U: "    << p.fMapU  << std::endl;
    out << "Map of D: "    << p.fMapD  << std::endl;
    out << std::endl;
    return out;
  }

  const TArtRIDFMap& GetMapU() const {return fMapU;}
  const TArtRIDFMap& GetMapD() const {return fMapD;}

private:
  Int_t     fID;
  TString   fDetName;
  Int_t     fFpl;
  Int_t     fLayer;
  Int_t     fSubLayer;
  Double_t  fDetPos[3];
  Double_t  fTCal[2];
  Double_t  fTOff[2];
  Double_t  fTSlw[2];
  Double_t  fTSlwLog[2][5];
  Double_t  fQCal[2];
  Double_t  fQPed[2];
  Double_t  fDTCal;
  Double_t  fDTOff;
  Double_t  fTAveOff;
  Double_t  fQAveCal;
  Double_t  fQAveCalAtt;
  Double_t  fQAveCalAtt2;
  Int_t     fIgnore;
  DetDirectionDef fDetDirect;

  void SetMapU(Int_t detector, Int_t tu_geo, Int_t tu_ch){
    fMapU.SetMap(fFpl,detector,tu_geo,tu_ch);
    //    std::cout << "U is mapped to " << fMapU.GetMapInfo() << " " << fMapU.GetName() << std::endl;
  }
  void SetMapD(Int_t detector, Int_t td_geo, Int_t td_ch){
    fMapD.SetMap(fFpl,detector,td_geo,td_ch);
    //    std::cout << "D is mapped to " << fMapD.GetMapInfo() << " " << fMapD.GetName() << std::endl;
  }

  TArtRIDFMap fMapU, fMapD;

  ClassDef(TArtHIMEPlaPara,1);
};

#endif
