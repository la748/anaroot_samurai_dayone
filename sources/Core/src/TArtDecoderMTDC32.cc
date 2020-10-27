#include "TArtDecoderMTDC32.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include <stdio.h>
#include <iostream>
TArtDecoderMTDC32::TArtDecoderMTDC32()
   : TArtDecoder(kID) {
}

TArtDecoderMTDC32::~TArtDecoderMTDC32()
{
}

TArtDecoder* TArtDecoderMTDC32::Instance()
{
   static TArtDecoderMTDC32 instance;
   return &instance;
}

int TArtDecoderMTDC32::Decode(unsigned char* &buf, const unsigned int& size,
			      TArtRawSegmentObject* rawseg)
{
   unsigned int *evtdata = (unsigned int*) buf;
   unsigned int evtsize = size/sizeof(unsigned int);
   int i, igeo, ich, msk, val;

   igeo = -1;
   for (i=0; i<evtsize; i++) {
     msk = (evtdata[i]&0xc0000000)>>30;
     //printf("%d : msk=%08x %08x\n", i, msk, evtdata[i]);
     switch(msk){
     case 0x01:
       // Hedaer
       igeo = (evtdata[i]&0x00ff0000)>> 16;
       //printf("header igeo=%d\n", igeo);
       break;
     case 0x00:
       if(igeo != -1){
	 if((evtdata[i] & 0x3fc00000) == 0x04000000){
	 // Event
	   ich = (evtdata[i]&0x003f0000) >> 16;
	   val = evtdata[i]&0xffff;
	   //std::cout << "MTDC32: ich, val: "<< ich << ", " << val << std::endl; 
	   TArtRawDataObject *rdata = new TArtRawDataObject(igeo,ich,val);
	   rawseg->PutData(rdata);
	 }
       }
       break;
     case 0x03:
       //printf("ender\n", igeo);
       // Ender
       igeo = -1;
     }
   }
   return 0;
}
