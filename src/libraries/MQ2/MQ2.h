/*
  MQ2.h - Library for flashing MQ2 code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
/**************************************************************************/
/*!
@file     MQ1.h(mudar)
@author   Elton de Andrade Rodrigues (mudar)
@license  GNU GPLv3


/**************************************************************************/

/************************mq2sensor************************************/
/************************Hardware Related Macros************************************/
#define         MQ2PIN                       (0)     //define which analog input channel you are going to use
#define         RL_VALUE_MQ2                 (1)     //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR_MQ2      (9.577)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
                                                     //which is derived from the chart in datasheet

/***********************Software Related Macros************************************/
#define         CALIBARAION_SAMPLE_TIMES     (50)    //define how many samples you are going to take in the calibration phase
#define         CALIBRATION_SAMPLE_INTERVAL  (500)   //define the time interal(in milisecond) between each samples in the
                                                     //cablibration phase
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in 
//normal operation

/**********************Application Related Macros**********************************/
#define         GAS_HYDROGEN                  (0)
#define         GAS_LPG                       (1)
#define         GAS_METHANE                   (2)
#define         GAS_CARBON_MONOXIDE           (3)
#define         GAS_ALCOHOL                   (4)
#define         GAS_SMOKE                     (5)
#define         GAS_PROPANE                   (6)
#define         accuracy                      (0)   //for linearcurves
//#define         accuracy                    (1)   //for nonlinearcurves, un comment this line and comment the above line if calculations 
                                                    //are to be done using non linear curve equations
/*****************************Globals************************************************/

#ifndef MQ2_h
#define MQ2_h

#include "Arduino.h"

//#include "WProgram.h"

class MQ2
{
  public:
    MQ2(int pin);
    float MQResistanceCalculation(int raw_adc);
    float MQCalibration();
    float MQRead();
    int MQGetGasPercentage(float rs_ro_ratio, int gas_id);
  private:
    int _pin;
};

#endif

