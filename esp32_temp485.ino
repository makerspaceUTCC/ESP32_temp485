/* esp32 dev module///
 *  
 *  baud rate 115200 
 *  Rs485 modbus on SWserial pin Rx2 16/ Tx2 17
 *  temperature (0x01)and humidity(0x00) RS485 modbus
*/

#include "crc16.h"
#include "mod.h"
#include "LCDDisp.h"

byte Adrr=0x01;
byte Fcode=0x04; // Fcode=0x04 read input register  modbus code

void setup()   
{
  Serial.begin(9600);
  SoilSerial.begin(9600); //9600 buadrate for XY-MD02
  u8g2.begin();
  u8g2.setBusClock(10000000); // seems to work reliably, and very fast
  u8g2.clearBuffer();  
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
    
    u8g2.sendBuffer();
    delay(2000);
}
