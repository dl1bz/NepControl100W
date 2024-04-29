//----------------------------------------------------------------------------------------------
// WiFi Stuff
//----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
#define NUM_SWERTE 16   /* Number of S-Werte */

// lower limits
constexpr short int lowlimits[NUM_SWERTE] = {
  -200,-141,-135,-129,-123,-117,-111,-105,-99,-93,-83,-73,-63,-53,-43,-33
};
// upper limits
constexpr short int uplimits[NUM_SWERTE] = {
  -142,-134,-130,-124,-118,-112,-106,-100,-94,-84,-74,-64,-54,-44,-34,100
};

const String (dbm2smeter[NUM_SWERTE + 1]) = {
    // 160     80   60    40     30      20     17      15     12     10      6     NDEF
    "no signal","S1","S2","S3","S4","S5","S6","S7","S8","S9","S9+10db","S9+20db","S9+30db","S9+40db","S9+50db","S9+60db","out of range"
};

byte get_SWert(short int dbm) {
    byte i;
    for (i = 0; i < NUM_SWERTE; i++) {
        if ((dbm >= lowlimits[i]) && (dbm <= uplimits[i])) {
           return i;
        }
    }
    return NUM_SWERTE; // no valid S-Werte -> return not defined
}
//-----------------------------------------------------------------------------------------

void init_WLAN() {
  WiFi.mode(WIFI_STA);           // Mode Station, not AP
  WiFi.disconnect();   
    
  // only if we won't use DHCP
  #ifdef STATICIP
  IPAddress local_IP(192, 168, 253, 16);    // Set your Static IP address
  IPAddress gateway(192, 168, 253, 1);      // Set your Gateway IP address
  IPAddress subnet(255, 255, 255, 0);       // Set your Subnet Mask
  IPAddress primaryDNS(8, 8, 8, 8);   // Set DNS primary (optional)
  IPAddress secondaryDNS(8, 8, 4, 4);       // Set DNS secondary (optional)
  
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
      Serial.println("Static IP Configuration failed or not set.");
    } else {
      Serial.println("Static IP Configuration is ready.");
    }
  #endif

  WiFi.begin(ssid, password);      // Connect WiFi with your crendentials
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }

  WiFi.setHostname(hostname);    // set the hostname if needed
  WiFi.setAutoReconnect(true);   // auto-reconnect WiFi if connection is interrupted
  WiFi.persistent(true); 
  // info output IP and hostname
  Serial.println();
  Serial.print("Hostname: "); Serial.println(WiFi.getHostname());
  Serial.print("IP      : "); Serial.println(WiFi.localIP());
  Serial.print("SSID:   : "); Serial.println(WiFi.SSID());
  Serial.print("RSSI    : "); Serial.print(WiFi.RSSI());Serial.println(" dbm");
  Serial.print("S-Wert  : "); Serial.println(dbm2smeter[get_SWert(WiFi.RSSI())]);
}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.print("Time    : "); Serial.println(&timeinfo, "%d.%m.%Y %H:%M:%S");
}

//generate timestamp from localtime
String timestamp() {
  const char* localTime;
  struct tm timeinfo;
   if(!getLocalTime(&timeinfo)){
     localTime = "NTP failed !";
     return localTime;
   }
   char timeStringBuff[25];
   strftime(timeStringBuff, sizeof(timeStringBuff), "%d.%m.%Y %H:%M:%S", &timeinfo);
   return timeStringBuff;
}

