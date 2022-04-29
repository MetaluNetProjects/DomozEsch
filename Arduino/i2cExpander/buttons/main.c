/*********************************************************************
 *               i2c slave example for Versa2.0
 *               emulates a 255 bytes i2c ram
 *********************************************************************/

#define BOARD Versa2

#include <fruit.h>
#include <analog.h>
#include <i2c_slave.h>

t_delay mainDelay;
byte i2cAddress = 0x10;
#define LOWSPEED_REG 10
#define ADCTHRES_REG 11
#define TIME_REG     12
#define adcThres (i2c_regs[ADCTHRES_REG] << (ANALOG_FILTER + 2))
#define setAdcThres(t) do { i2c_regs[ADCTHRES_REG] = (t >> (ANALOG_FILTER + 2));} while(0)
#define timeReg (i2c_regs[TIME_REG])

#define FUNC_BUTTONS(func) do {\
	func(0, 0, 0); \
	func(0, 1, 1); \
	func(0, 2, 2); \
	func(0, 3, 3); \
	func(0, 4, 4); \
	func(0, 5, 5); \
	func(0, 6, 6); \
	func(0, 7, 7); \
	func(1, 0, 8); \
	func(1, 1, 9); \
	func(1, 2, 10); \
	func(1, 3, 11); \
	func(1, 4, 12); \
} while(0)

//----------- Setup ----------------
#define CONFIG_BUTTON(reg, bit, num) analogSelect(num, BUT##num)
void setup(void) {
	fruitInit();
	// setup I2C slave
	i2c_slave_setup(i2cAddress);
	//i2c_regs[0] = 123;
	delayStart(mainDelay, 5000); 	// init the mainDelay to 5 ms

	analogInit();
	FUNC_BUTTONS(CONFIG_BUTTON);
	setAdcThres(1000);
}

#define PRECONF_BUTTON(reg, bit, num) do { \
	if(chan == num) { \
		digitalSet(BUT##num); \
		pinModeDigitalOut(BUT##num); \
		pinModeAnalogIn(BUT##num); \
	}} while(0)

void analogChannelPreconf(unsigned char chan)
{
	FUNC_BUTTONS(PRECONF_BUTTON);
}

#define GET_ANALOG_BUTTON(reg, bit, num) do {\
	if(analogGet(num) > adcThres) bitclr(i2c_regs[reg], bit); else bitset(i2c_regs[reg], bit); \
} while(0)

void getButtons()
{
	FUNC_BUTTONS(GET_ANALOG_BUTTON);
}

void checkCpuSpeed()
{
	static byte cpuLowSpeed = 0;
	if(cpuLowSpeed != (i2c_regs[LOWSPEED_REG] == 255)) {
		cpuLowSpeed = (i2c_regs[LOWSPEED_REG] == 255);
		if(cpuLowSpeed) {
			SSP1CON1 = 0;
			OSCTUNEbits.PLLEN = 0;
			i2c_slave_setup(i2cAddress); // set i2c address to 0x10
			fraiseSetInterruptEnable(0);
		}
		else {
			setup();
		}
	}
}

// ---------- Main loop ------------
byte state = 0;
byte count;

void loop() {
	fraiseService();	// listen to Fraise events
	analogService();
	if(delayFinished(mainDelay)) // when mainDelay triggers :
	{
		delayStart(mainDelay, 5000); 	// re-init mainDelay
		checkCpuSpeed();
		getButtons();
		if(++count == 20) {
			count = 0;
			timeReg = timeReg + 1;
		}
	}
}

// Interrupts
void lowInterrupts()
{
	i2c_slave_ISR();
}

void fraiseReceive()
{
	byte c, v;
	c = fraiseGetChar();

	if(c == 1) { // read
		c = fraiseGetChar();
		printf("CR %d %d\n", c, i2c_regs[c]);
	} else 
	if(c == 2) { // write
		c = fraiseGetChar();
		v = fraiseGetChar();
		i2c_regs[c] = v;
	}
}
