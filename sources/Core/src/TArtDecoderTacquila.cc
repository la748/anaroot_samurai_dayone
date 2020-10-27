/*
 * @file TArtDecoderTacquila.cc
 */
#include "TArtDecoderTacquila.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include "segidlist.hh"
#include <stdio.h>
#include <byteswap.h>

//#include<iostream>

TArtDecoderTacquila::TArtDecoderTacquila()
   : TArtDecoder(kID) {
}

TArtDecoderTacquila::~TArtDecoderTacquila()
{
}

TArtDecoder* TArtDecoderTacquila::Instance()
{
   static TArtDecoderTacquila instance;
   return &instance;
}

int TArtDecoderTacquila::Decode(unsigned char* &buf, 
				     const unsigned int& size,
				     TArtRawSegmentObject* rawseg)
{
  //  TArtCore::Info(__FILE__,"TArtDecoderTacquila::Decode %d",size);

  unsigned char newbuf[size];
  unsigned char *nbuf = newbuf;
  for(int i=0; i<size; i+=4){
    newbuf[i] = buf[i+3];
    newbuf[i+1] = buf[i+2];
    newbuf[i+2] = buf[i+1];
    newbuf[i+3] = buf[i];
  }

  unsigned int *evtdata = (unsigned int*) nbuf;
  unsigned int evtsize = size/sizeof(unsigned int);

  int sam, gtb, numdata;
  int module[2], ch[2], isadc[2], cycle[2], val[2];

  for (int i=0; i<evtsize; i++) {
    sam = (evtdata[i]&0xf0000000) >> 28;
    gtb = (evtdata[i]&0x0f000000) >> 24;
    numdata = (evtdata[i]&0x1ff);
    //    TArtCore::Info(__FILE__,"%d/%d SAM: %d, GTB:%d, NDATA: %d", i, evtsize, sam, gtb, numdata);
    for(;numdata>0;numdata-=2) {

      i ++;
      module[0] = (evtdata[i]&0xf8000000) >> 27;
      ch    [0] = (evtdata[i]&0x07c00000) >> 22;
      isadc [0] = (evtdata[i]&0x00200000) >> 21;
      //cycle [0] = 63 - ((evtdata[i]&0x0003f000) >> 12);
      cycle [0] = 63 - ((evtdata[i]&0x0003f000) >> 12);
      val   [0] = 4095 - (evtdata[i]&0x00000fff); // supposed to be tdc

      i ++;
      module[1] = (evtdata[i]&0xf8000000) >> 27;
      ch    [1] = (evtdata[i]&0x07c00000) >> 22;
      isadc [1] = (evtdata[i]&0x00200000) >> 21;
      cycle [1] = (evtdata[i]&0x000ff000) >> 12;
      val   [1] = (evtdata[i]&0x00000fff); // supposed to be adc

      if(!( (module[0] == module[1]) &&
	    (ch    [0] == ch    [1]) &&
	    (isadc [0] == 0 ) &&
	    (isadc [1] == 1 ) ) )
	TArtCore::Error(__FILE__,"decoded data is funny!!, MOD: %d,%d CH:%d,%d ISADC: %d,%d", module[0], module[1], ch[0], ch[1], isadc[0], isadc[1]);
      //      if(ch[0]!=16) TArtCore::Info(__FILE__,"     MOD: %d, CH:%d, ADC: %d, CYCLE: %d, TAC: %d", module[0], ch[0], val[1], cycle[0], val[0]); // skip tdc stop channel

      TArtRawTacquilaDataObject *tdata = new TArtRawTacquilaDataObject(sam, gtb, module[0], ch[0], val[1], cycle[0], val[0]);
      rawseg->PutTacquilaData(tdata);


    }
  }

  /*
  unsigned int *orgdata = (unsigned int*) buf;
  unsigned int evtsize = size/sizeof(unsigned int);
  unsigned int *newdata = (unsigned int*) newbuf;
  for (int i=0; i<evtsize; i++) newdata[i] = bswap_32(orgdata[i]);
  */

  //  TArtDecoderFactory::Instance()->GetDecoder(V792)->Decode(nbuf,size,rawseg);
  //  delete newbuf;

  return 0;
}
