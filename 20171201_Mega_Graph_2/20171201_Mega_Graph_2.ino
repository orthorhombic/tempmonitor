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
double ox , oy ;

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
  for (x = 0; x <= 7; x += 0.01) {
    //y=thermocouple.readFarenheit();
    y=thermocouple.readCelsius();
    yint=thermocouple.readInternal();
    if (isnan(y)) {
      y=0;
    }


   
  
 //   Graph(tft, x, y, 50, 290, 390, 260, 0, 60, 10, 70, 90, 5, "Room Temperature", " Time [s]", "Temperature [deg F]", TFT_NAVY, TFT_RED, TFT_GREEN, TFT_WHITE, TFT_BLACK, display7);
    Graph(tft, x, y, "Y",45, 290, 420, 260, 0, 7, 1, 20, 40, 5, "Room Temperature", " Time [s]", "Temperature [deg C]", TFT_ORANGE, TFT_RED, TFT_GREEN, TFT_WHITE, TFT_BLACK, display7);
    Graph(tft, x, yint, "int", 45, 290, 420, 260, 0, 7, 1, 20, 40, 5, "Room Temperature", " Time [s]", "Temperature [deg C]", TFT_ORANGE, TFT_RED, TFT_WHITE, TFT_WHITE, TFT_BLACK, display7);
 
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
  vname = variable legend display name
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


void Graph(TFT_HX8357 &d, double x, double y, String vname, double gx, double gy, double w, double h, double xlo, double xhi, double xinc, double ylo, double yhi, double yinc, String title, String xlabel, String ylabel, unsigned int gcolor, unsigned int acolor, unsigned int pcolor, unsigned int tcolor, unsigned int bcolor, boolean &redraw) {

  double ydiv, xdiv;
  // initialize old x and old y in order to draw the first point of the graph
  // but save the transformed value
  // note my transform funcition is the same as the map function, except the map uses long and we need doubles
  //static double ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
  //static double oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
  double i;
  double temp;
  double ox1;
  double oy1;
  int rot, newrot;

  if (redraw == true) {

    redraw = false;
    ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
    oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    //ox1 = (x - xlo) * ( w) / (xhi - xlo) + gx;
    //oy1 = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;   


    d.drawFastHLine(gx, gy+1, w+2, acolor);  //draw x-axis
    d.drawFastHLine(gx, gy-h-1, w+2, acolor); //draw x-axis     
    d.drawFastVLine(gx-1, gy-h-1, h+2, acolor); //draw y-axis
    d.drawFastVLine(gx+w+1, gy-h-1, h+2, acolor); //draw y-axis
        


    // draw y scale
    for ( i = ylo; i <= yhi; i += yinc) {
      // compute the transform
      temp =  (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
      d.drawFastHLine(gx-5, temp, 4,  gcolor);
 
      d.setTextSize(1);
      d.setTextColor(tcolor, bcolor);
      d.setCursor(gx - 40, temp); //y-axis labels
      // precision is default Arduino--this could really use some format control
      d.println(i);
    }
    // draw x scale
    for (i = xlo; i <= xhi; i += xinc) {

      // compute the transform

      temp =  (i - xlo) * ( w) / (xhi - xlo) + gx;
      d.drawFastVLine(temp, gy+4, 4, gcolor);

      d.setTextSize(1);
      d.setTextColor(tcolor, bcolor);
      d.setCursor(temp-20, gy + 10); //x-axis labels
      // precision is default Arduino--this could really use some format control
      d.println(i);
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

    //Build Legend color key
//    tft.setTextSize(1); tft.setTextColor(TFT_WHITE);
//    tft.fillRect(gx+w+1, gy-h-1,, 15, 8, pcolor); tft.setCursor(gx+w+1+16, gy-h-1+0); tft.print(vname);



  }

  //graph drawn now plot the data
  // the entire plotting code are these few lines...
  // recall that ox and oy are initialized as static above

  //Plot current measurement:
  tft.fillRect(gx+100 , gy + 20, 100, 8, TFT_BLACK); //clear out old x
  d.setCursor(gx+100 , gy + 20);
  d.println(y);

  // initialize x/y
  if (x == 0) {
     ox = (x - xlo) * ( w) / (xhi - xlo) + gx;
     oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
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

  if ((x > gx-20) && (x <= gx+w-20)) //in range with buffer at beginning
  {
    tft.fillRect(x+1, gy-h, 20, h+1, TFT_BLACK);
    
    //tft.fillRect(284, 208, 18, 8, TFTLCD_BLACK); //clear out old second
  }
  else if ((x > gx+w-20) && (x <= gx+w))
  {
    tft.fillRect(x+1, gy-h, gx+w-x, h+1, TFT_BLACK);
  }  

  //plot lines
  
  d.drawLine(ox, oy, x, y, pcolor);
  d.drawLine(ox, oy + 1, x, y + 1, pcolor);
  d.drawLine(ox, oy - 1, x, y - 1, pcolor);
  ox = x;
  oy = y;
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






