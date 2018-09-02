// Marque a opção new line no Monitor Serial e teste:
// Envie F Inicia o envio
// Envie S Para o envio

void setup(){
   Serial.begin(38400); // begin serial communication
}

void loop() {
  if(Serial.available()){
    if(Serial.read() == 49){
      Serial.print("33.3,44.33,444.44,33.3,55.55");
    }
  }
}