#include "app.h"

Sensor plantower;
Sensor gps;
Sensor dht11;
WifiConfig wifi;

void setup() {
  Serial.begin(9600);

  setupWifi(wifi);

  setupPlantower(plantower);
  setupGPS(gps);
  setupDHT11(dht11);
}

void loop() {
  plantower.read();
  gps.read();
  dht11.read();
}
