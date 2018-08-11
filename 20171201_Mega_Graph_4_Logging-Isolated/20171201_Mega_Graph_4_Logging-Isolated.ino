/*
from https://drive.google.com/file/d/0ByEQKtsOckWBNy1GVS1MMm0zSkk/view
https://www.youtube.com/watch?v=YejRbIKe6e0



This program provides cartesian type graph function

It requires and Arduino Mega (or UNO) and an Adafruit 3.5" TFT 320x480 + Touchscreen Breakout Board
https://learn.adafruit.com/adafruit-3-5-color-320x480-tft-touchscreen-breakout/overview

Adafruit libraries
https://github.com/adafruit/Adafruit_HX8357_Library/archive/master.zip
https://github.com/adafruit/Adafruit-GFX-Library/archive/master.zip

optional touch screen libraries
https://github.com/adafruit/Touch-Screen-Library/archive/master.zip

Revisions
rev     date        author      description
1       12-24-2015  kasprzak    initial creation


This pin setting will also operate the SD card

Pin settings

  Arduino   device
  5V        Vin
  GND       GND
  A0
  A1
  A2         Y+ (for touch screen use)
  A3         X- (for touch screen use)
  A4
  A5
  1
  2
  3
  4         CCS (42 for mega)
  5
  6
  7         Y- (44 for mega)
  8         X+ (46 for mega)
  9         DC (48 on mega * change define)
  10        CS (53 for mega * change define)
  11        MOSI (51 for mega)
  12        MISO  (50 for mega)
  13        CLK (SCK) (52 for mega)
  44        Y- (for touch screen only)
  46        X+ (for touch screen only)
  48        DC
  SDA
  SLC



Pin Info
http://arduino.cc/en/Hacking/PinMapping2560
http://forum.arduino.cc/index.php?topic=91079.0
http://www.dimdim.gr/2013/01/3-2-tft-pinout-connection-to-arduino-mega-or-due/


{ name = "0",  port = "E", bit = 0 },
{ name = "1",  port = "E", bit = 1 },
{ name = "2",  port = "E", bit = 4 },
{ name = "3",  port = "E", bit = 5 },
{ name = "4",  port = "G", bit = 5 },
{ name = "5",  port = "E", bit = 3 },
{ name = "6",  port = "H", bit = 3 },
{ name = "7",  port = "H", bit = 4 },
{ name = "8",  port = "H", bit = 5 },
{ name = "9",  port = "H", bit = 6 },

{ name = "10",  port = "B", bit = 4 },
{ name = "11",  port = "B", bit = 5 },
{ name = "12",  port = "B", bit = 6 },
{ name = "13",  port = "B", bit = 7 },
{ name = "14",  port = "J", bit = 1 },
{ name = "15",  port = "J", bit = 0 },
{ name = "16",  port = "H", bit = 1 },
{ name = "17",  port = "H", bit = 0 },
{ name = "18",  port = "D", bit = 3 },
{ name = "19",  port = "D", bit = 2 },

{ name = "20",  port = "D", bit = 1 },
{ name = "21",  port = "D", bit = 0 },
{ name = "22",  port = "A", bit = 0 },
{ name = "23",  port = "A", bit = 1 },
{ name = "24",  port = "A", bit = 2 },
{ name = "25",  port = "A", bit = 3 },
{ name = "26",  port = "A", bit = 4 },
{ name = "27",  port = "A", bit = 5 },
{ name = "28",  port = "A", bit = 6 },
{ name = "29",  port = "A", bit = 7 },

{ name = "30",  port = "C", bit = 7 },
{ name = "31",  port = "C", bit = 6 },
{ name = "32",  port = "C", bit = 5 },
{ name = "33",  port = "C", bit = 4 },
{ name = "34",  port = "C", bit = 3 },
{ name = "35",  port = "C", bit = 2 },
{ name = "36",  port = "C", bit = 1 },
{ name = "37",  port = "C", bit = 0 },
{ name = "38",  port = "D", bit = 7 },
{ name = "39",  port = "G", bit = 2 },

{ name = "40",  port = "G", bit = 1 },
{ name = "41",  port = "G", bit = 0 },
{ name = "42",  port = "L", bit = 7 },
{ name = "43",  port = "L", bit = 6 },
{ name = "44",  port = "L", bit = 5 },
{ name = "45",  port = "L", bit = 4 },
{ name = "46",  port = "L", bit = 3 },
{ name = "47",  port = "L", bit = 2 },
{ name = "48",  port = "L", bit = 1 },
{ name = "49",  port = "L", bit = 0 },

{ name = "50",  port = "B", bit = 3 },
{ name = "51",  port = "B", bit = 2 },
{ name = "52",  port = "B", bit = 1 },
{ name = "53",  port = "B", bit = 0 },

{ name = "A0",  port = "F", bit = 0 },
{ name = "A1",  port = "F", bit = 1 },
{ name = "A2",  port = "F", bit = 2 },
{ name = "A3",  port = "F", bit = 3 },
{ name = "A4",  port = "F", bit = 4 },
{ name = "A5",  port = "F", bit = 5 },
{ name = "A6",  port = "F", bit = 6 },
{ name = "A7",  port = "F", bit = 7 },
{ name = "A8",  port = "K", bit = 0 },
{ name = "A9",  port = "K", bit = 1 },

{ name = "A10",  port = "K", bit = 2 },
{ name = "A11",  port = "K", bit = 3 },
{ name = "A12",  port = "K", bit = 4 },
{ name = "A13",  port = "K", bit = 5 },
{ name = "A14",  port = "K", bit = 6 },
{ name = "A15",  port = "K", bit = 7 },



*/


