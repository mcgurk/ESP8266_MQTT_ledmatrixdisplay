//https://github.com/LeifBloomquist/MicroView/tree/master/Fonts/C64Font

/*
//------------------------------------------------------------------------------
// File generated by LCD Assistant
// http://en.radzio.dxp.pl/bitmap_converter/
//------------------------------------------------------------------------------

#ifndef C64FONT_UPPER_H
#define C64FONT_UPPER_H
//#include <avr/pgmspace.h>
static const unsigned char C64FontUpper [] PROGMEM = {
// first row defines - FONTWIDTH, FONTHEIGHT, ASCII START CHAR, TOTAL CHARACTERS, FONT MAP WIDTH HIGH, FONT MAP WIDTH LOW (2,56 meaning 256)
    //8,8,0,255,2,56,

0x00, 0x3E, 0x7F, 0x41, 0x4D, 0x4F, 0x2E, 0x00, 0x00, 0x7C, 0x7E, 0x0B, 0x0B, 0x7E, 0x7C, 0x00,
0x00, 0x7F, 0x7F, 0x49, 0x49, 0x7F, 0x36, 0x00, 0x00, 0x3E, 0x7F, 0x41, 0x41, 0x63, 0x22, 0x00,
0x00, 0x7F, 0x7F, 0x41, 0x63, 0x3E, 0x1C, 0x00, 0x00, 0x7F, 0x7F, 0x49, 0x49, 0x41, 0x41, 0x00,
0x00, 0x7F, 0x7F, 0x09, 0x09, 0x01, 0x01, 0x00, 0x00, 0x3E, 0x7F, 0x41, 0x49, 0x7B, 0x3A, 0x00,
0x00, 0x7F, 0x7F, 0x08, 0x08, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x41, 0x7F, 0x7F, 0x41, 0x00, 0x00,
0x00, 0x20, 0x60, 0x41, 0x7F, 0x3F, 0x01, 0x00, 0x00, 0x7F, 0x7F, 0x1C, 0x36, 0x63, 0x41, 0x00,
0x00, 0x7F, 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x7F, 0x7F, 0x06, 0x0C, 0x06, 0x7F, 0x7F,
0x00, 0x7F, 0x7F, 0x0E, 0x1C, 0x7F, 0x7F, 0x00, 0x00, 0x3E, 0x7F, 0x41, 0x41, 0x7F, 0x3E, 0x00,
0x00, 0x7F, 0x7F, 0x09, 0x09, 0x0F, 0x06, 0x00, 0x00, 0x1E, 0x3F, 0x21, 0x61, 0x7F, 0x5E, 0x00,
0x00, 0x7F, 0x7F, 0x19, 0x39, 0x6F, 0x46, 0x00, 0x00, 0x26, 0x6F, 0x49, 0x49, 0x7B, 0x32, 0x00,
0x00, 0x01, 0x01, 0x7F, 0x7F, 0x01, 0x01, 0x00, 0x00, 0x3F, 0x7F, 0x40, 0x40, 0x7F, 0x3F, 0x00,
0x00, 0x1F, 0x3F, 0x60, 0x60, 0x3F, 0x1F, 0x00, 0x00, 0x7F, 0x7F, 0x30, 0x18, 0x30, 0x7F, 0x7F,
0x00, 0x63, 0x77, 0x1C, 0x1C, 0x77, 0x63, 0x00, 0x00, 0x07, 0x0F, 0x78, 0x78, 0x0F, 0x07, 0x00,
0x00, 0x61, 0x71, 0x59, 0x4D, 0x47, 0x43, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x41, 0x41, 0x00, 0x00,
0x40, 0x68, 0x7C, 0x5E, 0x49, 0x49, 0x22, 0x00, 0x00, 0x00, 0x41, 0x41, 0x7F, 0x7F, 0x00, 0x00,
0x00, 0x08, 0x0C, 0xFE, 0xFE, 0x0C, 0x08, 0x00, 0x00, 0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4F, 0x4F, 0x00, 0x00, 0x00,
0x00, 0x07, 0x07, 0x00, 0x00, 0x07, 0x07, 0x00, 0x14, 0x7F, 0x7F, 0x14, 0x14, 0x7F, 0x7F, 0x14,
0x00, 0x24, 0x2E, 0x6B, 0x6B, 0x3A, 0x12, 0x00, 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00,
0x00, 0x32, 0x7F, 0x4D, 0x4D, 0x77, 0x72, 0x50, 0x00, 0x00, 0x00, 0x04, 0x06, 0x03, 0x01, 0x00,
0x00, 0x00, 0x1C, 0x3E, 0x63, 0x41, 0x00, 0x00, 0x00, 0x00, 0x41, 0x63, 0x3E, 0x1C, 0x00, 0x00,
0x08, 0x2A, 0x3E, 0x1C, 0x1C, 0x3E, 0x2A, 0x08, 0x00, 0x08, 0x08, 0x3E, 0x3E, 0x08, 0x08, 0x00,
0x00, 0x00, 0x80, 0xE0, 0x60, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00,
0x00, 0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x40, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02,
0x00, 0x3E, 0x7F, 0x49, 0x45, 0x7F, 0x3E, 0x00, 0x00, 0x40, 0x44, 0x7F, 0x7F, 0x40, 0x40, 0x00,
0x00, 0x62, 0x73, 0x51, 0x49, 0x4F, 0x46, 0x00, 0x00, 0x22, 0x63, 0x49, 0x49, 0x7F, 0x36, 0x00,
0x00, 0x18, 0x18, 0x14, 0x16, 0x7F, 0x7F, 0x10, 0x00, 0x27, 0x67, 0x45, 0x45, 0x7D, 0x39, 0x00,
0x00, 0x3E, 0x7F, 0x49, 0x49, 0x7B, 0x32, 0x00, 0x00, 0x03, 0x03, 0x79, 0x7D, 0x07, 0x03, 0x00,
0x00, 0x36, 0x7F, 0x49, 0x49, 0x7F, 0x36, 0x00, 0x00, 0x26, 0x6F, 0x49, 0x49, 0x7F, 0x3E, 0x00,
0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE4, 0x64, 0x00, 0x00, 0x00,
0x00, 0x08, 0x1C, 0x36, 0x63, 0x41, 0x41, 0x00, 0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00,
0x00, 0x41, 0x41, 0x63, 0x36, 0x1C, 0x08, 0x00, 0x00, 0x02, 0x03, 0x51, 0x59, 0x0F, 0x06, 0x00,
0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x5C, 0x7E, 0x7F, 0x7E, 0x5C, 0x18,
0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x18, 0x18, 0x38, 0xF0, 0xE0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x07, 0x0F, 0x1C, 0x18, 0x18, 0x18, 0x18, 0x1C, 0x0F, 0x07, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x03, 0x07, 0x0E, 0x1C, 0x38, 0x70, 0xE0, 0xC0,
0xC0, 0xE0, 0x70, 0x38, 0x1C, 0x0E, 0x07, 0x03, 0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0x00, 0x3C, 0x7E, 0x7E, 0x7E, 0x7E, 0x3C, 0x00,
0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x0E, 0x1F, 0x3F, 0x7E, 0x3F, 0x1F, 0x0E,
0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF0, 0x38, 0x18, 0x18,
0xC3, 0xE7, 0x7E, 0x3C, 0x3C, 0x7E, 0xE7, 0xC3, 0x00, 0x3C, 0x7E, 0x66, 0x66, 0x7E, 0x3C, 0x00,
0x00, 0x0C, 0x4C, 0x73, 0x73, 0x4C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00,
0x00, 0x08, 0x1C, 0x3E, 0x7F, 0x3E, 0x1C, 0x08, 0x18, 0x18, 0x18, 0xFF, 0xFF, 0x18, 0x18, 0x18,
0x33, 0x33, 0xCC, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00,
0x00, 0x10, 0x78, 0x78, 0x08, 0x78, 0x7C, 0x04, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x33, 0x33, 0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
0x30, 0x30, 0xC0, 0xC0, 0x30, 0x30, 0xC0, 0xC0, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x18, 0x18, 0x18,
0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x18, 0x18, 0x18,
0x18, 0x18, 0x18, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
0x00, 0x00, 0x00, 0xF8, 0xF8, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x1F, 0x18, 0x18, 0x18,
0x18, 0x18, 0x18, 0xF8, 0xF8, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0xFF, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0,
0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x18, 0x18, 0x18, 0x1F, 0x1F, 0x00, 0x00, 0x00,
0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0,
0xFF, 0xC1, 0x80, 0xBE, 0xB2, 0x90, 0xD1, 0xFF, 0xFF, 0x83, 0x81, 0xF4, 0xF4, 0x81, 0x83, 0xFF,
0xFF, 0x80, 0x80, 0xB6, 0xB6, 0x80, 0xC9, 0xFF, 0xFF, 0xC1, 0x80, 0xBE, 0xBE, 0x9C, 0xDD, 0xFF,
0xFF, 0x80, 0x80, 0xBE, 0x9C, 0xC1, 0xE3, 0xFF, 0xFF, 0x80, 0x80, 0xB6, 0xB6, 0xBE, 0xBE, 0xFF,
0xFF, 0x80, 0x80, 0xF6, 0xF6, 0xFE, 0xFE, 0xFF, 0xFF, 0xC1, 0x80, 0xBE, 0xB6, 0x84, 0xC5, 0xFF,
0xFF, 0x80, 0x80, 0xF7, 0xF7, 0x80, 0x80, 0xFF, 0xFF, 0xFF, 0xBE, 0x80, 0x80, 0xBE, 0xFF, 0xFF,
0xFF, 0xDF, 0x9F, 0xBE, 0x80, 0xC0, 0xFE, 0xFF, 0xFF, 0x80, 0x80, 0xE3, 0xC9, 0x9C, 0xBE, 0xFF,
0xFF, 0x80, 0x80, 0xBF, 0xBF, 0xBF, 0xBF, 0xFF, 0xFF, 0x80, 0x80, 0xF9, 0xF3, 0xF9, 0x80, 0x80,
0xFF, 0x80, 0x80, 0xF1, 0xE3, 0x80, 0x80, 0xFF, 0xFF, 0xC1, 0x80, 0xBE, 0xBE, 0x80, 0xC1, 0xFF,
0xFF, 0x80, 0x80, 0xF6, 0xF6, 0xF0, 0xF9, 0xFF, 0xFF, 0xE1, 0xC0, 0xDE, 0x9E, 0x80, 0xA1, 0xFF,
0xFF, 0x80, 0x80, 0xE6, 0xC6, 0x90, 0xB9, 0xFF, 0xFF, 0xD9, 0x90, 0xB6, 0xB6, 0x84, 0xCD, 0xFF,
0xFF, 0xFE, 0xFE, 0x80, 0x80, 0xFE, 0xFE, 0xFF, 0xFF, 0xC0, 0x80, 0xBF, 0xBF, 0x80, 0xC0, 0xFF,
0xFF, 0xE0, 0xC0, 0x9F, 0x9F, 0xC0, 0xE0, 0xFF, 0xFF, 0x80, 0x80, 0xCF, 0xE7, 0xCF, 0x80, 0x80,
0xFF, 0x9C, 0x88, 0xE3, 0xE3, 0x88, 0x9C, 0xFF, 0xFF, 0xF8, 0xF0, 0x87, 0x87, 0xF0, 0xF8, 0xFF,
0xFF, 0x9E, 0x8E, 0xA6, 0xB2, 0xB8, 0xBC, 0xFF, 0xFF, 0xFF, 0x80, 0x80, 0xBE, 0xBE, 0xFF, 0xFF,
0xBF, 0x97, 0x83, 0xA1, 0xB6, 0xB6, 0xDD, 0xFF, 0xFF, 0xFF, 0xBE, 0xBE, 0x80, 0x80, 0xFF, 0xFF,
0xFF, 0xF7, 0xF3, 0x01, 0x01, 0xF3, 0xF7, 0xFF, 0xFF, 0xE7, 0xC3, 0x81, 0xE7, 0xE7, 0xE7, 0xE7,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xB0, 0xB0, 0xFF, 0xFF, 0xFF,
0xFF, 0xF8, 0xF8, 0xFF, 0xFF, 0xF8, 0xF8, 0xFF, 0xEB, 0x80, 0x80, 0xEB, 0xEB, 0x80, 0x80, 0xEB,
0xFF, 0xDB, 0xD1, 0x94, 0x94, 0xC5, 0xED, 0xFF, 0xFF, 0x9C, 0xCC, 0xE7, 0xF3, 0x99, 0x9C, 0xFF,
0xFF, 0xCD, 0x80, 0xB2, 0xB2, 0x88, 0x8D, 0xAF, 0xFF, 0xFF, 0xFF, 0xFB, 0xF9, 0xFC, 0xFE, 0xFF,
0xFF, 0xFF, 0xE3, 0xC1, 0x9C, 0xBE, 0xFF, 0xFF, 0xFF, 0xFF, 0xBE, 0x9C, 0xC1, 0xE3, 0xFF, 0xFF,
0xF7, 0xD5, 0xC1, 0xE3, 0xE3, 0xC1, 0xD5, 0xF7, 0xFF, 0xF7, 0xF7, 0xC1, 0xC1, 0xF7, 0xF7, 0xFF,
0xFF, 0xFF, 0x7F, 0x1F, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xFF,
0xFF, 0xFF, 0xFF, 0x9F, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0x9F, 0xCF, 0xE7, 0xF3, 0xF9, 0xFD,
0xFF, 0xC1, 0x80, 0xB6, 0xBA, 0x80, 0xC1, 0xFF, 0xFF, 0xBF, 0xBB, 0x80, 0x80, 0xBF, 0xBF, 0xFF,
0xFF, 0x9D, 0x8C, 0xAE, 0xB6, 0xB0, 0xB9, 0xFF, 0xFF, 0xDD, 0x9C, 0xB6, 0xB6, 0x80, 0xC9, 0xFF,
0xFF, 0xE7, 0xE7, 0xEB, 0xE9, 0x80, 0x80, 0xEF, 0xFF, 0xD8, 0x98, 0xBA, 0xBA, 0x82, 0xC6, 0xFF,
0xFF, 0xC1, 0x80, 0xB6, 0xB6, 0x84, 0xCD, 0xFF, 0xFF, 0xFC, 0xFC, 0x86, 0x82, 0xF8, 0xFC, 0xFF,
0xFF, 0xC9, 0x80, 0xB6, 0xB6, 0x80, 0xC9, 0xFF, 0xFF, 0xD9, 0x90, 0xB6, 0xB6, 0x80, 0xC1, 0xFF,
0xFF, 0xFF, 0xFF, 0xDB, 0xDB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1B, 0x9B, 0xFF, 0xFF, 0xFF,
0xFF, 0xF7, 0xE3, 0xC9, 0x9C, 0xBE, 0xBE, 0xFF, 0xFF, 0xEB, 0xEB, 0xEB, 0xEB, 0xEB, 0xEB, 0xFF,
0xFF, 0xBE, 0xBE, 0x9C, 0xC9, 0xE3, 0xF7, 0xFF, 0xFF, 0xFD, 0xFC, 0xAE, 0xA6, 0xF0, 0xF9, 0xFF,
0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xFF, 0xE7, 0xA3, 0x81, 0x80, 0x81, 0xA3, 0xE7,
0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7,
0xF3, 0xF3, 0xF3, 0xF3, 0xF3, 0xF3, 0xF3, 0xF3, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9, 0xF9,
0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xE7, 0xE7, 0xC7, 0x0F, 0x1F, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xF8, 0xF0, 0xE3, 0xE7, 0xE7, 0xE7, 0xE7, 0xE3, 0xF0, 0xF8, 0xFF, 0xFF, 0xFF,
0x00, 0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0xFC, 0xF8, 0xF1, 0xE3, 0xC7, 0x8F, 0x1F, 0x3F,
0x3F, 0x1F, 0x8F, 0xC7, 0xE3, 0xF1, 0xF8, 0xFC, 0x00, 0x00, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC,
0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0x00, 0x00, 0xFF, 0xC3, 0x81, 0x81, 0x81, 0x81, 0xC3, 0xFF,
0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0xFF, 0xF1, 0xE0, 0xC0, 0x81, 0xC0, 0xE0, 0xF1,
0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x0F, 0xC7, 0xE7, 0xE7,
0x3C, 0x18, 0x81, 0xC3, 0xC3, 0x81, 0x18, 0x3C, 0xFF, 0xC3, 0x81, 0x99, 0x99, 0x81, 0xC3, 0xFF,
0xFF, 0xF3, 0xB3, 0x8C, 0x8C, 0xB3, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
0xFF, 0xF7, 0xE3, 0xC1, 0x80, 0xC1, 0xE3, 0xF7, 0xE7, 0xE7, 0xE7, 0x00, 0x00, 0xE7, 0xE7, 0xE7,
0xCC, 0xCC, 0x33, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
0xFF, 0xEF, 0x87, 0x87, 0xF7, 0x87, 0x83, 0xFB, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC, 0x33, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
0xCF, 0xCF, 0x3F, 0x3F, 0xCF, 0xCF, 0x3F, 0x3F, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xE7, 0xE7, 0xE7,
0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xE7, 0xE7, 0xE7,
0xE7, 0xE7, 0xE7, 0x07, 0x07, 0xFF, 0xFF, 0xFF, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
0xFF, 0xFF, 0xFF, 0x07, 0x07, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE0, 0xE0, 0xE7, 0xE7, 0xE7,
0xE7, 0xE7, 0xE7, 0x07, 0x07, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC,
0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xE7, 0xE7, 0xE7, 0xE0, 0xE0, 0xFF, 0xFF, 0xFF,
0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F
};
#endif
*/


