#include <Keypad.h>
int blueS = 0;
int redS = 0;
String answer;
String input;
char key;
String choice;
bool guardingOnA = true;
bool guardingOnB = false;
bool guardingOff = false;

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
#define blueSensor 19
#define redSensor 17
#define red 2
#define yellow 3
#define buzzer 13
#define green 4

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(blueSensor, INPUT);
  pinMode(redSensor, INPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int i = 0;
  while (guardingOnA)
  {
    while (i < 1)
    {
      alarmOn_A();
      i++;
    }
    blueS = digitalRead(blueSensor);
    redS = digitalRead(redSensor);
    sensorStatus();
    sendPinCode();
    answer = read();
    if (answer.equals("siren"))
      siren();
    if (answer.equals("off"))
      guardingOnA = false;
    guardingOff = true;
    guardingOnB = false;
  }
  while (guardingOnB)
  {
    while (i < 1)
    {
      alarmOn_B();
      i++;
    }
    blueS = digitalRead(blueSensor);
    sensorStatus();
    sendPinCode();
    answer = read();
    if (answer.equals("siren"))
      siren();
    else if (answer.equals("off"))
      guardingOnA = false;
  }
  while (guardingOff)
  {
    usersChoice();
    answer = read();
    if (answer.equals("siren"))
      siren();
    else if (answer.equals("on") && choice == 'A')
      guardingOnA = true;
    else if (answer.equals("on") && choice == 'B')
      guardingOnB = true;
  }
}

void alarmOn_B()
{
  tone(buzzer, 700);
  delay(200);
  pinMode(buzzer, LOW);
  delay(200);
  tone(buzzer, 700);
  delay(200);
  pinMode(buzzer, LOW);
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

void usersChoice()
{
  key = kpd.getKey();
  if (key != NO_KEY)
  {
    if (key == 'A' || key == 'B' || key == '0' || key == '1' || key == '2' || key == '3' || key == '4' ||
        key == '5' || key == '6' || key == '7' || key == '8' || key == '9')
    {
      choice += key;
      keyBuzz(key);
    }
    if (choice.length() == 5 || key == '#')
    {
      Serial.print(choice);
      choice = "";
    }

  }
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
  }
  if (input.length() == 4 || key == '#')
  {
    Serial.print(input);
    input = "";
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
}
void sensorStatus()
{
  if (blueS == 1)
  {
    Serial.write("b");
    siren();
  }
  if (redS == 1)
  {
    Serial.write("r");
    siren();
  }
}
void siren()
{
  bool on = true;
  int i;
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
