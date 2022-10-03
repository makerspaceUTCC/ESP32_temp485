/* esp32 dev module///
 *  
 *  baud rate 115200 
 *  Rs485 modbus on SWserial pin Rx2 16/ Tx2 17
 *  temperature (0x01)and humidity(0x00) RS485 modbus
*/
#include <WiFi.h>
#include "PubSubClient.h"
#include "crc16.h"
#include "mod.h"
#include "LCDDisp.h"

WiFiClient wifiClient;
PubSubClient client(wifiClient); 

const char* ssid = "xxxxx";                 // Your personal network SSID
const char* wifi_password = "xxxxx"; // Your personal network password

const char* mqtt_server = "broker.hivemq.com";  // IP of the MQTT broker

const char* humidity_topic = "UTCC/Computer0000/humidity";
const char* temperature_topic = "UTCC/Computer0000/temperature";
const char* mqtt_username = ""; // MQTT username
const char* mqtt_password = ""; // MQTT password
const char* clientID = "makerspaceUTCC0000"; // MQTT client ID

byte Adrr=0x01;
byte Fcode=0x04; // Fcode=0x04 read input register  modbus code




void setup()   
{
  Serial.begin(9600);
  SoilSerial.begin(9600); //9600 buadrate for XY-MD02
  u8g2.begin();
  u8g2.setBusClock(10000000); // seems to work reliably, and very fast
  u8g2.clearBuffer();  
  
  WiFi.begin(ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); 
  }
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, 1883);
  connect_MQTT();
  
}

void loop()   
{
    
    float humidity =  modRead(Adrr,Fcode,0x02)/10.0;
    float temp = modRead(Adrr, Fcode,0x01)/10.0;
    Serial.print("Temp:  "); Serial.println(temp);
    Serial.print("Humidity:  "); Serial.println(humidity);
   
    u8g2.setFont(u8g2_font_courR12_tr);
    u8g2.setFontMode(1);
    u8g2.clearBuffer();
    u8g2.setCursor(5,20);
    u8g2.print("Sensor ID1");
    u8g2.drawLine(0, 26, 127, 26);
    u8g2.setCursor(5,50);
    u8g2.print("Humidity");
    u8g2.setCursor(88,50);
    u8g2.print(humidity,1);
    
 /*   u8g2.setCursor(5,80);
    u8g2.print("temp");
    u8g2.setCursor(88,80);
    u8g2.print(temp,1);   
*/
    
    u8g2.sendBuffer();
    if (!client.connected()) {
    connect_MQTT();
    }
    client.loop();
    pubmqtt(humidity, temp);
    
    delay(2000);
}

void connect_MQTT(){
  while (!client.connect(clientID,mqtt_username,mqtt_password)) {
    //Serial.println("Re-Connected to MQTT Broker!");
    //Serial.print("failed, rc=");
    //Serial.print(client.state());
    delay(2000);
  }
    Serial.println("Connection to MQTT Broker ...");
}

void pubmqtt(float humidity, float temp)
{
    String hs="Hum: "+ String (humidity) +" % ";
    String ts="Temp: "+ String(temp) +" C ";
    // PUBLISH to the MQTT Broker 
    if (client.publish(temperature_topic, String(ts).c_str())) {
    Serial.println("Temperature sent!");
    }
    if (client.publish(humidity_topic, String(hs).c_str())) {
      Serial.println("Humidity sent!");
    }
    client.disconnect();
  }
