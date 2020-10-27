#ifndef _TARTNINJAPLAPARA_H_
#define _TARTNINJAPLAPARA_H_

#include "TArtMap.hh"

class TArtSAMURAIParameters;

class TArtNINJAPlaPara : public TObject
{
  friend class TArtSAMURAIParameters;

public:
  TArtNINJAPlaPara(): TObject(){
  fID=-1;
  fDetName="";
  fFpl=-1;
  fLayer=-1;
  fIgnore=0;
  fSubLayer=-1;
  fDetPos[0]=-90000;
  fDetPos[1]=-90000;
  fDetPos[2]=-90000;
  for(unsigned int i = 0 ; i < 2 ; i++){
    fT_LECal[i]=1;
    fT_LEOffset[i]=0;
    fT_TECal[i]=1;
    fT_TEOffset[i]=0;
    fQCal[i]=1;
    fQOffset[i]=0;
  }
 } 
  ~TArtNINJAPlaPara(){;}

  enum DetDirectionDef { Vert, Horiz};
  Int_t GetIgnore() const {return fIgnore;}

  friend std::ostream& operator<<(std::ostream& out, const TArtNINJAPlaPara& p){
    out << "ID: "          << p.fID      << std::endl;
    out << "Det. Name: "   << p.fDetName << std::endl;
    out << "Focal Plane: " << p.fFpl     << std::endl;
    out << "Layer: "       << p.fLayer   << std::endl;
    out << "SubLayer: "    << p.fSubLayer<< std::endl;
    out << "Det. Pos.: "   << p.fDetPos[0]  << " offset=" << p.fDetPos[1]<< " " << p.fDetPos[2]<< std::endl;
    out << "LE Time 0: slope=" <<  p.fT_LECal[0] << " offset=" << p.fT_LEOffset[0] << std::endl;
    out << "LE Time 1: slope=" <<  p.fT_LECal[1] << " offset=" << p.fT_LEOffset[1] << std::endl;
    out << "TE Time 0: slope=" <<  p.fT_TECal[0] << " offset=" << p.fT_TEOffset[0] << std::endl;
    out << "TE Time 1: slope=" <<  p.fT_TECal[1] << " offset=" << p.fT_TEOffset[1] << std::endl;
    out << "Q Cal 0 : slope= " <<  p.fQCal[0]  << " offset=" << p.fQOffset[0]  << std::endl ;
    out << "Q Cal 1 : slope= " <<  p.fQCal[1]  << " offset=" << p.fQOffset[1]  << std::endl ;
    out << "Ignore: "      << p.fIgnore  << std::endl;
    out << "Map of 0: "    << p.fMap0  << std::endl;
    out << "Map of 1: "    << p.fMap1  << std::endl;
    out << std::endl;
    return out;
  }

  const TArtRIDFMap& GetMap0() const {return fMap0;}
  const TArtRIDFMap& GetMap1() const {return fMap1;}

public:
  // Para specific
  Int_t     fID;
  TString   fDetName;
  Int_t     fFpl;

public:
  // Ninja shit coming up
  Int_t     fLayer;
  Int_t     fSubLayer;
  Double_t  fDetPos[3];
  Double_t  fT_LECal[2];
  Double_t  fT_LEOffset[2];
  Double_t  fT_TECal[2];
  Double_t  fT_TEOffset[2];
  Double_t  fQCal[2];
  Double_t  fQOffset[2];
  Int_t     fIgnore;
  DetDirectionDef fDetDirect;

  void SetMap0(Int_t detector, Int_t t0_geo, Int_t t0_ch){
    fMap0.SetMap(fFpl,detector,t0_geo,t0_ch);
    //    std::cout << "U is mapped to " << fMap0.GetMapInfo() << " " << fMap0.GetName() << std::endl;
  }
  void SetMap1(Int_t detector, Int_t t1_geo, Int_t t1_ch){
    fMap1.SetMap(fFpl,detector,t1_geo,t1_ch);
    //    st1::cout << "D is mapped to " << fMap1.GetMapInfo() << " " << fMap1.GetName() << st1::endl;
  }

  TArtRIDFMap fMap0, fMap1;

  ClassDef(TArtNINJAPlaPara,1);
};

#endif
