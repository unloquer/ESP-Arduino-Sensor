#include <Arduino.h>
#include <ArduinoJson.h>
#include "../FS/fs.h"

typedef struct {
  String ssid;
  String password;
  int ip[4];
} WifiConfig;

typedef struct {
  String type;
  String boardID;
  int mac[6];
} BoardConfig;

WifiConfig& getWifiConfig() {
  String config = readFile("/config/wifi");
  const int BUFFER_SIZE = JSON_OBJECT_SIZE(1);
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  JsonObject& jsonConfig = jsonBuffer.parseObject(config);
  WifiConfig wifiConfig = { jsonConfig["ssid"], jsonConfig["password"], jsonConfig["ip"] };
  return wifiConfig;
}

void setWifiConfig(WifiConfig config) {
  const int BUFFER_SIZE = JSON_OBJECT_SIZE(1);
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  JsonObject& jsonConfig = jsonBuffer.parseObject();

  // writeFile("config/wifi");
}

BoardConfig& getBoardConfig() {

}
