#include "SevSeg.h"
#include <Wire.h>
#include "RTClib.h"

SevSeg sevseg; 
RTC_DS3231 rtc;
int buttonPin = A1;
int mark = 0;
/*    
 7-seg 4 digit model A-564SR
     A
   *****
 F *   *
   * G * B
   *****
 E *   * 
   *   * C
   *****
     D
  
  A -> 11
  B -> 7
  C -> 4
  D -> 2
  E -> 1
  F -> 10
  G -> 5
  P -> 3
  
  
  1 -> 12
  2 -> 9
  3 -> 8
  4 -> 6
  digitPins from 1 to 4
  segmentPins from A to P
  */
 
void setup(){
  //serial for debugging
  Serial.begin(9600);

  // sevseg vars //
  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {6, 5, 2, 3, 4, 7, 8, 9};
  bool resistorsOnSegments = true; 
  bool updateWithDelaysIn = true;
  byte hardwareConfig = COMMON_ANODE; 
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);

  // rtc shit //
  rtc.begin();
  //if you lose the pila you set the time
  //yyyy, m, d, hh, mm, ss
  rtc.adjust(DateTime(2017, 4, 19, 00, 54, 0));

  //buton
  pinMode(buttonPin, INPUT_PULLUP);


}

void loop(){
    //time is now
    DateTime now = rtc.now();
        
    //if button push log that else display now-mark
    if (digitalRead(buttonPin) == LOW) {
      mark = now.second();
    }
    
    else {
      //sevseg.setNumber(100*(now.second()-mark)+mark,4);
      Serial.print(now.second()-mark, DEC);
      Serial.print(' ');
      Serial.print(mark, DEC);
      Serial.println();
    }
    delay(1000);
    
     //sevseg.refreshDisplay();
    
}

