#include <ESP8266WiFi.h>
#include <ESPNowW.h>
#include <TaskScheduler.h>

Scheduler taskScheduler;

const int sensorPin = D2;
const int buzzerPin = D1;

uint8_t broadcastAddress[] = {0x48, 0x55, 0x19, 0xF0, 0xC7, 0x9B};

struct struct_message {
   int sensorValue;
};
struct_message dataToSend;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Pengiriman sukses");
  } else {
    Serial.println("Pengiriman Gagal");
  }
}


void readSensor(); // Declare readSensor function

Task taskSensor(100, TASK_FOREVER, &readSensor);

void readSensor() {
  // Baca sensor MC38
  int sensorValue = digitalRead(sensorPin);

  // Buat objek struct_message dan isi nilai sensor
  dataToSend.sensorValue = sensorValue;

  // Kirim data melalui ESPNow
  esp_now_send(broadcastAddress, (uint8_t*)&dataToSend, sizeof(dataToSend));

  // Cek nilai sensor
  if (sensorValue == 1) {
    digitalWrite(buzzerPin,HIGH); //hidupkan buzzer
    delay(100);
    digitalWrite(buzzerPin,LOW); //matikan buzzer
    delay(100);
  } else {
    // Jika nilai sensor adalah 1, aktifkan buzzer
    digitalWrite(buzzerPin,LOW); // matikan buzzer
  }

  // Lakukan sesuatu dengan nilai sensor, contoh: cetak nilai
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  taskScheduler.init();
  taskScheduler.addTask(taskSensor);
  taskSensor.enable();
  taskScheduler.startNow();
  readSensor();
}

void loop() {
  // Eksekusi tugas pada jadwal yang ditentukan oleh scheduler
  taskScheduler.execute();
}