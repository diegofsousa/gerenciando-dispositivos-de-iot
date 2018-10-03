#include "DHT.h"
#define DHTPIN A1 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);
String received;
int rele9 = 9;

void setup(){
  pinMode(rele9, OUTPUT);
  Serial.begin(9600);
  dht.begin();
}
void loop(){
  received = "";
  // A leitura da temperatura e umidade pode levar 250ms!
  // O atraso do sensor pode chegar a 2 segundos.
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // testa se retorno é valido, caso contrário algo está errado.
  if(Serial.available()){
   while(Serial.available()){
    char mesagem = Serial.read();
    received += mesagem;
    delay(10);
   }
   if(received.indexOf("luz1on") >=0){
      digitalWrite(rele9,HIGH);
      Serial.println("r/luz1/on");
   }
   if(received.indexOf("luz1off") >=0){
      digitalWrite(rele9,LOW);
      Serial.println("r/luz1/off");
   }
   if(received.indexOf("tempstate") >=0){
      if (isnan(t) || isnan(h)) {
        Serial.println("Failed to read from DHT");
        } 
      else{
        Serial.print("rs/temp/");
        Serial.println(t);
        }
      }
}
}
