#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>

// WiFi 설정
const char* ssid = "This_is_mine"; 
const char* password = "12345qwerty"; 
const char* mqtt_server = "broker.hivemq.com"; 

WiFiClient espClient;
PubSubClient client(espClient);

// 핀 설정
const int relayPin = D8;
const int sensorPin = D3; // 디지털 센서 핀
const int analogSensorPin = A0; // 아날로그 센서 핀

bool pumpActive = false;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);

  // 명령을 확인하고 릴레이를 켭니다.
  if (String(topic) == "/pet/water/command") {
    if (message == "") {
      Serial.println("Turning relay ON");
      digitalWrite(relayPin, HIGH);  // 릴레이 켜기
      pumpActive = true;             // 펌프 활성화 상태 설정
      Serial.println("급수 버튼이 눌렸음");
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("/pet/water/command");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

String getCurrentTime() {
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);

  char buffer[30];
  sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d", p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday, p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
  return String(buffer);
}

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  pinMode(sensorPin, INPUT_PULLUP); // 디지털 센서 핀 설정
  digitalWrite(relayPin, LOW); // 릴레이 초기 상태 끄기

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  configTime(9 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("Waiting for time sync...");
  while (time(nullptr) < 8 * 3600 * 2) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("Time synchronized");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected. Reconnecting...");
    setup_wifi();
  }

  if (!client.connected()) {
    Serial.println("MQTT not connected. Reconnecting...");
    reconnect();
  }
  client.loop();

  int analogSensorValue = analogRead(analogSensorPin);
  Serial.print("Water Level Sensor Value: ");
  Serial.println(analogSensorValue);

//  String currentValue = String(analogSensorValue / 100 * 100);
//  client.publish("/pet/water/status", currentValue.c_str());
//  Serial.println("Water level status published");

  if (analogSensorValue <= 400) {
    String payload = "LOW_WATER_LEVEL - " + getCurrentTime();
    client.publish("/pet/water/alert", payload.c_str());
    Serial.println("LOW_WATER_LEVEL alert published");
  }

  int digitalSensorValue = digitalRead(sensorPin);
  Serial.print("Digital Sensor Value: ");
  Serial.println(digitalSensorValue);

  if (digitalSensorValue == LOW) {
    Serial.println("WATER REACHED TO THE LEVEL");

    String payload = "FULL_WATER_LEVEL - " + getCurrentTime();
    client.publish("/pet/water/alert", payload.c_str());
    Serial.println("FULL_WATER_LEVEL alert published");
    

    if (pumpActive) {
      Serial.println("Sensor LOW, turning relay OFF");
      digitalWrite(relayPin, LOW);   // 릴레이 끄기
      pumpActive = false;            // 펌프 비활성화 상태 설정
    }
  } else {
    Serial.println("Water level not reached");
  }

  delay(1000);
}
