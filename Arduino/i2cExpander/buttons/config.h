#ifndef _CONFIG_H_
#define _CONFIG_H_

#define I2CSLAVE_PORT 1
#define I2C_REG_SIZE 32

#define BUT0  K1
#define BUT1  K2
#define BUT2  K3
#define BUT3  K4
#define BUT4  K6
#define BUT5  K8

// VNH2:
#define BUT6  MB1
#define BUT7  MBEN
#define BUT8  MBPWM // K9
#define BUT9  MOTB_CURRENT  // K10
#define BUT10 MBEN2
#define BUT11 MB2
//            VDD
//            GND

#define BUT12 MA1

#define ANALOG_FILTER 1
#define ANALOG_CHANNEL_PRECONF analogChannelPreconf

#endif // _CONFIG_H_

