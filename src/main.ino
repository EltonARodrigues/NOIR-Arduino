/********************************************
  Projeto TCC
  Name: Projeto TCC
  Purpose: Medidor de poluição.

  @author EltonARodrigues
  @version 0.9.8 02/07/18
**********************************************/

#include <SoftwareSerial.h>
#include<DSM501.h>
#include "MQ135.h"
#include "MQ7.h"
#include "DHT.h"
#include "Streaming.h"
#include <SPI.h>
#include <SD.h>


// Calibration resistance at atmospheric CO2 level
float rzero = 76.63;

#define DSM501_PM10 3
#define DSM501_PM25 4
#define PIN_MICS2714 A1
#define DHTPIN 5
#define PIN_MQ135 A5
#define PIN_MQ7 A0  

#define DHTTYPE DHT22

///sdcard
bool sdcard_status = false;
#define FILE_NAME "valores.csv"
File sensorFile;

bool bluetooh_status = false;
#define BLUETOOTH_NAME "Dobrador  de AR"
#define BLUETOOTH_PASSWORD 0666

SoftwareSerial bluetooth(6, 7); // Emulate port (Rx, Tx) to use with hc-06

DSM501 dsm501(DSM501_PM10, DSM501_PM25);
DHT dht(DHTPIN, DHTTYPE); 
//MQ135 mq135_sensor = MQ135(PIN_MQ135);
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
  dsm501.begin(MIN_WIN_SPAN);
  digitalWrite(ledPinBlue, LOW);
  digitalWrite(ledPinSD, LOW);
  pinMode(ledPinSD, OUTPUT);
  pinMode(ledPinBlue, OUTPUT);
  pinMode(buttonPinBlue, INPUT);
  pinMode(buttonPinSD, INPUT);
  //(buttonPinMQC, INPUT);
  Serial.begin(9600);
  dht.begin();
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
    for (int i = 1; i <= 60; i++)
    {
      delay(1000); // 1s
      Serial.print(i);
      Serial.println(" s (wait 60s for DSM501 to warm up)");
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
      while(1){
        digitalWrite(ledPinSD, HIGH);
        delay(1000);
        digitalWrite(ledPinSD, LOW);
      }
    }
  }
}

void loop()
{
  
  delay(1000);
  dsm501.update();
  if(digitalRead(buttonPinBlue) == HIGH){
    //rzero = mq135_sensor.getRZero();
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
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  float ppmCO = mq7.getPPM();

  // Get sensor CO2 Resistence (RS/R0) to convert values in PPM
  //float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  //float resistance = mq135_sensor.getResistance();
  float ppmco2 = 0; // = mq135_sensor.getPPM(rzero);
  //float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity, rzero);
  float pm10_weight = dsm501.getParticleWeight(0);
  float pm25_weight = dsm501.getParticleWeight(1);
  float pm25 = dsm501.getPM25();
  Serial.println(" ug/m3");
  
  // get PM density of particles over 2.5 μm
  Serial.print("PM25: ");
  Serial.print(dsm501.getParticleWeight(1));
  Serial.println(" ug/m3");
  
  Serial.print("AQI: ");
  Serial.println(dsm501.getAQI());
  
  // get PM2.5 density of particles between 1.0~2.5 μm
  Serial.print("PM2.5: ");
  Serial.print(dsm501.getPM25());

    
  if(sdcard_status){
    sensorFile = SD.open(FILE_NAME, FILE_WRITE);
    sensorFile << temperature << ", " << humidity << ", " << ppmCO << ", " << ppmco2 << ", "  << pm10_weight << ", " <<  pm25_weight << pm25 << ", ""\n";
    sensorFile.close();
    if(digitalRead(buttonPinSD) == HIGH){
      digitalWrite(ledPinSD, LOW);
      while(1);
    }
  }
  if(bluetooh_status)
    bluetooth << temperature << ", " << humidity << ", " << ppmCO << ", " << ppmco2 << ", "  << pm10_weight << ", " <<  pm25_weight << pm25 << ", ""\n";
  else
    Serial << temperature << ", " << humidity << ", " << ppmCO << ", " << ppmco2 << ", "  << pm10_weight << ", " <<  pm25_weight << pm25 << ", ""\n";
}