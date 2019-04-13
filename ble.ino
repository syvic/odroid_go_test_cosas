//Scan Result: Name: NBScooter0575, Address: e3:0c:23:e1:b4:6c, manufacturer data: 4e422100000000de
//Scan Result: Name: NBScooter0575, Address: e3:0c:23:e1:b4:6c, manufacturer data: 4e422100000000de, serviceUUID: 6e400001-b5a3-f393-e0a9-e50e24dcca9e
static BLEUUID serviceUUID("6e400001-b5a3-f393-e0a9-e50e24dcca9e"); //Service UUID of fitnessband obtained through nRF connect application
static BLEUUID    charUUID("6e400001-b5a3-f393-e0a9-e50e24dcca9e"); //Characteristic  UUID of fitnessband obtained through nRF connect application
String My_BLE_Address = "e3:0c:23:e1:b4:6c"; //Hardware Bluetooth MAC of my fitnessband, will vary for every band obtained through nRF connect application
static BLERemoteCharacteristic* pRemoteCharacteristic;

BLEScan* pBLEScan; //Name the scanning device as pBLEScan
BLEScanResults foundDevices;

static BLEAddress *Server_BLE_Address;
static bool cacharro_encontrado = false;
String Scaned_BLE_Address;

boolean paired = false;


bool connectToserver (BLEAddress pAddress) {

  BLEClient*  pClient  = BLEDevice::createClient();
  GO.lcd.println(" - Created client");

  // Connect to the BLE Server.
  pClient->connect(pAddress);
  GO.lcd.println(" - Connected to scooter");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService != nullptr) {
    GO.lcd.println(" - Found our service");
    return true;
  }
  else {
    GO.lcd.println(" - NOT Found our service");
    return false;
  }

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic != nullptr)
    GO.lcd.println(" - Found our characteristic");

  return true;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf("Scan Result: %s \n", advertisedDevice.toString().c_str());
      //GO.lcd.printf("Scan Result: %s \n", advertisedDevice.toString().c_str());
      Server_BLE_Address = new BLEAddress(advertisedDevice.getAddress());

      Scaned_BLE_Address = Server_BLE_Address->toString().c_str();

      if (Scaned_BLE_Address == My_BLE_Address) {
        GO.lcd.println("Cacharro encontrado");
        cacharro_encontrado = true;

      }

    }
};

void ble_init() {
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); //Call the class that is defined above
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
}

void ble_connect() {

  //if (paired == true) return;

  foundDevices = pBLEScan->start(10); //Scan for 10 seconds

  while (cacharro_encontrado == true) {
    //if (Scaned_BLE_Address == My_BLE_Address && paired == false) {
    if (paired == false) {
      GO.lcd.println("Found Device :-)... connecting to Server as client");
      if (connectToserver(*Server_BLE_Address)) {
        paired = true;
        GO.lcd.println("********************LED turned ON************************");
        break;
      }
      else  {
        GO.lcd.println("Pairing failed");
        break;
      }
    }
    else {
      GO.lcd.printf("Dispositivo no encontrado: %d ", paired);
      GO.lcd.println(Scaned_BLE_Address);
      GO.lcd.println(My_BLE_Address);

    }

    /*if (Scaned_BLE_Address == My_BLE_Address && paired == true) {
      GO.lcd.println("Our device went out of range");
      paired = false;
      GO.lcd.println("********************LED OOOFFFFF************************");
      //digitalWrite (13, LOW);
      //ESP.restart();
      break;
      }
      else {
      Serial.println("We have some other BLe device in range");
      break;
      }*/
  }
}
