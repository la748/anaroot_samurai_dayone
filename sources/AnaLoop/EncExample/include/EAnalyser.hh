
#ifndef EANALYSER_HH
#define EANALYSER_HH

namespace Analyser{
  enum {
    GLOBAL=0,     //0
    PPAC,         //1
    PLASTIC,      //2
    IC=5,         //5

    MATRIX=10,    //10
    PID,          //11
    DALI,         //12
    CATANA,       //13

    NEBULA=21,    //21
    NEBULAHPC,    //22
    SAMURAIHOD,   //23
    SAMURAIPLA,   //24
    SAMURAIDC,    //25
    SAMURAITZero, //26
    SAMURAIIC,    //27
    SAMURAITED,   //28
    SAMURAIPID,   //29

    NEUTRON=31,   //31

    WINDS=40,     //40 // USE for PANDORA in SAMURAI Spring 2018 Campaign
    RPTOF,        //41
    RPDC,         //42
    ESPRI=45,     //45
    MINOS=51,     //51    
    SILICONS=52,  //52
    NEULAND=60,   //60
    HIME=61,      //61
    NINJA=62,     //62

//FIXME*/ /*FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*/  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    OUTOFRANGE=64, // DO NOT USE VALUE ABOVE 64, THEY MAKE ANALOOP CRASH, Giving an out of range valindex error!!
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*//*FIXME*/
    
    RAWDATA=99,   //99

  };
}

#endif
