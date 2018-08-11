// Demo based on:
// UTFT_Demo by Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
/*

 The delay between tests is set to 0. The tests run so fast you will need to
 change the WAIT value below to see what is being plotted!
 
 This sketch uses the GLCD and font 2 only.

 Make sure all the required fonts are loaded by editting the
 User_Setup.h file in the TFT_ILI9341 library folder.

 If using an UNO or Mega (ATmega328 or ATmega2560 processor) then for best
 performance use the F_AS_T option found in the User_Setup.h file in the
 TFT_ILI9341 library folder.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  ######            TO SELECT THE FONTS YOU USE, SEE ABOVE           ######
  #########################################################################
 */


//Colors

#define LTBLUE    0xB6DF
#define LTTEAL    0xBF5F
#define LTGREEN   0xBFF7
#define LTCYAN    0xC7FF
#define LTRED     0xFD34
#define LTMAGENTA 0xFD5F
#define LTYELLOW  0xFFF8
#define LTORANGE  0xFE73
#define LTPINK    0xFDDF
#define LTPURPLE  0xCCFF
#define LTGREY    0xE71C

#define BLUE      0x001F
#define TEAL      0x0438
#define GREEN     0x07E0
#define CYAN      0x07FF
#define RED       0xF800
#define MAGENTA   0xF81F
#define YELLOW    0xFFE0
#define ORANGE    0xFC00
#define PINK      0xF81F
#define PURPLE    0x8010
#define GREY      0xC618
#define WHITE     0xFFFF
#define BLACK     0x0000

#define DKBLUE    0x000D
#define DKTEAL    0x020C
#define DKGREEN   0x03E0
#define DKCYAN    0x03EF
#define DKRED     0x6000
#define DKMAGENTA 0x8008
#define DKYELLOW  0x8400
#define DKORANGE  0x8200
#define DKPINK    0x9009
#define DKPURPLE  0x4010
#define DKGREY    0x4A49


// Delay between demo pages
#define WAIT 2000 // Delay between tests, set to 0 to demo speed, 2000 to see what it does!

#define CENTRE 240

#include <TFT_HX8357.h> // Hardware-specific library

TFT_HX8357 tft = TFT_HX8357();       // Invoke custom library

#define TFT_GREY 0x7BEF

uint32_t runTime = 0;

void setup()
{
  randomSeed(analogRead(0));
  Serial.begin(38400);
// Setup the LCD
  tft.init();
  tft.setRotation(1);

  
}

void loop()
{
  int buf[478];
  int x, x2;
  int y, y2;
  int r;

  runTime = millis();


// Draw a moving sinewave
int col = 0;
  x=1;
  for (int i=1; i<(477*15); i++) 
  {
    x++;
    if (x==478)
      x=1;
    if (i>478)
    {
      if ((x==239)||(buf[x-1]==159))
        col = TFT_BLUE;
      else
        tft.drawPixel(x,buf[x-1],TFT_BLACK);
    }
    y=159+(sin(((i*0.7)*3.14)/180)*(90-(i / 100)));
    tft.drawPixel(x,y, TFT_BLUE);
    buf[x-1]=y;
  }

  delay(WAIT);
  
  tft.fillRect(1,15,478-1,304-15,TFT_BLACK);



  tft.fillRect(0, 0, 480, 320, TFT_BLUE);

  tft.fillRoundRect(160, 70, 319-160, 169-70, 3,TFT_RED);
  
  tft.setTextColor(TFT_WHITE,TFT_RED);
  tft.drawCentreString("That's it!", CENTRE, 93,2);
  tft.drawCentreString("Restarting in a", CENTRE, 119, 2);
  tft.drawCentreString("few seconds...", CENTRE, 132, 2);

  tft.setTextColor(TFT_GREEN,TFT_BLUE);
  tft.drawCentreString("Runtime: (msecs)", CENTRE, 280, 2);
  tft.setTextDatum(TC_DATUM);
  runTime = millis()-runTime;
  tft.drawNumber(runTime, CENTRE, 300,2);
  tft.setTextDatum(TL_DATUM);
  delay (10000);
}
