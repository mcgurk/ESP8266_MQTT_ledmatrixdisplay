#include <SPI.h>
#include "MAX7219.h"
#include "cp437font.h"
#include "C64font.h"
#include "minifont.h"

/**
 * Heavily influenced by the code and the blog posts from https://github.com/nickgammon/MAX7219_Dot_Matrix
 */
LedMatrix::LedMatrix(byte numberOfDevices, byte slaveSelectPin) {
    myNumberOfDevices = numberOfDevices;
    mySlaveSelectPin = slaveSelectPin;
    rows = new byte[numberOfDevices * 8];
}

/**
 *  numberOfDevices: how many modules are daisy changed togehter
 *  slaveSelectPin: which pin is controlling the CS/SS pin of the first module?
 */
void LedMatrix::init() {
    pinMode(mySlaveSelectPin, OUTPUT);
    
    SPI.begin ();
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV128);
    for(byte device = 0; device < myNumberOfDevices; device++) {
        sendByte (device, MAX7219_REG_SCANLIMIT, 7);   // show all 8 digits
        sendByte (device, MAX7219_REG_DECODEMODE, 0);  // using an led matrix (not digits)
        sendByte (device, MAX7219_REG_DISPLAYTEST, 0); // no display test
        sendByte (device, MAX7219_REG_INTENSITY, 0);   // character intensity: range: 0 to 15
        sendByte (device, MAX7219_REG_SHUTDOWN, 1);    // not in shutdown mode (ie. start it up)
    }
}

void LedMatrix::sendByte(const byte device, const byte reg, const byte data) {
    int offset=device;
    int maxbytes=myNumberOfDevices;
    
    for(int i=0;i<maxbytes;i++) {
        spidata[i] = (byte)0;
        spiregister[i] = (byte)0;
    }
    // put our device data into the array
    spiregister[offset] = reg;
    spidata[offset] = data;
    // enable the line
    digitalWrite(mySlaveSelectPin,LOW);
    // now shift out the data
    for(int i=0;i<myNumberOfDevices;i++) {
        SPI.transfer(spiregister[i]);
        SPI.transfer(spidata[i]);
    }
    digitalWrite(mySlaveSelectPin, HIGH);
    
}

void LedMatrix::sendByte(const byte reg, const byte data) {
    for(byte device = 0; device < myNumberOfDevices; device++) {
        sendByte(device, reg, data);
    }
}

void LedMatrix::setIntensity(const byte intensity) {
    sendByte(MAX7219_REG_INTENSITY, intensity);
}

void LedMatrix::setCharWidth(byte charWidth) {
    myCharWidth = charWidth;
}

void LedMatrix::setTextAlignment(byte textAlignment) {
    myTextAlignment = textAlignment;
    calculateTextAlignmentOffset();
}

void LedMatrix::calculateTextAlignmentOffset() {
    switch(myTextAlignment) {
        case TEXT_ALIGN_LEFT:
            myTextAlignmentOffset = 0;
            break;
        case TEXT_ALIGN_LEFT_END:
            myTextAlignmentOffset = myNumberOfDevices * 8;
            break;
        case TEXT_ALIGN_RIGHT:
            myTextAlignmentOffset = myText.length() * myCharWidth - myNumberOfDevices * 8;
            break;
        case TEXT_ALIGN_RIGHT_END:
            myTextAlignmentOffset = - myText.length() * myCharWidth;
            break;
    }
    
}

void LedMatrix::clearOffscreen() {
    /*for (byte row = 0; row < myNumberOfDevices * 8; row++) {
        rows[row] = 0;
    }*/
    memset(rows, 0, myNumberOfDevices * 8);
}

void LedMatrix::setOffscreen(byte *source) {
    memcpy(rows, source, myNumberOfDevices * 8);
}

void LedMatrix::clear() {
    clearOffscreen();
    commit();
}

void LedMatrix::commit() {
  for (byte row = 0; row < 8; row++) {
    digitalWrite(mySlaveSelectPin, LOW);
    // now shift out the data
    for(int i=0;i<myNumberOfDevices;i++) {
      SPI.transfer(row+1);
      SPI.transfer(rows[i + row*myNumberOfDevices]);
    }
    digitalWrite(mySlaveSelectPin, HIGH);
  }
}

void LedMatrix::setText(String text) {
    myText = text;
    myTextOffset = 0;
    calculateTextAlignmentOffset();
}

void LedMatrix::setNextText(String nextText) {
    myNextText = nextText;
}

void LedMatrix::scrollTextRight() {
    myTextOffset = (myTextOffset + 1) % ((int)myText.length() * myCharWidth - 5);
}

void LedMatrix::scrollTextLeft() {
    myTextOffset = (myTextOffset - 1) % ((int)myText.length() * myCharWidth + myNumberOfDevices * 8);
    if (myTextOffset == 0 && myNextText.length() > 0) {
        myText = myNextText;
        myNextText = "";
        calculateTextAlignmentOffset();
    }
}

void LedMatrix::oscillateText() {
    int maxColumns = (int)myText.length() * myCharWidth;
    int maxDisplayColumns = myNumberOfDevices * 8;
    if (maxDisplayColumns > maxColumns) {
        return;
    }
    if (myTextOffset - maxDisplayColumns == -maxColumns) {
        increment = 1;
    }
    if (myTextOffset == 0) {
        increment = -1;
    }
    myTextOffset += increment;
}

