#include "app.h"

const int DEBUG = 0;

GPSData gps;
DHT11Data dht11;
PlantowerData plantower;

void setup() {
  Serial.begin(115200); // Cambia para conectar directamente el PMS, hay que desconectarlo para subir un programa
  Serial.println("Starting...");

  SPIFFS.begin();
  // setupWifi();
  setupGPS();
  setupPlantower();
}

void loop() {
  gps = getGPSData();
  if(gps.ready) {
    dht11 = getDHT11Data();
    plantower = getPlantowerData();
    save();
  }
}

void save() {
  char filename [] = "log";                     // Assign a filename or use the format e.g. SD.open("datalog.txt",...);
  File file = SPIFFS.open(filename, "a+");        // Open a file for reading and writing (appending)
  if (!file) {
    Serial.println("file open failed");   // Check for errors
    return;
  }

  String frame = csvFrame();
  Serial.println(frame);
  file.println(frame);
  file.close();
}

String csvFrame() {
  /* CSV is ordered as:
   "lat","lng","date","time","altitude","course","speed","humidity",
   "temperature","pm1","pm25","pm10"
  */

  // First datum is the sensor_id
  String frame = SENSOR_ID + STR_COMMA;

  // Follows GPS data
  frame += gps.lat + STR_COMMA;
  frame += gps.lng + STR_COMMA;
  frame += gps.date + STR_COMMA;
  frame += gps.time + STR_COMMA;
  frame += gps.altitude + STR_COMMA;
  frame += gps.course + STR_COMMA;
  frame += gps.speed + STR_COMMA;

  // Follows DHT11
  if(dht11.ready) {
    frame += dht11.humidity + STR_COMMA;
    frame += dht11.temperature + STR_COMMA;
  } else {
    frame += STR_NULL + STR_COMMA + STR_NULL + STR_COMMA;
  }

  // Follows PlantowerData
  if(plantower.ready) {
    frame += plantower.pm1 + STR_COMMA;
    frame += plantower.pm25 + STR_COMMA;
    frame += plantower.pm10;
  } else {
    frame += STR_NULL + STR_COMMA + STR_NULL + STR_COMMA;
  }

  return frame;
}

String readLog() {

}

void send() {
  String data = readLog();

}
