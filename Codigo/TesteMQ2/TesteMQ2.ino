#include <MQ2.h>


float           Ro = 0;                            //Ro is initialized to 10 kilo ohms
MQ2 teste(A0);

void setup() {
  Serial.begin(9600);                               //UART setup, baudrate = 9600bps
  Serial.print("Calibrating...\n");    
            
  Ro =  teste.MQCalibration();                       //Calibrating the sensor. Please make sure the sensor is in clean air 
                                                    //when you perform the calibration                    
  Serial.print("Calibration is done...\n"); 
  Serial.print("Ro=");
  Serial.print(Ro);
  Serial.print("kohm");
  Serial.print("\n");

}

void loop() {

   Serial.print("HYDROGEN:"); 
   Serial.print(teste.MQGetGasPercentage(teste.MQRead()/Ro,GAS_HYDROGEN) );
   Serial.print( "ppm" );
   Serial.print("    ");   
   Serial.print("LPG:"); 
   Serial.print(teste.MQGetGasPercentage(teste.MQRead()/Ro,GAS_LPG) );
   Serial.print( "ppm" );
   Serial.print("    ");   
   Serial.print("METHANE:"); 
   Serial.print(teste.MQGetGasPercentage(teste.MQRead()/Ro,GAS_METHANE) );
   Serial.print( "ppm" );
   Serial.print("    ");   
   Serial.print("CARBON_MONOXIDE:"); 
   Serial.print(teste.MQGetGasPercentage(teste.MQRead()/Ro,GAS_CARBON_MONOXIDE) );
   Serial.print( "ppm" );
   Serial.print("    ");   
   Serial.print("ALCOHOL:"); 
   Serial.print(teste.MQGetGasPercentage(teste.MQRead()/Ro,GAS_ALCOHOL) );
   Serial.print( "ppm" );
   Serial.print("    ");   
   Serial.print("SMOKE:"); 
   Serial.print(teste.MQGetGasPercentage(teste.MQRead()/Ro,GAS_SMOKE) );
   Serial.print( "ppm" );
   Serial.print("    ");   
   Serial.print("PROPANE:"); 
   Serial.print(teste.MQGetGasPercentage(teste.MQRead()/Ro,GAS_PROPANE) );
   Serial.print( "ppm" );
   Serial.print("\n");
   delay(200);
}

