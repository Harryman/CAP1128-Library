/*Developed by:
/ Harrison Stahl https://github.com/Harryman
/ The Everything Corp
//// Please Donate if you find this useful: 151sgJ4z2tNGzVFJHGyBAd8H4F4RT62ebQ

*/

#include "SPI.h"
#include "CAP1128.h"
#include "Arduino.h"


CAP1128::CAP1128(uint8_t csPin){//figure out what is required for pins
	_csPin = csPin;
	pinMode(_csPin, OUTPUT);
	digitalWrite(_csPin,HIGH);
	SPI.begin();
}


/////////////// COMM functions 

void CAP1128::transInit(uint8_t reg){
	SPI.setBitOrder(MSBFIRST);
	digitalWrite(_csPin, LOW);
	pointReg(reg);
}

void CAP1128::endTrans(){
	digitalWrite(_csPin, HIGH);
}

void CAP1128::pointReg(uint8_t reg){
	SPI.transfer(0x7D);
	SPI.transfer(reg);
}

uint8_t CAP1128::writeCom(uint8_t message){// write command only not complete transaction
	SPI.transfer(0x7E);
	ret = SPI.transfer(message);
	return ret;
}

uint8_t CAP1128::writeReg(uint8_t reg, uint8_t message){ // full write transaction, returns old val from reg
	transInit(reg);
	SPI.transfer(0x7E);
	ret = SPI.transfer(message);
	digitalWrite(_csPin,HIGH);
	return ret;
}

uint8_t CAP1128::readCom(uint8_t reg){// read command only not complete transaction
	SPI.transfer(0x7E);
	ret = SPI.transfer(message);
	return ret;
}

uint8_t CAP1128::readReg(uint8_t reg, uint8_t message){ //full command transaction self contained
	transInit(reg);
	SPI.transfer(0x7F);
	ret = SPI.transfer(0x7F);
	digitalWrite(_csPin,HIGH);
	return ret;
}
 
	//reg address, data(not shifted), length in bits, starting bit 0 indexed from LSB
uint8_t CAP1128::setBits(uint8_t reg, uint8_t bits, uint8_t len, uint8_t start){
	uint8_t offset = (start+1)-len; // determines how far the data has to be shifted
	uint8_t mask = ((1 << len) - 1) << offset;// creates a bit mask for the data
	bits = bits << offset;// shifts down to the appropriate spot
	bits &= mask;// maskes of the data so that way if invalid data will be truncated

	transInit(reg);//start trans
	uint8_t old = readCom(reg);//gets old reg value
	old &= ~mask; //clears bits that will be changed
	old |= bits; //sets the data bits while preserving the old data
	pointReg(reg); // resets the address pointer 
	ret = writeCom(old); // writes the new value to the register 
	endTrans();
	return ret; // returns old value
}







////////////////// utility  functions
void CAP1128::reset(){
	SPI.setBitOrder(MSBFIRST);
	digitalWrite(_csPin, LOW);
	SPI.transfer(0x7A);
	SPI.transfer(0x7A);
	digitalWrite(_csPin, HIGH);
}


uint8_t CAP1128::prodID(){
	return readReg(0xfd);
}

uint8_t CAP1128::manufactID(){
	return readReg(0xfe);
}

uint8_t CAP1128::revID(){
	return readReg(0xff);
}

////////////// core functionality functions 
uint8_t CAP1128::genStat();
	return readReg(0x2);
}

uint8_t CAP1128::btnStat();
	return readReg(0x3);
}

uint8_t CAP1128::noiseFlag(){
	return readReg(0xa);
}

int8_t CAP1128::deltaCount(uint8_t btn){//btn 1-8 returns delta
	return readReg(0xF+btn);
}

uint8_t CAP1128::clrInt(){ //clears interrupt pin 
	return setBits(0,0,1,0);
}

uint8_t CAP1128::stby(bool state){ //puts into stby or wake
	return setBits(0,(uint8_t)state,1,5);
}

uint8_t CAP1128::dSleep(bool state){ // puts into deep sleep or wake
	return setBits(0,(uint8_t)state,1,4);
}






////////////// configuration functions
uint8_t CAP1128::repeatRate(uint8_t rate){//0-15 maps to 35-560ms
	return setBits(0x22,rate,4,3);
}

uint8_t CAP1128::minPress(uint8_t time){//0-15 maps to 35-560ms
	return setBits(0x23,time,4,3);
}


////////////////  sensitivity & noise Configuration

uint8_t CAP1128::setMainGain(uint8_t gain){//0-3 maps to 1-8 default 0
	return setBits(0,gain,2,7);
}

uint8_t CAP1128::deltaSense(uint8_t sense){//0-7 maps to 128-1 higher, higher sensitivity
	sense ~= sense;
	return setBits(0x1f, sense,3,6);
}

uint8_t CAP1128::avgSamples(uint8_t samples){//0-4 maps to 1-128 samples default 8
	return setBits(0x24,samples,3,6);	
}

uint8_t CAP1128::sampleDur(uint8_t dur){//0-4 maps to 320-2560us
	return setBits(0x24,dur,2,3);
}

uint8_t CAP1128::cycleTime(uint8_t time){//0-4 maps to 35-140ms
	return setBits(0x24,time,2,1);
}

uint8_t CAP1128::recalNoiseThres(uint8_t thres){//0-4 maps to 25-62.5%
	return setBits(0x38,thres,2,1);
}




/////////// BTN specific configuration
uint8_t CAP1128::inputEnable(uint8_t config){// each bit number corresponds with the same input number
	return writeReg(0x21, config);
}

uint8_t CAP1128::intEnable(uint8_t map){//maps to btn numbers to bit numbers 1 is enable
	return writeReg(0x27,map);
}

uint8_t CAP1128::repeatEnable(uint8_t map){//maps to btn numbers to bit numbers 1 is enable
	return writeReg(0x28,map);
}

uint8_t CAP1128::btnThres(uint8_t thres, uint8_t btn){//0-127 delta count threshold for touch detection
	return writeReg(0x2f+btn, thres);
}




////////////  Multi touch settings ///////////////////
uint8_t CAP1128::maxTouches(uint8_t num){//0-3 maps to 1-4 btn presses 
	return setBits(0x2a,num,2,3);
}

uint8_t CAP1128::multiEnable(bool en){//true or false
	return setBits(0x2b,en,1,7);
}

uint8_t CAP1128::multiThres(uint8_t thres){// 0-2 maps to 12.5, 25, 37.5, 100%
	return setBits(0x2b,thres,2,3);
}

uint8_t CAP1128::multiIntEnable(uint8_t en){//true or false
	return setBits(0x2b,en,1,0);
}

uint8_t CAP1128::multiPtrn(uint8_t map){//maps to btn numbers to bit numbers 1 is enable
	return writeReg(0x2d,map);
}
