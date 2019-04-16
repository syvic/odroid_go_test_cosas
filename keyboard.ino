#define PASO_LETRAS 10
#define INTERVALO_LECTURA 150
#define MAX_POS 20

const char letras[] = "_ABCDEFGHIJKLMNnOPQRSTUVWXYZ0123456789";
char pregunta[100] = "CUAL ES LA CAPITAL DE FRANCIA?";
char clave[40] = "PARIS";
char respuesta[MAX_POS];

byte letra_pos_x = 0;
byte letra_pos_y = 100;
byte letra_idx = 0;
byte pos = 0; // Posición del array donde guardamos la respuesta

void keyboard_first() {
  GO.lcd.setCursor(letra_pos_x, letra_pos_y - 20);
  GO.lcd.setTextSize(2);
  GO.lcd.print(pregunta);
  GO.lcd.setTextSize(1);
  GO.lcd.fillRect(letra_pos_x, letra_pos_y + 8, 5, 2,  RED);
}


void keyboard_check() {
  static unsigned long t_keyboard = 0;

  if (millis() - t_keyboard > INTERVALO_LECTURA) {
    t_keyboard = millis();

    if (GO.BtnA.isPressed() == 1) {
      GO.Speaker.tone(5000, 10);
    }

    if (GO.BtnB.isPressed() == 1) {
      GO.Speaker.tone(4000, 10);
    }

    if (GO.JOY_X.isAxisPressed() == 2) { //Tecla izquierda
      if (pos == 0) return;
      letra_pos_x -= PASO_LETRAS;
      pos--;
      GO.lcd.fillRect(0, letra_pos_y + 8, 240, 2,  BLACK);
      GO.lcd.fillRect(letra_pos_x, letra_pos_y + 8, 5, 2,  RED);
      //GO.lcd.setCursor(letra_pos_x, letra_pos_y);
      GO.Speaker.tone(300, 10);
    }

    if (GO.JOY_X.isAxisPressed() == 1) { //Tecla derecha
      if (pos == MAX_POS) return;
      letra_pos_x += PASO_LETRAS;
      pos++;
      GO.lcd.fillRect(0, letra_pos_y + 8, 240, 2,  BLACK);
      GO.lcd.fillRect(letra_pos_x, letra_pos_y + 8, 5, 2,  RED);
      //GO.lcd.setCursor(letra_pos_x, letra_pos_y);
      GO.Speaker.tone(300, 10);
    }

    if (GO.JOY_Y.isAxisPressed() == 2) { //Tecla arriba
      letra_idx++;
      if (letra_idx == 38) letra_idx = 0;
      GO.lcd.fillRect(letra_pos_x, letra_pos_y, 5, 7,  DARKCYAN);
      GO.lcd.setCursor(letra_pos_x, letra_pos_y);
      respuesta[pos] = letra_idx;
      GO.lcd.write(letras[letra_idx]);
      GO.Speaker.tone(200, 5);
    }

    if (GO.JOY_Y.isAxisPressed() == 1) { //Tecla abajo
      letra_idx--;
      if (letra_idx == 255) letra_idx = 38;
      GO.lcd.fillRect(letra_pos_x, letra_pos_y, 5, 7,  DARKCYAN);
      GO.lcd.setCursor(letra_pos_x, letra_pos_y);
      respuesta[pos] = letra_idx;
      GO.lcd.write(letras[letra_idx]);
      GO.Speaker.tone(200, 5);
    }

    if (GO.BtnStart.isPressed() == 1) {

      bool claveok = true;
      GO.lcd.setCursor(50, 50);
      for (int i = 0; i < strlen(clave); i++) if (letras[respuesta[i]] != clave[i]) claveok = false;

      GO.lcd.setCursor(150, 150);
      if (claveok) {
        GO.lcd.print("CLAVE OK!");
        sd_disable_roms(false);
        GO.Speaker.tone(700, 100);
        delay(100);
        GO.Speaker.tone(800, 100);
        delay(100);
        GO.Speaker.tone(900, 100);
        delay(100);
        GO.Speaker.tone(1100, 300);
        delay(250);

      }
      else {
        GO.lcd.print("CODIGO INCORRECTO");
        sd_disable_roms(true);
        GO.Speaker.tone(600, 120);
        delay(120);
        GO.Speaker.tone(500, 120);
        delay(120);
        GO.Speaker.tone(400, 120);
        delay(120);
        GO.Speaker.tone(300, 400);
        delay(400);
      }
      //delay(1000);
      GO.lcd.fillRect(150, 150, 8 * (MAX_POS - 1), 7,  BLACK); //Borramos el texto
      //GO.Speaker.tone(7000, 10);
    }
  }
}

void keyboard_print_response() {
  static unsigned long t_pr;

  if (millis() - t_pr > 200) {
    t_pr = millis();
    GO.lcd.setCursor(0, 200);
    GO.lcd.print("Respuesta es: ");
    GO.lcd.fillRect(6 * 14, 200, 6 * (MAX_POS - 1), 7,  RED);
    for (int i = 0; i < (MAX_POS - 1); i++) {
      GO.lcd.print(letras[respuesta[i]]);
    }
  }
}
