#include "WiFi.h"
#include "ArduinoJson.h"
 
const char* ssid = "Nokia 6.1 Plus";
const char* password =  "12345678";

#define SS_PIN 21  
#define RST_PIN 22 

#include <SPI.h>
#include <MFRC522.h>
#include <PubSubClient.h>

const char* mqtt_server = "lion.rmq.cloudamqp.com"; 
const char* mqtt_user = "ylhnwvqf:ylhnwvqf";
const char* mqtt_pass = "U0gpsudyXQBu8LiwAIOc7um4Av9ajPBd";
const char* clientSecret = "node_one";

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length)
{
    Serial.println("Message Recieved!");
    Serial.println(topic);
}

void reconnect() 
{
  // Loop until we're reconnected
  Serial.println("In reconnect...");
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientSecret, mqtt_user, mqtt_pass)) {
      client.subscribe("nodemcu_data_queue");
      client.setCallback(callback);
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");

  client.setServer(mqtt_server, 1883);
}
void loop() 
{
  client.loop();
  if (!client.connected()) 
  {
    reconnect();
  }

  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "8E 39 32 50") //change UID of the card that you want to give access
  {
    Serial.println(" Access Granted ");
    Serial.println(" Welcome Mr.Circuit ");
    delay(1000);
    Serial.println(" Have FUN ");
    Serial.println();
    statuss = 1;
  }
  
  else   {
    StaticJsonDocument<300> doc;
    char output[300];
    doc["content"] = content.substring(1);
    serializeJson(doc, output);
    Serial.println(output);
    client.publish("nodemcu_data_queue",  output);
    
    Serial.println(" Access Denied ");
    delay(3000);
  }
} 
