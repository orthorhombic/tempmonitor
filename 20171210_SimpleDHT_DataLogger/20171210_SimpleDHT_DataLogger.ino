/* A very simple RTC triggered datalogger script. Note that I have replaced the
analog pin reads in Tom’s Igoes starter code 
with DS3231 I2C register reading & the delay has been replaced with sleep & RTC interrupt alarms
*/
#include <SD.h>  // I much prefer SdFat.h by Greiman over the old SD.h library used here
#include  <SPI.h>
const int chipSelect = 10;    //CS moved to pin 10 on the arduino
#include <Wire.h>
#include "LowPower.h"     // from https://github.com/rocketscream/Low-Power
#include <RTClib.h>       // library from   https://github.com/MrAlvin/RTClib
RTC_DS3231 RTC;
// creates an RTC object in the code
//variables for reading the RTC time & handling the INT(0) interrupt it generates
#define DS3231_I2C_ADDRESS 0x68
int RTC_INTERRUPT_PIN = 2;
byte Alarmhour;
byte Alarmminute;
byte Alarmday;
//char CycleTimeStamp[ ] = "0000-00-00T00:00:00Z-05"; // 23 ascii characters 
char CycleTimeStamp[23];
String dataString = "0000-00-00T00:00:00, 00.00, 00.00, 00.00, 00.00, 00.00"; //this line simply sets the string globally



// the logging file
File logfile;  
//char filename[] = "LOGGER00.CSV";
char filename[]="TmpLog00.CSV";
//char filename1[12];
#define SampleIntervalMinutes 1
/*
// CHANGE SampleIntervalMinutes to the number of minutes you want between samples! Whole numbers 1-59 only
// NOTE yes the RTClib will do down to one second intervals with the right code
// HOWEVER if you cut power to an SD card while it
// is writing information you destroy the data file on the card….
// so how do you solve that problem if you start sampling too frequently?
*/
volatile boolean clockInterrupt = false;  
//this flag is set to true when the RTC interrupt handler is executed

//variables for reading the DS3231 RTC temperature register
float temp3231;
byte tMSB = 0;
byte tLSB = 0;

//DHT22 initialization
#include <SimpleDHT.h>

int dataPinSensor1 = 7;
int dataPinSensor2 = 8;    // what digital pin we're connected to

// DHT 22  (AM2302), AM2321
SimpleDHT22 dht22; // which sensor type



