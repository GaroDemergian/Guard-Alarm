#include <Keypad.h>
String answer;
String input;
char key;
bool activeAlarm = true;
String msg;
int i = 0;
bool sirenOn = false;

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
byte rowPins[ROWS] = { 9, 10, 11, 12 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 5, 6, 7, 8 };

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
#define sensor 0
#define Green 4
#define Yellow 3
#define Red 2
#define buzzer 13

void setup()
{
  pinMode(sensor, INPUT)
  pinMode(buzzer, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Yellow, OUTPUT);
  pinMode(Red, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  while (activeAlarm)
  {
    answer = read();
    if (answer.equals("on"))
      alarmOn_A();
    else if (answer.equals("off"))
      alarmOff();
    else if (answer.equals("siren"))
      {
        sirenOn = true;
        siren();
      }  
    sendPinCode();
  }
}
void siren() {
 while(sirenOn)
 {
   for(i=700;i<800;i++){
  tone(buzzer,i);
  digitalWrite(Green, HIGH);
  digitalWrite(Yellow, HIGH);
  digitalWrite(Red, HIGH);
  delay(15);
  }
  for(i=800;i>700;i--){
  tone(buzzer,i);
  digitalWrite(Green, LOW);
  digitalWrite(Yellow, LOW);
  digitalWrite(Red, LOW);
  delay(15);
  }
 }
}
char readMode()
{
  char mode;
  key = kpd.getKey();
  if (key != NO_KEY)
    if (key == 'a' || key == 'b')
      return mode;
}

void sendPinCode()
{
  key = kpd.getKey();
  if (key != NO_KEY) {
    if (key == '0' || key == '1' || key == '2' || key == '3' || key == '4' ||
        key == '5' || key == '6' || key == '7' || key == '8' || key == '9' ) {
      input += key;
      keyBuzz(key);
    }
  }
  int tries = 0;
  if (input.length() == 4 || key == '#' && tries < 3)
  {
    Serial.print(input);
    input = "";
    tries++;
  }
}
String read()
{
  if (Serial.available() > 0) {
    //read the whole string until '\n' delimiter is read
    String answer = Serial.readStringUntil('\n');
    return answer;
  }
}
void keyBuzz(char key)
{
  if (key) // Check for a valid key.
  {
    switch (key) {
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
        Serial.println(key);
    }
  }
}

void alarmOff()
{
 
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
  digitalWrite(Green, HIGH);
  digitalWrite(Yellow, HIGH);
}

bool sensorStatus()
{
  if (digitalRead(sensor) == 1)
  {
  Serial.print("1");
  return true;
  }
  else
  {
  Serial.print("0");
  return false;
  }
}
