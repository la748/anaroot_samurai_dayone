#ifndef _TARTTDCHITPARA_H_
#define _TARTTDCHITPARA_H_

#include <map>
#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

class TArtTDCHitPara : public TNamed {

 public:
  TArtTDCHitPara(){}
  TArtTDCHitPara(Int_t i, TString n, Int_t f, Int_t l, Int_t idp, Int_t wid, Double_t wa, Double_t wz, Int_t tz) : 
    id(i), detname(n), fpl(f), layer(l), id_plane(idp), wireid(wid), wireang(wa), wirez(wz), toffset(tz){ }
  ~TArtTDCHitPara() { }

  void SetPalameters(Int_t i, TString n, Int_t f, Int_t l, Int_t idp, Int_t wid, Double_t wa, Double_t wz, Int_t tz){
    id=i; detname=n; fpl=f; layer=l; id_plane=idp; wireid=wid; wireang=wa; wirez=wz; toffset=tz;}

  void SetMap(Int_t dt, Int_t tg, Int_t tc) {
    map.SetMap(fpl,dt,tg,tc); 
    //cout << "TDC is mapped to " << map.GetMapInfo() << " " << map.GetName() << endl;
  }

  Int_t GetID() const { return id; }
  Int_t GetFpl() const { return fpl; }
  Int_t GetLayer() const { return layer; }
  Int_t GetPlaneID() const { return id_plane; }
  Int_t GetWireID() const { return wireid; }
  TString * GetDetectorName() { return &detname; }
  Double_t GetWireAngle() const { return wireang; }
  Double_t GetWireZPosition() const { return wirez; }
  Int_t GetTzero() const { return toffset; }

  TArtRIDFMap * GetTDCMap() { return &map;  }

  friend std::ostream & operator << (std::ostream& out, const TArtTDCHitPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "Layer: " << p.layer << ", ";
    out << "Plane-ID: " << p.id_plane << ", ";
    out << "Wire ID: " << p.wireid << ", ";
    out << "Wire Angle: " << p.wireang << ", ";
    out << "Wire Z-Pos: " << p.wirez << ", ";
    out << "T zero: " << p.toffset << ", ";
    out << "Map of TDC: "  << p.map;
    return out;
  }

private:

  Int_t     id;
  TString   detname;
  Int_t     fpl;
  Int_t     layer;
  Int_t     id_plane;
  Int_t     wireid; // wire id
  Double_t  wireang; // wire angle X:+90,Y:0
  Double_t  wirez; // wire z position
  Int_t     toffset; // Tzero

  TArtRIDFMap map;

  ClassDef(TArtTDCHitPara,1);

};

#endif
