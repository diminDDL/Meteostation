//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded System Lab (ESL), KMUTNB, Thailand
// File: Four7Seg74hc595.h
// Last Modified: 2014-06-23
// Note:
//   A C++ class for Arduino that can be used to drive
//   a four-digit 74HC595-based 7-Segment display module.
//////////////////////////////////////////////////////////////////////////

#ifndef __FOUR7SEG_74HC595_H
#define __FOUR7SEG_74HC595_H

#include <Arduino.h>

namespace esl {
  class Four7Seg74hc595 {
     public:
       Four7Seg74hc595( int sclk, int rclk, int dio, boolean invert=true );
       void setDigits( const char *str, uint8_t len=4 );
       void update();

     private:
       int sclk_pin, rclk_pin, dio_pin;
       uint8_t invert_mask;
       uint8_t raw_digits[ 4 ];
       static const uint8_t DIGITS_7SEG[];
       static const uint8_t num_digits=4;
       uint16_t delay_usec;
  }; // end class
} // end namespace

#endif // __FOUR7SEG_74HC595_H

//////////////////////////////////////////////////////////////////////////


