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


#define PIN_MICS2714 1
#define DHTPIN A2
#define PIN_MQ135 3

#define DHTTYPE DHT11

//#define BLUETOOTH_ON

SoftwareSerial bSerial(11, 10);
DHT dht(DHTPIN, DHTTYPE);
MQ135 mq135_sensor = MQ135(PIN_MQ135);


//const int NO2=1; //NO2 sensor on analog 3
float Vout = 0; //output voltage
float Rs = 0; //Sensor Resistance
float ppbNO2 = 0; //ppb NO2
float ppmNO2 =0;


void setup()
{
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  bSerial.begin(9600);
  dht.begin();

}

void loop()
{
  float temperature= dht.readTemperature();
  float humidity= dht.readHumidity();

  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);
      
  // read NO2 sensor:
  Vout = analogRead(PIN_MICS2714)/409.2; // take reading and convert ADC value to voltage
  Rs = 22000/((5/Vout) - 1);   // find sensor resistance from Vout, using 5V input & 22kOhm load resistor
  ppbNO2 = (.000008*Rs - .0194);//*1000;    //convert Rs to ppb concentration NO2 (equation derived from data found on http://airpi.es/sensors.php
  ppmNO2 = ppmNO2/1000; //ppb to ppm
    
  //if(Serial.available()){
    
#ifdef BLUETOOTH_ON
    bSerial.print(temperature);
    bSerial.print(", ");
    bSerial.print(humidity);
    bSerial.print(", ");
    bSerial.print(ppbNO2);
    bSerial.print(", ");
    bSerial.println(correctedPPM);
#else

    Serial.print(temperature);
    Serial.print(", ");
    Serial.print(humidity);
    Serial.print(", ");
    Serial.print(ppbNO2);
    Serial.print(", ");
    Serial.println(ppm);
#endif  
    delay(1000);
  //}
}


