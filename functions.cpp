    #include "functions.h"

/*===================================================================================================================*/
    
    const int latchPin = 5;
    const int clockPin = 6;
    const int dataPin = 4;
    const int outEnable = 7;
  
    const int pinCount = 3;

    const int outPins[10] = {
      20, 21, 22, 23, 24, 25, 26, 27, 28, 29
    };

    const int inPins[10] = {
       30, 31, 32, 33, 34, 35, 36, 37, 38, 39
    };

    byte registers[4] = {
    0b00000000,    
    0b00000000,
    0b00000000,
    0b00000000
    };
    
    LED leds[10] = {
      LED(0, 1, 2, 0, 0, 0),
      LED(3, 4, 5, 0, 0, 0),
      LED(6, 7, 0, 0, 0, 1),
      LED(1, 2, 3, 1, 1, 1),
      LED(4, 5, 6, 1, 1, 1),
      LED(7, 0, 1, 1, 2, 2),
      LED(2, 3, 4, 2, 2, 2),
      LED(5, 6, 7, 2, 2, 2),
      LED(0, 1, 2, 3, 3, 3),
      LED(3, 4, 5, 3, 3, 3)
    };

/*===================================================================================================================*/    
      
    void updateRegisters() {
      digitalWrite(outEnable, HIGH);

      for (int i = 3; i >= 0; i--) {
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, registers[i]);
        digitalWrite(latchPin, HIGH);
      }
      digitalWrite(outEnable, LOW);
    }

    void ledCheck() {
      for (int i = 0; i < 4; i++) { // cycles through each led in turn to check I haven't broken anything
        for (int j = 0; j < 8; j++) {
          bitSet(registers[i], j);
          updateRegisters();
          delay(200);
          bitClear(registers[i], j);
          updateRegisters();
          delay(20);
        }
      }
    }

    void pinTest(int pinCount) {
      for (int i = 1; i <= pinCount; i++) { // iterate through output pins
        digitalWrite(outPins[i], HIGH); // set i high to test that pin
        for (int j = 1; i <= pinCount; j++) { // iterate through input pins
          if (digitalRead(j) == HIGH && i == j) {
            //Some way of making LED[i] green
          }
          else if (digitalRead(j) == HIGH && i != j) {
            //Some way to set LED[i] blue
          }
          else if (digitalRead(j) == LOW) {
            //Some way to set LED[i] red
          }
        }
        digitalWrite(outPins[i], LOW); // set i low again
      }
      updateRegisters();
    }

/*===================================================================================================================*/   

      LED::LED(int redBit, int greenBit, int blueBit, int redReg, int greenReg, int blueReg) {
        rBit = redBit;
        gBit = greenBit;
        bBit = blueBit;
        rReg = redReg;
        gReg = greenReg;
        bReg = blueReg;
      }

      void LED::rOn() {
        bitSet(registers[rReg], rBit);
      }
      void LED::rOff() {
        bitClear(registers[rReg], rBit);
      }
      void LED::gOn() {
        bitSet(registers[gReg], gBit);
      }
      void LED::gOff() {
        bitClear(registers[gReg], gBit);
      }
      void LED::bOn() {
        bitSet(registers[bReg], bBit);
      }
      void LED::bOff() {
        bitClear(registers[bReg], bBit);
      }