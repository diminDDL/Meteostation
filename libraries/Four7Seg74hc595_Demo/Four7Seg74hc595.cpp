//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded System Lab (ESL), KMUTNB, Thailand
// File: Four7Seg74hc595.cpp
// Last Modified: 2014-06-23
//////////////////////////////////////////////////////////////////////////
#include "Four7Seg74hc595.h"

using namespace esl;

Four7Seg74hc595::Four7Seg74hc595( int sclk, int rclk, int dio, boolean invert )
   : sclk_pin(sclk), rclk_pin(rclk), dio_pin(dio)
{
   pinMode( sclk_pin, OUTPUT );
   pinMode( rclk_pin, OUTPUT );
   pinMode( dio_pin, OUTPUT );
   digitalWrite( sclk_pin, LOW );
   digitalWrite( rclk_pin, LOW );
   invert_mask = (invert) ? 0xff : 0x00;
   delay_usec = 500;
}

void Four7Seg74hc595::setDigits( const char *str, uint8_t len ) {
  char ch;
  uint8_t digit;
  if ( len == 0 ) {
    len = strlen(str);
  }
  if ( len > num_digits ) {
    len = num_digits;
  }
  for ( uint8_t i=0; i < len; i++ ) {
     ch = tolower(str[i]);
     if ('0' <= ch && ch <= '9') {
       digit = (ch - '0');
       raw_digits[i] = DIGITS_7SEG[digit];
     }
     else if ('a' <= ch && ch <= 'f' ) {
       digit = (ch - 'a') + 10;
       raw_digits[i] = DIGITS_7SEG[digit];
     }
     else {
       switch (str[i]) {
         case 'r': raw_digits[i] = 0b01010000; break;
         case 'h': raw_digits[i] = 0b01110100; break;
         case 'H': raw_digits[i] = 0b01110110; break;
         case 'L': raw_digits[i] = 0b00111000; break;
         case '-': raw_digits[i] = 0b01000000; break;
         default: raw_digits[i] = 0; break; // ignored (off)
       }
     }
  }
}

void Four7Seg74hc595::update() {
  uint8_t value, max_index = num_digits-1;
  for ( uint8_t i=0; i < num_digits; i++ ) {
     value = raw_digits[ max_index-i ] ^ invert_mask;
     shiftOut( dio_pin, sclk_pin, MSBFIRST, value ); // seven segments + dot (common-anode type)
     shiftOut( dio_pin, sclk_pin, MSBFIRST, (1 << i) ); // select digit
     digitalWrite( rclk_pin, HIGH );
     digitalWrite( rclk_pin, LOW );
     delayMicroseconds( delay_usec );
  }
  shiftOut( dio_pin, sclk_pin, MSBFIRST, 0xff ); // seven segments + dot (common-anode type)
  shiftOut( dio_pin, sclk_pin, MSBFIRST, 0 ); // select digit
  digitalWrite( rclk_pin, HIGH );
  digitalWrite( rclk_pin, LOW );
}

const uint8_t Four7Seg74hc595::DIGITS_7SEG[] = {
 //  gfedcba
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111, // 9
  0b01110111, // A
  0b01111100, // b
  0b00111001, // C
  0b01011110, // d
  0b01111001, // E
  0b11110001  // F
};

//////////////////////////////////////////////////////////////////////////

