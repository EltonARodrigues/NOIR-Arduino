/********************************************
  Projeto TCC
  Name: Projeto TCC
  Purpose: Medidor de poluição.

  @author EltonARodrigues
  @version 0.9.5 29/06/18
**********************************************/

#include <SoftwareSerial.h>
#include "MQ135.h"
#include "MQ7.h"
#include "DHT.h"
#include "Streaming.h"
#include <SPI.h>
#include <SD.h>


#define PIN_MICS2714 A1
#define DHTPIN 5
#define PIN_MQ135 A5
#define PIN_MQ7 A0  

#define DHTTYPE DHT11

///sdcard
#define SDCARD_ON
#define FILE_NAME "valores.csv"
File senforFile;

//#define BLUETOOTH_ON
#define BLUETOOTH_NAME "Dobrardor de AR"
#define BLUETOOTH_PASSWORD 0666

  SoftwareSerial bluetooth(7, 6); // Emulate port (Rx, Tx) to use with hc-06

DHT dht(DHTPIN, DHTTYPE); 
MQ135 mq135_sensor = MQ135(PIN_MQ135);
MQ7 mq7(PIN_MQ7,3.33);


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
  
  bluetooth.print("AT");
  delay(1000);
  bluetooth.print("AT+NAME");
  bluetooth.print("Sensor do AR");
  bluetooth.print("\r\n");
  delay(1000);
  bluetooth.print("AT+PIN");
  bluetooth.print(BLUETOOTH_PASSWORD);
  bluetooth.print("\r\n");
  delay(1000);
#endif

#ifdef SDCARD_ON
  if (!SD.begin(4)) {
    return;
  }
#endif


dht.begin();
}

void loop()
{
  // Get temp and humidity values with DHT11
  float temperature= dht.readTemperature();
  float humidity= dht.readHumidity();

  float ppmCO = mq7.getPPM();

  // Get sensor CO2 Resistence (RS/R0) to convert values in PPM
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);
      
  // Read NO2 sensor:
  Vout = analogRead(PIN_MICS2714)/409.2; // take reading and convert ADC value to voltage
  Rs = 22000/((5/Vout) - 1);   // find sensor resistance from Vout, using 5V input & 22kOhm load resistor
  ppbNO2 = (.000008*Rs - .0194)*1000;    //convert Rs to ppb concentration NO2 (equation derived from data found on http://airpi.es/sensors.php
  //ppmNO2 = ppmNO2/1000; //ppb to ppm
    
#ifdef SDCARD_ON
  char teste[150];
  sprintf(teste, "%f, %f, %f, %f, %f",temperature, humidity, ppbNO2, ppm, rzero, ppmCO);
  senforFile = SD.open(FILE_NAME, FILE_WRITE);
  senforFile.println(teste);
  senforFile.close();
#endif  
#ifdef BLUETOOTH_ON
  bluetooth << temperature << ", " << humidity << ", " << ppbNO2 << ", " << ppm << ","<< correctedPPM << "," << rzero <<"\n";
#else
  Serial << temperature << ", " << humidity << ", " << ppbNO2 << ", " << ppm << ", " << rzero << ", "<< ppmCO << "\n";
#endif  
  delay(1000);
}