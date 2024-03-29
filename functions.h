    #ifndef _HF_
    #define _HF_
    #include "Arduino.h"

/*===================================================================================================================*/

    extern const int latchPin;
    extern const int clockPin;
    extern const int dataPin;
    extern const int outEnable;    

    extern const int outPins[10];

    extern const int inPins[10];

    extern const int dipPins[16];

    extern byte ledRegisters[8];

    extern const bool pinMask[10][10];

    extern bool pinRegisters[10][10];
/*===================================================================================================================*/

    class LED {
      int rBit;
      int gBit;
      int bBit;
      int rReg;
      int gReg;
      int bReg;

      public:

      LED(int redBit, int greenBit, int blueBit, int redReg, int greenReg, int blueReg);

      void rOn();
      void rOff();
      void gOn();
      void gOff();
      void bOn();
      void bOff();
    };

    extern LED leds[20];

/*===================================================================================================================*/
 
    void updateRegisters();
    
    void ledCheck();

    void ledChase();

    void pinTest(int pinCount);

    void checkRegisters();

    void clearRegisters();

    int cableType();

    void dipCheck();

    #endif