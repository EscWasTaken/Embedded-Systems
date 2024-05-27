#include <ArduinoBLE.h>

#define SERVICE_UUID           "40DB8631-DF2E-42D3-8D56-9AFD9487EA76" // UART service UUID
#define CHARACTERISTIC_UUID_READ "65D35962-BCCE-4784-B6B3-39A49DA1F4DC"
#define CHARACTERISTIC_UUID_NOTIFY "9E35CAAE-271B-4370-BAA8-ED8CEC59063F"

// create service
BLEService uartService(SERVICE_UUID); // create service

BLEStringCharacteristic readCharacteristic(CHARACTERISTIC_UUID_READ, BLERead, 30);
BLEStringCharacteristic notifyCharacteristic(CHARACTERISTIC_UUID_NOTIFY, BLENotify, 30);

const int pinNumber = 13;

void setup() {
  pinMode(pinNumber, INPUT); // Dont use pull up for 3v
  Serial.begin(9600);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }
  
  // set the local name peripheral advertises
  BLE.setLocalName("GarageStatus");
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(uartService);

  // add the characteristic to the service
  uartService.addCharacteristic(readCharacteristic);
  uartService.addCharacteristic(notifyCharacteristic);

  // add service
  BLE.addService(uartService);

  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  readCharacteristic.setEventHandler(BLEWritten, readCharacteristicWritten);
  // set an initial value for the characteristic
  readCharacteristic.setValue("False");

  // start advertising
  BLE.advertise();

  Serial.println(("Bluetooth device active, waiting for connections..."));
  Serial.println(BLE.address());
}

void loop() {
  

  // read the digital pin state
  bool pinState = digitalRead(pinNumber);
  Serial.println(digitalRead(pinNumber));
  // check if the pin state has changed
  static bool prevPinState = pinState;
  if (pinState != prevPinState) {
    Serial.println("Pinstate changed!");
    // update the readCharacteristic value based on the pin state
    if (pinState) { 
      // garage is open
      readCharacteristic.setValue("False");
    } else {
      // garage is closed
      readCharacteristic.setValue("True");
    }

    // update the notifyCharacteristic with the new value
    notifyCharacteristic.setValue(readCharacteristic.value());

    // update the previous pin state
    prevPinState = pinState;
  }

  // poll for BLE events
  BLE.poll();
  delay(1000);
}

void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void readCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.print("Characteristic event, written: ");

  Serial.println("len=" + 
                  String(readCharacteristic.valueLength()));
  String valString = readCharacteristic.value();
  Serial.println(valString);
  valString.toUpperCase();
  Serial.println(valString);
  notifyCharacteristic.setValue(valString);
}
