/*
 * @file TArtDecoderBigEndianV7XX.cc
 */
#include "TArtDecoderBigEndianV7XX.hh"
#include "TArtDecoderFactory.hh"
#include "TArtCore.hh"
#include "segidlist.hh"
#include <stdio.h>
#include <byteswap.h>
TArtDecoderBigEndianV7XX::TArtDecoderBigEndianV7XX()
   : TArtDecoder(kID) {
}

TArtDecoderBigEndianV7XX::~TArtDecoderBigEndianV7XX()
{
}

TArtDecoder* TArtDecoderBigEndianV7XX::Instance()
{
   static TArtDecoderBigEndianV7XX instance;
   return &instance;
}

int TArtDecoderBigEndianV7XX::Decode(unsigned char* &buf, 
				     const unsigned int& size,
				     TArtRawSegmentObject* rawseg)
{
  //  TArtCore::Info(__FILE__,"TArtDecoderBigEndianV7XX::Decode %d",size);

  unsigned char newbuf[size];
  unsigned char *nbuf = newbuf;
  for(int i=0; i<size; i+=4){
    newbuf[i] = buf[i+3];
    newbuf[i+1] = buf[i+2];
    newbuf[i+2] = buf[i+1];
    newbuf[i+3] = buf[i];
  }

  /*
  unsigned int *orgdata = (unsigned int*) buf;
  unsigned int evtsize = size/sizeof(unsigned int);
  unsigned int *newdata = (unsigned int*) newbuf;
  for (int i=0; i<evtsize; i++) newdata[i] = bswap_32(orgdata[i]);
  */

  TArtDecoderFactory::Instance()->GetDecoder(V792)->Decode(nbuf,size,rawseg);
  //  delete newbuf;

  return 0;
}
