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

    const int dipPins[16] = {
      14, 15, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53
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

    bool const pinMask[10][10] = {
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
    }

    void ledChase() {
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
      for (int i = 0; i < pinCount; i++) { // Iterate through output pins
        digitalWrite(outPins[i], LOW); // set i high to test that pin
        
        for (int j = 0; j < pinCount; j++) { // iterate through input pins and test for high or low state
          if (digitalRead(inPins[j]) == HIGH && pinMask[i][j] == false) { //If nothing is expected and the pin is high, continue
            continue;
          }
          else if (digitalRead(inPins[j]) == LOW && pinMask[i][j] == true) { //If the mask expects a true value and the pin is low, light the green led
            leds[i].gOn();
            leds[i+10].gOn();
          }
          else if (digitalRead(inPins[j]) == HIGH && pinMask[i][j] == true) { // Else, if the mask expects a true value and the pin is high,
            for (int k = 0; k <= pinCount; k++) {                             // iterate through inPins and check if any of them are low, 
              if (digitalRead(inPins[k]) == LOW) {                            // turning on the blue led and exiting the loop if they are
                leds[i].bOn();                                                // as this indicates a crossed cable.
                leds[k+10].bOn();
                break;
              }
              else if (digitalRead(inPins[k]) == HIGH && k < pinCount) {
                continue;
              }
              else if (digitalRead(inPins[k]) == HIGH && k == pinCount) { // If the last inPin is reached and no pins have been low,
                digitalWrite(outPins[i], HIGH);                           // invert outPin polarity so the pin being tested is now high and the rest are low.
                for (int m = 0; m < 10; m++) {
                  if (m == i) {
                    continue;
                  }
                  else {
                    digitalWrite(outPins[m], LOW);
                  }
                }
                for (int n = 0; n <= pinCount; n++) { // Iterate again through the inPins. 
                  if (n == i) {
                    continue;
                  }
                  else if (digitalRead(inPins[n]) == HIGH && n < pinCount) { // If any are high this means current is leaking onto them and indicates a short.
                    leds[i].rOn();
                    leds[n+10].rOn();
                  }
                  else if (digitalRead(inPins[n]) == LOW && n < pinCount) {
                    continue;
                  }
                  else if (digitalRead(inPins[n]) == LOW && n == pinCount) { // If no pins are high by the end of the loop, 
                    leds[i].rOn();                                           // this indicates that there is just a break on the pin being tested
                    leds[i+10].rOn();
                  }
                }
                digitalWrite(outPins[i], LOW); // Revert outPin polarity at the end of this if statement ready to test the next pin
                for (int p = 0; p < 10; p++) {
                  if (p == i) {
                    continue;
                  }
                  else {
                    digitalWrite(outPins[p], HIGH);
                  }
                }
              }
            }
          }
          else if (digitalRead(inPins[j]) == LOW && pinMask[i][j] == false) {
            continue;
          }
        }
        digitalWrite(outPins[i], HIGH);
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

    int cableType() {
      for (int i = 0; i < 10; i++) {
        if (digitalRead(dipPins[i]) == LOW) {
          return (i);
        }
      }
    }

    void dipCheck() { //Optional calls for the remaining 6 dip switches after the first 10 that determin cable type
      if (digitalRead(dipPins[10]) == LOW) {
        ledCheck(); //Checks each led element is functional
      }
      if (digitalRead(dipPins[11]) == LOW) {
        ledChase(); //Produces a chasing led pattern
      }
      if (digitalRead(dipPins[12]) == LOW) {
        //do something
      }
      if (digitalRead(dipPins[13]) == LOW) {
        //do something
      }
      if (digitalRead(dipPins[14]) == LOW) {
        //do something
      }
      if (digitalRead(dipPins[15]) == LOW) {
        //do something
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