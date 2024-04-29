//----------------------------------------------------------------------------------------------
// bandvoltage stuff
//----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
// read from ADC connected to HL2 band voltage (via 4.7k from bandvoltage PIN and 10uF against GND at the HL2)
// put an 100nF capacitor near ESP32 ADC input against GND as RF protection 
int Read_BV(int PIN) {
  int mV = 0;
  int Xiegu_mV = 0;

  for(int8_t i=1; i<=16; i++) {            // we measure 16x the bandvoltage from ADC input
    mV += analogReadMilliVolts(PIN);       // this function analogReadMilliVolts() works only with ESP32-S3
  }
  mV /= 16;                                // and form an average value

  #ifdef DEBUG_ADC
  Serial.print("ADC HL2 real_Voltage: "); Serial.print(mV);Serial.print(" mV at PIN "); Serial.println(PIN);
  #endif
  
  if (mV > 0) {
    /*
    The build-in ADC of the ESP32 are not the best, that's a known issue. But good enough for the bandvoltage measurement.
    We get the real voltage from the HL2 at it's bandvoltage output, BUT calculate this new,
    so that the results are compatible with the bandvoltages used by the XIEGU XPA125B PA.
    That's why I wrote the bandvoltage routines original for the XIEGU XPA125B and not for the Neptune.
    I don't want change this. The calculated values differ from the real voltages at the HL2 output you measure there.
    If you want use the real values from the HL2 you need to change the calc_Band() function as you need and comment out the calc_mV formula. 
    */
    Xiegu_mV = (-0.00002*pow(mV, 2))+(1.375*mV)+18; // convert the bandvoltage from actually measured values of HL2 to XIEGU protocol (with 470k against GND at the ADC input)
    #ifdef DEBUG_ADC
    Serial.print("ADC HL2 re-calculate voltage with XIEGU spec: "); Serial.print(Xiegu_mV); Serial.println(" mV");
    #endif
    // return mV;      // return real value if not using recalculating bandvoltage
    return Xiegu_mV;    // return value with recalculating with the XIEGU specification
  } else {
    // return mV;      // return real value if not using recalculating bandvoltage
    return Xiegu_mV;    // return value with recalculating with the XIEGU specification
  }
}
//-----------------------------------------------------------------------------------------