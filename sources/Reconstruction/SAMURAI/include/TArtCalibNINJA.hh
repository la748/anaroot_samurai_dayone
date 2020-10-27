// NINJA calibration class
// 

#ifndef _TARTCALIBNINJA_H_
#define _TARTCALIBNINJA_H_

#include <TNamed.h>
#include <vector>
#include <map>

#include "TArtReconstruction.hh"

#include "TArtRawEventObject.hh"
#include "TArtCore.hh" 
#include "TArtStoreManager.hh"
#include "TArtMap.hh"
#include "segidlist.hh"

#include "TArtSAMURAIParameters.hh"
#include "TArtNINJAPla.hh"
#include "TArtNINJAPlaPara.hh"

#include <TClonesArray.h>
#include <iostream>
#include <cmath>



class TClonesArray;
class TArtStoreManager;
class TArtSAMURAIParameters;

class TArtNINJAPla;
typedef TClonesArray TArtNINJAPlaArray;
class TArtNINJAPlaPara;
typedef TClonesArray TArtNINJAPlaParaArray;
class TArtCalibSAMURAIT0;

class TString;
class TArtCalibNINJA : public TArtReconstruction
{
public:
  TArtCalibNINJA();
  virtual ~TArtCalibNINJA();

  void LoadData();
  void LoadData(TArtRawSegmentObject *);
  void ReconstructData();
  void ClearData();

  Int_t GetNumNINJAPla() const;
  TArtNINJAPla* GetNINJAPla(const Int_t& i) const;
  TArtNINJAPla* FindNINJAPla(const Int_t& id) const;//find by id
  TArtNINJAPlaArray* GetNINJAPlaArray() const;
  Int_t GetNumNINJAPlaPara() const;
  TArtNINJAPlaPara* GetNINJAPlaPara(const Int_t& i) const;
  TArtNINJAPlaPara* FindNINJAPlaPara(const Int_t& id) const;//find by id
  TArtNINJAPlaParaArray* GetNINJAPlaParaArray() const;

  inline Int_t GetMultGlobal() const {return fMultGlobal;};
  inline Int_t GetMult(const Int_t& iLayer) const {return fMult[iLayer];};

 
private:
  //  TArtCalibSAMURAIT0* fCalibSAMURAIT0;
  TClonesArray* fT0Array;
  TArtNINJAPlaArray* fNINJAPlaArray;
  TArtNINJAPlaParaArray* fNINJAPlaParaArray;

  const TArtSAMURAIParameters* fSAMURAIParameters;

  TArtStoreManager* fStoreManager;

//  TArtCalibNINJAMult fMult;

  const Int_t fNLayers;
  std::vector<int> fMult;
  Int_t fMultGlobal;

  //for optimization
  std::map<int,int> fIDNPlaMap;
  std::map<int,int> fIDNPlaParaMap;

  ClassDef(TArtCalibNINJA,1);
};

#endif
