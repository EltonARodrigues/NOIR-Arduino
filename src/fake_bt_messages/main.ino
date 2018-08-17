#include <Streaming.h>
#include <SoftwareSerial.h>
//PM1.0( ug/m3), PM2.5( ug/m3)  Não tem valores coletados, por isso tudo igual

/* SoftwareSerial serial_values(10, 11); */

void setup() {
  Serial.begin(9600);
  delay(1000);
}

int values;

void loop() {
  // Temperatura(ªC), Umidade(%), CO2(ppm), CO(ppm), PM1.0( ug/m3), PM2.5( ug/m3)
  Serial <<  "3.9,58.5,3.16,320.93,16.34";
  delay(1000);
}
