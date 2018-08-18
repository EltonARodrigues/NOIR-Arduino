// Marque a opção new line no Monitor Serial e teste:
// Envie F Inicia o envio
// Envie S Para o envio

void setup(void){
   Serial.begin(9600); // begin serial communication
}

void loop(void) {
   if (Serial.available()>0) {
      while(Serial.available()>0) {
        if(Serial.read() == 102){
          while(true){
            Serial.println("33.3,44.33,444.44,33.3,55.55");
            if(Serial.read() == 115) break;
          delay(1000);
          }
        }
      }
      delay(100);
   }
}
