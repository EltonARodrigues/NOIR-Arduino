/********************************************
  Projeto TCC
  Name: Projeto TCC
  Purpose: Medidor de poluição.

  @author EltonARodrigues
  @version 0.9.8 02/07/18
**********************************************/

#include <SoftwareSerial.h>
#include "MQ135.h"
#include "MQ7.h"
#include "DHT.h"
#include "Streaming.h"
#include <SPI.h>
#include <SD.h>

// Calibration resistance at atmospheric CO2 level
float rzero = 76.63;

#define PIN_MICS2714 A1
#define DHTPIN 5
#define PIN_MQ135 A5
#define PIN_MQ7 A0  

#define DHTTYPE DHT11

///sdcard
bool sdcard_status = false;
#define FILE_NAME "valores.csv"
File sensorFile;

bool bluetooh_status = false;
#define BLUETOOTH_NAME "Dobrardor de AR"
#define BLUETOOTH_PASSWORD 0666

  SoftwareSerial bluetooth(7, 6); // Emulate port (Rx, Tx) to use with hc-06

//DHT dht(DHTPIN, DHTTYPE); 
MQ135 mq135_sensor = MQ135(PIN_MQ135);
MQ7 mq7(PIN_MQ7,5.0);


float Vout = 0; //output voltage
float Rs = 0; //Sensor Resistance
float ppbNO2 = 0; //ppb NO2
float ppmNO2 =0;  //ppm NO2

// led and button config 
const int buttonPinBlue = 2;
const int buttonPinSD = 3;
//const int buttonPinMQC = 10;
const int ledPinBlue =  8;
const int ledPinSD =  9;


void setup()
{
  digitalWrite(ledPinBlue, LOW);
  digitalWrite(ledPinSD, LOW);
  pinMode(ledPinSD, OUTPUT);
  pinMode(ledPinBlue, OUTPUT);
  pinMode(buttonPinBlue, INPUT);
  pinMode(buttonPinSD, INPUT);
  //(buttonPinMQC, INPUT);
  Serial.begin(9600);
  //dht.begin();
  Serial.println("INICIO");
  while(1){
    if (digitalRead(buttonPinBlue) == HIGH){
      digitalWrite(ledPinBlue, HIGH);
      bluetooh_status = true;
      break;
    }
    else if (digitalRead(buttonPinSD) == HIGH){
      digitalWrite(ledPinSD, HIGH);
      sdcard_status = true;
      break;
    }
  }
  if(bluetooh_status){
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
  }
  if(sdcard_status){
    if (!SD.begin(4)) {
      Serial.println("Erro to Start SDCARD");
      while(1);
    }
  }
}

void loop()
{
  delay(1000);
  if(digitalRead(buttonPinBlue) == HIGH){
    rzero = mq135_sensor.getRZero();
    if(digitalRead(ledPinBlue) == HIGH){
      digitalWrite(ledPinBlue, LOW);
      delay(200);
      digitalWrite(ledPinBlue, HIGH);
    }
    else{
      digitalWrite(ledPinBlue, HIGH);
      delay(200);
      digitalWrite(ledPinBlue, LOW);
    }
  }
  // Get temp and humidity values with DHT11
  float temperature= 25.0;//dht.readTemperature();
  float humidity= 80.0;//dht.readHumidity();

  float ppmCO = mq7.getPPM();

  // Get sensor CO2 Resistence (RS/R0) to convert values in PPM
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM(rzero);
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity, rzero);
      
  // Read NO2 sensor:
  Vout = analogRead(PIN_MICS2714)/409.2; // take reading and convert ADC value to voltage
  Rs = 22000/((5/Vout) - 1);   // find sensor resistance from Vout, using 5V input & 22kOhm load resistor
  ppbNO2 = (.000008*Rs - .0194)*1000;    //convert Rs to ppb concentration NO2 (equation derived from data found on http://airpi.es/sensors.php
  //ppmNO2 = ppmNO2/1000; //ppb to ppm
    
  if(sdcard_status){
    sensorFile = SD.open(FILE_NAME, FILE_WRITE);
    sensorFile << temperature << ", " << humidity << ", " << ppbNO2<< ", " << ppm << ", " << rzero << ", " << ppmCO << "\n";
    sensorFile.close();
    if(digitalRead(buttonPinSD) == HIGH){
      digitalWrite(ledPinSD, LOW);
      while(1);
    }
  }
  if(bluetooh_status)
    bluetooth << temperature << ", " << humidity << ", " << ppbNO2 << ", " << ppm << ","<< correctedPPM << "," << rzero <<"\n";
  else
    Serial << temperature << ", " << humidity << ", " << ppbNO2 << ", " << ppm << ", " << rzero << ", "<< ppmCO << "\n";
}