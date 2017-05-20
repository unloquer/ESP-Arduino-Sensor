#include "app.h"

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
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
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
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
}

int postCsvFile(String url, String filename) {
  http.begin(url);
  http.addHeader("Content-Type", "application/csv");

  int content_length = 0;

  File file = SPIFFS.open(filename, "a+");
  content_length = file.size();

  http.addHeader("Content-Length", String(content_length));
  int httpCode = http.POST((String) file);
  if(httpCode > 0) {
    String payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.print("[HTTP] failed, error: ");Serial.println(http.errorToString(httpCode).c_str());
  }
  http.end();

  return httpCode;
}
