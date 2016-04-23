/*
 * CC 2016, Un/loquer - http://unloquer.org
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
// #include <ESP8266mDNS.h>

#include "FS.h"

#include "http.h"
#include "rest.h"

char ssid[] = "C3P";
char password[] = "trespatios";

ESP8266WebServer server ( 80 );

const int led = 13;
const String BASE_URL = "http://104.131.1.214:3000/api";

boolean signedIn = false;
byte MAC[6];

#include "templates.h"

// Declare local functions
boolean ackDevice();
String macToString();
void getRoot();
void handleNotFound();

void getRoot() {
  if(signedIn) {
    return;
  }
  ackDevice();
}

// String representation of the MAC, without the colons
String macToString() {
  String mac;
  for (int i = 0; i <= 5; i++) {
    mac += String(MAC[i], HEX);
  }
  return mac;
}

// Acknowledge this ESP by MAC Address
boolean ackDevice() {
  String mac = macToString();

  JsonObject* ackDeviceJson;
  // _getApi("/sensors?filter[where][mac][eq]="+mac, &ackDeviceJson);
  return false;
}

boolean existsSensor(String sensorID) {
  String mac = macToString();

  const int bufferSize = JSON_OBJECT_SIZE(1);
  StaticJsonBuffer<bufferSize> jsonBuffer;
  JsonObject& exists = jsonBuffer.createObject();
  // Response: { exists: true }
  String url("/sensors/"+sensorID+"/exists");
  _getApi(url, exists, 0);
  return exists["exists"];
}

boolean registerDevice() {
  const int bufferSize = JSON_OBJECT_SIZE(1);
  StaticJsonBuffer<bufferSize> resJsonBuffer;
  JsonObject& res = resJsonBuffer.createObject();

  // Build request body object
  StaticJsonBuffer<bufferSize> dataJsonBuffer;
  JsonObject& data = dataJsonBuffer.createObject();

  // Response: { exists: true }
  String url("/devices");
  _postApi("/devices", data, res, 0);
  if(data.containsKey("_id")) {

  }
}

void connectWifi() {
  WiFi.begin ( ssid, password );
}

void setup ( void ) {
  pinMode ( led, OUTPUT );
  digitalWrite ( led, 0 );
  Serial.begin ( 115200 );

  Serial.println ( "" );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  /*if ( mdns.begin ( "esp8266", WiFi.localIP() ) ) {
    Serial.println ( "MDNS responder started" );
  }*/

  server.on ( "/", handleRoot );
  server.on ( "/test.svg", drawGraph );
  server.on ( "/inline", []() {
    server.send ( 200, "text/plain", "this works as well" );
  });

  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );
}

void setupFilesystem() {
  // if(SPIFFS.begin()) {
  //   if(SPIFFS.exists("/config")) {
  //     SPIFFS.open("/config");
  //   }
  // }
  //
  // return
}

JsonObject* renderConfigJson() {
  const int bufferSize = JSON_OBJECT_SIZE(1);
  StaticJsonBuffer<bufferSize> resJsonBuffer;
  JsonObject &res = resJsonBuffer.createObject();
  JsonObject *out = &res;
  return out;
}

void loop ( void ) {
  // mdns.update();
  server.handleClient();
}

void handleNotFound() {
  digitalWrite ( led, 1 );
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  digitalWrite ( led, 0 );
}
