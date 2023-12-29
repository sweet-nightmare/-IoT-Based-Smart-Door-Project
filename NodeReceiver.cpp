#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESPNowW.h>

const int relayPin = 2;


const char *ssid = "Ikooo's WiFi";
const char *password = "23112311";

const char *mqtt_broker = "broker.emqx.io";
const char *topic1 = "kelasiotesp/Al-Alamin/relay";
const char *mqtttopic = "kelasiotesp/Al-Alamin/sensor";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const char *clientId = "MASKO";

const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
uint32_t counter;
char str[80];

typedef struct struct_message {
    int sensorValue;
} struct_message;

struct_message receivedData;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&receivedData, incomingData, sizeof(struct_message));
if (receivedData.sensorValue == 1) {
    digitalWrite(relayPin, HIGH); // Aktifkan alarm
    Serial.println("Alarm: Terdeteksi Pembobolan Pintu!");
    delay(1000); // Tambahkan delay untuk suara alarm atau tindakan lainnya
    digitalWrite(relayPin, LOW); // Matikan alarm
    if (client.connected()) {
      client.publish(mqtttopic, "Terdeteksi Pembobolan Pintu!");
    }
}
}
void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");

  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("Relay status: ");

  String payloadStr = "";
  for (unsigned int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }

  if (strcmp(topic1, topic1) == 0) { 
    if (payloadStr.equals("0")) { 
      digitalWrite(relayPin, HIGH); 
      Serial.println("Relay turned ON");
    } else if (payloadStr.equals("1")) { 
      digitalWrite(relayPin, LOW);  
      Serial.println("Relay turned OFF");
    }
  }

  Serial.println();
}



void setup() {

 Serial.begin(115200);

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
    if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
 }
 
  client.publish(topic1, "Hi EMQX I'm esp ^^");
  client.subscribe(topic1);
 
  pinMode(relayPin, OUTPUT);
}

void reconnect() {
  while (!client.connected()) {
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("Attempting to reconnect to the public MQTT broker as %s\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Reconnected to MQTT broker");

      client.subscribe(topic1);
  
    } else {
      Serial.print("Reconnect failed, state=");
      Serial.print(client.state());
      Serial.println(". Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void loop() {
 client.loop();
 if (!client.connected()) {
    reconnect();
  }
  delay(1000);
}