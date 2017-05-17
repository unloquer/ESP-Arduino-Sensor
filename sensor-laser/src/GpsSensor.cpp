#include "app.h"

const int READ_TIME = 5000;

// GPSSensor::GPSSensor(const int rx, const int tx): serial(rx, tx) {
// }

GPSSensor::GPSSensor() {
// GPSSensor::GPSSensor(SoftwareSerial *ss) {
  // _serial = ss;
  Serial.print("GONO");
  Serial.println("RREA");

  Serial.print("READ_TIME:");
  Serial.println(READ_TIME);
}

void GPSSensor::begin() {
  static const uint32_t GPSBaud = 9600;
  SoftwareSerial ss(13, 7);
  ss.begin(GPSBaud);
  _serial = &ss;
  // _serial->begin(GPSBaud);
}

// void GPSSensor::_begin() {
//   static const uint32_t GPSBaud = 9600;
//   _serial->begin(GPSBaud);
// }

void GPSSensor::read() {
  String timeStr = "00:00:00.00";
  String dateStr = "1970-01-01";
  TinyGPSPlus gps;
  unsigned long start = millis();

  Serial.println("Reading GPS ...");
  // serial.available();
  // while(true) {
  //   Serial.print(".");
  // }

  // Try to read GPS sensor data
  do {
    while (_serial->available()) {
      Serial.println("... serial available ...");
      delay(500);
      // gps.encode(serial.read());
      _serial->read();
    }
  } while (millis() - start < READ_TIME);

  if(gps.location.isValid()) {
    data.lat = gps.location.lat();
    data.lng = gps.location.lng();
  }

  if(gps.altitude.isValid()) {
    data.altitude = gps.altitude.meters();
  }

  if(gps.course.isValid()) {
    data.course = gps.course.deg();
  }

  if(gps.speed.isValid()) {
    data.speed = gps.speed.kmph();
  }

  if (gps.time.isValid()) {
    timeStr = "";

    if (gps.time.hour() < 10) timeStr += "0";
    timeStr += gps.time.hour();
    timeStr += ":";

    if (gps.time.minute() < 10) timeStr += "0";
    timeStr += gps.time.minute();
    timeStr += ":";

    if (gps.time.second() < 10) timeStr += "0";
    timeStr += gps.time.second();
    timeStr += ".";

    if (gps.time.centisecond() < 10) timeStr += "0";
    timeStr += gps.time.centisecond();

    data.time = timeStr;
  }

  if (gps.date.isValid()) {
    dateStr = "";
    dateStr += gps.date.year();
    dateStr += "-";
    dateStr += gps.date.month();
    dateStr += "-";
    dateStr += gps.date.day();

    data.date = dateStr;
  }

  // Reenable WatchDog
  // wdt_enable(1000);
}
