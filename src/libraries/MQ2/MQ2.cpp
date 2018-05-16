/*
  MQ2.cpp - Library for flashing MQ2 code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

//#include "WProgram.h"
#include "MQ2.h"
#include "Arduino.h"

MQ2::MQ2(uint8_t pin, int rli_value,float r0_clear_air_Factor)
{
  _pin = pin;
  _rli_value = rli_value;
  _r0_clear_air_Factor = r0_clear_air_Factor;
}



float MQ2::MQResistanceCalculation(int raw_adc)
{
  return ( ((float)_rli_value*(1023-raw_adc)/raw_adc));
}

float MQ2::MQCalibration()
{
  int i;
  float RS_AIR_val=0,r0;

  for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) {                     //take multiple samples
    RS_AIR_val += MQResistanceCalculation(analogRead(_pin));
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  RS_AIR_val = RS_AIR_val/CALIBARAION_SAMPLE_TIMES;              //calculate the average value

  r0 = RS_AIR_val/_r0_clear_air_Factor;                      //RS_AIR_val divided by RO_CLEAN_AIR_FACTOR yields the Ro 
                                                                 //according to the chart in the datasheet 

  return r0; 
}

float MQ2::MQRead()
{
  int i;
  float rs=0;

  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += MQResistanceCalculation(analogRead(_pin));
    delay(READ_SAMPLE_INTERVAL);
  }

  rs = rs/READ_SAMPLE_TIMES;

  return rs;  
}


int MQ2::MQGetGasPercentage(float rs_ro_ratio, int gas_id)
{ 
  if ( accuracy == 0 ) {
  if ( gas_id == GAS_HYDROGEN ) {
    return (pow(10,((-2.109*(log10(rs_ro_ratio))) + 2.983)));
  } else if ( gas_id == GAS_LPG ) {
    return (pow(10,((-2.123*(log10(rs_ro_ratio))) + 2.758)));
  } else if ( gas_id == GAS_METHANE ) {
    return (pow(10,((-2.622*(log10(rs_ro_ratio))) + 3.635)));
  } else if ( gas_id == GAS_CARBON_MONOXIDE ) {
    return (pow(10,((-2.955*(log10(rs_ro_ratio))) + 4.457)));
  } else if ( gas_id == GAS_ALCOHOL ) {
    return (pow(10,((-2.692*(log10(rs_ro_ratio))) + 3.545)));
  } else if ( gas_id == GAS_SMOKE ) {
    return (pow(10,((-2.331*(log10(rs_ro_ratio))) + 3.596)));
  } else if ( gas_id == GAS_PROPANE ) {
    return (pow(10,((-2.174*(log10(rs_ro_ratio))) + 2.799)));
  }    
} 

  else if ( accuracy == 1 ) {
    if ( gas_id == GAS_HYDROGEN ) {
    return (pow(10,((-2.109*(log10(rs_ro_ratio))) + 2.983)));
  } else if ( gas_id == GAS_LPG ) {
    return (pow(10,((-2.123*(log10(rs_ro_ratio))) + 2.758)));
  } else if ( gas_id == GAS_METHANE ) {
    return (pow(10,((-2.622*(log10(rs_ro_ratio))) + 3.635)));
  } else if ( gas_id == GAS_CARBON_MONOXIDE ) {
    return (pow(10,((-2.955*(log10(rs_ro_ratio))) + 4.457)));
  } else if ( gas_id == GAS_ALCOHOL ) {
    return (pow(10,((-2.692*(log10(rs_ro_ratio))) + 3.545)));
  } else if ( gas_id == GAS_SMOKE ) {
    return (pow(10,(-0.976*pow((log10(rs_ro_ratio)), 2) - 2.018*(log10(rs_ro_ratio)) + 3.617)));
  } else if ( gas_id == GAS_PROPANE ) {
    return (pow(10,((-2.174*(log10(rs_ro_ratio))) + 2.799)));
  }
}    
  return 0;
}

