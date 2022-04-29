#include "rf.h"
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
Adafruit_I2CDevice *i2c_dev = new Adafruit_I2CDevice(0x10);

// max 15 bits
struct s_myData {
  int16_t quat[4];        //  8   8
  unsigned char knobs[3]; //  3   11
  uint8_t keys[2];        //  2   13
  uint8_t incr[2];        //  2   15
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
  pinMode(pin, INPUT);
  //bitWrite(myData->keys, num, digitalRead(pin));
}

byte read8(byte addr) {
  uint8_t buffer[1] = {addr};
  i2c_dev->write_then_read(buffer, 1, buffer, 1);
  return (byte)buffer[0];
}

void aquire() {
  bnoGetQuat(&(myData->quat));  
  myData->knobs[0] = analogRead(A0) >> 2;
  myData->knobs[1] = analogRead(A1) >> 2;
  myData->knobs[2] = analogRead(A2) >> 2;
  /*getKey(5, 0);
  getKey(6, 1);
  getKey(A1, 2);
  getKey(10, 3);
  getKey(11, 4);
  //getKey(13, 5); // LED...
  getKey(12, 5);
  //getKey(0, 6);
  //getKey(1, 7);*/

  myData->keys[0] = read8(0);
  myData->keys[1] = read8(1);

  int but = analogRead(A3);
  if(but > 700) but = 2;
  else if(but < 200) but = 0;
  else but = 1;
  //bitWrite(myData->keys[1], 6, bitRead(but, 0));
  bitWrite(myData->keys[1], 7, bitRead(but, 1));

  myData->incr[0] = read8(2);
  myData->incr[1] = read8(3);
  
  byte incr2 = read8(4);
  bitWrite(myData->keys[0], 7, bitRead(incr2, 6));
  bitWrite(myData->keys[0], 3, bitRead(incr2, 5));
  bitWrite(myData->keys[1], 6, bitRead(incr2, 4));
  bitWrite(myData->keys[1], 5, bitRead(incr2, 3));
  bitWrite(myData->keys[1], 4, bitRead(incr2, 2));
  bitWrite(myData->incr[1], 7, bitRead(incr2, 1));
  bitWrite(myData->incr[0], 7, bitRead(incr2, 0));
}

void loop() {
  aquire();
  rfService();
}
