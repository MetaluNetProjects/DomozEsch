#include "rf.h"

struct s_myData {
  int16_t quat[4];
  int16_t souffle;
  unsigned char keys;
};

void setup() 
{
  rfSetup();
}

void getKey(int pin, int num)
{
  pinMode(pin, INPUT_PULLUP);
  pinMode(pin, INPUT);
  bitWrite(myData->keys, num, digitalRead(pin));
}

void aquire() {
  bnoGetQuat(&(myData->quat));
  myData->souffle = analogRead(A0);
  getKey(5, 0);
  getKey(6, 1);
  getKey(A4, 2);
  getKey(10, 3);
  getKey(11, 4);
  getKey(A1, 5);
  getKey(A2, 6);
  getKey(A3, 7);
}


void loop() {
  aquire();
  rfService();
}
