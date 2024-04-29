/*
  This project is developed with Arduino IDE 2.x and the Expressif ESP32 extensions/libraries
  I use a WAVESHARE ESP32-S3 Zero as SoC for replace the control with onboard STM32,
  which has a perfect small form factor for put it all inside the Neptune 100W PA

  CONDITIONS you need to agree if want using my modification:
  +---------------------------------------------------------------------------------+
  | No warranty, no guarantee that everything works correctly!                      |
  | All what you do is at your own risk !                                           |
  | For amateur radio use only !                                                    |
  | No software support from me, you can use or change the code for free "as it is" |
  +---------------------------------------------------------------------------------+

  Please add a extra 7805 +5V power source for the ESP32, because the current for the ESP32 can be rising up to 500mA !

  DO NOT START THE AMP WITH THE ORIGINAL [ON] KNOB ! THE ONBOARD STM32 NEEDS TO BE OFF !
  THE ESP32 STARTS IF +5V VCC ARE CONNECTED AND CONTROL THE PA COMPLETLY !
  YOU LOST THE AUTOMATIC BAND DETECTION - IT'S REPLACED THROUGH THE HL2 BANDVOLTAGE OUTPUT ! 

  This sketch is made and checked only with a WAVESHARE ESP32-S3 Zero and in combination with my schematic.
  Please read the instructions at the WAVESHARE website, how the ESP32-S3 Zero needs to be programmed.

  ONLY IF YOU WANT THE DASHBOARD USE:
  Additional libraries are needed, otherwise the compiling will be interrupted and cannot completed:
  - AsyncTCP
  - ESPAsyncWebServer
  - Arduino_JSON

  You can add this libraries with the libraries managment inside the Arduino IDE.

  All important things are defined in the config.h
  
*/

// define only if want using the dashboard and WiFi => don't forget to define WiFi credentials in config.h !!!
// #define DASHBOARD

// system libraries installed from the Arduino IDE
#ifdef DASHBOARD
#include <WiFi.h>              // activate WiFi with ESP32
#include "time.h"              // time stuff
#include <AsyncTCP.h>          // requirement for the webserver
#include <ESPAsyncWebServer.h> // webserver and websockets
#include <Arduino_JSON.h>      // add JSON data handling
#endif

// read global project config
#include "config.h"

// other includes
#include "onboard_LED.h"        // control the onboard LED

#ifdef DASHBOARD
#include "wlan.h"               // own WiFi stuff
#include "webpage.h"            // webpage used by webserver as startpage
#endif

#include "bv.h"                 // own bandvoltage stuff

#ifdef DASHBOARD
#include "wsserver.h"           // own webserver and websocket stuff
#endif

