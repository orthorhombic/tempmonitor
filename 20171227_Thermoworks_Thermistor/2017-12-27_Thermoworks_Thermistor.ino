
/* Steinhart-Hart coefficients from:
 * https://tvwbb.com/showthread.php?69233-Thermoworks-TX-1001X-OP-amp-TX-1003X-AP-Probe-Steinhart-Hart-Coefficients
 * 
 * Coefficient TX-1001X-OP / TX-1003X-AP
 * Steinhart A 0.0007343140544
 * Steinhart B 0.0002157437229
 * Steinhart C 0.0000000951568577
 * 
 * 
 * https://arduinodiy.wordpress.com/2015/11/10/measuring-temperature-with-ntc-the-steinhart-hart-formula/
 * https://edwardmallon.wordpress.com/2016/06/09/better-thermistor-readings-with-an-arduino-series-resistors-aref/
 * https://github.com/OSBSS/Thermistor_v2/blob/master/Thermistor_v2.ino
 * 
 * http://playground.arduino.cc/ComponentLib/Thermistor2
 * 
 * 
 * 
 */





/*
 * Inputs ADC Value from Thermistor and outputs Temperature in Celsius
 *  requires: include <math.h>
 * Utilizes the Steinhart-Hart Thermistor Equation:
 *    Temperature in Kelvin = 1 / {A + B[ln(R)] + C[ln(R)]3}
 *    where A = 0.001129148, B = 0.000234125 and C = 8.76741E-08
 *
 * These coefficients seem to work fairly universally, which is a bit of a 
 * surprise. 
 *
 * Schematic:
 *   [Ground] -- [10k-pad-resistor] -- | -- [thermistor] --[Vcc (5 or 3.3v)]
 *                                               |
 *                                          Analog Pin 0
 *
 * In case it isn't obvious (as it wasn't to me until I thought about it), the analog ports
 * measure the voltage between 0v -> Vcc which for an Arduino is a nominal 5v, but for (say) 
 * a JeeNode, is a nominal 3.3v.
 *
 * The resistance calculation uses the ratio of the two resistors, so the voltage
 * specified above is really only required for the debugging that is commented out below
 *
 * Resistance = PadResistor * (1024/ADC -1)  
 *
 * I have used this successfully with some CH Pipe Sensors (http://www.atcsemitec.co.uk/pdfdocs/ch.pdf)
 * which be obtained from http://www.rapidonline.co.uk.
 *
 */

#include <math.h>

#define ThermistorPIN 0                 // Analog Pin 0

float vcc = 3.3;                       // only used for display purposes, if used
                                        // set to the measured Vcc.
float pad = 14950;                       // balance/pad resistor value, set this to
                                        // the measured resistance of your pad resistor
float thermr = 100000;                   // thermistor nominal resistance

float Thermistor(float RawADC) {
  long Resistance;  
  float Temp;  // Dual-Purpose variable to save space.

  Resistance=pad*((1023.0 / RawADC) - 1); //divide if divider is reversed
  Temp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
  Temp = 1 / (0.0007343140544 + (0.0002157437229 * Temp) + (0.0000000951568577 * Temp * Temp * Temp));
  Temp = Temp - 273.15;  // Convert Kelvin to Celsius                      

  // BEGIN- Remove these lines for the function not to display anything
  //Serial.print("ADC: "); 
  //Serial.print(RawADC); 
  //Serial.print("/1024");                           // Print out RAW ADC Number
  //Serial.print(", vcc: ");
  //Serial.print(vcc,2);
  //Serial.print(", pad: ");
  //Serial.print(pad/1000,3);
  //Serial.print(" Kohms, Volts: "); 
  //Serial.print(((RawADC*vcc)/1024.0),3);   
  //Serial.print(", Resistance: "); 
  //Serial.print(Resistance);
  //Serial.print(" ohms, ");
  // END- Remove these lines for the function not to display anything

  // Uncomment this line for the function to return Fahrenheit instead.
  Temp = (Temp * 9.0)/ 5.0 + 32.0;                  // Convert to Fahrenheit
  return Temp;                                      // Return the Temperature
}

void setup() {
  Serial.begin(115200);
// connect AREF to 3.3V and use that as VCC for the resistor and NTC!
analogReference(EXTERNAL);
  
}

float sample(byte z)
/* This function will read the Pin 'z' 128 times and take an average.
 *  Need to incopporate at some point:
 *  Gain = ( ideal VrefH – ideal VrefL ) / ( ADC Measured VrefH – ADC Measured VrefL )
 *  Offset = ideal VrefL  – ( Gain * ADC Measured VrefL )
 *  Corrected ADC reading = (Gain * raw ADC reading) + Offset
 *  
 */
{
  byte i;
  float sval = 0;
  for (i = 0; i < 128; i++)
  {
  sval = sval + analogRead(z);// sensor on analog pin 'z'
  //Serial.println(analogRead(z));
  delay(1);
  }
  sval = sval / 128.0;    // average
  return sval;
}

void loop() {
  float temp;

  //temp=Thermistor(analogRead(ThermistorPIN));       // read ADC and  convert it to Celsius
  temp=Thermistor(sample(ThermistorPIN));       // read ADC and  convert it to Celsius
  Serial.print("Celsius: "); 
  Serial.print(temp,1);                             // display Celsius
  //temp = (temp * 9.0)/ 5.0 + 32.0;                  // converts to  Fahrenheit
  //Serial.print(", Fahrenheit: "); 
  //Serial.print(temp,1);                             // display  Fahrenheit
  Serial.println("");                                   
  delay(1000);                                      // Delay a bit... 
}
