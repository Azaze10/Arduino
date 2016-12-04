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

void colorChange(int Pin, int start, int stop, int step)
{
		if !mode{return 0;}
		int i;
		if (step>0){
			for (i = start; i < stop; i = i+step){
			if !mode{return 0;}
				analogWrite(Pin, i);
				delay(FADESPEED);
			} 
		else if(step<0){
			for (i = start; i > stop; i = i+step){ 
			if !mode{return 0;}
				analogWrite(Pin, i);
				delay(FADESPEED);
			}	
		}
}

void rainbow (){
	colorChange(redPin, 0, 256, 1);
	colorChange(bluePin, 255, 0, -1);
	colorChange(greenPin, 0, 256, 1);
	colorChange(redPin, 255, 0, -1);
	colorChange(bluePin, 0, 256, 1);
	colorChange(greenPin, 255, 0, -1);
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
