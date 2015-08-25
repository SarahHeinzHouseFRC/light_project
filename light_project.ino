
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>

// This is calibration data for the raw touch data to the screen coordinates
#define TS_MINX 50
#define TS_MINY 150
#define TS_MAXX 3800
#define TS_MAXY 3800

#define STMPE_CS 8
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define BOXSIZE 15
#define LED_WIDTH 20
#define LED_HEIGHT 16

bool pattern[20][16];
bool bErasing = false;
TS_Point p;


// set z pressure to x < 50


void setup(void)
{
  Serial.begin(9600);
  tft.begin();
  if (!ts.begin())
    Serial.println("Unable to start touchscreen.");
  else
    Serial.println("Touchscreen started.");

  tft.fillScreen(ILI9341_BLACK);
  // origin = left,top landscape (USB left upper)
  tft.setRotation(1); 
   
  tft.setTextColor(ILI9341_ORANGE);
  tft.setTextSize(8);
  tft.print("S");
  tft.print("H");
  tft.setTextColor(ILI9341_BLUE);
  tft.print("A");
  tft.setTextColor(ILI9341_GREEN);
  tft.print("R");
  tft.print("P");
  //delay(100);
  tft.fillScreen(ILI9341_BLACK);
  draw_sidebar();
}


void loop()
{
  if(ts.bufferEmpty())
    return;
    
    
 //if(p.z <
    update_ts();
    // Serial.println(p.z); 
  if(p.y < 20)
   {
     
    if(bErasing)
     {
      pattern[p.x][p.y] = false;
      tft.fillRect(BOXSIZE*p.y, BOXSIZE*p.x, BOXSIZE, BOXSIZE, ILI9341_BLACK);
    }
    
    else
    {
      pattern[p.x][p.y] = true;
      tft.fillRect(BOXSIZE*p.y, BOXSIZE*p.x, BOXSIZE, BOXSIZE, ILI9341_WHITE);
    }
   }

  check_sidebar();
 
}


void draw_sidebar()
{
    tft.fillRect(BOXSIZE*20, BOXSIZE*0, BOXSIZE*2, BOXSIZE*5, ILI9341_GREEN);
    
    tft.fillRect(BOXSIZE*20, BOXSIZE*5, BOXSIZE*2, BOXSIZE*6, ILI9341_WHITE);
    
    tft.fillRect(BOXSIZE*20, BOXSIZE*11, BOXSIZE*2, BOXSIZE*5, ILI9341_RED);
}

void check_sidebar()
{
 Serial.println(p.y);
  if(p.y >= 19)
 {
    while(finishedButtonPressed() && ts.touched())
    {
      update_ts();
      tft.fillRect(BOXSIZE*21, BOXSIZE*0, BOXSIZE, BOXSIZE*5, ILI9341_PINK);
       
       //to send to lights
       
    }
    draw_sidebar();
    while(toggleButtonPressed() && ts.touched())
    {
           update_ts();
       tft.fillRect(BOXSIZE*21, BOXSIZE*5, BOXSIZE, BOXSIZE*6, ILI9341_PINK);
       //to change to eraser
       
    }
    draw_sidebar();
    while(resetButtonPressed() && ts.touched())
    {
       update_ts();
       tft.fillRect(BOXSIZE*21, BOXSIZE*11, BOXSIZE, BOXSIZE*5, ILI9341_PINK);
       
       //to erase screen
    }
    if(ts.touched()) 
    {
       Serial.println("no button pressed");
    }
    else
    {
     if (finishedButtonPressed())
       Serial.println("finished button pressed");
     else if (toggleButtonPressed()) 
       bErasing = !bErasing;
     else if (resetButtonPressed())
       resetScreen();
    }
    
     draw_sidebar();
       
  }
 
}

void update_ts()
{
  if (!ts.bufferEmpty()) {
    p = ts.getPoint();
    p.x = map(p.x, TS_MAXX, TS_MINX, 0, 16);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 21);
  }
}

bool finishedButtonPressed()
{
  return(p.x <= 4 && p.y >= 19);
}
bool toggleButtonPressed()
{
  return (4 < p.x && p.x < 11 && p.y >= 19);
}
bool resetButtonPressed()
{
  return (p.x >= 11 && p.y >= 19);
}

void resetScreen()
{
  for(int i = 0; i < LED_WIDTH; i++)
    for(int n = 0; n < LED_HEIGHT; n++)
      pattern[i][n] = false;
  tft.fillScreen(ILI9341_BLACK);
}
