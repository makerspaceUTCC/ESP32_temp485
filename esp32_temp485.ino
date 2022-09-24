/* esp32 dev module///
 *  
 *  baud rate 115200 
 *  Rs485 modbus on SWserial pin Rx2 16/ Tx2 17
 *  temperature (0x01)and humidity(0x00) RS485 modbus
*/

#include "crc16.h"
#include "mod.h"


byte Adrr=0x01;
byte Fcode=0x04; // Fcode=0x04 read input register  modbus code

void setup()   
{
  Serial.begin(9600);
  SoilSerial.begin(9600); //9600 buadrate for XY-MD02

}

void loop()   
{
   
    float humidity =  modRead(Adrr,Fcode,0x02)/10.0;
    float temp = modRead(Adrr, Fcode,0x01)/10.0;
    Serial.print("Temp:  "); Serial.println(temp);
    Serial.print("Humidity:  "); Serial.println(humidity);

    delay(2000);
}
