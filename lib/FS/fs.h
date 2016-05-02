#include "FS.h"

bool setupFS() {
  if(SPIFFS.begin()) {
    if(SPIFFS.exists("/config/wifi")) {
      SPIFFS.open("/config/wifi");
    }
  }

  return true;
}

String readFile(String path) {
  String c;
  if(SPIFFS.exists(path)) {
    File f = SPIFFS.open(path, "r");
    c = f.readStringUntil();
  }
  return c;
}

void writeFile(String path, String contents) {
  File f = SPIFFS.open(path, "r");
  f.print(contents);
}

void writeJSON(String path, JsonObject& json) {

}
