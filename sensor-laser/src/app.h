#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

typedef struct {
  String ssid;
  String password;
  int ip[4];
} WifiConfig;

typedef struct {
  int date;
  int time;
  float altitude;
  float course;
  float speed;
  float lat;
  float lng;
} GPSData;

typedef struct {
  String name;
  int data;
  void read() {
    Serial.println("I read!"+data);
  }
} Sensor;

typedef struct {
  float temperature;
  float humidity;
} DHT11Data;

typedef struct {
  GPSData gps;
  DHT11Data dht11;
} SensorData;

void setupWifi(WifiConfig wifiConfig);
void setupPlantower(Sensor s);
void setupGPS(Sensor s);
void setupDHT11(Sensor s);
