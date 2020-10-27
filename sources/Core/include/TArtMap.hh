#ifndef TARTMAP_H
#define TARTMAP_H

#include <Riostream.h>
#include <TNamed.h>

class TArtRIDFMap : public TNamed {

 protected:
  int device;
  int fpl;
  int detid;
  int geo;
  int ch;
  char mapinfo[64];

 public:
  TArtRIDFMap(){device=-1;fpl=-1;detid=-1;geo=-1;ch=-1;}
  TArtRIDFMap(const int indev, const int fp, const int did, const int ingeo, const int inch ) :
    device(indev), fpl(fp), detid(did), geo(ingeo), ch(inch) {
    sprintf(mapinfo,"Dev%d_Fpl%d_DetID%d_Geo%d_Ch%d",device,fpl,detid,geo,ch);
    SetName(mapinfo);
    sprintf(mapinfo,"Dev:%d, Fpl:%d, DetID:%d, Geo:%d, Ch:%d",device,fpl,detid,geo,ch);
  }
  TArtRIDFMap(const int fp, const int did, const int ingeo, const int inch ) :
    device(-1), fpl(fp), detid(did), geo(ingeo), ch(inch) {
    sprintf(mapinfo,"Fpl%d_DetID%d_Geo%d_Ch%d",fpl,detid,geo,ch);
    SetName(mapinfo);
    sprintf(mapinfo,"Fpl:%d, DetID:%d, Geo:%d, Ch:%d",fpl,detid,geo,ch);
  }
  virtual ~TArtRIDFMap() {}

  virtual void SetMap(const int indev, const int fp, const int did, const int ingeo, const int inch ) {
    device=indev; fpl=fp; detid=did; geo=ingeo; ch=inch;
    sprintf(mapinfo,"Dev%d_Fpl%d_DetID%d_Geo%d_Ch%d",device,fpl,detid,geo,ch);
    SetName(mapinfo);
    sprintf(mapinfo,"Dev:%d, Fpl:%d, DetID:%d, Geo:%d, Ch:%d",device,fpl,detid,geo,ch);
  }
  virtual void SetMap(const int fp, const int did, const int ingeo, const int inch ) {
    fpl=fp; detid=did; geo=ingeo; ch=inch;
    sprintf(mapinfo,"Dev%d_Fpl%d_DetID%d_Geo%d_Ch%d",device,fpl,detid,geo,ch);
    SetName(mapinfo);
    sprintf(mapinfo,"Dev:%d, Fpl:%d, DetID:%d, Geo:%d, Ch:%d",device,fpl,detid,geo,ch);
  }

  void SetDevice(int val) {device=val;}
  virtual int GetDevice(){return device;}
  virtual int GetFpl   (){return fpl;}
  virtual int GetDetID (){return detid;}
  virtual int GetGeo   (){return geo;}
  virtual int GetCh    (){return ch;}

  virtual bool operator< (const TArtRIDFMap &m) const{ 
//    if(device>=0){
//      if(device != m.device){return device < m.device;}
//    }
    if(device >= 0 && m.device >= 0 && device != m.device){return device < m.device;}
    else if(fpl != m.fpl){return fpl < m.fpl;}
    else if(detid != m.detid){return detid < m.detid;}
    else if(geo != m.geo){return geo < m.geo;}
    else{return ch < m.ch;}
  }

  virtual bool operator== (const TArtRIDFMap &m) const{ 
    if(device>=0&&m.device>=0){
      return device==m.device && fpl==m.fpl && detid==m.detid && geo==m.geo && ch==m.ch;
    }
    else{
      return fpl==m.fpl && detid==m.detid && geo==m.geo && ch==m.ch;
    }
  }

  virtual char * GetMapInfo(){ return mapinfo; }
  friend std::ostream & operator << (std::ostream& out, const TArtRIDFMap& p) {
    out << "Device, Fpl, DetID, Geo, Ch: " 
	<< p.device << " " 
	<< p.fpl << " " 
	<< p.detid << " " 
	<< p.geo << " " 
	<< p.ch << std::endl;
    return out;
  }

  ClassDef(TArtRIDFMap,1);

};

#endif // end of #ifdef TARTMAP_H