void LedMatrix::drawText() {
    char letter;
    int position = 0;
    for (int i = 0; i < myText.length(); i++) {
        letter = myText.charAt(i);
        for (byte col = 0; col < 8; col++) {
            position = i * myCharWidth + col + myTextOffset + myTextAlignmentOffset;
            if (position >= 0 && position < myNumberOfDevices * 8) {
                //setColumn(position, pgm_read_byte (&cp437_font [letter] [col]));
                //if (letter >= 'a' && letter <= 'z') setColumn(position, pgm_read_byte (&C64FontLower [(letter-'a'+1)*8+col]));
                //if (letter >= 'A' && letter <= 'Z') setColumn(position, pgm_read_byte (&C64FontUpper [(letter-'A'+1)*8+col]));
                //setColumn(position, pgm_read_byte (&C64FontLower [letter*8+col]));
                setColumn(position, pgm_read_byte (&cp437_font [letter] [col]));
            }
        }
    }
}

void LedMatrix::drawChar(char letter, uint8_t elem) {
  if (elem >= myNumberOfDevices) elem = myNumberOfDevices-1;
  int position = 0;
  for (byte col = 0; col < 8; col++) {
    position = elem * 8 + col;
    //setColumn(position, pgm_read_byte (&cp437_font [letter] [col]));
    //if (letter >= 'a' && letter <= 'z') setColumn(position, pgm_read_byte (&C64FontLower [(letter-'a'+1)*8+col]));
    //if (letter >= 'A' && letter <= 'Z') setColumn(position, pgm_read_byte (&C64FontUpper [(letter-'A'+1)*8+col]));
    //setColumn(position, pgm_read_byte (&C64FontLower [letter*8+col]));
    setColumn(position, pgm_read_byte (&cp437_font [letter] [col]));
  }
}

/*void LedMatrix::drawText() {
    char letter;
    int position = 0;
    for (int i = 0; i < myText.length(); i++) {
        letter = myText.charAt(i);
        for (byte col = 0; col < 8; col++) {
            position = i * myCharWidth + col + myTextOffset + myTextAlignmentOffset;
            if (position >= 0 && position < myNumberOfDevices * 8) {
                //setColumn(position, pgm_read_byte (&cp437_font [letter] [col]));
                //setColumn(position, pgm_read_byte (&C64FontUpper [letter] [col]));
                byte a1=pgm_read_byte (&C64FontUpper [letter*8+0]);
                byte a2=pgm_read_byte (&C64FontUpper [letter*8+1]);
                byte a3=pgm_read_byte (&C64FontUpper [letter*8+2]);
                byte a4=pgm_read_byte (&C64FontUpper [letter*8+3]);
                byte a5=pgm_read_byte (&C64FontUpper [letter*8+4]);
                byte a6=pgm_read_byte (&C64FontUpper [letter*8+5]);
                byte a7=pgm_read_byte (&C64FontUpper [letter*8+6]);
                byte a8=pgm_read_byte (&C64FontUpper [letter*8+7]);
                byte out = bitRead(a1,col)<<7 || bitRead(a2,col)<<6 || bitRead(a3,col)<<5 || bitRead(a4,col)<<4 || bitRead(a5,col)<<3 || bitRead(a6,col)<<2 || bitRead(a7,col)<<1 || bitRead(a8,col)<<0;
                out = a5;
                setColumn(position, out);
                //setColumn(position, pgm_read_byte (&C64FontUpper [letter*8+col]));
                //setColumn(position, pgm_read_byte (&mini_font [letter-'0'] [col]));
            }
        }
    }
}*/

void LedMatrix::drawMiniText(String text) {
    char letter;
    int position = 0;
    int p = 0;
    for (int i = 0; i < text.length(); i++) {
/*        letter = myText.charAt(i);
        for (byte col = 0; col < 8; col++) {
            position = i * myCharWidth + col + myTextOffset + myTextAlignmentOffset;
            if (position >= 0 && position < myNumberOfDevices * 8) {
                setColumn(position, pgm_read_byte (&cp437_font [letter] [col]));
                //setColumn(position, pgm_read_byte (&mini_font [letter-'0'] [col]));
            }
        }*/
        letter = text.charAt(i);
        for (byte row =  0; row < 8; row++) {
          char c = pgm_read_byte (&mini_font [letter-'0'] [row]);
          for (byte x=0; x<8; x++) {
            if( (c&(1<<(7-x))) and ((x+p)<32) ) setPixel(x+p,row);
          }
        }
        if (letter==':') p+=2; else p+=4;
    }
}

void LedMatrix::setColumn(int column, byte value) {
  if (column < 0 || column >= myNumberOfDevices * 8) {
    return;
  }
  for (byte row = 0; row < 8; row++) {
    if (value & (1<<row)) setPixel(column, row);
  }
}

void LedMatrix::setPixel(byte x, byte y) {
  if (x >= 0 and x < myNumberOfDevices*8 and y >= 0 and y < 8)
    bitSet(rows[y*myNumberOfDevices + (x>>3)], ( 7-(x&7) ));
}

void LedMatrix::writePixel(byte x, byte y, byte p) {
  if (x >= 0 and x < myNumberOfDevices*8 and y >= 0 and y < 8)
    bitWrite(rows[y*myNumberOfDevices + (x>>3)], ( 7-(x&7) ), p);
}
