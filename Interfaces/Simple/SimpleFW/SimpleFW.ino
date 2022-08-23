#include "rf.h"
//#include <Wire.h>
//#include <Adafruit_I2CDevice.h>

// max 14 bits
struct s_myData {
  int16_t quat[4];
  int16_t acc[3];
};

void setup() 
{
  rfSetup();
}


void aquire() {
  bnoGetQuat(&(myData->quat));  
  bnoGetAcc(&(myData->acc));  
}


void loop() {
  aquire();
  rfService();
}
