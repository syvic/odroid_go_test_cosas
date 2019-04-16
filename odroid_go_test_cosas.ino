#include <odroid_go.h>
#include "BluetoothSerial.h"
#include <Adafruit_NeoPixel.h>
#include <BLEDevice.h>

#define PIN_BLUE_LED 2
#define NEOPIXEL_PIN 12
#define NEOPIXEL_COUNT 12

extern char respuesta[];
extern const char letras[];
extern char pregunta[];


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

BluetoothSerial ESP_BT;

void setup() {
  Serial.begin(115200);
  GO.begin();
  pinMode(PIN_BLUE_LED, OUTPUT);
  GO.lcd.println("\nGO OK");

  GO.Speaker.setVolume(1); //El volumen va de 0 a 11. Parece que 11 es apagado
  GO.Speaker.playMusic(m5stack_startup_music, 25000);

  strip.begin();
  GO.lcd.println("Strip init OK");

  //ble_init();
  //GO.lcd.println("BLE init OK");
  //ble_connect();
  //GO.lcd.println("BLE Connect OK");

  //delay(5000);
  GO.lcd.println("Ready!");
  
  sd_init();
  sd_info();
  keyboard_first();
}

void loop() {
  GO.update();
  sd_check_dir_status();
  
  keyboard_check();

  keyboard_print_response();


  //neoxipel_rainbow(8);
  //led_blink();
  //bat_show();

  //display_go();

}
