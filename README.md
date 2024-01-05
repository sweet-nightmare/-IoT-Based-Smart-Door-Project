# Project Smart Door Berbasis IoT 
**Nama Kelompok:**
Al-Alamin

**Anggota:**
1. FERIKHO FATIH AZHAR (1103202190)
2. MOHAMAD AQWAM FARID (1103201263)
3. KRESHNA PUTRA S.    (1103202215)
4. NURDIN              (1103204006)
5. ADAM AL-AZIZ OLII   (1103204045)

**Institusi:**
Telkom University

# Ringkasan Project
Project ini bertujuan untuk merancang sistem smart door atau pintu cerdas berbasis IoT. Sistem ini nantinya akan dapat mendeteksi apabila terjadi usaha pembobolan pintu yang tidak diinginkan, lalu mengirim sinyal yang dapat memicu alarm untuk berbunyi.

**Perkiraan waktu pengerjaan yang dibutuhkan**

Sekitar 3 jam


# Pendahuluan
**Latar Belakang:**
Kemajuan teknologi mendorong peningkatkan kualitas serta efektivitas segala aspek kehidupan manusia. Keamanan adalah salah satu aspek yang perlu untuk diperhatikan dan senantiasa diperbarui kualitasnya. Dengan sistem smart door yang kami buat, proses monitoring keamanan pintu dapat dilakukan dengan lebih mudah dan efisien serta lebih optimal dalam penggunaan sumber dayanya.

**Tujuan:**
Dapat memahami konsep IoT sederhana serta menerapkannya dengan cara merancang suatu alat, yakni Sistem Smart Door Berbasis IoT.

**Manfaat:**
Dapat memahami dasar-dasar dalam perancangan alat IoT meliputi pembuatan kodingan, rangkaian elektronik, jaringan, serta database. 

# Alat-alat
Komponen | Fungsi | Jumlah | Harga Satuan
-| -| -| -
ESP8266 NodeMCU V3 | Mengumpulkan data dari sensor, melakukan transfer data, menggerakkan aktuator | 2 | Rp 55.000
Buzzer | Digunakan untuk alarm ketika pintu terbuka | 1 | Rp 1.800
MC-38 Sensor | Sensor magnetik yang dapat mengirimkan sinyal biner 0 atau 1 | 1 | Rp 9.000
Solenoid Lock | Sebagai aktuator untuk mengunci pintu | 1 | Rp 43.000
Relay 5V | Sebagai pengendali arus listrik rangkaian | 1 | Rp 5.000
Baterai 9V | Sebagai penyuplai daya pada rangkaian | 2 | Rp 10.000

![material](aaa.jpg)
Gambar 1 : Alat-alat yang digunakan

# Setup
**Setup IDE:**

Download dan Install PlatformIO pada VSCode untuk menyusun kode program.

Download dan install library yang diperlukan untuk project, antara lain :
1. PubSubClient
2. ESPNow
3. TaskScheduler

# Desain Rangkaian
![material](https://github.com/sweet-nightmare/IoT-Based-Smart-Door-Project/blob/main/NodeDesign.png)
Gambar 2 : Desain Rangkaian

Sistem terdiri dari dua node. Node pertama sebagai sender dan node kedua sebagai receiver.

Untuk node pertama, kabel positif sensor MC-38 terhubung ke pin D2 node MCU dan kabel negatif ke pin ground. Lalu, pin positif buzzer terhubung ke pin D1 dan kabel negatif ke pin ground node MCU.

Untuk node kedua, pin D1 pada relay dihubungkan dengan pin D1 node MCU, pin 5V terhubung ke pin Vin node MCU, dan pin ground relay terhubung ke pin ground node MCU. Lalu, untuk kabel positif solenoid lock terhubung ke pin COM relay, dan kabel negatif solenoid lock terhubung ke kabel negatif power supply. Sedangkan kabel positif power supply terhubung ke pin NO pada relay.

# Platform yang digunakan
Platform yang digunakan sebagai antarmuka atau dashboard adalah Node-RED. Node-RED adalah alat pemrograman visual berbasis web yang dibangun di atas Node.js. Node-RED dapat digunakan untuk membuat desain antarmuka grafis berbasis browser yang memungkinkan pengguna untuk membuat alur kerja (flow-based programming) dengan menghubungkan node-node yang mewakili fungsi atau layanan berbeda. 

![Node-RED](https://github.com/sweet-nightmare/IoT-Based-Smart-Door-Project/blob/main/NodeRED1.jpg)
Gambar 3 : Node-RED Flow


![Node-REDUI](https://github.com/sweet-nightmare/IoT-Based-Smart-Door-Project/blob/main/NodeRED2.jpg)
Gambar 4 : Dashboard Node-RED

# Coding Program
**Node Sender:**
```cpp
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
```

**Node Receiver:**
```cpp
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <espnow.h>

const int relayPin = D2;

// WiFi
const char *ssid = "Ikooo's WiFi";
const char *password = "23112311";


// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "kelasiotesp/Al-Alamin/relay";
const char *mqtttopic = "kelasiotesp/Al-Alamin/sensor";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

struct struct_message {
  int sensorValue;
};
struct_message receivedData;

void onDataReceived(uint8_t *mac, uint8_t *data, uint8_t len) {
  memcpy(&receivedData, data, sizeof(receivedData));
  Serial.print("Received Sensor Value: ");
  Serial.println(receivedData.sensorValue);

String notification;
  if (receivedData.sensorValue == 1) {
    notification = "Pintu Terbuka";
  } else if (receivedData.sensorValue == 0) {
    notification = "Pintu Tertutup";
  }

  // Publish the notification to MQTT
  client.publish(mqtttopic, notification.c_str());
}


void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("Relay status: ");

  String payloadStr = "";
  for (int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }

  if (strcmp(topic, topic) == 0) {
    if (payloadStr.equals("1")) {
      digitalWrite(relayPin, LOW);
      Serial.println("Relay turned ON");
    } else if (payloadStr.equals("0")) {
      digitalWrite(relayPin, HIGH);
      Serial.println("Relay turned OFF");
    }
  }

  Serial.println();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()) {
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public emqx mqtt broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }

  esp_now_init();
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onDataReceived);

  client.publish(topic, "Hi EMQX I'm esp ^^");
  client.subscribe(topic);
  client.subscribe(mqtttopic);
  pinMode(relayPin, OUTPUT);
}

void loop() {
  client.loop();

}
```

# Konektivitas dan Pertukaran Data


# Presenting the data


# Desain Final
Berikut ini adalah desain final dari rangkaian yang telah kami buat.
![Sender](https://github.com/sweet-nightmare/IoT-Based-Smart-Door-Project/blob/main/Node_Sender.jpg)
Gambar 5 : Desain Sender

![Receiver](https://github.com/sweet-nightmare/IoT-Based-Smart-Door-Project/blob/main/Node_Receiver.jpg)
Gambar 6 : Desain Receiver

# Kesimpulan
Meskipun banyak tantangan yang dilalui, namun pada akhirnya kami berhasil menyelesaikan project ini. Setelah berhasil menyelesaikan project Smart Door Berbasis IoT ini, kami mendapatkan banyak pelajaran terutama terkait dengan pemahaman mengenai konsep IoT. Kami belajar bagaimana cara mendesain suatu rangkaian, menyusun rangkaian, membuat kode supaya rangkaiannya bisa berjalan sesuai dengan yang kita inginkan, serta menghubungkan kode tersebut dengan dashbord UI.

