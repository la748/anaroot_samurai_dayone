/*
 * @file TArtDecoderHINP.cc
 * @date  Created : 2008/11/26 18:59:56 JST<BR>
 *  Last Modified : 2008/11/27 00:19:14 JST
 *--------------------------------------------------------
 *    Comment : 
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008 by Shinsuke OTA <ota@ribf.riken.jp>
 */
#include "TArtDecoderHINP.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include <stdio.h>
#include <iostream>
TArtDecoderHINP::TArtDecoderHINP()
   : TArtDecoder(kID) {
}

TArtDecoderHINP::~TArtDecoderHINP()
{
}

TArtDecoder* TArtDecoderHINP::Instance()
{
   static TArtDecoderHINP instance;
   return &instance;
}

int TArtDecoderHINP::Decode(unsigned char* &buf, const unsigned int& size,
			      TArtRawSegmentObject* rawseg)
{
   unsigned short *evtdata = (unsigned short*) buf;
   unsigned int evtsize = size/sizeof(unsigned short);
   int i, igeo, ich;

   if(evtsize < 10) return 0;

   for (i=7; i<evtsize; i+=3) {
     //std::cout << "evtsize" << evtsize << " i=" << i << std::endl;
     if(evtsize < i+3) break;

     igeo = (evtdata[i] >> 5) & 0xff;
     ich = evtdata[i] & 0x000f;

     //std::cout << "geo=" << igeo << "  ch=" << ich << std::endl;

     // 0-15 = Analog, 16-31 = Timing
     // for Analog
     TArtRawDataObject *ardata = new TArtRawDataObject(igeo,ich,evtdata[i+1]&0x7fff);
     rawseg->PutData(ardata);

     // for Timing
     TArtRawDataObject *trdata = new TArtRawDataObject(igeo,ich+16,evtdata[i+2]&0x7fff);
     rawseg->PutData(trdata);

   }

   return 0;
}
