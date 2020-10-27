#ifndef TARTTACQUILAMAP_H
#define TARTTACQUILAMAP_H

#include <Riostream.h>
#include <TNamed.h>

class TArtTacquilaMap : public TNamed {

 protected:
  int sam;
  int gtb;
  int mod;
  int ch;
  char mapinfo[64];

 public:
  TArtTacquilaMap(){mod=-1;gtb=-1;ch=-1;sam=-1;}
  TArtTacquilaMap(const int insam, 
		  const int ingtb, 
		  const int inmod, 
		  const int inch ) :
    sam(insam), gtb(ingtb), mod(inmod), ch(inch) {
    sprintf(mapinfo,"Sam%d_Gtb%d_Mod%d_Ch%d",sam,gtb,mod,ch);
    SetName(mapinfo);
    sprintf(mapinfo,"Sam:%d, Gtb:%d, Mod:%d, Ch:%d",sam,gtb,mod,ch);
  }
  virtual ~TArtTacquilaMap() {}

  virtual void SetMap(const int insam, 
		      const int ingtb, 
		      const int inmod, 
		      const int inch ) {
    sam=insam; gtb=ingtb; mod = inmod; ch = inch;
    sprintf(mapinfo,"Sam%d_Gtb%d_Mod%d_Ch%d",sam,gtb,mod,ch);
    SetName(mapinfo);
    sprintf(mapinfo,"Sam:%d, Gtb:%d, Mod:%d, Ch:%d",sam,gtb,mod,ch);
  }

  virtual int GetSam(){return sam;}
  virtual int GetGtb(){return gtb;}
  virtual int GetModule(){return mod;}
  virtual int GetChannel(){return ch;}

  virtual bool operator< (const TArtTacquilaMap &m) const{ 
    if(sam != m.sam){return sam < m.sam;}
    else if(gtb != m.gtb){return gtb < m.gtb;}
    else if(mod != m.mod){return mod < m.mod;}
    else{return ch < m.ch;}
  }

  virtual bool operator== (const TArtTacquilaMap &m) const{ 
    return sam==m.sam && gtb==m.gtb && mod==m.mod && ch==m.ch;
  }

  virtual char * GetMapInfo(){ return mapinfo; }
  friend std::ostream & operator << (std::ostream& out, const TArtTacquilaMap& p) {
    out << "Detector Sam, Gtb, Mod, Ch: " 
	<< p.sam << " " 
	<< p.gtb << " " 
	<< p.mod << " " 
	<< p.ch << std::endl;
    return out;
  }

  ClassDef(TArtTacquilaMap,1);

};

#endif // end of #ifdef TARTTACQUILAMAP_H