//-----------------------------------------------------------------------------------------
// init all UARTs
void init_UARTs() {
  Serial.end();                                 // 1 = means full terminate
  delay(100);
  Serial.begin(115200);                              // initialize internal UART0 with 115200/8N1
  Serial.println("Serial0 TXD is on pin: "+String(TX));
  Serial.println("Serial0 RXD is on pin: "+String(RX));
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// init ESP32 PINs for digital in/out, the PINs are defined further up
void init_GPIO() {
  pinMode(pttPIN, INPUT_PULLUP);    // PIN input from the PTT line at RCA Socket, LOW ACTIVE with internal PullUp activated for reliable detection
  pinMode(ptt2PIN, INPUT_PULLUP);   // PIN input from the Sub-D socket, LOW ACTIVE with internal PullUp activated for reliable detection
  // pinMode(pttPIN, INPUT);        // PIN input from the PTT line, LOW ACTIVE without using internal PullUp
  pinMode(biasPIN, OUTPUT);         // PIN output BIAS
  pinMode(pwrPIN, OUTPUT);          // PIN output Power ON
  pinMode(txPIN, OUTPUT);           // PIN output for RX/TX relais
  pinMode(PIN160, OUTPUT);          // PIN output for RX/TX relais
  pinMode(PIN80, OUTPUT);           // PIN output for RX/TX relais
  pinMode(PIN6040, OUTPUT);         // PIN output for RX/TX relais
  pinMode(PIN3020, OUTPUT);         // PIN output for RX/TX relais
  pinMode(PIN1715, OUTPUT);         // PIN output for RX/TX relais
  pinMode(PIN1210, OUTPUT);         // PIN output for RX/TX relais
  pinMode(PIN6, OUTPUT);            // PIN output for RX/TX relais
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// init the ESP32 ADC input for measure the bandvoltage come from HL2
void init_ADCs() {
  adcAttachPin(bvHLPIN);
  analogReadResolution(12);                  // 12bit = 0 – 4095, 11bit = 0 - 2047
  // ESP32 ADC ranges depends of selected attenuation, default is 11db attenuation for maximum voltage range
  //   0db 100mV ~  950mV
  // 2.5db 100mV ~ 1250mV
  //   6db 150mV ~ 1750mV
  //  11db 150mV ~ 3100mV
  analogSetPinAttenuation(bvHLPIN, ADC_11db); // select ATT 11db (default)
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// clear the LPF state complete, all relais OFF
void clear_LPF() {
  // set all LPF GPIO 7-13 off/low
  for (int i = PIN160; i <= PIN6; i++) {
    digitalWrite(i,0);
  }
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// test the LPF complete, all LPF relais 1x ON => wait 500ms => and OFF
void test_LPF() {
  // set all LPF GPIO 7-13 off/low
  for (int i = PIN160; i <= PIN6; i++) {
    digitalWrite(i,1);
    delay(500);
    digitalWrite(i,0);
  }
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// clear TX state independ from PTT line, RX/TX relais to RX/bypass and MOSFET BIAS off
void clear_TX() {
  digitalWrite(biasPIN,0);
  digitalWrite(txPIN,0);
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// set the POWER ON LED on/off
void pwr_LED(bool state) {
  if (state) {
    digitalWrite(pwrPIN,state);
  } else {
    digitalWrite(pwrPIN,state);
  }
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// get band as numeric from the input bandvoltage of HL2
int calc_Band(int mV) {
  int band;

  if (mV <= 345) { band = 160; };
  if (mV > 345) { band = 80; };
  if (mV > 575) { band = 60; };
  if (mV > 805) { band = 40; };
  if (mV > 1035) { band = 30; };
  if (mV > 1265) { band = 20; };
  if (mV > 1495) { band = 17; };
  if (mV > 1725) { band = 15; };
  if (mV > 1955) { band = 12; };
  if (mV > 2185) { band = 10; };
  if (mV > 2415) { band = 6; };

  #ifdef DEBUG_ADC
  Serial.print("Band selected: ");Serial.print(band); Serial.println("m");
  #endif

  return band;
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// set LPF to the selected band
void set_LPF(int selBand) {
  // first set all LPF GPIO 7-13 off/low so we have a clear LPF start state
  int i;
  for (int i = PIN160; i <= PIN6; i++) {
    digitalWrite(i,0);
  }

/*
some bands are combined and use the same LPF
Band: 160m  , 80m  , 60m+40m, 30m+20m, 17m+15m, 12m+10m, 6m
Var : PIN160, PIN80, PIN6040, PIN3020, PIN1715, PIN1210, PIN6
PIN : 7     , 8    , 9      , 10     , 11     , 12     , 13
*/
  switch (selBand) {
    case 160:
      digitalWrite(PIN160,1);   // set LPF 160m
    break;
    case 80:
      digitalWrite(PIN80,1);    // set LPF 80m
    break;
    case 60:
      digitalWrite(PIN6040,1);  // set LPF 60m
    break;
    case 40:
      digitalWrite(PIN6040,1);  // set LPF 40m
    break;
    case 30:
      digitalWrite(PIN3020,1);  // set LPF 30m
    break;
    case 20:
      digitalWrite(PIN3020,1);  // set LPF 20m
    break;
    case 17:
      digitalWrite(PIN1715,1);  // set LPF 17m
    break;
    case 15:
      digitalWrite(PIN1715,1);  // set LPF 15m
    break;
    case 12:
      digitalWrite(PIN1210,1);  // set LPF 12m
    break;
    case 10:
      digitalWrite(PIN1210,1);  // set LPF 10m
    break;
    case 6:
      digitalWrite(PIN6,1);     // set LPF 6m
    break;
    default:
      // emergency procedure if no valid band detected
      digitalWrite(biasPIN,0);                                    // bias off
      digitalWrite(txPIN,0);                                      // RX/TX relais to RX
      for (int i = PIN160; i <= PIN6; i++) { digitalWrite(i,0); } // set all LPF relais off
  }  
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// set Amp to TX
void amp_TX() {
    digitalWrite(txPIN,1);    // switch RX/TX relais to TX
    delay(G_PTTDelay);        // wait shortly before bias switch on
    digitalWrite(biasPIN,1);  // set bias switch on for power up the MOSFETs
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// set Amp to RX
void amp_RX() {
    digitalWrite(biasPIN,0);  // switch MOSFET bias OFF
    digitalWrite(txPIN,0);    // switch RX/TX relais to RX/bypass  
}
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
void setup() {
// run once at startup of the ESP32
setLED("off");          // onboard LED off
setLED("b");            // set onboard LED to blue

init_UARTs();           // init UARTs

Serial.println(VERSION);              // print Version
Serial.print("This application is using ESP-SDK: "); Serial.println(ESP.getSdkVersion());
#ifdef DASHBOARD
Serial.println("WiFi & Dashboard ENABLED");
#else
Serial.println("WiFi & Dashboard DISABLED");
#endif

init_GPIO();            // init GPIOs
init_ADCs();            // init ADCs

pwr_LED(0);             // set PowerLED off

#ifdef DASHBOARD
init_WLAN();                                   // init WiFi
configTime(0, 0, ntpServer);                   // get time from NTP server
setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1); // for your TZ look here https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
tzset();                                       // set correct timezone
printLocalTime();
#endif

clear_TX();             // clear state RX/TX relais and bias off
test_LPF();             // test 1x at POWER ON all LPF relais
clear_LPF();            // clear state of LPF
delay(200);             // wait 200ms
pwr_LED(1);             // set PowerLED on

#ifdef DASHBOARD
/*
     init webserver and websockets stuff
*/
initWebSocket();        // init websocket

// define webserver root
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/html", index_html);});

// Start webserver
server.begin();
#endif

}
//-----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------
void loop() {
  // main program, run repeatedly as loop
  bool PTTState;
  bool PTT2State;

  PTTState = !digitalRead(pttPIN);    // read PTT input line at GPIO input PIN and negotiate it, because PTT line of PA is Low active: RX=H TX=L
  PTT2State = !digitalRead(ptt2PIN);

  /*
  For understand the switch/control logic, first have a look in the schematic of the Neptune 100W PA,
  you can all get from here: https://www.60dbm.com/product/100-watt-hf-6m-power-amplifier-for-yaesu-ft-817-icom-ic-703-705-elecraft-kx3-automatic-band-switching/

  main function for control the PA depend from the incoming PTT line
  PTT, follow the input signal "ptt" at J4 RCA:
  Beware ! PTT line is low active: L = PTT ON, H = PTT OFF
  
  BIAS:
  The bias for the MOSFETs is controlled by U1 [LP2951-ADJ] with the PIN 3 SHUTDOWN with signal "bias on/off" 
  - we use a transisistor BC547C for switching bias with its collector-emitter line
  - the base from the BC547C is connected via 1k resistor to the GPIO PIN of the ESP32
  BIAS off = SHUTDOWN PIN at U1 is set to +3.3V
  BIAS on  = SHUTDOWN PIN at U1 need set to GND

  All relais include the LPF relais are switched by IC1 [TBD62783AFNG], because they are all 12V relais
  RX/TX relais, follow the signal "BFR":
  - set GPIO to H means relais is in TX state
  - set GPIO to L means relais is in RX/bypass state

  If RX/TX relais is set to RX/bypass the bias for the MOSFETs should be OFf too !
    No TX - no BIAS on !

  All together, Low means 0V/GND, High means +3.3V level:
  TX => PTT input line set Low / BIAS ON set U1 PIN3 Low / RX/TX relais ON = set GPIO to High
  RX => PTT input line set High / BIAS OFF set U1 PIN3 High / RX/TX relais OFF = set GPIO to Low
  */

  // MAIN PA CONTROL
  if(PTTState || PTT2State) {
    amp_TX(); // Amp ON
  } else {
    amp_RX(); // Amp OFF
  }

  // here we using an exact timer with millis(), time is defined with timerDelay
  if ((millis() - lastTime) > timerDelay) {

    // debug output if needed
    #ifdef DEBUG_STATE
    Serial.print("PTT_RCA: ");Serial.print(PTTState);
    Serial.print(" | PTT_2: ");Serial.print(PTT2State);
    Serial.print(" | BIAS: ");Serial.print(digitalRead(biasPIN));
    Serial.print(" | TX Relais: ");Serial.println(digitalRead(txPIN));
    #endif

    G_bandcode = calc_Band(Read_BV(bvHLPIN)); // read bandvoltage from the HL2
    
    // switch LPF only if RX at both PTT lines
    if (!PTTState && !PTT2State) { 
      set_LPF(G_bandcode);
    }

    #ifdef DASHBOARD 
     // sending to browser via websocket
    String sensorReadings = getSensorReadings();  
    notifyClients(sensorReadings);                // update webpage in realtime with websockets
    #endif

    // gimmick: blinking LEDs as heartbeat visualisation to show the running program is alive
    if (G_LEDState) {
      setLED("g");             // onboard LED green
      pwr_LED(G_LEDState);  
    } else {
      setLED("r");             // onboard LED red
      pwr_LED(G_LEDState);
    }
    G_LEDState = !G_LEDState;  // negotiate LED state for the next loop run, we want a blinking LED :-)

  lastTime = millis();
  }
    #ifdef DASHBOARD
    ws.cleanupClients();
    #endif
}
//-----------------------------------------------------------------------------------------