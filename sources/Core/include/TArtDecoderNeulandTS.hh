/*
 * @file TArtDecoderNeulandTS.h
 * written by T. Isobe   
 *--------------------------------------------------------
 */
#ifndef TARTDECODERNEULANDTS_H
#define TARTDECODERNEULANDTS_H
#include "TArtDecoder.hh"
class TArtDecoderNeulandTS  : public TArtDecoder {
private:
   TArtDecoderNeulandTS();
   TArtDecoderNeulandTS(const TArtDecoderNeulandTS&rhs) {;}
public:
   static const int kID = 28;
   static TArtDecoder* Instance();
   ~TArtDecoderNeulandTS();
   int Decode(unsigned char* &buf, const unsigned int& size,
                      TArtRawSegmentObject *rawseg) ;
};
#endif // end of #ifdef TARTDECODERNeulandTS_H
