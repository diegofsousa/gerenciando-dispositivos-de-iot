#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
 
// Update these with values suitable for your network.


//SoftwareSerial Xbee(0,1);

String confirmation;

byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192,168,0,122);
IPAddress server(192,168,0,101); // IP DO BROKER 34.238.150.50 192.168.0.113

String a;
String b;

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

String service(byte* payload, unsigned int length){
  a = "";
  for(int i=0;i<length;i++){
    a += ((char)payload[i]);
  }
  
  if(a == "on"){
    return "1";
  } else if (a == "off"){
    return "0";
  } else{
    return "state";
  }
  
}

int con = 0;

char *convert(int key){
  if(key){
    return {"1"};
  }
  return {"0"};
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, "luz1") == 0){ 
     if(service(payload, length).equals("1") == true){
      Serial.println("luz1on");
    }else if(service(payload, length).equals("0") == true){
      Serial.println("luz1off");
    }else if(service(payload, length).equals("state") == true){
      Serial.println("luz1state");
    }
  }else if (strcmp(topic, "luz2") == 0){
     if(service(payload, length).equals("1") == true){
      Serial.println("luz2on");
    }else if(service(payload, length).equals("0") == true){
      Serial.println("luz2off");
    }else if(service(payload, length).equals("state") == true){
      Serial.println("luz2state");
    }
  }else if (strcmp(topic, "temp") == 0){
    if(service(payload, length).equals("state") == true){
      Serial.println("tempstate");
    }
  }
}

 
void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    // Attempt to connect
    if (mqttClient.connect("diego")) {
      // Once connected, publish an announcement...
      mqttClient.publish("outTopic","hello world");
      // ... and resubscribe
      mqttClient.subscribe("#");
    } else {
        
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void setup()
{
  Serial.begin(9600);
 
  mqttClient.setServer(server, 1883); //padrao 1883
  mqttClient.setCallback(callback);
 
  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}
 
void loop()
{
  confirmation = "";
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
  
  if(Serial.available()){
    while(Serial.available()){
      char a = Serial.read();
      confirmation += a;
      delay(10);
    }
    Serial.println(confirmation);
    if(confirmation.indexOf("r/luz1/on") >= 0){
      mqttClient.publish("r/luz1","1");
    }else if (confirmation.indexOf("r/luz1/off") >= 0){
      mqttClient.publish("r/luz1","0");
    }else if (confirmation.indexOf("r/luz2/on") >= 0){
      mqttClient.publish("r/luz2","1");
    }else if (confirmation.indexOf("r/luz2/off") >= 0){
      mqttClient.publish("r/luz2","0");
    }
    else if(confirmation.indexOf("rs/luz1/on") >= 0){
      mqttClient.publish("rs/luz1","1");
    }else if (confirmation.indexOf("rs/luz1/off") >= 0){
      mqttClient.publish("rs/luz1","0");
    }else if (confirmation.indexOf("rs/luz2/on") >= 0){
      mqttClient.publish("rs/luz2","1");
    }else if (confirmation.indexOf("rs/luz2/off") >= 0){
      mqttClient.publish("rs/luz2","0");
    }
    if (confirmation.startsWith("rs/temp/")){
        Serial.print("tentou publicar temperatura");
         char copy[3];
         String con = confirmation.substring(8, 10);
         con.toCharArray(copy, 3);
         mqttClient.publish("rs/temp",copy); 
    }          
  }
}

