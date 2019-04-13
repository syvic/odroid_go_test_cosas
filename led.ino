

void led_blink(){
  static unsigned long t_led=0;

  if (millis()-t_led > 100){
    t_led=millis();
    digitalWrite(PIN_BLUE_LED, !digitalRead(PIN_BLUE_LED));
  }
}
