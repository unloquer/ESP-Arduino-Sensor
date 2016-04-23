#include "util.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

HTTPClient http;

// Facade to HTTP GET request
String _get(String url) {
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(url);
    int httpCode = http.GET();

    // httpCode will be negative on error
    if(httpCode > 0) {
      // resource found at server
      if(httpCode == HTTP_CODE_OK) {
        return http.getString();
      }
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
}

String _post(String url, String json) {
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(url);

    int httpCode = http.POST(json);

    if(httpCode > 0) {
      // resource found at server
      if(httpCode == HTTP_CODE_OK) {
        return http.getString();
      }
    } else {
      USE_SERIAL.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
}

void _parseJSON(JsonObject &json, int bufferSize) {
  // const int _bufferSize = const_cast<const int>(bufferSize);
  // StaticJsonBuffer<_bufferSize> jsonBuffer;
  // jsonBuffer.parseObject(json);
}

void _jsonToString(JsonObject &json, char *buffer) {
  // json.printTo(&buffer, sizeof(buffer));
}
