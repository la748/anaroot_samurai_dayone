#ifndef _TARTFADCPARA_H_
#define _TARTFADCPARA_H_

#include <map>
#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

class TArtFADCPara : public TNamed {

 public:
  TArtFADCPara(){}
  TArtFADCPara(Int_t i, TString n, Int_t f) : 
    id(i), detname(n), fpl(f) { }
  ~TArtFADCPara() { }

  void SetPalameters(Int_t i, TString n, Int_t f){
    id=i; detname=n; fpl=f;}

  void SetMap(Int_t dt, Int_t tg, Int_t tc) {
    map.SetMap(fpl,dt,tg,tc); 
    //cout << "TDC is mapped to " << map.GetMapInfo() << " " << map.GetName() << endl;
  }

  Int_t GetID() const { return id; }
  Int_t GetFpl() const { return fpl; }
  TString * GetDetectorName() { return &detname; }

  TArtRIDFMap * GetFADCMap() { return &map;  }

  friend std::ostream & operator << (std::ostream& out, const TArtFADCPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "Map of FADC: "  << p.map;
    return out;
  }

private:

  Int_t     id;
  TString   detname;
  Int_t     fpl;

  TArtRIDFMap map;

  ClassDef(TArtFADCPara,1);

};

#endif
