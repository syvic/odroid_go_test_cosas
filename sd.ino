
#include "FS.h"
#include "SD.h"
#include "SPI.h"

void sd_list_dir(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        sd_list_dir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void sd_create_dir(fs::FS &fs, const char * path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void sd_remove_dir(fs::FS &fs, const char * path) {
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

void sd_read_file_orig(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

char* sd_read_file(fs::FS &fs, const char * path) {
  char contenido[100];
  byte contenido_idx=0;
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return 0;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    contenido[contenido_idx]= file.read();
  }
  file.close();
  return contenido;
}

void sd_write_file(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void sd_append_file(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

bool sd_rename_file(fs::FS &fs, const char * path1, const char * path2) {
  GO.lcd.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    GO.lcd.println("File renamed");
    return 1;
  } else {
    GO.lcd.println("Rename failed");
    return 0;
  }
}

void sd_delete_file(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void sd_test_file_IO(fs::FS &fs, const char * path) {
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if (file) {
    len = file.size();
    size_t flen = len;
    start = millis();
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }


  file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for (i = 0; i < 2048; i++) {
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}

void sd_info() {
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    GO.lcd.println("No SD card attached");
    return;
  }

  GO.lcd.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    GO.lcd.println("MMC");
  } else if (cardType == CARD_SD) {
    GO.lcd.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    GO.lcd.println("SDHC");
  } else {
    GO.lcd.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  GO.lcd.printf("SD Card Size: %lluMB\n", cardSize);

  GO.lcd.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  GO.lcd.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}


void sd_init() {
  if (!SD.begin()) {
    GO.lcd.println("Card Mount Failed");
    return;
  }

  //sd_read_file(SD, "/pregunta.txt", *pregunta);

}

void sd_disable_roms(bool action) {

  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    GO.lcd.println("No SD card attached");
    return;
  }
  if (action == true) {
    if (sd_rename_file(SD, "/roms", "/roms2")) {
      sd_remove_dir(SD, "/roms");
      //GO.lcd.println("\nROMS -> ROMS2");
    }
    else {
      //GO.lcd.println("\nROMS -> ROMS2 FALLO");
    }
  }
  else {
    if (sd_rename_file(SD, "/roms2", "/roms")) {
      sd_remove_dir(SD, "/roms2");
      //GO.lcd.println("\nROMS2 -> ROMS");
    }
    else {
      //GO.lcd.println("\nROMS2 -> ROMS FALLO");
    }
  }

}

void sd_check_dir_status() {
  static unsigned long t_status = 0;
  static bool status = false;
  File roms = SD.open("/roms");
  if (!roms) {
    //GO.lcd.print("No ROMS dir");
    status = false;
  }
  else {
    //GO.lcd.print("ROMS dir OK");
    status = true;
  }

  if (millis() - t_status > 2000) {
    t_status = millis();
    GO.lcd.setCursor(0, 0);
    if (status) {
      GO.lcd.fillRect(0, 0, 320, 8,  GREEN);
      GO.lcd.print("CODIGO DESBLOQUEADO");
    }
    else {
      GO.lcd.fillRect(0, 0, 320, 8,  RED);
      GO.lcd.print("CODIGO BLOQUEADO");
    }
  }
}
