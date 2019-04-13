
void neoxipel_rainbow(uint8_t wait) {
  static unsigned long t_last;
  static uint16_t i = 0, j = 0;

  if (millis() - t_last > wait) {
    t_last = millis();

    strip.setPixelColor(i, neopixel_wheel((i + j) & 255));

    if (j++ == 255) j = 0;   
    
    if (i++ == strip.numPixels()-1) {
      i = 0;
      strip.show(); 
    }
  }

}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t neopixel_wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