void setup() {
  
  // Setting the SPI pins high helps some sd cards go into sleep mode 
  // the following pullup resistors only need to be enabled for the stand alone logger builds - not the UNO loggers
  pinMode(chipSelect, OUTPUT); digitalWrite(chipSelect, HIGH); //Always pullup the CS pin with the SD library
  //and you may need to pullup MOSI/MISO
  //pinMode(MOSIpin, OUTPUT); digitalWrite(MOSIpin, HIGH); //pullup the MOSI pin
  //pinMode(MISOpin, INPUT); digitalWrite(MISOpin, HIGH);  //pullup the MISO pin
  //delay(100);
  
  Serial.begin(57600);    // Open serial communications and wait for port to open:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  
  Wire.begin();          // start the i2c interface for the RTC
  
  Serial.print("CompTime:");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  RTC.begin();           // start the RTC
  delay(2000);
  Serial.println("RTCTime:");
  //get time 
  DateTime now = RTC.now(); //this reads the time from the RTC
    Serial.print(now.hour(), DEC);
    Serial.print(":");
    Serial.println(now.minute(), DEC);
    //Serial.print(":");
    //Serial.println(now.second(), DEC);
  

//check temps
  Serial.println("Sensor 1:");
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(dataPinSensor1, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Com err-Sen1="); Serial.println(err);delay(1000);
    return;
  }
  Serial.print((float)temperature); Serial.print(" *C, "); 
  Serial.print((float)humidity); Serial.println(" H humidity");

   Serial.println("Sensor 2:");
  float temperature2 = 0;
  float humidity2 = 0;
  int err2 = SimpleDHTErrSuccess;
  if ((err2 = dht22.read2(dataPinSensor2, &temperature2, &humidity2, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Com err-Sen2="); Serial.println(err);delay(1000);
    return;
  }
  Serial.print((float)temperature2); Serial.print(" *C, "); 
  Serial.print((float)humidity2); Serial.println(" H humidity"); 




  Serial.println("SDchk");
// print lines in the setup loop only happen once
// see if the card is present and can be initialized
  if (!SD.begin(chipSelect)) {
    Serial.println("SD failed/not there");
    // don’t do anything more:
    return;
  }
  Serial.println("SDinit");


  
  // create a new file
  //File logfile; //defined globally
  //filename[0] = "TmpLog00.CSV"; //defined globally
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }

  //static char filename = filename;
  
  if (! logfile) {
    Serial.println("can't make file");
  }

  
  Serial.print("Logging: ");
  Serial.println(filename);

  //headers
  if (logfile) { // if the file is available, write to it:
    logfile.println("Timestamp,RTC Temp(F),DHT1 T,DHT1 H,DHT2 T,DHT2 H");
    //I often print many extra lines of text in file headers, identifying details about the hardware being used, the code version that was running, etc
    logfile.close();
  }
  else {
    Serial.println("file err"); // if the file isn’t open, pop up an error:
  }


  
  /*
  //————-print a header to the data file———-
  File dataFile = SD.open("datalog3.txt", FILE_WRITE);
  if (dataFile) { // if the file is available, write to it:
    dataFile.println("Timestamp, RTC Temp(F),DHT1 T, DHT1 H, DHT2 T, DHT2 H");
    //I often print many extra lines of text in file headers, identifying details about the hardware being used, the code version that was running, etc
    dataFile.close();
  }
  else {
    Serial.println("err open file"); // if the file isn’t open, pop up an error:
  }

  */
  // initialize digital pin LED_BUILTIN as an output.
  //doesn't work with sd - led is on pin D13
  //pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(LED_BUILTIN, HIGH); // startup with led lit
  delay(2000);
} // end of setup

void loop() {
  //—–This part reads the time and disables the RTC alarm
  DateTime now = RTC.now(); //this reads the time from the RTC
  //sprintf(CycleTimeStamp, " % 04d / % 02d / % 02d % 02d: % 02d", now.year(), now.month(), now.day(), now.hour(), now.minute());
  //char CycleTimeStamp[23]; //defined above
  //Serial.println();
  //RtcDateTime now = Rtc.GetDateTime();
  snprintf_P(CycleTimeStamp, 
        25,
 //       PSTR("%04u-%02u-%02uT%02u:%02u:%02uZ-05"), //with timezone
        PSTR("%04u-%02u-%02uT%02u:%02u:%02u"),
        now.year(),
        now.month(),
        now.day(),
        now.hour(),
        now.minute(),
        now.second() );
  
  
  //loads the time into a string variable
  //don’t record seconds in the time stamp because
  //the interrupt to time reading interval is <1s, so seconds are always ’00’
  
  // We set the clockInterrupt in the ISR, deal with that now:
  if (clockInterrupt) {
    if (RTC.checkIfAlarm(1)) {       //Is the RTC alarm still on?
      RTC.turnOffAlarm(1);              //then turn it off.
    }
    //print (optional) debugging message to the serial window if you wish
    Serial.print("RTC Alarm triggered ");
    Serial.println(CycleTimeStamp);
    clockInterrupt = false;                //reset the interrupt flag to false
  }//—————————————————————–
  // read the RTC temp register and print that out
  // Note: the DS3231 temp registers (11h-12h) are only updated every 64seconds
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x11);                     //the register where the temp data is stored
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 2);   //ask for two bytes of data
  if (Wire.available()) {
  tMSB = Wire.read();            //2’s complement int portion
  tLSB = Wire.read();             //fraction portion
  temp3231 = ((((short)tMSB << 8) | (short)tLSB) >> 6) / 4.0;  // Allows for readings below freezing: thanks to Coding Badly
  //temp3231 = (temp3231 * 1.8) + 32.0; // To Convert Celcius to Fahrenheit
}
else {
  temp3231 = 0;
  //if temp3231 contains zero, then you know you had a problem reading the data from the RTC!
}

