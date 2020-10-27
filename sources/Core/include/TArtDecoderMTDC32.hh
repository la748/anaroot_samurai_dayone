#ifndef TARTDECODERMTDC32_H
#define TARTDECODERMTDC32_H
#include "TArtDecoder.hh"
class TArtDecoderMTDC32  : public TArtDecoder {
private:
   TArtDecoderMTDC32();
   TArtDecoderMTDC32(const TArtDecoderMTDC32&rhs) {;}
   //TArtDecoderMTDC32& operator=(const TArtDecoderMTDC32&rhs) {;}
public:
   static const int kID = 34;
   static TArtDecoder* Instance();
   ~TArtDecoderMTDC32();
   int Decode(unsigned char* &buf, const unsigned int& size,
	      TArtRawSegmentObject *rawseg);
};
#endif // end of #ifdef TARTDECODERMTDC32_H
