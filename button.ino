void button_check() {
  if (GO.BtnA.wasPressed()) {
    GO.lcd.printf("wasPressed: A \r\n");
    //GO.lcd.clear();
    GO.Speaker.beep();
  }

  if (GO.BtnB.wasPressed()) {
    GO.Speaker.tone(500, 100);
    GO.lcd.printf("wasPressed: B. Llamando a BLE connect \r\n");
    //ble_connect();
    GO.Speaker.tone(500, 100);
  }

  if (GO.JOY_Y.isAxisPressed() == 2) {
    GO.lcd.printf("wasPressed: B \r\n");
    GO.Speaker.playMusic(m5stack_startup_music, 25000);
  }

  if (GO.BtnStart.wasPressed()) {
    GO.lcd.printf("wasPressed: Start \r\n");
    GO.Speaker.playMusic(m5stack_startup_music, 25000);
  }
}
