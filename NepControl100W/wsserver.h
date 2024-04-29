//----------------------------------------------------------------------------------------------
// webserver and websockets
//----------------------------------------------------------------------------------------------

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket object
AsyncWebSocket ws("/ws");

// Json Variable to Hold Sensor Readings
JSONVar readings;

//-----------------------------------------------------------------------------------------
// Get Sensor Readings and return JSON object
String getSensorReadings(){
  readings["ptt"] = bool(!digitalRead(pttPIN) || !digitalRead(ptt2PIN));
  readings["bias"] = bool(digitalRead(biasPIN));
  readings["rxtxrelais"] = bool(digitalRead(txPIN));
  readings["bv"] = String(Read_BV(bvHLPIN));
  readings["band"] = String(G_bandcode);
  readings["ssid"] = String(WiFi.SSID());
  readings["rssi"] = String(WiFi.RSSI());
  readings["rst"] = dbm2smeter[get_SWert(WiFi.RSSI())];
  readings["time"] = String(timestamp());
  String jsonString = JSON.stringify(readings); // convert answer to JSON format
  return jsonString;
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
void notifyClients(String sensorReadings) {
  ws.textAll(sensorReadings);
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    // String message = (char*)data;
    Serial.print("Websocket message received: "); Serial.println((char*)data);
    // Check if the websocket message is "getReadings"
    if (strcmp((char*)data, "getReadings") == 0) {
      String sensorReadings = getSensorReadings();
      #ifdef DEBUG_STATE
      Serial.print(sensorReadings);
      #endif
      notifyClients(sensorReadings);
    }
  }
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}
//-----------------------------------------------------------------------------------------