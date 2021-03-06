#include <Arduino.h>
// bit patterns for the CP437 font

const byte mini_font [][8] PROGMEM = {
  { B11100000,
    B10100000,
    B10100000,
    B10100000,
    B11100000,
    0x00, 0x00, 0x00 }, //0

  { B01000000,
    B01000000,
    B01000000,
    B01000000,
    B01000000,
    0x00, 0x00, 0x00 }, //1

  { B11100000,
    B00100000,
    B11100000,
    B10000000,
    B11100000,
    0x00, 0x00, 0x00 }, //2

  { B11100000,
    B00100000,
    B01100000,
    B00100000,
    B11100000,
    0x00, 0x00, 0x00 }, //3

  { B10100000,
    B10100000,
    B11100000,
    B00100000,
    B00100000,
    0x00, 0x00, 0x00 }, //4

  { B11100000,
    B10000000,
    B11100000,
    B00100000,
    B11100000,
    0x00, 0x00, 0x00 }, //5

  { B11100000,
    B10000000,
    B11100000,
    B10100000,
    B11100000,
    0x00, 0x00, 0x00 }, //6

  { B11100000,
    B00100000,
    B00100000,
    B01000000,
    B01000000,
    0x00, 0x00, 0x00 }, //7

  { B11100000,
    B10100000,
    B11100000,
    B10100000,
    B11100000,
    0x00, 0x00, 0x00 }, //8

  { B11100000,
    B10100000,
    B11100000,
    B00100000,
    B11100000,
    0x00, 0x00, 0x00 }, //9

  { B00000000,
    B10000000,
    B00000000,
    B10000000,
    B00000000,
    0x00, 0x00, 0x00 }, //:

};

/*
#include <Arduino.h>
// bit patterns for the CP437 font

const byte mini_font [][8] PROGMEM = {
  { B01000000,
    B10100000,
    B10100000,
    B10100000,
    B01000000,
    0x00, 0x00, 0x00 }, //0

  { B01000000,
    B01000000,
    B01000000,
    B01000000,
    B01000000,
    0x00, 0x00, 0x00 }, //1

  { B11000000,
    B00100000,
    B01000000,
    B10000000,
    B01100000,
    0x00, 0x00, 0x00 }, //2

  { B11000000,
    B00100000,
    B01100000,
    B00100000,
    B11000000,
    0x00, 0x00, 0x00 }, //3

  { B10100000,
    B10100000,
    B11100000,
    B00100000,
    B00100000,
    0x00, 0x00, 0x00 }, //4

  { B11100000,
    B10000000,
    B11000000,
    B00100000,
    B11000000,
    0x00, 0x00, 0x00 }, //5

  { B01000000,
    B10000000,
    B11000000,
    B10100000,
    B01000000,
    0x00, 0x00, 0x00 }, //6

  { B11100000,
    B00100000,
    B00100000,
    B01000000,
    B01000000,
    0x00, 0x00, 0x00 }, //7

  { B01000000,
    B10100000,
    B01000000,
    B10100000,
    B01000000,
    0x00, 0x00, 0x00 }, //8

  { B01000000,
    B10100000,
    B01100000,
    B00100000,
    B01000000,
    0x00, 0x00, 0x00 }, //9

  { B00000000,
    B01000000,
    B00000000,
    B01000000,
    B00000000,
    0x00, 0x00, 0x00 }, //:

};

*/

