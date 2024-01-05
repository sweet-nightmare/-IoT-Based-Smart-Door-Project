#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

const int sensorPin = D2;
const int buzzerPin = D1;

uint8_t receiverAddress[] = {0x48, 0x55, 0x19, 0xf0, 0xc7, 0x9b};

struct struct_message {
  int sensorValue;
};
struct_message dataToSend;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 1) {
    Serial.println("Pengiriman sukses");
  } else {
    Serial.println("Pengiriman Gagal");
  }
}

void readSensor() {
  int sensorValue = digitalRead(sensorPin);
  dataToSend.sensorValue = sensorValue;

  esp_now_send(receiverAddress, (uint8_t*)&dataToSend, sizeof(dataToSend));

  if (sensorValue == 1) {
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  if (esp_now_add_peer(receiverAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0) != 0) {
    Serial.println("Failed to add peer");
  } else {
    Serial.println("Peer added");
  }

  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  readSensor();
}
