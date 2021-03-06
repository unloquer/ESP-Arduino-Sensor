#include "app.h"

int INIT = 1;
const int DEBUG = 0;
const int READ_LOG = 0;
const int DELETE_LOG = 1;
const int SEND_LOG = 0;
const int SEND_RECORD = 1;

GPSData gps;
DHT11Data dht11;
PlantowerData plantower;

#include <DoubleResetDetector.h>

// Number of seconds after reset during which a
// subseqent reset will be considered a double reset.
#define DRD_TIMEOUT 100

// RTC Memory Address for the DoubleResetDetector to use
#define DRD_ADDRESS 0

DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);

void readLog() {
  Serial.println("Reading log ...");
  File file = SPIFFS.open("log", "r");
  String line = "";
  if (!file) Serial.println("file open failed");  // Check for errors
  while (file.available()) {
    wdt_disable();
    // Read all the data from the file and display it

    char c = file.read();
    if(c == '\r') {
      if(!line.endsWith("NULL")) {
        Serial.println("Posting: "+line);
        if(SEND_RECORD) {
          postCsv("http://45.55.34.88:3000/api/v0/air.csv", line);
        }
      }

      // Discard the \n, which is the next byte
      file.read();
      line = "";
    } else {
      line += String(c);
    }
    wdt_enable(1000);
  }
}

void deleteLog() {
  Serial.println("Deleting log");
  SPIFFS.remove("log");
}

void sendLog() {
  String url = "http://192.168.0.18:3000/api/v0/air.csv";
  postCsvFile(url, "log");
}

void syncLog() {
  if(!INIT) { return; }
  INIT = 0;

  if(READ_LOG) {
    readLog();
  }

  if(SEND_LOG) {
    sendLog();
  }

  if(DELETE_LOG) {
    deleteLog();
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nStarting ...");

  SPIFFS.begin();
  setupLeds();
  setupGPS();
  setupPlantower();
  setupDHT11();

  if (drd.detectDoubleReset()) {
    Serial.println("Connecting to network ...");
    setupWifi();
    syncLog();
  }

}

void loop() {
  gps = getGPSData();
  if(gps.ready) {
    dht11 = getDHT11Data();
    plantower = getPlantowerData();

    if(plantower.ready) {
      save();
      ledParticulateQuality(plantower);

      String frame = influxFrame();
      post2Influx("http://aqa.unloquer.org:8086/write?db=aqa", frame);
    }
  }

  drd.loop();
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
   "id","lat","lng","date","time","altitude","course","speed","humidity",
   "temperature","pm1","pm25","pm10"
  */

  // First datum is the sensor_id
  String frame = SENSOR_ID + STR_COMMA;

  // Add GPS data
  char strlat[25],strlng[25];
  dtostrf(gps.lat, 3, 6, strlat);
  dtostrf(gps.lng, 3, 6, strlng);

  frame += strlat + STR_COMMA;
  frame += strlng + STR_COMMA;
  frame += gps.date + STR_COMMA;
  frame += gps.time + STR_COMMA;
  frame += gps.altitude + STR_COMMA;
  frame += gps.course + STR_COMMA;
  frame += gps.speed + STR_COMMA;

  // Add DHT11 data
  if(dht11.ready) {
    frame += dht11.humidity + STR_COMMA;
    frame += dht11.temperature + STR_COMMA;
  } else {
    frame += STR_NULL + STR_COMMA + STR_NULL + STR_COMMA;
  }

  // Add Plantower data
  if(plantower.ready) {
    frame += plantower.pm1 + STR_COMMA;
    frame += plantower.pm25 + STR_COMMA;
    frame += plantower.pm10;
  } else {
    frame += STR_NULL + STR_COMMA + STR_NULL + STR_COMMA + STR_NULL;
  }

  return frame;
}

String influxFrame() {
  /* CSV is ordered as:
   "id","lat","lng","date","time","altitude","course","speed","humidity",
   "temperature","pm1","pm25","pm10"
  */

  // First datum is the sensor_id
  String frame = SENSOR_ID + STR_COMMA + "id=" + SENSOR_ID +  STR_SPACE;

  // Add GPS data
  char strlat[25],strlng[25];
  dtostrf(gps.lat, 3, 6, strlat);
  dtostrf(gps.lng, 3, 6, strlng);
  frame += "lat=";
  frame += strlat + STR_COMMA;
  frame += "lng=";
  frame += strlng + STR_COMMA;
  //frame += gps.date + STR_COMMA;
  //frame += gps.time + STR_COMMA;
  frame += "altitude=";
  frame += gps.altitude + STR_COMMA;
  frame += "course=";
  frame += gps.course + STR_COMMA;
  frame += "speed=";
  frame += gps.speed + STR_COMMA;

  //Add DHT11 data
  //if
    frame += "humidity=";
    frame += dht11.humidity + STR_COMMA;
    frame += "temperature=";
    frame += dht11.temperature + STR_COMMA;
  // } else {
  //   frame += "humidity=" + STR_NULL + STR_COMMA + "temperature=" + STR_NULL + STR_COMMA;
  // }

  // Add Plantower data
    // if
    frame += "pm1=";
    frame += plantower.pm1 + STR_COMMA;
    frame += "pm25=";
    frame += plantower.pm25 + STR_COMMA;
    frame += "pm10=";
    frame += plantower.pm10;
  // } else {
  //   frame += "pm1=" + STR_NULL + STR_COMMA + "pm25=" + STR_NULL + STR_COMMA + "pm10=" + STR_NULL;
  // }

  return frame;
}
