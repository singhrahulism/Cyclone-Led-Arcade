#include <FastLED.h>

#define NUM_LEDS  100                 // Total number of Leds
#define DATA_PIN 7 
#define LED_TYPE WS2811
#define SPEED_INCREASE_VALUE  25
#define MINIMUM_DELAY 6
#define RUNNING_LED_COLOR Green
#define CENTER_LED_COLOR  Red

#define BUTTON1  8
#define BUTTON2 8
#define BUTTON3 8

#define OUTER_CIRCLE_START  1
#define OUTER_CIRCLE_END  41
#define MIDDLE_CIRCLE_START  41
#define MIDDLE_CIRCLE_END  73
#define INNER_CIRCLE_START  73
#define INNER_CIRCLE_END  100

#define FAILURE_COUNT 3
#define SUCCESS_COUNT 5

CRGBArray<NUM_LEDS> leds;

unsigned long  tm ;
int index, pressDelay=200 ;

int path[3][2] = {
               {OUTER_CIRCLE_START, OUTER_CIRCLE_END},
               {MIDDLE_CIRCLE_START, MIDDLE_CIRCLE_END},
               {INNER_CIRCLE_START, INNER_CIRCLE_END}
              } ;

int button[3] = {BUTTON1, BUTTON2, BUTTON3} ;

int centerLed, centerLedArray[3][3] = {{19, 20, 21},           // Center Leds for Outer Circle
                    {56, 57, 58},                           // Center Leds for Middle Circle
                    {86, 87, 88}                            // Center Leds for Inner Circle
                   } ;

int gameScore=0 ;

void blankAllLeds(int), showFailurePattern(int), showSuccessPattern(int) ;

void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS) ;
  FastLED.setBrightness(60) ;
  // turning off all leds

  for(int i=0 ; i < NUM_LEDS ; ++i)
    leds[i] = CRGB::Black ;
  FastLED.show() ;

  // done.
  // leds[chooseCenterLed()] = CRGB::Red ;
  FastLED.show() ;
  Serial.begin(9600) ;
  pinMode(12, OUTPUT) ;
  digitalWrite(12, HIGH) ;
  Serial.begin(9600) ;
}

void loop()
{
    startAgain :
    if(pressDelay <= MINIMUM_DELAY)
      pressDelay = MINIMUM_DELAY ;
    for(int i=0 ; i < NUM_LEDS ; ++i)
      leds[i] = CRGB::Black ;
    FastLED.show() ;
    index = random(0,3) ;
    centerLed = centerLedArray[index][random(0,3)] ;          // Center Led according to current circle
    Serial.println("Here") ;
    Serial.println(centerLed) ;
    leds[centerLed] = CRGB::Red ;
    FastLED.show() ;
    for(int i=path[index][0] ; i < path[index][1] ; ++i)
    {
      leds[i] = CRGB::RUNNING_LED_COLOR ;
      FastLED.show() ;
      tm = millis() ;
      while((unsigned long)(millis()-tm) <= pressDelay)
      {
          if(digitalRead(button[index]))
          {
//            Serial.print("i : ") ;
//            Serial.println(i) ;
//            Serial.print("centerLed : ") ;
//            Serial.println(centerLed) ;
            if(i == centerLed)
            {
              ++gameScore ;
              Serial.print("Score : ") ;
              Serial.println(gameScore) ;
              pressDelay = abs(pressDelay-SPEED_INCREASE_VALUE) ;
              blankAllLeds(index) ;
              showSuccessPattern(index) ;
              blankAllLeds(index) ;
              // leds[centerLed] = CRGB::CENTER_LED_COLOR ;
              FastLED.show() ;
            }
            else
            {
              blankAllLeds(index) ;
              showFailurePattern(index) ;
              blankAllLeds(index) ;
              // leds[centerLed] = CRGB::CENTER_LED_COLOR ;
              FastLED.show() ;
            }
            blankAllLeds(index) ;
            // postPressPattern() ;
            goto startAgain ;
          }
      }
      (i != centerLed) ? (leds[i] = CRGB::Black) : (leds[i] = CRGB::CENTER_LED_COLOR) ;
      FastLED.show() ;
    }
}

void showSuccessPattern(int index)
{
    for(int j=0 ; j < SUCCESS_COUNT ; ++j)
    {
      static uint8_t hue;
      for(int i = path[index][0] ; i < path[index][1] ; ++i )
      {   
        // fade everything out
        leds.fadeToBlackBy(40);
    
        // let's set an led value
        leds[i] = CHSV(hue++,255,255);
    
        // now, let's first 20 leds to the top 20 leds, 
        leds((path[index][1]-path[index][0])/2+1, path[index][1]-1) = leds((path[index][1]-path[index][0])/2, path[index][0]) ;
        FastLED.delay(33);
      }
    } 
}

void showFailurePattern(int index)
{
  for(int j=0 ; j < FAILURE_COUNT ; ++j)
  {
    for(int i=path[index][0] ; i < path[index][1] ; ++i)
      leds[i] = CRGB::Red ;
    FastLED.show() ;
    delay(1000) ;
    blankAllLeds(index) ;
    delay(1000) ;
  }
}

void blankAllLeds(int index)
{
  for(int i=path[index][0] ; i < path[index][1] ; ++i)
    leds[i] = CRGB::Black ;
  FastLED.show() ;
}

void postPressPattern()
{
  for(int i=0 ; i < NUM_LEDS ; ++i)
  {
    leds[i] = CRGB::Red ;
    FastLED.show() ;
    delay(30) ;
  }
  for(int i=0 ; i < NUM_LEDS ; ++i)
  {
    leds[i] = CRGB::Black ;
    FastLED.show() ;
    delay(30) ;
  }
  FastLED.show() ;
}
