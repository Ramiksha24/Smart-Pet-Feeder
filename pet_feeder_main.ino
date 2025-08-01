// pet_feeder_main.ino

#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <Servo.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "secrets.h"  // WiFi and MQTT credentials

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);  // IST

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;

const int relayPin = D5;
const int alertPin = D6;
const int servoPin = D3;

const int CLOSE_ANGLE = 0;
const int OPEN_ANGLE = 120;

int feedHour = 0;
int feedMinute = 0;
bool feedNow = true;

// WiFi and MQTT clients
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_KEY);
Adafruit_MQTT_Subscribe onoff = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "/feeds/feedcontrol");

void setup() {
  Serial.begin(9600);

  Wire.begin(D2, D1);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("WELCOME");
  delay(1000);

  pinMode(relayPin, OUTPUT);
  pinMode(alertPin, OUTPUT);
  digitalWrite(alertPin, HIGH);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  timeClient.begin();
  mqtt.subscribe(&onoff);
  servo.attach(servoPin);
  servo.write(CLOSE_ANGLE);
}

void loop() {
  MQTT_connect();
  timeClient.update();

  int hh = timeClient.getHours();
  int mm = timeClient.getMinutes();
  int ss = timeClient.getSeconds();

  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(hh > 12 ? hh - 12 : hh);
  lcd.print(":");
  lcd.print(mm);
  lcd.print(":");
  lcd.print(ss);
  lcd.print(hh >= 12 ? " PM" : " AM");

  lcd.setCursor(0, 1);
  lcd.print("Feed Time: ");
  lcd.print(feedHour);
  lcd.print(":");
  lcd.print(feedMinute);

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(500))) {
    if (subscription == &onoff) {
      String msg = String((char *)onoff.lastread);
      Serial.println("MQTT: " + msg);

      if (msg == "ON") {
        openDoor();
        delay(1000);
        closeDoor();
      } else if (msg == "Morning") {
        feedHour = 10; feedMinute = 30;
      } else if (msg == "Afternoon") {
        feedHour = 13; feedMinute = 30;
      } else if (msg == "Evening") {
        feedHour = 18; feedMinute = 30;
      }
    }
  }

  if (hh == feedHour && mm == feedMinute && feedNow) {
    openDoor();
    delay(1000);
    closeDoor();
    feedNow = false;
  }

  if (mm != feedMinute) {
    feedNow = true;
  }
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) return;

  Serial.print("Connecting to MQTT...");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    mqtt.disconnect();
    delay(5000);
    if (--retries == 0) {
      while (1);
    }
  }
  Serial.println("Connected!");
}

void openDoor() {
  digitalWrite(relayPin, HIGH);
  delay(200);
  sendAlert();
  servo.write(OPEN_ANGLE);
}

void closeDoor() {
  digitalWrite(relayPin, LOW);
  delay(200);
  servo.write(CLOSE_ANGLE);
}

void sendAlert() {
  digitalWrite(alertPin, LOW);
  delay(1000);
  digitalWrite(alertPin, HIGH);
}
