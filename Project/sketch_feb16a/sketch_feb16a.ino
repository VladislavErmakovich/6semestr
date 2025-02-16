#include <FastLED.h>
#define LENGTH_LED 15
#define PIN 7

CRGB leds[LENGTH_LED];

String msg; 
byte parseStart = 0;

void setup() {
  FastLED.addLeds<NEOPIXEL, PIN>(leds, LENGTH_LED);
  Serial.begin(57600);
}

void loop() {
  if (Serial.available())
  {
    char in = Serial.read();
    if(!(in == '\n' || in == '|r'))
    {
      if(in == ';')
      {
        parseStart = 1;
      }
      if(in =='#')
      {
        parseStart = 2;
      }
      if((parseStart == 2) && (in != '#'))
      {
        msg += in;
      }
    }
  }
  if(parseStart ==1)
  {
    int message = msg.toInt();
    if (message < 85)
    {
      message = 85;
    }
    if(message > 800)
    {
      message = 800;
    }
    message = map(message , 85, 800, 0 , 255);
    for (int led = 0;led< LENGTH_LED; led++)
    {
      leds[led] = CHSV(180,255, message);
    }
    FastLED.show();
    parseStart = 0;
    msg = "";
  }
}
