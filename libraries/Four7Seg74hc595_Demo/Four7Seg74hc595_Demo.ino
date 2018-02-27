//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded System Lab (ESL), KMUTNB, Thailand
// File: Four7Seg74hc595.cpp
// Last Modified: 2014-06-23
// Description: This Arduino sketch shows how to use the
//   Four7Seg74hc595 class to interface with the 4-digit 7-segment
//   display module with dual 74hc595 shift regsiters.
//   It shows the current value of a four-digit counter which counts up
//   every 1 second.
//////////////////////////////////////////////////////////////////////////

#include "Four7Seg74hc595.h"

esl::Four7Seg74hc595 display( 5,6,7 );

char sbuf[5];
uint16_t count;
uint32_t ts1, ts2;

void setup() {
  for (uint8_t i=0; i < 100; i++) {
    display.setDigits( "----", 4 );
    display.update();
    delay(10);
  }
  delay(1000);
  count = 0;
  sprintf( sbuf, "%04u", count );
  display.setDigits( sbuf, 4 );
  display.update();
  ts1 = ts2 = millis();
}

uint32_t ts;

void loop() {
  ts = millis();
  if ( ts - ts1 >= 1000 ) {  
     count = (count+1) % 10000;
     sprintf( sbuf, "%04u", count );
     ts1 += 1000; // increment counter by 1 every 1sec
  }
  if ( ts - ts2 >= 5 ) {
     display.setDigits( sbuf, 4 );
     display.update();
     ts2 += 5; // display-update interval = 5msec
  }
}
////////////////////////////////////////////////////////////////////////////

