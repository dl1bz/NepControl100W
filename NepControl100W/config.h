//----------------------------------------------------------------------------------------------
//    global Config file
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
#define VERSION "Neptune PA Control replacement V2.0 by DL1BZ"
#define DEBUG_STATE
#define DEBUG_ADC
// #define STATICIP               // if using Static IP, not DHCP


//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// WAVESHARE ESP32-S3 Zero GPIO PIN definition
const int ptt2PIN = 1;     // PTT2 input line from Sub-D Pin 4 as additional, second PTT line
const int bvHLPIN = 2;     // bandvoltage input from HL2 Sub-D Pin 1
const int pttPIN = 6;      // PTT input line for reading PTT at RCA socket
const int pwrPIN = 5;      // set the Power ON LED
const int txPIN = 4;       // set the RX/TX relais which is connected with the TX LED too
const int biasPIN = 3;     // set the base of the BC547C transisitor for bias on/off, H means Bias ON
// GPIO PIN 7-13 are the LPF relais, H means relais is ON
const int PIN160 = 7;      // set the LPF 160m relais
const int PIN80 = 8;       // set the LPF 80m relais
const int PIN6040 = 9;     // set the LPF 60m+40m relais
const int PIN3020 = 10;    // set the LPF 30m+40m relais
const int PIN1715 = 11;    // set the LPF 17m+15m relais
const int PIN1210 = 12;    // set the LPF 12m+10m relais
const int PIN6 = 13;       // set the LPF 6m relais
const int ledPIN = 21;     // onboard LED at GPIO PIN 21 (fix)
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// global vars (start with G_)
boolean G_LEDState;
const int G_PTTDelay = 20; // in ms, delay for BIAS on
int G_bandcode;
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Timer variables
unsigned long lastTime = 0;
unsigned long pLastTime = 0;
unsigned long timerDelay = 500; // in ms, 1000 = 1s
//----------------------------------------------------------------------------------------------

#ifdef DASHBOARD
//----------------------------------------------------------------------------------------------
// WiFi Setup
//----------------------------------------------------------------------------------------------
// Replace with your network credentials
// const char* ssid = "YOUR_SSID";
// const char* password = "YOUR_WIFI_PASSWORD";

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

const char* hostname = "NEPTUNE-PA";             // define hostname for network
const char* ntpServer = "192.53.103.108";        // define NTP server you will use
//----------------------------------------------------------------------------------------------
#endif