#include <ArduinoJson.h>
#include "http.h"

typedef struct {
  int object;
  int array;
} JsonSize;



void _getApi(String url, JsonObject &json, JsonSize& jsonSize) {
  String text = _get(url);

  const int i = jsonSize.object;
  const int BUFFER_SIZE = JSON_ARRAY_SIZE(i);
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
}

void _postApi(String url, JsonObject &data, JsonObject &res, int bufferSize) {
}
