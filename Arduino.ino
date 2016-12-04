#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SimpleTimer.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

char auth[] = "3118b38de7c7430baf2d369d487d9f03";

#define EspSerial Serial
#define ESP8266_BAUD 115200
#define Pin 7
#define redPin 9
#define greenPin 10
#define bluePin 11
#define FADESPEED 2
int mode ;

const int buttonPin = 5;
const int relePin = 7;
int buttonState = 0;

OneWire ourWire(Pin); 
DallasTemperature sensors(&ourWire);
SimpleTimer timer;
ESP8266 wifi(&EspSerial);

void setup()
{
  mode = 0;
  pinMode(relePin,OUTPUT);
  pinMode(buttonPin,INPUT);
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  digitalWrite(greenPin,LOW);
  digitalWrite(bluePin,LOW);
  digitalWrite(redPin,LOW);
  
  Serial.begin(115200);
  delay(10);
  EspSerial.begin(115200);
  delay(10);
  sensors.begin();
  delay(10);
  timer.setInterval(2000L, leeTEMP);
  delay(10);
  Blynk.begin(auth, wifi, "iSeven", "$$$$$$$$");
}

BLYNK_WRITE(1) 
{
 // Change state on button press 
 if( param.asInt() == 1 ) {
  digitalWrite(greenPin,LOW);
  digitalWrite(bluePin,LOW);
  digitalWrite(redPin,LOW);
  mode = 1 ; 
 } else {
  mode = 0 ; 
 } 
}

void buttonCheck()


void colorChange() 
{
  int r, g, b;
   
  // fade от голубого к фиолетовому
  for (r = 0; r < 256; r++) { 
    analogWrite(redPin, r);
    delay(FADESPEED);
  } 
  // fade от фиолетового к красному
  for (b = 255; b > 0; b--) { 
    analogWrite(bluePin, b);
    delay(FADESPEED);
  } 
  // fade от красного к желтому
  for (g = 0; g < 256; g++) { 
    analogWrite(greenPin, g);
    delay(FADESPEED);
  } 
  // fade от желтого к зеленому
  for (r = 255; r > 0; r--) { 
    analogWrite(redPin, r);
    delay(FADESPEED);
  } 
  // fade от зеленого к зеленовато-голубому
  for (b = 0; b < 256; b++) { 
    analogWrite(bluePin, b);
    delay(FADESPEED);
  } 
  // fade от зеленовато-голубого к голубому
  for (g = 255; g > 0; g--) { 
    analogWrite(greenPin, g);
    delay(FADESPEED);
  } 
}

void leeTEMP()
{
  sensors.requestTemperatures();
  Blynk.virtualWrite(0, sensors.getTempCByIndex(0)); 
}

void loop()
{
  Blynk.run();
  timer.run();
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH)
  {
    digitalWrite(relePin,HIGH);
  }
  else
  {
    digitalWrite(relePin,LOW);
  }
  switch( mode ) {
  case 1:
  colorChange();
  break;
  case 0:
  digitalWrite(greenPin,LOW);
  digitalWrite(bluePin,LOW);
  digitalWrite(redPin,LOW);
  break;
  }
}
