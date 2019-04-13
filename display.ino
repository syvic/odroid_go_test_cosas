
void display_go() {
  static uint8_t idx;
  static uint8_t rotate;
  static unsigned long t_last;
  if (millis() - t_last > 100) {
    Serial.println("In");
    t_last = millis();
    //GO.lcd.clearDisplay();
    GO.lcd.setRotation(rotate);
    GO.lcd.setCursor(30, 40);

    if (idx) {
      GO.lcd.setTextSize(1);
      GO.lcd.setTextFont(4);
      GO.lcd.setTextColor(MAGENTA);
    } else {
      GO.lcd.setTextSize(2);
      GO.lcd.setTextFont(1);
      GO.lcd.setTextColor(GREEN);
    }
    GO.lcd.print("PECON GO!");

    idx = !idx;
    rotate++;
    rotate = rotate % 4;
  }

}
