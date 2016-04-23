#include <ArduinoJson.h>
#include "http.h"

void _getApi(String url, JsonObject &res, int bufferSize) {
  String text = _get(url);
  
}

void _postApi(String url, JsonObject &data, JsonObject &res, int bufferSize) {
}
