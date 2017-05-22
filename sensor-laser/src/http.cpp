#include "app.h"

const uint16_t HTTP_TIMEOUT = 1000 * 60;
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

int postCsv(String url, String csv) {
  if (WiFi.status() != WL_CONNECTED) { return 0; }

  http.begin(url);
  http.setTimeout(HTTP_TIMEOUT);
  http.addHeader("Content-Type", "text/csv");
  http.addHeader("Content-Length", String(csv.length()));

  int httpCode = http.POST(csv);
  if(httpCode > 0) {
    String payload = http.getString();
    Serial.println(payload);
    Serial.println("CSV file sent successfully");
  } else {
    Serial.print("[HTTP] failed, error: ");
    Serial.println(http.errorToString(httpCode).c_str());
  }

  http.end();

  return httpCode;
}

int postCsvFile(String url, String filename) {
  if (WiFi.status() != WL_CONNECTED) { return 0; }

  int content_length = 0;

  File file = SPIFFS.open(filename, "r");
  content_length = file.size();
  Serial.print("Size:");
  Serial.println(content_length);

  String data = "";
  char *_data; 
  Serial.println("Reading file ...");
  while(file.available()) {
    Serial.print(".");
    data += String((char)file.read());
  }
  data.toCharArray(_data, content_length);

  //uint8_t data[content_length];
  //int i = 0;
  //while(file.available()) {
  //*(data + i++) = file.read();
    //data[i++] = file.read();
    //Serial.print(i);
  //}

  //*(data + i) = '\0';

  //Serial.print("Count size:");
  //Serial.println(i);

  http.begin(url);
  http.setTimeout(HTTP_TIMEOUT);
  http.addHeader("Content-Type", "text/csv");
  http.addHeader("Content-Length", String(content_length));

  Serial.println("\nSending file to "+url);
  int httpCode = http.POST((uint8_t *)_data, strlen(_data));
  //int httpCode = http.POST(data);
  if(httpCode > 0) {
    String payload = http.getString();
    Serial.println(payload);
    Serial.println("CSV file sent successfully");
  } else {
    Serial.print("[HTTP] failed, error: ");
    Serial.println(http.errorToString(httpCode).c_str());
  }

  http.end();

  return httpCode;
}