//------------------------------------------------------------------------------
// File generated by LCD Assistant
// http://en.radzio.dxp.pl/bitmap_converter/
//------------------------------------------------------------------------------

// LB Note: I had to manually swap around the Lowercase and PETSCII sections so 
// this works with ASCII and regular uView.print()

#ifndef C64FONT_LOWER_H
#define C64FONT_LOWER_H
//#include <avr/pgmspace.h>
static const unsigned char C64FontLower [] PROGMEM = {
// first row defines - FONTWIDTH, FONTHEIGHT, ASCII START CHAR, TOTAL CHARACTERS, FONT MAP WIDTH HIGH, FONT MAP WIDTH LOW (2,56 meaning 256)
    //8,8,0,255,2,56,

// 96-127 Some PETSCII Graphics (Normal)
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x33, 0x33, 0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
0x30, 0x30, 0xC0, 0xC0, 0x30, 0x30, 0xC0, 0xC0, 0x33, 0x99, 0xCC, 0x66, 0x33, 0x99, 0xCC, 0x66,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x18, 0x18, 0x18,
0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x18, 0x18, 0x18,
0x18, 0x18, 0x18, 0xF8, 0xF8, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
0x00, 0x00, 0x00, 0xF8, 0xF8, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x1F, 0x18, 0x18, 0x18,
0x18, 0x18, 0x18, 0xF8, 0xF8, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0xFF, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0,
0x00, 0x78, 0x78, 0x30, 0x18, 0x0C, 0x06, 0x03, 0xF0, 0xF0, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x18, 0x18, 0x18, 0x1F, 0x1F, 0x00, 0x00, 0x00,
0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0xF0, 0xF0, 0xF0, 0xF0,

// 32-63 Punctuation (Normal)
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4F, 0x4F, 0x00, 0x00, 0x00,
0x00, 0x07, 0x07, 0x00, 0x00, 0x07, 0x07, 0x00, 0x14, 0x7F, 0x7F, 0x14, 0x14, 0x7F, 0x7F, 0x14,
0x00, 0x24, 0x2E, 0x6B, 0x6B, 0x3A, 0x12, 0x00, 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00,
0x00, 0x32, 0x7F, 0x4D, 0x4D, 0x77, 0x72, 0x50, 0x00, 0x00, 0x00, 0x04, 0x06, 0x03, 0x01, 0x00,
0x00, 0x00, 0x1C, 0x3E, 0x63, 0x41, 0x00, 0x00, 0x00, 0x00, 0x41, 0x63, 0x3E, 0x1C, 0x00, 0x00,
0x08, 0x2A, 0x3E, 0x1C, 0x1C, 0x3E, 0x2A, 0x08, 0x00, 0x08, 0x08, 0x3E, 0x3E, 0x08, 0x08, 0x00,
0x00, 0x00, 0x80, 0xE0, 0x60, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00,
0x00, 0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x40, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02,
0x00, 0x3E, 0x7F, 0x49, 0x45, 0x7F, 0x3E, 0x00, 0x00, 0x40, 0x44, 0x7F, 0x7F, 0x40, 0x40, 0x00,
0x00, 0x62, 0x73, 0x51, 0x49, 0x4F, 0x46, 0x00, 0x00, 0x22, 0x63, 0x49, 0x49, 0x7F, 0x36, 0x00,
0x00, 0x18, 0x18, 0x14, 0x16, 0x7F, 0x7F, 0x10, 0x00, 0x27, 0x67, 0x45, 0x45, 0x7D, 0x39, 0x00,
0x00, 0x3E, 0x7F, 0x49, 0x49, 0x7B, 0x32, 0x00, 0x00, 0x03, 0x03, 0x79, 0x7D, 0x07, 0x03, 0x00,
0x00, 0x36, 0x7F, 0x49, 0x49, 0x7F, 0x36, 0x00, 0x00, 0x26, 0x6F, 0x49, 0x49, 0x7F, 0x3E, 0x00,
0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE4, 0x64, 0x00, 0x00, 0x00,
0x00, 0x08, 0x1C, 0x36, 0x63, 0x41, 0x41, 0x00, 0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00,
0x00, 0x41, 0x41, 0x63, 0x36, 0x1C, 0x08, 0x00, 0x00, 0x02, 0x03, 0x51, 0x59, 0x0F, 0x06, 0x00,

// 64-95 Uppercase Letters (Normal)
0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x7C, 0x7E, 0x0B, 0x0B, 0x7E, 0x7C, 0x00,
0x00, 0x7F, 0x7F, 0x49, 0x49, 0x7F, 0x36, 0x00, 0x00, 0x3E, 0x7F, 0x41, 0x41, 0x63, 0x22, 0x00,
0x00, 0x7F, 0x7F, 0x41, 0x63, 0x3E, 0x1C, 0x00, 0x00, 0x7F, 0x7F, 0x49, 0x49, 0x41, 0x41, 0x00,
0x00, 0x7F, 0x7F, 0x09, 0x09, 0x01, 0x01, 0x00, 0x00, 0x3E, 0x7F, 0x41, 0x49, 0x7B, 0x3A, 0x00,
0x00, 0x7F, 0x7F, 0x08, 0x08, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x41, 0x7F, 0x7F, 0x41, 0x00, 0x00,
0x00, 0x20, 0x60, 0x41, 0x7F, 0x3F, 0x01, 0x00, 0x00, 0x7F, 0x7F, 0x1C, 0x36, 0x63, 0x41, 0x00,
0x00, 0x7F, 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x7F, 0x7F, 0x06, 0x0C, 0x06, 0x7F, 0x7F,
0x00, 0x7F, 0x7F, 0x0E, 0x1C, 0x7F, 0x7F, 0x00, 0x00, 0x3E, 0x7F, 0x41, 0x41, 0x7F, 0x3E, 0x00,
0x00, 0x7F, 0x7F, 0x09, 0x09, 0x0F, 0x06, 0x00, 0x00, 0x1E, 0x3F, 0x21, 0x61, 0x7F, 0x5E, 0x00,
0x00, 0x7F, 0x7F, 0x19, 0x39, 0x6F, 0x46, 0x00, 0x00, 0x26, 0x6F, 0x49, 0x49, 0x7B, 0x32, 0x00,
0x00, 0x01, 0x01, 0x7F, 0x7F, 0x01, 0x01, 0x00, 0x00, 0x3F, 0x7F, 0x40, 0x40, 0x7F, 0x3F, 0x00,
0x00, 0x1F, 0x3F, 0x60, 0x60, 0x3F, 0x1F, 0x00, 0x00, 0x7F, 0x7F, 0x30, 0x18, 0x30, 0x7F, 0x7F,
0x00, 0x63, 0x77, 0x1C, 0x1C, 0x77, 0x63, 0x00, 0x00, 0x07, 0x0F, 0x78, 0x78, 0x0F, 0x07, 0x00,
0x00, 0x61, 0x71, 0x59, 0x4D, 0x47, 0x43, 0x00, 0x18, 0x18, 0x18, 0xFF, 0xFF, 0x18, 0x18, 0x18,
0x33, 0x33, 0xCC, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00,
0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC, 0x33, 0x33, 0x66, 0xCC, 0x99, 0x33, 0x66, 0xCC, 0x99, 0x33,

// 0-31 Lowercase Letters (Normal)
0x00, 0x3E, 0x7F, 0x41, 0x4D, 0x4F, 0x2E, 0x00, 0x00, 0x20, 0x74, 0x54, 0x54, 0x7C, 0x78, 0x00,
0x00, 0x7E, 0x7E, 0x48, 0x48, 0x78, 0x30, 0x00, 0x00, 0x38, 0x7C, 0x44, 0x44, 0x44, 0x00, 0x00,
0x00, 0x30, 0x78, 0x48, 0x48, 0x7E, 0x7E, 0x00, 0x00, 0x38, 0x7C, 0x54, 0x54, 0x5C, 0x18, 0x00,
0x00, 0x00, 0x08, 0x7C, 0x7E, 0x0A, 0x0A, 0x00, 0x00, 0x98, 0xBC, 0xA4, 0xA4, 0xFC, 0x7C, 0x00,
0x00, 0x7E, 0x7E, 0x08, 0x08, 0x78, 0x70, 0x00, 0x00, 0x00, 0x48, 0x7A, 0x7A, 0x40, 0x00, 0x00,
0x00, 0x00, 0x80, 0x80, 0x80, 0xFA, 0x7A, 0x00, 0x00, 0x7E, 0x7E, 0x10, 0x38, 0x68, 0x40, 0x00,
0x00, 0x00, 0x42, 0x7E, 0x7E, 0x40, 0x00, 0x00, 0x00, 0x7C, 0x7C, 0x18, 0x38, 0x1C, 0x7C, 0x78,
0x00, 0x7C, 0x7C, 0x04, 0x04, 0x7C, 0x78, 0x00, 0x00, 0x38, 0x7C, 0x44, 0x44, 0x7C, 0x38, 0x00,
0x00, 0xFC, 0xFC, 0x24, 0x24, 0x3C, 0x18, 0x00, 0x00, 0x18, 0x3C, 0x24, 0x24, 0xFC, 0xFC, 0x00,
0x00, 0x7C, 0x7C, 0x04, 0x04, 0x0C, 0x08, 0x00, 0x00, 0x48, 0x5C, 0x54, 0x54, 0x74, 0x24, 0x00,
0x00, 0x04, 0x04, 0x3E, 0x7E, 0x44, 0x44, 0x00, 0x00, 0x3C, 0x7C, 0x40, 0x40, 0x7C, 0x7C, 0x00,
0x00, 0x1C, 0x3C, 0x60, 0x60, 0x3C, 0x1C, 0x00, 0x00, 0x1C, 0x7C, 0x70, 0x38, 0x70, 0x7C, 0x1C,
0x00, 0x44, 0x6C, 0x38, 0x38, 0x6C, 0x44, 0x00, 0x00, 0x9C, 0xBC, 0xA0, 0xE0, 0x7C, 0x3C, 0x00,
0x00, 0x44, 0x64, 0x74, 0x5C, 0x4C, 0x44, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x41, 0x41, 0x00, 0x00,
0x40, 0x68, 0x7C, 0x5E, 0x49, 0x49, 0x22, 0x00, 0x00, 0x00, 0x41, 0x41, 0x7F, 0x7F, 0x00, 0x00,
0x00, 0x08, 0x0C, 0xFE, 0xFE, 0x0C, 0x08, 0x00, 0x00, 0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18,

// 224-255 Some PETSCII Graphics (Inverted)
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC, 0x33, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
0xCF, 0xCF, 0x3F, 0x3F, 0xCF, 0xCF, 0x3F, 0x3F, 0xCC, 0x66, 0x33, 0x99, 0xCC, 0x66, 0x33, 0x99,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xE7, 0xE7, 0xE7,
0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xE7, 0xE7, 0xE7,
0xE7, 0xE7, 0xE7, 0x07, 0x07, 0xFF, 0xFF, 0xFF, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
0xFF, 0xFF, 0xFF, 0x07, 0x07, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE0, 0xE0, 0xE7, 0xE7, 0xE7,
0xE7, 0xE7, 0xE7, 0x07, 0x07, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC,
0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
0xFF, 0x87, 0x87, 0xCF, 0xE7, 0xF3, 0xF9, 0xFC, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xE7, 0xE7, 0xE7, 0xE0, 0xE0, 0xFF, 0xFF, 0xFF,
0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0x0F, 0x0F, 0x0F, 0x0F,

// 160-191 Punctuation (Inverted)  
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xB0, 0xB0, 0xFF, 0xFF, 0xFF,
0xFF, 0xF8, 0xF8, 0xFF, 0xFF, 0xF8, 0xF8, 0xFF, 0xEB, 0x80, 0x80, 0xEB, 0xEB, 0x80, 0x80, 0xEB,
0xFF, 0xDB, 0xD1, 0x94, 0x94, 0xC5, 0xED, 0xFF, 0xFF, 0x9C, 0xCC, 0xE7, 0xF3, 0x99, 0x9C, 0xFF,
0xFF, 0xCD, 0x80, 0xB2, 0xB2, 0x88, 0x8D, 0xAF, 0xFF, 0xFF, 0xFF, 0xFB, 0xF9, 0xFC, 0xFE, 0xFF,
0xFF, 0xFF, 0xE3, 0xC1, 0x9C, 0xBE, 0xFF, 0xFF, 0xFF, 0xFF, 0xBE, 0x9C, 0xC1, 0xE3, 0xFF, 0xFF,
0xF7, 0xD5, 0xC1, 0xE3, 0xE3, 0xC1, 0xD5, 0xF7, 0xFF, 0xF7, 0xF7, 0xC1, 0xC1, 0xF7, 0xF7, 0xFF,
0xFF, 0xFF, 0x7F, 0x1F, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xF7, 0xFF,
0xFF, 0xFF, 0xFF, 0x9F, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0x9F, 0xCF, 0xE7, 0xF3, 0xF9, 0xFD,
0xFF, 0xC1, 0x80, 0xB6, 0xBA, 0x80, 0xC1, 0xFF, 0xFF, 0xBF, 0xBB, 0x80, 0x80, 0xBF, 0xBF, 0xFF,
0xFF, 0x9D, 0x8C, 0xAE, 0xB6, 0xB0, 0xB9, 0xFF, 0xFF, 0xDD, 0x9C, 0xB6, 0xB6, 0x80, 0xC9, 0xFF,
0xFF, 0xE7, 0xE7, 0xEB, 0xE9, 0x80, 0x80, 0xEF, 0xFF, 0xD8, 0x98, 0xBA, 0xBA, 0x82, 0xC6, 0xFF,
0xFF, 0xC1, 0x80, 0xB6, 0xB6, 0x84, 0xCD, 0xFF, 0xFF, 0xFC, 0xFC, 0x86, 0x82, 0xF8, 0xFC, 0xFF,
0xFF, 0xC9, 0x80, 0xB6, 0xB6, 0x80, 0xC9, 0xFF, 0xFF, 0xD9, 0x90, 0xB6, 0xB6, 0x80, 0xC1, 0xFF,
0xFF, 0xFF, 0xFF, 0xDB, 0xDB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1B, 0x9B, 0xFF, 0xFF, 0xFF,
0xFF, 0xF7, 0xE3, 0xC9, 0x9C, 0xBE, 0xBE, 0xFF, 0xFF, 0xEB, 0xEB, 0xEB, 0xEB, 0xEB, 0xEB, 0xFF,
0xFF, 0xBE, 0xBE, 0x9C, 0xC9, 0xE3, 0xF7, 0xFF, 0xFF, 0xFD, 0xFC, 0xAE, 0xA6, 0xF0, 0xF9, 0xFF,

// 192-223 Uppercase Letters (Inverted)
0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7, 0xFF, 0x83, 0x81, 0xF4, 0xF4, 0x81, 0x83, 0xFF,
0xFF, 0x80, 0x80, 0xB6, 0xB6, 0x80, 0xC9, 0xFF, 0xFF, 0xC1, 0x80, 0xBE, 0xBE, 0x9C, 0xDD, 0xFF,
0xFF, 0x80, 0x80, 0xBE, 0x9C, 0xC1, 0xE3, 0xFF, 0xFF, 0x80, 0x80, 0xB6, 0xB6, 0xBE, 0xBE, 0xFF,
0xFF, 0x80, 0x80, 0xF6, 0xF6, 0xFE, 0xFE, 0xFF, 0xFF, 0xC1, 0x80, 0xBE, 0xB6, 0x84, 0xC5, 0xFF,
0xFF, 0x80, 0x80, 0xF7, 0xF7, 0x80, 0x80, 0xFF, 0xFF, 0xFF, 0xBE, 0x80, 0x80, 0xBE, 0xFF, 0xFF,
0xFF, 0xDF, 0x9F, 0xBE, 0x80, 0xC0, 0xFE, 0xFF, 0xFF, 0x80, 0x80, 0xE3, 0xC9, 0x9C, 0xBE, 0xFF,
0xFF, 0x80, 0x80, 0xBF, 0xBF, 0xBF, 0xBF, 0xFF, 0xFF, 0x80, 0x80, 0xF9, 0xF3, 0xF9, 0x80, 0x80,
0xFF, 0x80, 0x80, 0xF1, 0xE3, 0x80, 0x80, 0xFF, 0xFF, 0xC1, 0x80, 0xBE, 0xBE, 0x80, 0xC1, 0xFF,
0xFF, 0x80, 0x80, 0xF6, 0xF6, 0xF0, 0xF9, 0xFF, 0xFF, 0xE1, 0xC0, 0xDE, 0x9E, 0x80, 0xA1, 0xFF,
0xFF, 0x80, 0x80, 0xE6, 0xC6, 0x90, 0xB9, 0xFF, 0xFF, 0xD9, 0x90, 0xB6, 0xB6, 0x84, 0xCD, 0xFF,
0xFF, 0xFE, 0xFE, 0x80, 0x80, 0xFE, 0xFE, 0xFF, 0xFF, 0xC0, 0x80, 0xBF, 0xBF, 0x80, 0xC0, 0xFF,
0xFF, 0xE0, 0xC0, 0x9F, 0x9F, 0xC0, 0xE0, 0xFF, 0xFF, 0x80, 0x80, 0xCF, 0xE7, 0xCF, 0x80, 0x80,
0xFF, 0x9C, 0x88, 0xE3, 0xE3, 0x88, 0x9C, 0xFF, 0xFF, 0xF8, 0xF0, 0x87, 0x87, 0xF0, 0xF8, 0xFF,
0xFF, 0x9E, 0x8E, 0xA6, 0xB2, 0xB8, 0xBC, 0xFF, 0xE7, 0xE7, 0xE7, 0x00, 0x00, 0xE7, 0xE7, 0xE7,
0xCC, 0xCC, 0x33, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
0x33, 0x33, 0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC, 0x99, 0x33, 0x66, 0xCC, 0x99, 0x33, 0x66, 0xCC,

// 128-159 Lowercase Letters (Inverted)
0xFF, 0xC1, 0x80, 0xBE, 0xB2, 0x90, 0xD1, 0xFF, 0xFF, 0xDF, 0x8B, 0xAB, 0xAB, 0x83, 0x87, 0xFF,
0xFF, 0x81, 0x81, 0xB7, 0xB7, 0x87, 0xCF, 0xFF, 0xFF, 0xC7, 0x83, 0xBB, 0xBB, 0xBB, 0xFF, 0xFF,
0xFF, 0xCF, 0x87, 0xB7, 0xB7, 0x81, 0x81, 0xFF, 0xFF, 0xC7, 0x83, 0xAB, 0xAB, 0xA3, 0xE7, 0xFF,
0xFF, 0xFF, 0xF7, 0x83, 0x81, 0xF5, 0xF5, 0xFF, 0xFF, 0x67, 0x43, 0x5B, 0x5B, 0x03, 0x83, 0xFF,
0xFF, 0x81, 0x81, 0xF7, 0xF7, 0x87, 0x8F, 0xFF, 0xFF, 0xFF, 0xB7, 0x85, 0x85, 0xBF, 0xFF, 0xFF,
0xFF, 0xFF, 0x7F, 0x7F, 0x7F, 0x05, 0x85, 0xFF, 0xFF, 0x81, 0x81, 0xEF, 0xC7, 0x97, 0xBF, 0xFF,
0xFF, 0xFF, 0xBD, 0x81, 0x81, 0xBF, 0xFF, 0xFF, 0xFF, 0x83, 0x83, 0xE7, 0xC7, 0xE3, 0x83, 0x87,
0xFF, 0x83, 0x83, 0xFB, 0xFB, 0x83, 0x87, 0xFF, 0xFF, 0xC7, 0x83, 0xBB, 0xBB, 0x83, 0xC7, 0xFF,
0xFF, 0x03, 0x03, 0xDB, 0xDB, 0xC3, 0xE7, 0xFF, 0xFF, 0xE7, 0xC3, 0xDB, 0xDB, 0x03, 0x03, 0xFF,
0xFF, 0x83, 0x83, 0xFB, 0xFB, 0xF3, 0xF7, 0xFF, 0xFF, 0xB7, 0xA3, 0xAB, 0xAB, 0x8B, 0xDB, 0xFF,
0xFF, 0xFB, 0xFB, 0xC1, 0x81, 0xBB, 0xBB, 0xFF, 0xFF, 0xC3, 0x83, 0xBF, 0xBF, 0x83, 0x83, 0xFF,
0xFF, 0xE3, 0xC3, 0x9F, 0x9F, 0xC3, 0xE3, 0xFF, 0xFF, 0xE3, 0x83, 0x8F, 0xC7, 0x8F, 0x83, 0xE3,
0xFF, 0xBB, 0x93, 0xC7, 0xC7, 0x93, 0xBB, 0xFF, 0xFF, 0x63, 0x43, 0x5F, 0x1F, 0x83, 0xC3, 0xFF,
0xFF, 0xBB, 0x9B, 0x8B, 0xA3, 0xB3, 0xBB, 0xFF, 0xFF, 0xFF, 0x80, 0x80, 0xBE, 0xBE, 0xFF, 0xFF,
0xBF, 0x97, 0x83, 0xA1, 0xB6, 0xB6, 0xDD, 0xFF, 0xFF, 0xFF, 0xBE, 0xBE, 0x80, 0x80, 0xFF, 0xFF,
0xFF, 0xF7, 0xF3, 0x01, 0x01, 0xF3, 0xF7, 0xFF, 0xFF, 0xE7, 0xC3, 0x81, 0xE7, 0xE7, 0xE7, 0xE7,

};
#endif

