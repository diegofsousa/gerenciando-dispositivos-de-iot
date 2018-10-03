#include <SoftwareSerial.h>

SoftwareSerial bluetooth(10,11);

String comando;

int rele9 = 9;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
bluetooth.begin(9600);
}

void loop() {
  comando = "";
if(bluetooth.available()){
    while(bluetooth.available()){
    char caractere =  bluetooth.read();
    comando += caractere;
  }
    Serial.println(comando);
}
    if(comando.indexOf('1') >=0){
      digitalWrite(rele9,HIGH);
      }
    if(comando.indexOf('0') >=0){
      digitalWrite(rele9,LOW);
      }   
}
