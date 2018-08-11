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

*/


#include <TFT_HX8357.h> // Hardware-specific library
TFT_HX8357 tft = TFT_HX8357();       // Invoke custom library
#define TFT_GREY 0x7BEF



//#define ADJ_PIN A0


// this is the only external variable used by the graph
// it's a flat to draw the coordinate system only on the first pass
// boolean display1 = true;
// boolean display2 = true;
// boolean display3 = true;
// boolean display4 = true;
// boolean display5 = true;
// boolean display6 = true;
boolean display7 = true;
// boolean display8 = true;
// boolean display9 = true;
//initialize graph variables
boolean var[6] = {true,true,true,true,true,true};
//boolean Intern = true;

double legoff;
double ox[6] , oy[6] ;

//MAX31855
#include "Adafruit_MAX31855.h"
int vccPin = 2;
//3v3 out
int gndPin = 4;
#define MAXDO   5
#define MAXCS   6
#define MAXCLK  7

// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);




void setup() {

  tft.init();
  tft.setRotation(1);

  //power up MAX31855
  pinMode(vccPin, OUTPUT); digitalWrite(vccPin, HIGH);
  pinMode(gndPin, OUTPUT); digitalWrite(gndPin, LOW);

  Serial.begin(38400);

  delay(1000);
  tft.fillScreen(TFT_BLACK);




}


void loop(void) {

  
  double x, y, yint;
  //for (x = 0; x <= 7; x += 0.0023809524) {
  for (x = 0; x <= 7; x += 0.1) {
    //y=thermocouple.readFarenheit();
    //y=thermocouple.readCelsius();
    //yint=thermocouple.readInternal();
    
    
  //from https://github.com/heypete/MAX31855-Linearization
  // Initialize variables.
   int i = 0; // Counter for arrays
   double internalTemp = thermocouple.readInternal(); // Read the internal temperature of the MAX31855.
   double rawTemp = thermocouple.readCelsius(); // Read the temperature of the thermocouple. This temp is compensated for cold junction temperature.
   double thermocoupleVoltage= 0;
   double internalVoltage = 0;
   double correctedTemp = 0;
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
    
    
    

    //move to graphing function and plot a black box
    if (isnan(y)) {
      correctedTemp=0;
    }


   
  
 //   Graph(tft, x, y, "", 50, 290, 390, 260, 0, 60, 10, 70, 90, 5, "Room Temperature", " Time [s]", "Temperature [deg F]", TFT_NAVY, TFT_RED, TFT_GREEN, TFT_WHITE, TFT_BLACK, display7);
    //Graph(tft, x, y, 0, "TC", 45, 290, 420, 260, 0, 7, 1, 20, 40, 5, "Room Temperature", " Time [s]", "Temperature [deg C]", TFT_ORANGE, TFT_RED, TFT_GREEN, TFT_WHITE, TFT_BLACK, display7);
    Graph(tft, x, correctedTemp, 1, "Corrected", 45, 290, 420, 260, 0, 7, 1, 20, 40, 5, "Room Temperature", " Time [s]", "Temperature [deg C]", TFT_ORANGE, TFT_RED, TFT_WHITE, TFT_WHITE, TFT_BLACK, display7);
 
    delay(100);
  }
  //tft.fillScreen(TFT_BLACK);

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


    d.drawFastHLine(gx, gy+1, w+2, acolor);  //draw x-axis
    d.drawFastHLine(gx, gy-h-1, w+2, acolor); //draw x-axis     
    d.drawFastVLine(gx-1, gy-h-1, h+2, acolor); //draw y-axis
    d.drawFastVLine(gx+w+1, gy-h-1, h+2, acolor); //draw y-axis
        


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
    tft.fillRect(gx+w+1-150+legoff, gy-h-1-15, 15, 8, pcolor); tft.setCursor(gx+w+1-150+20+legoff, gy-h-1-15+0); tft.print(sname);
  
    //print current for debugging
    //tft.fillRect(gx+175 , gy + 20, 300, 8, TFT_BLACK); //clear out old var
    //d.setCursor(gx+175 , gy + 20);
    //d.println(legoff);
    //d.println("  ");
    //d.println(vname);
    //delay(5000);

    
    var[iname] = false;
    legoff += 50;

  }

  //graph drawn now plot the data
  // the entire plotting code are these few lines...
  // recall that ox and oy are initialized as static above

  //Plot current measurement:
  tft.fillRect(gx+100+legoff , gy + 20, 100, 8, TFT_BLACK); //clear out old x
  tft.setTextColor(pcolor);
  d.setCursor(gx+100+legoff , gy + 20);
  d.println(y);

  // initialize x/y
  if (x == 0) {
     ox[iname] = (x - xlo) * ( w) / (xhi - xlo) + gx;
     oy[iname] = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
     } 
  if (y > yhi) {
     y = yhi;
     } 
  if (y < ylo) {
     y = ylo;
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
    
    //tft.fillRect(284, 208, 18, 8, TFTLCD_BLACK); //clear out old second
  }
  else if ((x > gx+w-20) && (x <= gx+w))
  {
    tft.fillRect(x, gy-h, gx+w-x, h+1, TFT_BLACK);
  }  
  else if (x <= gx-20)
  {
    tft.fillRect(gx-20, gy-h, 20, h+1, TFT_BLACK);
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






