#include "rf.h"
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
Adafruit_I2CDevice *i2c_dev = new Adafruit_I2CDevice(0x10);

// max 14 bits  
struct s_myData {
  int16_t quat[4];
  //unsigned char knobs[3];
  //unsigned char bend;
  uint8_t keys;
  //uint8_t expkey1;
  //uint8_t expkey2;
};

void setup() 
{
  rfSetup();
  if (!i2c_dev->begin()) {
    Serial.println("i2c dev error!");
  } else {
    Serial.println("i2c dev OK");
  }
}

void getKey(int pin, int num)
{
  pinMode(pin, INPUT_PULLUP);
  //pinMode(pin, INPUT);
  bitWrite(myData->keys, num, digitalRead(pin));
}

byte read8(byte addr) {
  uint8_t buffer[1] = {addr};
  i2c_dev->write_then_read(buffer, 1, buffer, 1);
  return (byte)buffer[0];
}

void aquire() {
  bnoGetQuat(&(myData->quat));  

  getKey(5, 3);
  getKey(6, 2);
  getKey(10, 1);
  getKey(11, 0);

  getKey(A0, 7);
  getKey(A1, 6);
  getKey(A2, 5);
  getKey(A3, 4);
}


void loop() {
  aquire();
  rfService();
}
