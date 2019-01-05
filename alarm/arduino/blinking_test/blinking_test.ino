#include "Metro.h"

#define red 2
#define yellow 3
#define green 4
#define buzz 13

bool redOn = false;
bool greenOn = false;
bool yellowOn = false;
bool buzzOn = HIGH;

Metro blinkRed = Metro(200);
Metro blinkGreen = Metro(1000);
Metro timeElapsed = Metro(3000);
Metro buzztone = Metro(200);

void setup()
{
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
}

void loop()
{
  someBlinking();


}


void someBlinking()
{
  if (blinkRed.check())
  {
    redOn = !redOn;
    digitalWrite(red, redOn);
  }
  if (timeElapsed.check())
    digitalWrite(yellow, HIGH);
  if(buzztone.check())
  {
    //tone(buzz, 700, 50);
  }
  
}
