/********************************************
  Projeto TCC
  Name: Projeto TCC
  Purpose: Medidor de poluição.

  @author EltonARodrigues
  @version 0.9 21/04/18
**********************************************/


#include <SoftwareSerial.h>
#include "MQ135.h"
#include "DHT.h"
#include "Streaming.h"


#define PIN_MICS2714 1
#define DHTPIN A2
#define PIN_MQ135 3

#define DHTTYPE DHT11

// Comment to show in Serial Monitor 
//#define BLUETOOTH_ON
#define BLUETOOTH_NAME "Sensor do AR"
#define BLUETOOTH_PASSWORD 1234

SoftwareSerial bluetooth(11, 10); // Emulate port (Rx, Tx) to use with hc-06

DHT dht(DHTPIN, DHTTYPE); 
MQ135 mq135_sensor = MQ135(PIN_MQ135);


float Vout = 0; //output voltage
float Rs = 0; //Sensor Resistance
float ppbNO2 = 0; //ppb NO2
float ppmNO2 =0;  //ppm NO2


void setup()
{
  Serial.begin(9600);
#ifdef BLUETOOTH_ON
  // bluetooh set config for name and password
  bluetooth.begin(9600);
  dht.begin();
  bluetooth.print("AT");
  delay(1000);
  bluetooth.print("AT+NAME");
  bluetooth.print("Sensor do AR");
  bluetooth.print("\r\n");
  delay(1000);
  bluetooth.print("AT+PIN");
  bluetooth.print(senha);
  bluetooth.print("\r\n");
  delay(1000);
#endif
}

void loop()
{
  // Get temp and humidity values with DHT11
  float temperature= dht.readTemperature();
  float humidity= dht.readHumidity();

  // Get sensor CO2 Resistence (RS/R0) to convert values in PPM
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);
      
  // Read NO2 sensor:
  Vout = analogRead(PIN_MICS2714)/409.2; // take reading and convert ADC value to voltage
  Rs = 22000/((5/Vout) - 1);   // find sensor resistance from Vout, using 5V input & 22kOhm load resistor
  ppbNO2 = (.000008*Rs - .0194);//*1000;    //convert Rs to ppb concentration NO2 (equation derived from data found on http://airpi.es/sensors.php
  ppmNO2 = ppmNO2/1000; //ppb to ppm
    
     
#ifdef BLUETOOTH_ON
  bluetooth << temperature << ", " << "humidity" << ", " << ppbNO2 << ", " << correctedPPM; 

#else
  Serial << temperature << ", " << humidity << ", " << ppbNO2 << ", " << correctedPPM; 

#endif  

  delay(1000);
}
