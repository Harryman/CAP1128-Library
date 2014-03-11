/*Developed by:
/ Harrison Stahl https://github.com/Harryman
/ The Everything Corp
//// Please Donate if you find this useful: 151sgJ4z2tNGzVFJHGyBAd8H4F4RT62ebQ
*/


#include "SPI.h"
#include <CAP1128.h>


uint8_t maxD = 0;
uint8_t tmp = 0;

volatile uint8_t btnMask = 0;
volatile bool tog = false;

CAP1128 cap(4);

void setup(){
	Serial.begin(9600);
	attachInterrupt(1, chkbtns, FALLING);
}

void loop(){
	for(uint8_t i = 0;i<6;i++){
		tmp = cap.deltaCount(i)
		if(tmp>maxD){
			maxD = tmp;
		}
	}
	Serial.println(maxD);
	maxD = 0;
	if(tog == true){
		Serial.println(btnMask);
	}
}


void chkbtns(){
	cap.clrInt();
	btnMask = cap.btnStat();
	tog = ~tog;
}
