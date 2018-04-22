#include "SevSeg.h"
#include <Wire.h>
#include "RTClib.h"
#include "EEPROM.h"
#include "EEPROMAnything.h"

SevSeg sevseg;
RTC_DS3231 rtc;
int buttonPin = A1;
DateTime markTime;
TimeSpan recordTime;
TimeSpan showTime;
float pressLength = 0;
float showLength = 1000;

void setup() {
  //serial for debugging
  //Serial.begin(9600);

  // sevseg vars //
  byte numDigishowTime = 4;
  byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {6, 5, 2, 3, 4, 7, 8, 9};
  bool resistorsOnSegmenshowTime = true;
  bool updateWithDelaysIn = true;
  byte hardwareConfig = COMMON_ANODE;
  sevseg.begin(hardwareConfig, numDigishowTime, digitPins, segmentPins, resistorsOnSegmenshowTime);
  sevseg.setBrightness(90);

  // rtc shit //
  rtc.begin();
  //if you lose the pila sets the time
  //yyyy, m, d, hh, mm, ss
  //rtc.adjust(DateTime(2018, 4, 21, 13, 40, 30));

  //buton
  pinMode(buttonPin, INPUT_PULLUP);


  //EEPROM things
  EEPROM_readAnything(0, markTime);
  EEPROM_readAnything(1, recordTime);


}

void loop() {
  //the time is now
  DateTime now = rtc.now();

  showTime = now - markTime;

  //testing
  //Serial.print("start");
  //Serial.println();
  //Serial.print(showTime.hours(), DEC);
  //Serial.print(":");
  //Serial.print(showTime.minutes(), DEC);
  //Serial.print(":");
  //Serial.print(showTime.seconds(), DEC);
  //Serial.println();
  //Serial.print(markTime.hour(), DEC);
  //Serial.print(':');
  //Serial.print(markTime.minute(), DEC);
  //Serial.print(':');
  //Serial.print(markTime.second(), DEC);
  //Serial.println();
  //Serial.print(now.hour(), DEC);
  //Serial.print(':');
  //Serial.print(now.minute(), DEC);
  //Serial.print(':');
  //Serial.print(now.second(), DEC);
  //Serial.println();
  //Serial.print(recordTime.hours(), DEC);
  //Serial.print(':');
  //Serial.print(recordTime.minutes(), DEC);
  //Serial.print(':');
  //Serial.print(recordTime.seconds(), DEC);
  //Serial.println();
  //Serial.println(boolString[now < markTime]);

  //displays info in 7seg, less than a day shows hours and minutes. More than a day shows days and hours
  if (showTime.days() < 1) {
    sevseg.setNumber(100 * showTime.hours() + showTime.minutes(), 4);
  }
  else {
    sevseg.setNumber(100 * showTime.days() + showTime.hours(), 4);
  }
  
  //meassure press button length
  //while (digitalRead(buttonPin) == LOW ){
  //  delay(100);
  //  pressLength = pressLength + 100;
  //}

  //if button push log that
  //if (pressLength < showLength) {
  if (digitalRead(buttonPin) == LOW) {
    markTime = rtc.now();
    EEPROM_writeAnything(0, markTime);
    sevseg.blank();
    
    //if showTime is the new record, save that
    if (showTime > recordTime) {
      EEPROM_writeAnything(1, recordTime);
    }
  }

  //if (pressLength > showLength) {
  //   sevseg.setNumber(100*recordTime.hours()+recordTime.minutes(),4);
  // }

  sevseg.refreshDisplay();
  //delay(1000);

}

