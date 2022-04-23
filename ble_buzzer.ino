#include <ArduinoBLE.h>

#define BUZZER_PIN 2
#define LED_PIN 8

byte prevButtonState = 0x00;
long previousBuzzerMillis = 0;
long previousLedMillis = 0;

BLEService buzzerService("be09ae42-9157-4eed-87d1-1f3b4e31510f");
BLEByteCharacteristic buzzerCharacteristic(
  "22f64ef5-b567-4e5d-bbac-4b384e6a6c36",
  BLERead | BLENotify
);

void setup() {
  Serial.println("starting..");

  if (!BLE.begin()) {
    Serial.println("starting BLE module failed!");
    while (1);
  }

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, INPUT);

  BLE.setLocalName("Buzzer");

  BLE.setAdvertisedService(buzzerService);
  buzzerService.addCharacteristic(buzzerCharacteristic);
  BLE.addService(buzzerService);

  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  buzzerCharacteristic.writeValue(prevButtonState);

  BLE.advertise();
}

void loop() {
  BLE.poll();
  BLEDevice central = BLE.central();
  buzzerPolling(central && central.connected());
  ledPolling(central && central.connected());
  delay(50);
}

void blePeripheralConnectHandler(BLEDevice central) {
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void buzzerPolling(bool connected) {
  if (connected) {
    byte newButtonState = digitalRead(BUZZER_PIN) ? 0x00 : 0xFF;
    long currentMillis = millis();
    if (currentMillis - previousBuzzerMillis >= 200 && prevButtonState != newButtonState) {
      prevButtonState = newButtonState;
      Serial.print("new State");
      Serial.println(newButtonState);
      buzzerCharacteristic.writeValue(newButtonState);
    }
  }
}

void ledPolling(bool connected) {
  long currentMillis = millis();
  byte currentLed = digitalRead(LED_PIN);
  if (connected && !currentLed) {
    // if connected and not yet turned on, turn on
    digitalWrite(LED_PIN, HIGH);
  } else if (!connected && currentMillis - previousLedMillis >= 250) {
    // if not connected, change led state all 250ms
    previousLedMillis = currentMillis;
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
}
