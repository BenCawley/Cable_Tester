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

    byte ledRegisters[8] = { //Used for LED output
    0b00000000, 
    0b00000000, 
    0b00000000, 
    0b00000000, 
    0b00000000,    
    0b00000000,
    0b00000000,
    0b00000000
    };

    bool pinMask[10][10] = {
    {1,0,0,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,1}  
    };

    bool pinRegisters[10][10] = {
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0}
    };
    
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
        shiftOut(dataPin, clockPin, MSBFIRST, ledRegisters[i]);
        digitalWrite(latchPin, HIGH);
      }
      digitalWrite(outEnable, LOW);
    }

    void ledCheck() {
      for (int i = 0; i < 8; i++) { // Cycles through each led in turn to check I haven't broken anything
        for (int j = 0; j < 8; j++) {
          bitSet(ledRegisters[i], j);
          updateRegisters();
          delay(20);
          bitClear(ledRegisters[i], j);
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
        for (int j = 0; j < pinCount; j++) { // iterate through input pins and test for high or low state
          if (digitalRead(inPins[j]) == LOW) {
            pinRegisters[i][j] = true;
          }
          else if (digitalRead(inPins[j]) == HIGH) {
            pinRegisters[i][j] = false;
          }
          else {
            Serial.print("pinTest conditions not met, time to debug!!\n");
            break;
          }
        }
        digitalWrite(outPins[i], HIGH); // set i low again
        updateRegisters();
      }
      // Next we iterate through pinRegisters and pinMask, comparing each part of the array to find the breaks, crosses and shorts
      for (int k = 0; k < pinCount; k++) {
        if (pinRegisters[k] == pinMask[k]) {
          leds[k].gOn();
        }
        for (int m = 0; m < pinCount; m++) {//if pinMask true pin is 0/false, turn on led red and if pinMask false pin is true/1, turn led blue
          if (pinMask[k][m] == true && pinRegisters[k][m] == false) {
            leds[k].rOn();
            leds[m+10].rOn();
          }
          else if (pinMask[k][m] == false && pinRegisters[k][m] == true) {
            leds[k].bOn();
            leds[m+10].bOn();
          }
          else if (pinMask[k][m] == true && pinRegisters[k][m] == true) {
            leds[k].gOn();
            leds[m+10].gOn();
          }
          else if (pinMask[k][m] == false && pinRegisters[k][m] == false) {
            continue;
          }
          else {
            Serial.print("pinRegister comparison conditions not met, time to debug!!\n");
          }
          updateRegisters();
        }
      updateRegisters();
      }
    }

    void checkRegisters() {
      Serial.print("LED Registers: \n");
      for (int i = 0; i < 8; i++) {
        Serial.print(ledRegisters[i], BIN);
        Serial.print("\n");          
      }
      Serial.print("\nPin Registers:\n");
      for (int k = 0; k < 10; k++) {
        Serial.print("\n");
        for (int m = 0; m < 10; m++) {
          Serial.print(pinRegisters[k][m]);
          if (m < 9) {
            Serial.print(",\t");
          }
        }
      }
      Serial.print("\n");
    }

    void clearRegisters() {
      for (int i = 7; i >= 0; i--) {
        for (int j = 7; j >= 0; j--) {
          bitClear(ledRegisters[i], j);
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
        bitSet(ledRegisters[rReg], rBit);
      }
      void LED::rOff() {
        bitClear(ledRegisters[rReg], rBit);
      }
      void LED::gOn() {
        bitSet(ledRegisters[gReg], gBit);
      }
      void LED::gOff() {
        bitClear(ledRegisters[gReg], gBit);
      }
      void LED::bOn() {
        bitSet(ledRegisters[bReg], bBit);
      }
      void LED::bOff() {
        bitClear(ledRegisters[bReg], bBit);
      }