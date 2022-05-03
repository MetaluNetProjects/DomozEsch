#include "rf.h"
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
Adafruit_I2CDevice *i2c_dev = new Adafruit_I2CDevice(0x10);

// max 14 bits
struct s_myData {
  int16_t quat[4];
  unsigned char knobs[3];
  unsigned char bend;
  uint8_t keys;
  uint8_t expkey1;
  uint8_t expkey2;
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
  bitWrite(myData->keys, num, digitalRead(pin));
}

byte read8(byte addr) {
  uint8_t buffer[1] = {addr};
  i2c_dev->write_then_read(buffer, 1, buffer, 1);
  return (byte)buffer[0];
}

void aquire() {
  bnoGetQuat(&(myData->quat));  
  myData->bend = analogRead(A0) - 800;
  myData->knobs[0] = analogRead(A2) >> 2;
  myData->knobs[1] = analogRead(A3) >> 2;
  myData->knobs[2] = analogRead(A4) >> 2;
  getKey(5, 0);
  getKey(6, 1);
  getKey(A1, 2);
  getKey(10, 3);
  getKey(11, 4);
  //getKey(13, 5); // LED...
  getKey(12, 5);
  //getKey(0, 6);
  //getKey(1, 7);

  int but = analogRead(A5);
  //Serial.println(but*1);
  if(but > 700) but = 2;
  else if(but < 200) but = 0;
  else but = 1;
  bitWrite(myData->keys, 6, bitRead(but, 0));
  bitWrite(myData->keys, 7, bitRead(but, 1));

  /*if(Wire.requestFrom(0x10, 10) != 10){
      Serial.println("error i2c 0.x10");
  }
  else {
     byte c;
     c = Wire.read();
     Serial.print((int)c);
     c = Wire.read();
     Serial.print((int)c);
     c = Wire.read();
     Serial.print((int)c);
     c = Wire.read();
     Serial.print((int)c);
     c = Wire.read();
     Serial.print((int)c);
     c = Wire.read();
     Serial.print((int)c);
     c = Wire.read();
     Serial.print((int)c);
     c = Wire.read();
     Serial.println((int)c);
     myData->expkey1 = c;
  }*/
  /*Serial.print("i2c_dev byte 0: "); Serial.println(read8(0));
  Serial.print("i2c_dev byte 1: "); Serial.println(read8(1));
  Serial.print("i2c_dev byte 2: "); Serial.println(read8(2));*/
  myData->expkey1 = read8(0);
}


void loop() {
  aquire();
  rfService();
}
