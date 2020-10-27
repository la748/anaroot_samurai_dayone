/*
 * @file TArtDecoderVmeEasyroc.h
 */
#ifndef TARTDECODERVMEEASYROC_H
#define TARTDECODERVMEEASYROC_H
#include "TArtDecoder.hh"
class TArtDecoderVmeEasyroc  : public TArtDecoder {

private:
   TArtDecoderVmeEasyroc();
   TArtDecoderVmeEasyroc(const TArtDecoderVmeEasyroc&rhs) {;}
   //TArtDecoderVmeEasyroc& operator=(const TArtDecoderVmeEasyroc&rhs) {;}
public:
   static const int kID = 35;
   static TArtDecoder* Instance();
   ~TArtDecoderVmeEasyroc();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);
private:
  bool isAdcHg(unsigned int data);
  bool isAdcLg(unsigned int data);
  bool isTdcLeading(unsigned int data);
  bool isTdcTrailing(unsigned int data);
  bool isScaler(unsigned int data);

};
#endif // end of #ifdef TARTDECODERVMEEASYROC_H
