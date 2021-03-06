#include <Keypad.h>
#include "Metro.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

int blueS;
int redS;
String answer;
String input;
char key;
bool guardingOnA = true;
bool guardingOnB;
bool guardingOff;
Metro blinkRed = Metro(200);
Metro blinkGreen = Metro(1000);
Metro timeElapsed = Metro(3000);
bool redOn = false;
bool greenOn = false;
bool yellowOn = false;
bool blueOk = true;
bool redOk = true;

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = {12, 11, 10, 9};
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = {8, 7, 6, 5};
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
#define blueSensor 14
#define redSensor 15
#define red 2
#define yellow 3
#define buzzer 13
#define green 4

void setup()
{
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("LOADING SYSTEM");

  pinMode(buzzer, OUTPUT);
  pinMode(blueSensor, INPUT);
  pinMode(redSensor, INPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  blueOk = true;
  redOk = true;
  while (guardingOnA)
  {
    checkActiveSensors();
    if (digitalRead(blueSensor) == HIGH && digitalRead(redSensor) == HIGH)
      introGreenA();
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sensor faulty...");
    }
    lcd.clear();
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    while (guardingOnA)
    {
      
      lcd.setCursor(0, 0);
      lcd.print("Guarding A mode");
      blueS = digitalRead(blueSensor);
      redS = digitalRead(redSensor);
      sensorStatus();
      sendPinCode();
      answer = read();
      if (answer.equals("siren"))
        siren();
      if (answer.equals("off"))
      {
        guardingOff = true;
        guardingOnB = false;
        guardingOnA = false;
        break;
      }
    }
  }
  while (guardingOnB)
  {

    checkActiveSensors();
    if (digitalRead(blueSensor) == HIGH && digitalRead(redSensor) == HIGH)
      introGreenB();
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sensor faulty");
    }

    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
    lcd.clear();
    while (guardingOnB)
    {
      if (blinkGreen.check())
      {
        greenOn = !greenOn;
        digitalWrite(green, greenOn);
      }
      lcd.setCursor(0, 0);
      lcd.print("Guarding B mode");
      blueS = digitalRead(blueSensor);
      sensorStatus_B();
      sendPinCode();
      answer = read();
      if (answer.equals("siren"))
        siren();
      else if (answer.equals("off"))
      {
        guardingOnA = false;
        guardingOnB = false;
        guardingOff = true;
        break;
      }
    }
  }
  int i = 0;
  while (guardingOff)
  {
    while (i < 1)
    {
      lcd.clear();
      alarmOff();
      i++;
    }
    
    lcd.setCursor(0, 0);
    lcd.print("Guarding Off");
    String choice = usersChoice();
    while (choice == "A" || choice == "B")
    {
      sendPinCode();
      answer = read();
      if (answer.length() > 0)
      {
        if (answer.equals("siren"))
          siren();
        else if (choice.equals("A") && answer.equals("on"))
        {
          guardingOnA = true;
          guardingOff = false;
          break;
        }
        else if (choice.equals("B") && answer.equals("on"))
        {
          guardingOnB = true;
          guardingOff = false;
          break;
        }
      }
    }
  }
  i = 0;
}

void checkActiveSensors()
{
  if (digitalRead(blueSensor) == LOW || digitalRead(redSensor) == LOW)
  {
    if (digitalRead(blueSensor) == LOW)
      blueOk = false;
    if (digitalRead(redSensor) == LOW)
      redOk = false;

    introRed();
  }
}
void introRed()
{
  Metro tim = Metro(22000);
  while (tim.check())
  {
    if (blinkRed.check())
    {
      redOn = !redOn;
      digitalWrite(red, redOn);
      tone(buzzer, 700, 50);
    }
  }
  digitalWrite(yellow, HIGH);
}

