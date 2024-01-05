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
