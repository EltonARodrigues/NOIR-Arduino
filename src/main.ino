/********************************************
  Projeto TCC
  Name: Projeto TCC
  Purpose: Medidor de poluição.

  @author EltonARodrigues
  @version 0.9.8.5 27/07/18
**********************************************/

#include <SoftwareSerial.h>
//#include<DSM501.h>
#include "MQ135.h"
#include "MQ7.h"
#include "DHT.h"
#include "Streaming.h"
#include <SPI.h>
#include <SD.h>


// Calibration resistance at atmospheric CO2 level
float rzero = 76.63;

#define DSM501_PM10 6
#define DSM501_PM25 3
#define DHTPIN 5
#define PIN_MQ135 A4
#define PIN_MQ7 A0  

#define DHTTYPE DHT22

///sdcard
bool sdcard_status = false;
#define FILE_NAME "valores.csv"
File sensorFile;

bool bluetooh_status = false;
#define BLUETOOTH_NAME "Dobrador  de AR"
#define BLUETOOTH_PASSWORD 0666

//SoftwareSerial bluetooth(6, 7); // Emulate port (Rx, Tx) to use with hc-06

//uint32_t win_total[2];
//uint32_t start;
//uint32_t get_span;
//uint32_t low_total[2];

const int pin = 3 ;// the input pin connect with  the Vout2 of Dust Sensor 
float duration;
float starttime;
float sampletime_ms = 30000;
float lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

#define TIME_MAX  30
float pm25val = 0.05; 
float pm25coef = 0.00207916725464941;//The coefficient of pm2.5
float pm25[TIME_MAX+1] ;//collect data of pm2.5 after a period of time

//DSM501 dsm501(DSM501_PM10, DSM501_PM25);
DHT dht(DHTPIN, DHTTYPE); 
//MQ135 mq135_sensor = MQ135(PIN_MQ135);
MQ7 mq7(PIN_MQ7,5.0);


float Vout = 0; //output voltage
float Rs = 0; //Sensor Resistance
float ppbNO2 = 0; //ppb NO2
float ppmNO2 =0;  //ppm NO2

// led and button config 
const int buttonPinBlue = 2;
const int buttonPinSD = 10;
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

  Serial.begin(9600);
  //get_span = dsm501.begin(MIN_WIN_SPAN);
  dht.begin();

  Serial.println("INICIO"); // deve ser removido no futuro 

  for (int i = 1; i <= 60; i++)
  {
    digitalWrite(ledPinSD, HIGH);
    digitalWrite(ledPinBlue, HIGH);
    delay(500);
    digitalWrite(ledPinSD, LOW);
    digitalWrite(ledPinBlue, LOW);
    delay(500);
  }

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
    //bluetooth.begin(9600);
    
    //bluetooth.print("AT");
    delay(1000);
    //bluetooth.print("AT+NAME");
    //bluetooth.print("Sensor do AR");
    //bluetooth.print("\r\n");
    delay(1000);
    //bluetooth.print("AT+PIN");
    //bluetooth.print(BLUETOOTH_PASSWORD);
    //bluetooth.print("\r\n");
    delay(1000);
  }
  if(sdcard_status){
    if (!SD.begin(4)) {
      Serial.println("Erro to Start SDCARD");
      while(1){
        digitalWrite(ledPinSD, HIGH);
        delay(200);
        digitalWrite(ledPinSD, LOW);
      }
    }
  }
  starttime = millis();
  
  for(int j=TIME_MAX;j>0;j--)
    pm25[j]=0.05;
    pm25[0]=1.5;
  //start = millis();
}

void loop()
{
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
   /*if (millis() < start || millis() - start >= get_span){
      dsm501.update(win_total, low_total);
      win_total[0] = 0;
      win_total[1] =0;
      low_total[0] = 0;
      low_total[1] = 0;
      start = millis();
      Serial.println("deve ter ido");
  }
  win_total[0] ++;
  low_total[0] += !digitalRead(DSM501_PM10);
	win_total[1] ++;
	low_total[1] += !digitalRead(DSM501_PM25);
  */
  //dsm501.update();
  if ((millis()-starttime) > sampletime_ms)
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10);  // Integer percentage 0=>100. The unit for time of low pulse occupancy is us.
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    pm25val = pm25coef * concentration * 10; 
    
    pm25[0] = pm25val;
    
    lowpulseoccupancy = 0;
    starttime = millis();
  }
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
  float ppmco2 = 0;//mq135_sensor.getPPM(rzero);
  //float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity, rzero);
  //float pm10_weight = dsm501.getParticleWeight(0);
  //float pm25_weight = dsm501.getParticleWeight(1);
  //float pm25 = dsm501.getPM25();
    
  if(sdcard_status){
    sensorFile = SD.open(FILE_NAME, FILE_WRITE);
    sensorFile << temperature << ", " << humidity << ", " << ppmCO << ", " << ppmco2 << ", " <<  pm25val  << "\n";
    sensorFile.close();
    if(digitalRead(buttonPinSD) == HIGH){
      digitalWrite(ledPinSD, LOW);
      while(1);
    }
  }
  //if(bluetooh_status)
    //bluetooth << temperature << ", " << humidity << ", " << ppmCO << ", " << ppmco2 << ", "  << pm10_weight << ", " <<  pm25_weight << pm25 << ", ""\n";
  //else
    Serial << temperature << ", " << humidity << ", " << ppmCO << ", " << ppmco2 << ", " << pm25val  << "\n";
  delay(1000);
}