//check temps
  Serial.println("Sensor 1:");
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(dataPinSensor1, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Com err-Sen1="); Serial.println(err);delay(1000);
    return;
  }
  Serial.print((float)temperature); Serial.print(" *C, "); 
  Serial.print((float)humidity); Serial.println(" H humidity");

   Serial.println("Sensor 2:");
  float temperature2 = 0;
  float humidity2 = 0;
  int err2 = SimpleDHTErrSuccess;
  if ((err2 = dht22.read2(dataPinSensor2, &temperature2, &humidity2, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Com err-Sen2="); Serial.println(err);delay(1000);
    return;
  }
  Serial.print((float)temperature2); Serial.print(" *C, "); 
  Serial.print((float)humidity2); Serial.println(" H humidity"); 

  delay(2500);

//read2
  //Serial.println("Read 2 Sensor 1:");
   temperature = 0;
   humidity = 0;
   //err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(dataPinSensor1, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
  //  Serial.print("Com err-Sen1="); Serial.println(err);delay(1000);
    return;
  }
  //Serial.print((float)temperature); Serial.print(" *C, "); 
  //Serial.print((float)humidity); Serial.println(" H humidity");

   Serial.println("Read 2 Sensor 2:");
   temperature2 = 0;
   humidity2 = 0;
   //err2 = SimpleDHTErrSuccess;
  if ((err2 = dht22.read2(dataPinSensor2, &temperature2, &humidity2, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Com err-Sen2="); Serial.println(err);delay(1000);
    return;
  }
  //Serial.print((float)temperature2); Serial.print(" *C, "); 
  //Serial.print((float)humidity2); Serial.println(" H humidity"); 

  //delay(2500);

/*
//read3
  Serial.println("Read 3 Sensor 1:");
   temperature = 0;
   humidity = 0;
   //err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(dataPinSensor1, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Com err-Sen1="); Serial.println(err);delay(1000);
    return;
  }
  Serial.print((float)temperature); Serial.print(" *C, "); 
  Serial.print((float)humidity); Serial.println(" H humidity");

   Serial.println("Read 3 Sensor 2:");
   temperature2 = 0;
   humidity2 = 0;
   //err2 = SimpleDHTErrSuccess;
  if ((err2 = dht22.read2(dataPinSensor2, &temperature2, &humidity2, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Com err-Sen2="); Serial.println(err);delay(1000);
    return;
  }
  Serial.print((float)temperature2); Serial.print(" *C, "); 
  Serial.print((float)humidity2); Serial.println(" H humidity"); 
*/



// You could read in other variables here …like the analog pins, etc,
// and just add them to the dataString before you write it to the file


//——– concatenate data into a string ———-
// Add each piece of information to the string that gets written to the SD card with:dataFile.println(dataString);
//delay(2000); //stabilize?
dataString = ""; //this line simply erases the string
//char dataString[42]; //this line simply erases the string
dataString += CycleTimeStamp;
//delay(1);
dataString += ", ";     //puts a comma between the two bits of data
//delay(1);
dataString = dataString + String(temp3231);
//delay(1);
dataString += ", ";     //puts a comma between the two bits of data
//delay(1);
dataString = dataString + String((float)temperature);
//delay(1000);
dataString += ", ";     //puts a comma between the two bits of data
//delay();
dataString = dataString + String((float)humidity);
//delay(1000);
dataString += ", ";     //puts a comma between the two bits of data
//delay(1);
dataString = dataString + String((float)temperature2);
//delay(1000);
dataString += ", ";     //puts a comma between the two bits of data
//delay();
dataString = dataString + String((float)humidity2);
//delay(1);

/*
char dataString2[42]; //this line simply erases the string
snprintf_P(dataString2, 
        42,
 //       PSTR("%04u-%02u-%02uT%02u:%02u:%02uZ-05"), //with timezone
        PSTR("%04u-%02u-%02uT%02u:%02u:%02u, %f"),//, %5.2f, %5.2f, %5.2f, %5.2f"),
        now.year(),
        now.month(),
        now.day(),
        now.hour(),
        now.minute(),
        now.second(), 
        //float(temp3231)
        float(temperature)
        //(float)humidity
        //(float)temperature2
        //(float)humidity2
        );

        Serial.println(float(temperature));
*/
//Serial.println(String(temp3231)+", "+String(dht1.readTemperature(true)+", "+String(dht1.readHumidity()+", "+String(dht2.readTemperature(true))+", "+String(dht2.readHumidity());
//Serial.println("Timestamp, DS3231 Temp(F),DHT1 Temp, DHT1 Humidity, DHT2 Temp, DHT2 Humidity ");
//Serial.println(dataString);
//Serial.println(dataString2);


//——– Now write the data to the SD card ——–
Serial.println(filename);

File logfile = SD.open(filename, FILE_WRITE);// if the file is available, write to it:
if (logfile) {
  logfile.println(dataString);
  Serial.println(dataString);
  logfile.close();
}
else {
  Serial.println("log err"); // if the file isn’t open, pop up an error:
}

/*
File dataFile = SD.open("datalog2.txt", FILE_WRITE);// if the file is available, write to it:
if (dataFile) {
  dataFile.println(dataString);
  dataFile.close();
}
else {
  Serial.println("error opening datalog2.txt"); // if the file isn’t open, pop up an error:
}
*/
// delay(10000);
// instead of using the delay we will use RTC interrupted sleeps

//——– Set the next alarm time ————–
Alarmhour = now.hour();
Alarmminute = now.minute() + SampleIntervalMinutes;
Alarmday = now.day();

// check for roll-overs
if (Alarmminute > 59) { //error catching the 60 rollover!
  Alarmminute = 0;
  Alarmhour = Alarmhour + 1;
  if (Alarmhour > 23) {
    Alarmhour = 0;
    // put ONCE-PER-DAY code here -it will execute on the 24 hour rollover
  }
}
// then set the alarm
RTC.setAlarm1Simple(Alarmhour, Alarmminute);
RTC.turnOnAlarm(1);
if (RTC.checkAlarmEnabled(1)) {
  //you would comment out most of this message printing
  //if your logger was actually being deployed in the field
  Serial.print("Alarm Enabled: ");
  Serial.print("sleeping ");
  Serial.print(SampleIntervalMinutes);
  Serial.println(" min");
  Serial.println();                                      //just adds a carriage return
}
  delay(100); //this delay is only here so we can see the LED’s it is totally optional!
  //Serial.println("Sleeping");
  //delay(5000); //this delay is only here so we can see the LED’s it is totally optional!
  //otherwise the entire loop would execute too fast for us to see it!
  //digitalWrite(GREEN_PIN, LOW);
  //digitalWrite(RED_PIN, HIGH);
  // Turn on red led as our indicator that the Arduino is sleeping.
  //digitalWrite(LED_BUILTIN, LOW); //turn off led to show sleeping
  // Note: Normally you would NOT leave an LED on like this during sleep! This is just so you can see what is going on..
  
  //——– sleep and wait for next RTC alarm ————–
  // Enable interrupt on pin2 & attach it to rtcISR function:
  attachInterrupt(0, rtcISR, LOW);
  // Enter power down state with ADC module disabled to save power:
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_ON);
  //processor starts HERE AFTER THE RTC ALARM WAKES IT UP
  detachInterrupt(0); // immediately disable the interrupt on waking
  //digitalWrite(RED_PIN, LOW);
  //digitalWrite(GREEN_PIN, HIGH); //Interupt woke processor, turn on green led
  //digitalWrite(LED_BUILTIN, HIGH);
  //Serial.println("Awake");

}// this is the END of the MAIN LOOP
  // This is the Interrupt subroutine that only executes when the rtc alarm goes off

void rtcISR() {
    clockInterrupt = true;
  }
