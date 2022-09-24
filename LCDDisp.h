#include <Wire.h>
#include <U8g2lib.h>
bool LCD = true;


U8G2_SSD1327_MIDAS_128X128_F_HW_I2C u8g2( // note "F_HW" version gave best results
/* No Rotation*/U8G2_R0,
/* reset=/ U8X8_PIN_NONE,
/ clock=/ 25, // use NON-adefault I2C pins for NodeMCU-32S
/ data=*/ 26); // data pin

u8g2_uint_t u8g2_draw_unifont_Thai(u8g2_uint_t x, u8g2_uint_t y, const char *str);
u8g2_uint_t u8g2_draw_unifont_Thai(u8g2_uint_t x, u8g2_uint_t y, const char *str)
{
  uint16_t e;
  u8g2_uint_t delta, sum;
  u8g2.getU8g2()->u8x8.next_cb = u8x8_utf8_next;
  u8x8_utf8_init(u8g2.getU8x8());
  sum = 0;
  for(;;)
  {
    e = u8g2.getU8g2()->u8x8.next_cb(u8g2.getU8x8(), (uint8_t)*str);
    if ( e == 0x0ffff )
      break;
    str++;
    if ( e != 0x0fffe )
    {
      
      switch(e)
      {
  /* many more glyphs and corrections are missing */
  /* please report to https://github.com/olikraus/u8g2/issues/584 */
  /* แปลงอักษรไทยให้อยู่ในรูป Hex ที่ https://r12a.github.io/app-conversion/ */
  /*  ธ-0E18        า-0E32        
      ต-0E15        ุ-0E38
      อ-0E2D        ห-0E2B 
      ร-0E23        ใ-0E43
      น-0E19        ด-0E14
      ิ-0E34        น-0E19    */
  /*ใส่ชุดขยับสระและวรรณยุกต์และตัวอักษร*/
  //case 0x0E18: x-= 3;break;//ธ
  case 0x0E32: x-= 5; break;//า
  //case 0x0E15: x-= 3; break;//ต
  case 0x0E38: x-= 14; break;//ุ
  case 0x0E2D: x-= 3; break;//อ
  //case 0x0E2B: x-= 2; break;//ห
  //case 0x0E23: x-= 3; break;
  //case 0x0E43: x-= 3; break;
  //case 0x0E19: x-= 3; break;
  case 0x0E14: x-= 3; break;
  case 0x0E34: x-= 14; break;
  case 0x0E19: x-= 3; break;
  //case 0x0E19: x-= 3; break;

      }
      delta = u8g2.drawGlyph(x, y, e);    
      switch(e)
      {
   /*การขยับตัวอักษรในกรณีที่ซ้ำกับด้านบน*/
  case 0x0E32: x-= 3; break;//า
  //case 0x0E19: x-= 3; break;// น ทำซ้ำกับด้านบนไม่ได้
  
      }
      x += delta;
      sum += delta;    
    }
  }
  return sum;
}

void Monitoring(float Soil_Hum,float Soil_Temp,float Soil_EC,float Soil_pH,float Soil_N,float Soil_P,float Soil_K)
{
    u8g2.firstPage(); 
    do {
  if (LCD==true){
  u8g2.setFont(u8g2_font_etl24thai_t);
  
  /* Important: do not write background pixel */
  u8g2.setFontMode(1);


    u8g2.clearBuffer();
    u8g2_draw_unifont_Thai(3,18,"ธาตุอาหารดิน");  // Hello World
    u8g2.drawLine(0, 26, 127, 26); /*  - bottom*/

    u8g2.setFont(u8g2_font_courR10_tr);
    u8g2.setFontDirection(0);
    u8g2.setCursor(0,39);
    u8g2.print("N :");
    u8g2.setCursor(30, 39);
    u8g2.print(Soil_N,0);

    u8g2.setFont(u8g2_font_courR10_tr);
    u8g2.setFontDirection(0);
    u8g2.setCursor(0, 52);
    u8g2.print("P :");
    u8g2.setFontDirection(0);
    u8g2.setCursor(30, 52);
    u8g2.print(Soil_P,0);

    u8g2.setFont(u8g2_font_courR10_tr);
    u8g2.setFontDirection(0);
    u8g2.setCursor(0, 66);
    u8g2.print("K :");
    u8g2.setCursor(30,66);
    u8g2.print(Soil_K,0);

    u8g2.setFont(u8g2_font_courR10_tr);
    u8g2.setFontDirection(0);
    u8g2.setCursor(0, 78);
    u8g2.print("pH:");
    u8g2.setCursor(30, 78);
    u8g2.print(Soil_pH*0.1);

    u8g2.setFont(u8g2_font_courR10_tr);
    u8g2.setFontDirection(0);
    u8g2.setCursor(0, 91);
    u8g2.print("H :");
    u8g2.setCursor(30, 91);
    u8g2.print(Soil_Hum*0.1);

  u8g2.setFont(u8g2_font_courR10_tr);
  u8g2.setFontDirection(0);
  u8g2.setCursor(0, 102);
  u8g2.print("T :");
  u8g2.setCursor(30, 102);
  u8g2.print(Soil_Temp*0.1);

  u8g2.setFont(u8g2_font_courR10_tr);
  u8g2.setFontDirection(0);
  u8g2.setCursor(0, 116);
  u8g2.print("EC:");
  u8g2.setCursor(30, 116);
  u8g2.print(Soil_EC*0.001);
  u8g2.sendBuffer();
  }
  delay(5000);
  } while( u8g2.nextPage() );
 }
