#include <HCSR04.h>
#define buzzer 33
#define ON LOW
#define OFF HIGH

unsigned int buzzerOnPeriod = 100; //milliseconds
unsigned int buzzerOffPeriodMax = 1000; //milliseconds
unsigned int buzzerOffPeriodMin = 50; //milliseconds
unsigned long now = 0;
unsigned long lastOffTime = 0;
unsigned long nextOnTime = 0;
unsigned long nextOffTime = 0;

unsigned int maxDetectionRange = 250; //centimeters
unsigned int minDetectionRange = 5; //centimeters
int notAvailable = -1;

byte triggerPin = 13;
byte echoCount = 2;
byte* echoPins = new byte[echoCount] { 12, 14 };

void setup () {
  Serial.begin(9600);
  pinMode(33, OUTPUT);
  digitalWrite(buzzer, OFF);
  HCSR04.begin(triggerPin, echoPins, echoCount);
}

void loop () {
  //Reading distances
  double* distances = HCSR04.measureDistanceCm();
  Serial.print(distances[0]);
  Serial.print("    ");
  Serial.println(distances[1]);

  //Select smaller distances to use
  unsigned int tmpDis = (distances[0] < distances[1]) ? distances[0] : distances[1];

  //If distances not in detection range
  if (tmpDis == notAvailable || tmpDis < minDetectionRange || tmpDis > maxDetectionRange)
  {
    Serial.println("Not in detection, turn off buzzer");
    digitalWrite(buzzer, OFF);
  }

  //If distances in detection range
  else
  {
    //Mapping current distances to OFF period.
    //The bigger OFF period then buzzer beeps with low frequency.
    //The smaller OFF period then buzzer beeps with high frequency.
    //if ( tmpDis > 50 && tmpDis < 200 )
    //  OnOffBlink(2000, buzzerOnPeriod);
    if ( tmpDis < 50 )
      OnOffBlink(50, buzzerOnPeriod);
    else
      digitalWrite(buzzer,OFF);
  }
}

void OnOffBlink(int tOn, int tOff) {

  static int timer = tOn;
  static long previousMillis;

  if ((millis() - previousMillis) >= timer) {

    if (digitalRead(buzzer) == HIGH) {
      timer = tOff;
    } else {
      timer = tOn;
    }

    digitalWrite(buzzer, !digitalRead(buzzer));
    previousMillis = millis();
  }
}
