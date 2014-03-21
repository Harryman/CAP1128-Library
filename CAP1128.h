// file generated using https://github.com/Harryman/Arduino-header-keyword-creator

/*Developed by:
/ Harrison Stahl https://github.com/Harryman
/ The Everything Corp
//// Please Donate if you find this useful: 151sgJ4z2tNGzVFJHGyBAd8H4F4RT62ebQ

*/

#ifndef CAP1128_h
#define CAP1128_h

#include "Arduino.h"

class CAP1128{
  public:
    CAP1128(uint8_t csPin);
    void transInit(uint8_t reg);
    void endTrans();
    void pointReg(uint8_t reg);
    uint8_t writeCom(uint8_t message);// write command only not complete transaction
    uint8_t writeReg(uint8_t reg, uint8_t message); // full write transaction, returns old val from reg
    uint8_t readCom(uint8_t reg);// read command only not complete transaction
    uint8_t readReg(uint8_t reg, uint8_t message); //full command transaction self contained
    uint8_t setBits(uint8_t reg, uint8_t bits, uint8_t len, uint8_t start);
    void reset();
    uint8_t prodID();
    uint8_t manufactID();
    uint8_t revID();
    uint8_t btnStat();
    uint8_t genStat();
    uint8_t noiseFlag();
    int8_t deltaCount(uint8_t btn);//btn 1-8 returns delta
    uint8_t clrInt(); //clears interrupt pin 
    uint8_t stby(bool state); //puts into stby or wake
    uint8_t dSleep(bool state); // puts into deep sleep or wake
    uint8_t repeatRate(uint8_t rate);//0-15 maps to 35-560ms
    uint8_t minPress(uint8_t time);//0-15 maps to 35-560ms
    uint8_t setMainGain(uint8_t gain);//0-3 maps to 1-8 default 0
    uint8_t deltaSense(uint8_t sense);//0-7 maps to 128-1 higher, higher sensitivity
    uint8_t avgSamples(uint8_t samples);//0-4 maps to 1-128 samples default 8
    uint8_t sampleDur(uint8_t dur);//0-4 maps to 320-2560us
    uint8_t cycleTime(uint8_t time);//0-4 maps to 35-140ms
    uint8_t recalNoiseThres(uint8_t thres);//0-4 maps to 25-62.5%
    uint8_t inputEnable(uint8_t config);// each bit number corresponds with the same input number
    uint8_t intEnable(uint8_t map);//maps to btn numbers to bit numbers 1 is enable
    uint8_t repeatEnable(uint8_t map);//maps to btn numbers to bit numbers 1 is enable
    uint8_t btnThres(uint8_t thres, uint8_t btn);//0-127 delta count threshold for touch detection
    uint8_t maxTouches(uint8_t num);//0-3 maps to 1-4 btn presses 
    uint8_t multiEnable(bool en);//true or false
    uint8_t multiThres(uint8_t thres);// 0-2 maps to 12.5, 25, 37.5, 100%
    uint8_t multiIntEnable(uint8_t en);//true or false
    uint8_t multiPtrn(uint8_t map);//maps to btn numbers to bit numbers 1 is enable

    uint8_t ret;
    //????offset;
    //????mask;
    //????old;

  private:

    uint8_t _csPin;

};

#endif