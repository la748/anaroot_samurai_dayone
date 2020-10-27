#ifndef _TARTSILICONSPARA_H_
#define _TARTSILICONSPARA_H_

#include <map>
#include <Riostream.h>

#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtSILICONS.hh"

class TArtSILICONSPara : public TNamed {

public:
  
  struct DetDirection
  {
    enum Type
      {
	Vert, Horiz, Angle
      };
    Type t_;
    DetDirection() : t_(Angle) {};
    DetDirection(Type t) : t_(t) {}
    operator Type () const {return t_;}
  private:
    template<typename T>
    operator T () const;
  };

  TArtSILICONSPara(){ }
  TArtSILICONSPara(UShort_t id, TString n, UShort_t f, 
		   UShort_t l, UShort_t s, DetDirection d, Double_t a,
		   Double_t p[], 
		   Double_t tc, Double_t to, Double_t qc, Double_t qp){
    SetParameters(id, n,  f, l,  s, d, a,  p, tc,  to,  qc,  qp);
  }
  ~TArtSILICONSPara() { }
  

  
  void SetParameters(UShort_t i, TString n, UShort_t f, 
		     UShort_t l, UShort_t s, DetDirection d, Double_t a,
		     Double_t p[], 
		     Double_t tc, Double_t to, Double_t qc, Double_t qp){
    id = i;
    detname = n;
    fpl = f;
    strip = s;
    layer = l;
    direction = d;
    angle = a;
    pedestal = qp;
    ch2mev = qc;
    
    for(int j=0;j<3;j++) position[j] = p[j] ;
   
    t_offset = to;
    ch2ns = tc;

  }

  void SetMap(Int_t tgeo, Int_t tch, 
	      Int_t adc_geo, Int_t adc_ch) {
    map_adc.SetMap(fpl,TEL,adc_geo,adc_ch); 
    map_tdc.SetMap(fpl,TEL,tgeo,tch); 
  }

  Int_t GetID() const { return id; }
  TString * GetDetectorName() { return &detname; }
  Int_t GetFpl() const { return fpl; }

  Double_t GetLayer()     const { return layer; } 
  Double_t GetStrip()     const { return strip; } 
  Double_t GetDirection() const { return direction; } 
  Double_t GetAngle() const { return angle; } 
  Double_t GetAngleRadian() const { return angle; } 
  Double_t GetAngleDegree() const { return TMath::RadToDeg()*angle; } 
  const Double_t* GetPosition() { return position; } 
  Double_t GetPosition(UShort_t i) const { return position[i]; } 

  Double_t GetCh2MeV() const { return ch2mev; } 
  Double_t GetPedestal() const { return pedestal; } 
  Double_t GetCh2Ns() const { return ch2ns; } 
  Double_t GetTOffset() const { return t_offset; } 


  TArtRIDFMap * GetADCMap() { return &map_adc;  }
  TArtRIDFMap * GetTDCMap() { return &map_tdc;  }

  friend std::ostream & operator << (std::ostream& out, const TArtSILICONSPara& p) {
    out << "ID: " << p.id << ", ";
    out << "DetectorName: " << p.detname << ", ";
    out << "Focal Plane: " << p.fpl << ", ";
    out << "adc pedestal: " << p.pedestal << ", ";
    out << "ch2mev: " << p.ch2mev << ", "; 
    out << "t_offset: " << p.t_offset << ", ";
    out << "ch2ns: " << p.ch2ns << ", "; 
    out << "Direction/angle:" << p.angle << ", ";
    out << "Position: (" << p.position[0] << ", " << p.position[1] << ", " << p.position[2] << ") " ;
    out << std::endl;
    out << "Map of adc: "  << p.map_adc;
    out << "Map of tdc: "  << p.map_tdc;
    return out;
  }

private:

  UShort_t     id;
  TString   detname;
  UShort_t   fpl;
  UShort_t strip;
  UShort_t layer;
  DetDirection direction;
  Double_t angle; // in radian

  Double_t  pedestal;
  Double_t  ch2mev;

  Double_t position[3];

  Double_t  t_offset;
  Double_t  ch2ns;

  TArtRIDFMap map_adc;
  TArtRIDFMap map_tdc;

  ClassDef(TArtSILICONSPara,1);

};

#endif
