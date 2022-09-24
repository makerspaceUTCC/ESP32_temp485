#include <SoftwareSerial.h>
#include "crc16.h"

SoftwareSerial SoilSerial(32,33);   // (RX,TX)
byte values[11];
unsigned int modRead(byte Adrr,byte Fcode,byte reg){

  byte RegisterH=0x00,RegisterL=reg, len =0x01;
  //Fcode =0x03;
  byte x[6] = { Adrr, Fcode,RegisterH,RegisterL,0x00,len};
  uint16_t u16CRC=0xffff;
  for (int i = 0; i < 6; i++)
    {
        u16CRC = crc16_update(u16CRC, x[i]);

    }   
  byte request[8] = { Adrr, Fcode,RegisterH,RegisterL,0x00,len, lowByte(u16CRC),highByte(u16CRC)};
   /*  for(byte i=0;i<=7;i++){ //debug
        Serial.print(" 0x");
        Serial.print(request[i],HEX);
     }
     Serial.println("");*/
  if(SoilSerial.write(request,sizeof(request))==8){
     
     delay(100);
     for(byte i=0;i<7;i++){
        
        values[i] = SoilSerial.read();
        /*Serial.print(" 0x");
        Serial.print(values[i],HEX);*/
        
     }
     //Serial.println("");
     //Serial.flush();
  }
   unsigned int val = (values[3]<<8)| values[4];
     
   //Serial.print(val);
  return val;
  }
