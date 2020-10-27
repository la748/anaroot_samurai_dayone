/*
 * @file TArtDecoderVmeEasyroc.cc
 * geo:0 High gain ADC
 * geo:1 Low gain ADC
 * geo:2 Leading edge TDC
 * geo:3 Trailing edge TDC
 * geo:4 Scaler
 */
#include "TArtDecoderVmeEasyroc.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include "segidlist.hh"
#include <stdio.h>
#include <byteswap.h>

TArtDecoderVmeEasyroc::TArtDecoderVmeEasyroc()
   : TArtDecoder(kID) {
}

TArtDecoderVmeEasyroc::~TArtDecoderVmeEasyroc()
{
}

TArtDecoder* TArtDecoderVmeEasyroc::Instance()
{
   static TArtDecoderVmeEasyroc instance;
   return &instance;
}

bool TArtDecoderVmeEasyroc::isAdcHg(unsigned int data)
{
    return (data & 0x00680000) == 0x00000000;
}

bool TArtDecoderVmeEasyroc::isAdcLg(unsigned int data)
{
    return (data & 0x00680000) == 0x00080000;
}

bool TArtDecoderVmeEasyroc::isTdcLeading(unsigned int data)
{
    return (data & 0x00601000) == 0x00201000;
}

bool TArtDecoderVmeEasyroc::isTdcTrailing(unsigned int data)
{
    return (data & 0x00601000) == 0x00200000;
}

bool TArtDecoderVmeEasyroc::isScaler(unsigned int data)
{
    return (data & 0x00600000) == 0x00400000;
}

int TArtDecoderVmeEasyroc::Decode(unsigned char* &buf, 
				  const unsigned int& size,
				  TArtRawSegmentObject* rawseg)
{
  //  TArtCore::Info(__FILE__,"TArtDecoderVmeEasyroc::Decode %d",size);

  /*
  unsigned char newbuf[size];
  unsigned char *nbuf = newbuf;
  for(int i=0; i<size; i+=4){
    newbuf[i] = buf[i+3];
    newbuf[i+1] = buf[i+2];
    newbuf[i+2] = buf[i+1];
    newbuf[i+3] = buf[i];
  }
  */

  unsigned int *evtdata = (unsigned int*) buf;
  unsigned int evtsize = size/sizeof(unsigned int);

  int numdat = evtdata[0] & 0xfff;
  TArtCore::Debug(__FILE__,"header:%x, num dat: %d", evtdata[0], numdat);

  if(numdat + 1 != evtsize)
    TArtCore::Debug(__FILE__,"data size is different from expected size from header:%x, num dat: %d, evtsize: %d", evtdata[0], numdat, evtsize);

  TArtRawDataObject *rdata = NULL;
  for (int i=1; i<evtsize; i++) {
    if(isAdcHg(evtdata[i])) {
      int ch = (evtdata[i] >> 13) & 0x3f;
      bool otr = ((evtdata[i] >> 12) & 0x01) != 0;
      int value = evtdata[i] & 0x0fff;
      if(!otr) {
	TArtCore::Debug(__FILE__,"Ch: %d, ADC Hg:%d", ch, value);
      }
      rdata = new TArtRawDataObject(0,ch,value);
    }else if(isAdcLg(evtdata[i])) {
      int ch = (evtdata[i] >> 13) & 0x3f;
      bool otr = ((evtdata[i] >> 12) & 0x01) != 0;
      int value = evtdata[i] & 0x0fff;
      if(!otr) {
	TArtCore::Debug(__FILE__,"Ch: %d, ADC Lg:%d", ch, value);
      }
      rdata = new TArtRawDataObject(1,ch,value);
    }else if(isTdcLeading(evtdata[i])) {
      int ch = (evtdata[i] >> 13) & 0x3f;
      int value = evtdata[i] & 0x0fff;
      TArtCore::Debug(__FILE__,"Ch: %d, TDC Ld:%d", ch, value);
      rdata = new TArtRawDataObject(2,ch,value);
    }else if(isTdcTrailing(evtdata[i])) {
      int ch = (evtdata[i] >> 13) & 0x3f;
      int value = evtdata[i] & 0x0fff;
      TArtCore::Debug(__FILE__,"Ch: %d, TDC Td:%d", ch, value);
      rdata = new TArtRawDataObject(3,ch,value);
    }else if(isScaler(evtdata[i])) {
      int ch = (evtdata[i] >> 14) & 0x7f;
      int value = evtdata[i] & 0x3fff;
      rdata = new TArtRawDataObject(4,ch,value);
    }
    rawseg->PutData(rdata);

  }


  return 0;
}
