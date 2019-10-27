    #include "functions.h"

/*===================================================================================================================*/
    
    const int latchPin = 5;
    const int clockPin = 6;
    const int dataPin = 4;
    const int outEnable = 7;

    const int outPins[10] = {
      20, 21, 22, 23, 24, 25, 26, 27, 28, 29
    };

    const int inPins[10] = {
       30, 31, 32, 33, 34, 35, 36, 37, 38, 39
    };

    byte registers[8] = { //Used for LED output
    0b00000000, 
    0b00000000, 
    0b00000000, 
    0b00000000, 
    0b00000000,    
    0b00000000,
    0b00000000,
    0b00000000
    };

    bool pinRegisters[10][10] = {
    {0,0,0,0,0,0,0,0,0,0}
    {0,0,0,0,0,0,0,0,0,0}
    {0,0,0,0,0,0,0,0,0,0}
    {0,0,0,0,0,0,0,0,0,0}
    {0,0,0,0,0,0,0,0,0,0}
    {0,0,0,0,0,0,0,0,0,0}
    {0,0,0,0,0,0,0,0,0,0}
    {0,0,0,0,0,0,0,0,0,0}
    {0,0,0,0,0,0,0,0,0,0}
    {0,0,0,0,0,0,0,0,0,0}
}
    
    LED leds[20] = {
      LED(0, 1, 2, 0, 0, 0),
      LED(3, 4, 5, 0, 0, 0),
      LED(6, 7, 0, 0, 0, 1),
      LED(1, 2, 3, 1, 1, 1),
      LED(4, 5, 6, 1, 1, 1),
      LED(7, 0, 1, 1, 2, 2),
      LED(2, 3, 4, 2, 2, 2),
      LED(5, 6, 7, 2, 2, 2),
      LED(0, 1, 2, 3, 3, 3),
      LED(3, 4, 5, 3, 3, 3),
      LED(0, 1, 2, 4, 4, 4),
      LED(3, 4, 5, 4, 4, 4),
      LED(6, 7, 0, 4, 4, 5),
      LED(1, 2, 3, 5, 5, 5),
      LED(4, 5, 6, 5, 5, 5),
      LED(7, 0, 1, 5, 6, 6),
      LED(2, 3, 4, 6, 6, 6),
      LED(5, 6, 7, 6, 6, 6),
      LED(0, 1, 2, 7, 7, 7),
      LED(3, 4, 5, 7, 7, 7)
    };

/*===================================================================================================================*/    
      
    void updateRegisters() {
      digitalWrite(outEnable, HIGH);
      for (int i = 7; i >= 0; i--) {
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, registers[i]);
        digitalWrite(latchPin, HIGH);
      }
      digitalWrite(outEnable, LOW);
    }

    void ledCheck() {
      for (int i = 0; i < 8; i++) { // Cycles through each led in turn to check I haven't broken anything
        for (int j = 0; j < 8; j++) {
          bitSet(registers[i], j);
          updateRegisters();
          delay(20);
          bitClear(registers[i], j);
          updateRegisters();
          delay(20);
        }
      }
      for (int k = 0; k < 45; k++) { // Produces a chassing array of 5 rgb leds
        leds[k].gOn();
        leds[k-10].rOn();
        leds[k-20].bOn();
        updateRegisters();
        delay(50);
        leds[k-5].gOff();
        leds[k-15].rOff();
        leds[k-25].bOff();
        updateRegisters();
        delay(20);
      }
    }

    void pinTest(int pinCount) {
      for (int i = 0; i < pinCount; i++) { // iterate through output pins

        digitalWrite(outPins[i], LOW); // set i high to test that pin
        leds[i + 10].gOn();
        for (int j = 0; j < pinCount; j++) { // iterate through input pins
        
          if (digitalRead(inPins[j]) == LOW) {
            pinRegisters[i][j] = true;
          }
          else if (digitalRead(inPins[j]) == HIGH) {
            pinRegisters[i][j] = false;
          }
          else {
            Serial.print("Logic has failed to reach an outcome, time to debug!\n");
            break;
          }
        }
        digitalWrite(outPins[i], HIGH); // set i low again
        updateRegisters();
      }
      // Write a for loop here that iterates through pinRegisters and compares each one to the others 
      //in order to work out where the breaks, shorts and crosses are.
      updateRegisters();
    }

    void checkRegisters() {
      Serial.print("Registers: \n");
      for (int i = 0; i < 8; i++) {
        Serial.print(registers[i], BIN);
        Serial.print("\n");          
      }
    }

    void clearRegisters() {
      for (int i = 7; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
          bitClear(registers[i], j);
        }
      }
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