#include <TFT_HX8357.h> // Hardware-specific library
TFT_HX8357 tft = TFT_HX8357();       // Invoke custom library
#define TFT_GREY 0x7BEF

//
//
// SD Card

#include <SPI.h>
#include <SD.h>
const int chipSelect = 53;


//RTC
/* for normal hardware wire use below */
#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);
/* for normal hardware wire use above */

#define countof(a) (sizeof(a) / sizeof(a[0]))
void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}



// the logging file
File logfile;

// how many milliseconds between grabbing data and logging it. 1000 ms is once a second
#define LOG_INTERVAL  5000 // mills between entries (reduce to take more/faster data)

// how many milliseconds before writing the logged data permanently to disk
// set it to the LOG_INTERVAL to write each time (safest)
// set it to 10*LOG_INTERVAL to write all data every 10 datareads, you could lose up to 
// the last 10 reads if power is lost but it uses less power and is much faster!
#define SYNC_INTERVAL 120000 // mills between calls to flush() - to write data to the card
uint32_t syncTime = 0; // time of last sync()

#define ECHO_TO_SERIAL   0 // echo data to serial port

void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  
  // LED indicates error
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(100);                       // wait for a second

  while(1);
}
// end sd card
//
//



//Graph initialization
// this is the only external variable used by the graph
// it's a flat to draw the coordinate system only on the first pass
boolean display7 = true;

//initialize graph variables (maximum 6 graphs)
boolean var[6] = {true,true,true,true,true,true};
//boolean Intern = true;

double legoff;
double ox[6] , oy[6], legpos[6] ;

//MAX31855
#include "Adafruit_MAX31855.h"
//int vccPin = 2;
//3v3 out
//int gndPin = 4;
#define MAXDO   7
#define MAXCS   6
#define MAXCLK  5

// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);