void introGreenA()
{
  if (digitalRead(blueSensor) == HIGH || digitalRead(redSensor) == HIGH)
  {
    for (int i = 0; i < 3; i++)
    {
      tone(buzzer, 700, 500);
      delay(750);
    }
  }
}
void introGreenB()
{
  if (digitalRead(blueSensor) == HIGH)
  {
    for (int i = 0; i < 2; i++)
    {
      tone(buzzer, 700, 200);
      delay(450);
    }
  }
}
void alarmOff()
{
  tone(buzzer, 700);
  delay(200);
  pinMode(buzzer, LOW);
  delay(200);
  tone(buzzer, 700);
  delay(200);
  pinMode(buzzer, LOW);
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
}
void alarmOn_A()
{
  tone(buzzer, 700);
  delay(500);
  pinMode(buzzer, LOW);
  delay(200);
  tone(buzzer, 700);
  delay(500);
  pinMode(buzzer, LOW);
  delay(200);
  tone(buzzer, 700);
  delay(500);
  pinMode(buzzer, LOW);
  delay(200);
  digitalWrite(green, HIGH);
  digitalWrite(yellow, HIGH);
}

String usersChoice()
{
  String z;
  key = kpd.getKey();
  if (key != NO_KEY)
    if (key == 'A' || key == 'B')
    {
      z = key;
      keyBuzz(key);
    }
  return z;
}

void sendPinCode()
{
  key = kpd.getKey();
  if (key != NO_KEY)
  {
    if (key == '0' || key == '1' || key == '2' || key == '3' || key == '4' ||
        key == '5' || key == '6' || key == '7' || key == '8' || key == '9')
    {
      input += key;
      keyBuzz(key);
    }
    else
      keyBuzz(key);
  }
  if (key == '#')
  {
    if (input.length() == 4)
    {
      Serial.print(input);
      Serial.flush();
      input.remove(0);
    }
    else
      input.remove(0);
  }
}

String read()
{
  if (Serial.available() > 0)
  {
    //read the whole string until '\n' delimiter is read
    String answer = Serial.readStringUntil('\n');
    return answer;
  }
  else
    return "";
}
void sensorStatus()
{
  if (blueS == 0 && blueOk == true)
  {
    Serial.write("b");
    Serial.flush();
    siren();
  }
  if (redS == 0 && redOk == true)
  {
    Serial.write("r");
    Serial.flush();
    siren();
  }
}
void sensorStatus_B()
{
  if (blueS == 0 && blueOk == true)
  {
    Serial.write("b");
    Serial.flush();
    siren();
  }
}
void siren()
{
  bool on = true;
  int i;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("* * *ALARM* * *");
  while (on)
  {
    for (i = 700; i < 800; i++)
    {
      tone(buzzer, i);
      digitalWrite(yellow, HIGH);
      digitalWrite(red, HIGH);
      digitalWrite(green, HIGH);
      delay(15);
    }
    for (i = 800; i > 700; i--)
    {
      tone(buzzer, i);
      digitalWrite(yellow, LOW);
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);
      delay(15);
    }
  }
}
void keyBuzz(char key)
{
  if (key) // Check for a valid key.
  {
    switch (key)
    {
      case '1':
        tone(buzzer, 261);
        delay(100);
        pinMode(buzzer, LOW);
        break;
      case '2':
        tone(buzzer, 294);
        delay(100);
        pinMode(buzzer, LOW);
        break;
      case '3':
        tone(buzzer, 329);
        delay(100);
        pinMode(buzzer, LOW);
        break;
      case '4':
        tone(buzzer, 349);
        delay(100);
        pinMode(buzzer, LOW);
        break;
      case '5':
        tone(buzzer, 392);
        delay(100);
        pinMode(buzzer, LOW);
        break;
      case '6':
        tone(buzzer, 440);
        delay(100);
        pinMode(buzzer, LOW);
        break;
      case '7':
        tone(buzzer, 493);
        delay(100);
        pinMode(buzzer, LOW);
        break;
      case '8':
        tone(buzzer, 523);
        delay(100);
        pinMode(buzzer, LOW);
        break;
      case '9':
        tone(buzzer, 588);
        delay(100);
        pinMode(buzzer, LOW);
        break;
      case '0':
        tone(buzzer, 660);
        delay(100);
        pinMode(buzzer, LOW);
        break;
      default:
        tone(buzzer, 660);
        delay(100);
        pinMode(buzzer, LOW);
    }
  }
}
