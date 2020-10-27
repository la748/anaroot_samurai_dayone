#include "TArtDecoderSIS3301.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include <stdio.h>

TArtDecoderSIS3301::TArtDecoderSIS3301()
   : TArtDecoder(kID) {
}

TArtDecoderSIS3301::~TArtDecoderSIS3301()
{
}

TArtDecoder* TArtDecoderSIS3301::Instance()
{
  static TArtDecoderSIS3301 instance;
  return &instance;
}

Int_t TArtDecoderSIS3301::Decode(unsigned char* &buf, const unsigned int& size,
				 TArtRawSegmentObject* rawseg)
{
//   seg->Dump();
   UInt_t *evtdata = (UInt_t*) buf;
   UInt_t evtsize = size/sizeof(UInt_t);
   UInt_t ih;
   Int_t igeo, ich, itb;
   UInt_t hf, tf;
   UInt_t pagesize, evtid, clock;
   Int_t data1st,data2nd;
   Int_t sumsample1st,sumsample2nd;
   hf = tf = 0;
   igeo = -1;
   ich = -1;
   pagesize = -1;

   TArtRawFADCDataObject *fadcdata = NULL;

   //printf("evtsize = %d\n",evtsize);
   for (UInt_t i=0; i<evtsize;) {
      //printf("evtdata[%d] = 0x%08x\n",i,evtdata[i]);
     ih = (evtdata[i]&0xc0000000)>>30;
     if ((hf == 0) && (tf == 0) && (ih == 0x3)) {
        hf = 1;
        igeo = (evtdata[i] & kMaskGeometry) >> kShiftGeometry;
        ich  = (evtdata[i] & kMaskChannel) >>  kShiftChannel;
        //printf("igeo = %d, ich = %d\n",igeo,ich);
        sumsample1st = 0.;
        sumsample2nd = 0.;
        fadcdata = new TArtRawFADCDataObject(ich);
        fadcdata->SetGeo(igeo);
	itb = 0;
        i += 1;
     } else if ((hf == 1) && (tf == 0) && (ih == 0x2)) {
        tf = 1;
        //pagesize = (evtdata[i] & kMaskPagesize) >> kShiftPagesize;
        pagesize = (evtdata[i] & 0x3fffffff) >> 0;
        //evtid = (evtdata[i] & kMaskEventID) >> kShiftEventID;
        evtid = 0;
        //clock = (evtdata[i] & kMaskClock) >> kShiftClock;
        clock = 0;
        //printf("pagesize = %d, evtid = %d, clock = %d\n",pagesize,evtid,clock);
        //TRawDataFadc* data = (TRawDataFadc*) New();
        if(!fadcdata) {
	  TArtCore::Info(__FILE__,"no data\n");
	  continue;
        }
	//        data->SetFadcInfo(pagesize,evtid,clock);
        i += 1;
     } else if ((hf == 1) && (tf == 1)){
        //printf("ch %d : {",ich);
        for (UInt_t j=i;j<i+pagesize/2;) {
           data1st = (evtdata[j] & kMaskData1st) >> kShiftData1st;
           data2nd = (evtdata[j] & kMaskData2nd) >> kShiftData2nd;
           sumsample1st += data1st;
           sumsample2nd += data2nd;
           //printf("ich = %2d, data1st = 0x%04x, data2nd = 0x%04x\n",
           //       ich,data1st,data2nd);
           //printf(" 0x%04x 0x%04x",data1st,data2nd);
           if(!fadcdata) {
	     TArtCore::Info(__FILE__,"no data\n");
	     continue;
           }
           fadcdata->AddVal(itb,data1st); itb++;
           fadcdata->AddVal(itb,data2nd); itb++;
           j++;
        }
        rawseg->PutFADCData(fadcdata);
        //printf(" }\n");
        i += pagesize/2;
        hf = tf = 0;
     } else {
       TArtCore::Info(__FILE__,"Decode""Unknown Header 0x%08x\n",evtdata[i]);
       return 0;
     }
   }
   return 0;
}