void setup() {

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  //initialize RTC

  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);
  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();

  if (!Rtc.IsDateTimeValid()) 
  {
      // Common Cuases:
      //    1) first time you ran and the device wasn't running yet
      //    2) the battery on the device is low or even missing

      Serial.println("RTC lost confidence in the DateTime!");

      // following line sets the RTC to the date & time this sketch was compiled
      // it will also reset the valid flag internally unless the Rtc device is
      // having an issue

      Rtc.SetDateTime(compiled);
  }

  if (!Rtc.GetIsRunning())
  {
      Serial.println("RTC was not actively running, starting now");
      Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) 
  {
      Serial.println("RTC is older than compile time!  (Updating DateTime)");
      Rtc.SetDateTime(compiled);
  }
  else if (now > compiled) 
  {
      Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled) 
  {
      Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }

  // never assume the Rtc was last configured by you, so
  // just clear them to your needed state
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); 


  //end RTC

  //initialize screen
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  //power up MAX31855
  //pinMode(vccPin, OUTPUT); digitalWrite(vccPin, HIGH);
  //pinMode(gndPin, OUTPUT); digitalWrite(gndPin, LOW);

  Serial.begin(38400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //initialize SD https://learn.adafruit.com/adafruit-data-logger-shield?view=all#using-the-real-time-clock-3
  // https://github.com/adafruit/Light-and-Temp-logger/blob/master/lighttemplogger.ino
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(chipSelect, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  // create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }
  
  if (! logfile) {
    error("couldnt create file");
  }

  
  Serial.print("Logging to: ");
  Serial.println(filename);

  //append headers
  logfile.println("millis,datetime,internalTemp,raw,corrected");    
  #if ECHO_TO_SERIAL
    Serial.println("millis,datetime,internalTemp,raw,corrected");
  #endif //ECHO_TO_SERIAL




  

  delay(1000); //sensor warm up
  





}


void loop(void) {

  
  double x, y, yint;
  //for (x = 0; x <= 7; x += 0.0023809524) {
  for (x = 0; x <= 35; x += 0.083333) {
    //y=thermocouple.readFarenheit();
    //y=thermocouple.readCelsius();
    //yint=thermocouple.readInternal();



    // delay for the amount of time we want between readings
    delay((LOG_INTERVAL -1) - (millis() % LOG_INTERVAL));

  
    // log milliseconds since starting
    uint32_t m = millis();
    //logfile.print(m);           // milliseconds since start, moved lower to logging block
    //logfile.print(", ");    

  
    // fetch the time
    //now = RTC.now();
    if (!Rtc.IsDateTimeValid()) 
    {
        // Common Cuases:
        //    1) the battery on the device is low or even missing and the power line was disconnected
        Serial.println("RTC lost confidence in the DateTime!");
    }

    RtcDateTime now = Rtc.GetDateTime();

    
      
    
  //from https://github.com/heypete/MAX31855-Linearization
  // Initialize variables.
   int i = 0; // Counter for arrays
   double internalTemp = thermocouple.readInternal(); // Read the internal temperature of the MAX31855.
   double internalTempF = (internalTemp * 9 / 5) + 32; // internal temperature of the MAX31855.
   double rawTemp = thermocouple.readCelsius(); // Read the temperature of the thermocouple. This temp is compensated for cold junction temperature.
   double rawTempF = thermocouple.readFarenheit(); // Read the temperature of the thermocouple. This temp is compensated for cold junction temperature.
   double thermocoupleVoltage= 0;
   double internalVoltage = 0;
   double correctedTemp = 0;
   double correctedTempF = 0;
   double totalVoltage = 0;
  
    // Check to make sure thermocouple is working correctly.
   if (isnan(rawTemp)) {
    Serial.println("Something wrong with thermocouple!");
    
   }
   else {
      // Steps 1 & 2. Subtract cold junction temperature from the raw thermocouple temperature.
      thermocoupleVoltage = (rawTemp - internalTemp)*0.041276;  // C * mv/C = mV
  
      // Step 3. Calculate the cold junction equivalent thermocouple voltage.
  
      if (internalTemp >= 0) { // For positive temperatures use appropriate NIST coefficients
         // Coefficients and equations available from http://srdata.nist.gov/its90/download/type_k.tab
  
         double c[] = {-0.176004136860E-01,  0.389212049750E-01,  0.185587700320E-04, -0.994575928740E-07,  0.318409457190E-09, -0.560728448890E-12,  0.560750590590E-15, -0.320207200030E-18,  0.971511471520E-22, -0.121047212750E-25};
  
         // Count the the number of coefficients. There are 10 coefficients for positive temperatures (plus three exponential coefficients),
         // but there are 11 coefficients for negative temperatures.
         int cLength = sizeof(c) / sizeof(c[0]);
  
         // Exponential coefficients. Only used for positive temperatures.
         double a0 =  0.118597600000E+00;
         double a1 = -0.118343200000E-03;
         double a2 =  0.126968600000E+03;
  
  
         // From NIST: E = sum(i=0 to n) c_i t^i + a0 exp(a1 (t - a2)^2), where E is the thermocouple voltage in mV and t is the temperature in degrees C.
         // In this case, E is the cold junction equivalent thermocouple voltage.
         // Alternative form: C0 + C1*internalTemp + C2*internalTemp^2 + C3*internalTemp^3 + ... + C10*internaltemp^10 + A0*e^(A1*(internalTemp - A2)^2)
         // This loop sums up the c_i t^i components.
         for (i = 0; i < cLength; i++) {
            internalVoltage += c[i] * pow(internalTemp, i);
         }
            // This section adds the a0 exp(a1 (t - a2)^2) components.
            internalVoltage += a0 * exp(a1 * pow((internalTemp - a2), 2));
      }
      else if (internalTemp < 0) { // for negative temperatures
         double c[] = {0.000000000000E+00,  0.394501280250E-01,  0.236223735980E-04, -0.328589067840E-06, -0.499048287770E-08, -0.675090591730E-10, -0.574103274280E-12, -0.310888728940E-14, -0.104516093650E-16, -0.198892668780E-19, -0.163226974860E-22};
         // Count the number of coefficients.
         int cLength = sizeof(c) / sizeof(c[0]);
  
         // Below 0 degrees Celsius, the NIST formula is simpler and has no exponential components: E = sum(i=0 to n) c_i t^i
         for (i = 0; i < cLength; i++) {
            internalVoltage += c[i] * pow(internalTemp, i) ;
         }
      }
  
      // Step 4. Add the cold junction equivalent thermocouple voltage calculated in step 3 to the thermocouple voltage calculated in step 2.
      totalVoltage = thermocoupleVoltage + internalVoltage;
  
      // Step 5. Use the result of step 4 and the NIST voltage-to-temperature (inverse) coefficients to calculate the cold junction compensated, linearized temperature value.
      // The equation is in the form correctedTemp = d_0 + d_1*E + d_2*E^2 + ... + d_n*E^n, where E is the totalVoltage in mV and correctedTemp is in degrees C.
      // NIST uses different coefficients for different temperature subranges: (-200 to 0C), (0 to 500C) and (500 to 1372C).
      if (totalVoltage < 0) { // Temperature is between -200 and 0C.
         double d[] = {0.0000000E+00, 2.5173462E+01, -1.1662878E+00, -1.0833638E+00, -8.9773540E-01, -3.7342377E-01, -8.6632643E-02, -1.0450598E-02, -5.1920577E-04, 0.0000000E+00};
  
         int dLength = sizeof(d) / sizeof(d[0]);
         for (i = 0; i < dLength; i++) {
            correctedTemp += d[i] * pow(totalVoltage, i);
         }
      }
      else if (totalVoltage < 20.644) { // Temperature is between 0C and 500C.
         double d[] = {0.000000E+00, 2.508355E+01, 7.860106E-02, -2.503131E-01, 8.315270E-02, -1.228034E-02, 9.804036E-04, -4.413030E-05, 1.057734E-06, -1.052755E-08};
         int dLength = sizeof(d) / sizeof(d[0]);
         for (i = 0; i < dLength; i++) {
            correctedTemp += d[i] * pow(totalVoltage, i);
         }
      }
      else if (totalVoltage < 54.886 ) { // Temperature is between 500C and 1372C.
         double d[] = {-1.318058E+02, 4.830222E+01, -1.646031E+00, 5.464731E-02, -9.650715E-04, 8.802193E-06, -3.110810E-08, 0.000000E+00, 0.000000E+00, 0.000000E+00};
         int dLength = sizeof(d) / sizeof(d[0]);
         for (i = 0; i < dLength; i++) {
            correctedTemp += d[i] * pow(totalVoltage, i);
         }
      } else { // NIST only has data for K-type thermocouples from -200C to +1372C. If the temperature is not in that range, set temp to impossible value.
         // Error handling should be improved.
         Serial.print("Temperature is out of range. This should never happen.");
         correctedTemp = NAN;
      }
  
      //Serial.print("Corrected Temp = ");
      //Serial.println(correctedTemp, 5);
      //Serial.println("");
  
   }

   correctedTempF=(correctedTemp * 9 / 5) + 32;
    //end of sensor linearization

    //
    // logging block
    //
    
    // make a string for assembling the data to log:
    String dataString = "";
  
    // read three sensors and append to the string:
    //for (int analogPin = 0; analogPin < 3; analogPin++) {
    //  int sensor = analogRead(analogPin);
    //  dataString += String(sensor);
    //  if (analogPin < 2) {
    //    dataString += ",";
    //  }
    //}

    logfile.print(m);           // milliseconds
    logfile.print(", ");    

    
    // log time
    //logfile.print(now.unixtime()); // seconds since 1/1/1970
    //logfile.print(", ");
    logfile.print('"');
    logfile.print(now.Year(), DEC);
    logfile.print("-");
    logfile.print(now.Month(), DEC);
    logfile.print("-");
    logfile.print(now.Day(), DEC);
    logfile.print(" ");
    logfile.print(now.Hour(), DEC);
    logfile.print(":");
    logfile.print(now.Minute(), DEC);
    logfile.print(":");
    logfile.print(now.Second(), DEC);
    logfile.print('"');
    logfile.print(", ");        

    logfile.print(internalTemp);           // var
    logfile.print(", ");    
    logfile.print(rawTemp);           // var
    logfile.print(", ");    
    logfile.print(correctedTemp);           // var
    logfile.println();

    #if ECHO_TO_SERIAL
      Serial.print(m);         // milliseconds since start
      Serial.print(", ");  

 //     Serial.print(now.unixtime()); // seconds since 1/1/1970
 //     Serial.print(", ");
      Serial.print('"');
      Serial.print(now.Year(), DEC);
      Serial.print("-");
      Serial.print(now.Month(), DEC);
      Serial.print("-");
      Serial.print(now.Day(), DEC);
      Serial.print(" ");
      Serial.print(now.Hour(), DEC);
      Serial.print(":");
      Serial.print(now.Minute(), DEC);
      Serial.print(":");
      Serial.print(now.Second(), DEC);
      Serial.print('"');
      Serial.print(", ");          
      Serial.print(internalTemp);           // var
      Serial.print(", ");    
      Serial.print(rawTemp);           // var
      Serial.print(", ");    
      Serial.print(correctedTemp);           // var
      Serial.println();   
    #endif







    
    /*
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("datalog2.txt", FILE_WRITE);
  
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog2.txt");
    }
    */
    //
    // end log setup
    //
    

   
  //graph block
 //   Graph(tft, x, y, "", 50, 290, 390, 260, 0, 60, 10, 70, 90, 5, "Room Temperature", " Time [s]", "Temperature [deg F]", TFT_NAVY, TFT_RED, TFT_GREEN, TFT_WHITE, TFT_BLACK, display7);
    Graph(tft, x, internalTempF, 0, "Inter", 45, 290, 420, 260, 0, 35, 5, 70, 90, 5, "Room Temperature", " Time [s]", "Temperature [deg F]", TFT_ORANGE, TFT_RED, TFT_GREEN, TFT_WHITE, TFT_BLACK, display7);
    Graph(tft, x, rawTempF, 1, "Raw", 45, 290, 420, 260, 0, 35, 5, 70, 90, 5, "Room Temperature", " Time [s]", "Temperature [deg F]", TFT_ORANGE, TFT_RED, TFT_RED, TFT_WHITE, TFT_BLACK, display7);
    Graph(tft, x, correctedTempF, 2, "Corr", 45, 290, 420, 260, 0, 35, 5, 70, 90, 5, "Room Temperature", " Time [s]", "Temperature [deg F]", TFT_ORANGE, TFT_RED, TFT_WHITE, TFT_WHITE, TFT_BLACK, display7);


    //Plot current time:
    tft.fillRect(45+300 , 290 + 20, 120, 8, TFT_BLACK); //clear out old x
    tft.setTextColor(TFT_ORANGE);
    tft.setCursor(45+300 , 290 + 20);
    tft.print(now.Year(), DEC);
    tft.print("-");
    tft.print(now.Month(), DEC);
    tft.print("-");
    tft.print(now.Day(), DEC);
    tft.print(" ");
    tft.print(now.Hour(), DEC);
    tft.print(":");
    tft.print(now.Minute(), DEC);
    tft.print(":");
    tft.print(now.Second(), DEC);

  
  } //end of screen loop (each data point)
  //tft.fillScreen(TFT_BLACK);


    // Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card
    // which uses a bunch of power and takes time
    if ((millis() - syncTime) < SYNC_INTERVAL) return;
    syncTime = millis();
  
    // blink LED to show we are syncing data to the card & updating FAT!
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Writing Data");
    logfile.flush();
       //delay(2000);
    digitalWrite(LED_BUILTIN, LOW);

}

/*

  function to draw a cartesian coordinate system and plot whatever data you want
  just pass x and y and the graph will be drawn

  huge arguement list
  &d name of your display object
  x = x data point
  y = y datapont
  iname = integer variable for legend flag and referencing previous values
  sname = variable legend display name
  gx = x graph location (lower left)
  gy = y graph location (lower left)
  w = width of graph
  h = height of graph
  xlo = lower bound of x axis
  xhi = upper bound of x asis
  xinc = division of x axis (distance not count)
  ylo = lower bound of y axis
  yhi = upper bound of y asis
  yinc = division of y axis (distance not count)
  title = title of graph
  xlabel = x asis label
  ylabel = y asis label
  gcolor = graph line colors
  acolor = axi ine colors
  pcolor = color of your plotted data
  tcolor = text color
  bcolor = background color
  &redraw = flag to redraw graph on fist call only
*/


void Graph(TFT_HX8357 &d, double x, double y, int iname, String sname, double gx, double gy, double w, double h, double xlo, double xhi, double xinc, double ylo, double yhi, double yinc, String title, String xlabel, String ylabel, unsigned int gcolor, unsigned int acolor, unsigned int pcolor, unsigned int tcolor, unsigned int bcolor, boolean &redraw) {

  double ydiv, xdiv;
  // initialize old x and old y in order to draw the first point of the graph
  // but save the transformed value
  // note my transform funcition is the same as the map function, except the map uses long and we need doubles
  //static double ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
  //static double oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  double j;
  double temp;
  double ox1[iname];
  double oy1[iname];

  int rot, newrot;

  if (redraw == true) {
    
    redraw = false;
    legoff=0; //set legend offset to zero

    ox[iname] = (x - xlo) * ( w) / (xhi - xlo) + gx;
    oy[iname] = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    //ox1 = (x - xlo) * ( w) / (xhi - xlo) + gx;
    //oy1 = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;   


    d.drawFastHLine(gx-1, gy+1, w+2, acolor);  //draw x-axis low
    d.drawFastHLine(gx-1, gy-h-1, w+2, acolor); //draw x-axis   high  
    d.drawFastVLine(gx-1, gy-h-1, h+2, acolor); //draw y-axis low
    d.drawFastVLine(gx+w+0, gy-h-1, h+2, acolor); //draw y-axis high
        


    // draw y scale
    for ( j = ylo; j <= yhi; j += yinc) {
      // compute the transform
      temp =  (j - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
      d.drawFastHLine(gx-5, temp, 4,  gcolor);
 
      d.setTextSize(1);
      d.setTextColor(tcolor, bcolor);
      d.setCursor(gx - 40, temp); //y-axis labels
      // precision is default Arduino--this could really use some format control
      d.println(j);
    }
    // draw x scale
    for (j = xlo; j <= xhi; j += xinc) {

      // compute the transform

      temp =  (j - xlo) * ( w) / (xhi - xlo) + gx;
      d.drawFastVLine(temp, gy+4, 4, gcolor);

      d.setTextSize(1);
      d.setTextColor(tcolor, bcolor);
      d.setCursor(temp-20, gy + 10); //x-axis labels
      // precision is default Arduino--this could really use some format control
      d.println(j);
    }




    //now draw the labels
    d.setTextSize(2);
    d.setTextColor(tcolor, bcolor);
    d.setCursor(gx , gy - h - 30);
    d.println(title);

    d.setTextSize(1);
    d.setTextColor(acolor, bcolor);
    d.setCursor(gx , gy + 20);
    d.println(xlabel);

    d.setTextSize(1);
    d.setTextColor(acolor, bcolor);
    d.setCursor(gx - 30, gy - h - 10);
    d.println(ylabel);

  }


 
  if (var[iname] == true) {

    //Build Legend color key
    tft.setTextSize(1); tft.setTextColor(TFT_WHITE);
    tft.fillRect(gx+w+1-225+legoff, gy-h-1-15, 15, 8, pcolor); tft.setCursor(gx+w+1-225+20+legoff, gy-h-1-15+0); tft.print(sname);
  
    //print current for debugging
    //tft.fillRect(gx+175 , gy + 20, 300, 8, TFT_BLACK); //clear out old var
    //d.setCursor(gx+175 , gy + 20);
    //d.println(legoff);
    //d.println("  ");
    //d.println(vname);
    //delay(5000);

    legpos[iname] = legoff;
    var[iname] = false;
    legoff += 50;

  }

  //graph drawn now plot the data
  // the entire plotting code are these few lines...
  // recall that ox and oy are initialized as static above

  //Plot current measurement:
  tft.fillRect(gx+100+legpos[iname] , gy + 20, 100, 8, TFT_BLACK); //clear out old x
  tft.setTextColor(pcolor);
  d.setCursor(gx+100+legpos[iname] , gy + 20);
  d.println(y);

  // initialize x/y
  if (x == 0) {
     ox[iname] = (x - xlo) * ( w) / (xhi - xlo) + gx;
     oy[iname] = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
     } 
  //move to graphing function and plot a black box
  if (isnan(y)) {
      y=ylo+2;
     }
  else if (y > yhi) {
     y = yhi-2; //offset to avoid overwriting axis
     } 
  else if (y < ylo) {
     y = ylo+2;//offset to avoid overwriting axis
     } 
  x =  (x - xlo) * ( w) / (xhi - xlo) + gx;
  y =  (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  
  ////Draw preceding black 'boxes' to erase old plot lines, !!!WEIRD CODE TO COMPENSATE FOR BUTTONS AND COLOR KEY SO 'ERASER' DOESN'T ERASE BUTTONS AND COLOR KEY!!!
  //if (x >= ox1+w+20) //in range with buffer at beginning
  //{
  //  tft.fillRect(gx, gy-h, 20, h, TFT_GREY);
  //}
  //Serial.print(x);
  //Serial.println("");
  if ((x > gx-20) && (x <= gx+w-20)) //in range with buffer at beginning
  {
    tft.fillRect(x, gy-h, 20, h+1, TFT_BLACK);
  }
  else if ((x > gx+w-20) && (x <= gx+w))
  {
    tft.fillRect(x, gy-h, gx+w-x+1, h+1, TFT_BLACK);
  }  
  else if (x <= gx-20)
  {
    tft.fillRect(gx-20, gy-h, 20+1, h+1, TFT_BLACK);
  //  Serial.print("less than");
  }  

  //plot lines
  
  d.drawLine(ox[iname], oy[iname], x, y, pcolor);
  d.drawLine(ox[iname], oy[iname] + 1, x, y + 1, pcolor);
  d.drawLine(ox[iname], oy[iname] - 1, x, y - 1, pcolor);
  ox[iname] = x;
  oy[iname] = y;
  //print current x
  //tft.fillRect(gx+100 , gy + 20, 300, 8, TFT_BLACK); //clear out old x
  //d.setCursor(gx+100 , gy + 20);
  //d.println(x);
  //d.setCursor(gx+200 , gy + 20);
  //d.println(gx+w-x);
}

/*
  End of graphing functioin
*/






