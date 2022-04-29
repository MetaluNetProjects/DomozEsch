/*********************************************************************
 *               analog example for Versa2.0
 *	Analog capture on connectors K1, K2, K3 and K5. 
 *********************************************************************/

#define BOARD Versa1

#include <fruit.h>
#include <analog.h>

t_delay mainDelay;

byte inc1, inc2, inc;

void setup(void) {	
//----------- Setup ----------------
	fruitInit();
			
	pinModeDigitalOut(LED); 	// set the LED pin mode to digital out
	digitalClear(LED);		// clear the LED
	delayStart(mainDelay, 5000); 	// init the mainDelay to 5 ms

//----------- Analog setup ----------------
	analogInit();		// init analog module
	//analogInitTouch();	// enable capacitive touch (for K5)
	
	analogSelect(0,AN0);
	analogSelect(1,AN1);
	analogSelect(2,AN2);
	analogSelect(3,AN3);
	analogSelect(4,AN4);
	analogSelect(5,AN5);
	analogSelect(6,AN6);
	analogSelect(7,AN7);
	analogSelect(8,AN8);
	
	//analogSelect(TOUCH1_CHAN, TOUCH1_PIN);
	//analogSelectTouch(3,K5); // assign connector K5 to analog channel 3, but configure it for capacitive touch measurement.

	//pinModeDigitalIn(INC1);
	//pinModeDigitalIn(INC2);
	
	//pinModeDigitalOut(STOUCH);
	//digitalSet(STOUCH);
}

#define ACCONF(i) do{ \
	if(chan == i) { \
		digitalSet(AN##i); \
		pinModeDigitalOut(AN##i); \
		pinModeAnalogIn(AN##i); \
	} } while(0)

void analogChannelConfig(unsigned char chan) {
	ACCONF(0);
	ACCONF(1);
	ACCONF(2);
	ACCONF(3);
	ACCONF(4);
	ACCONF(5);
	ACCONF(6);
	ACCONF(7);
	ACCONF(8);
}

/*unsigned int scount;
unsigned int stouchService()
{
	//pinModeDigitalIn(STOUCH);
	if(digitalRead(STOUCH)) scount++;
	//pinModeDigitalOut(STOUCH);
	//digitalClear(STOUCH);
	//pinModeDigitalIn(STOUCH);
}*/


void loop() {
// ---------- Main loop ------------
	fraiseService();	// listen to Fraise events
	analogService();	// analog management routine
	//incService();
	//stouchService();
	
	if(delayFinished(mainDelay)) // when mainDelay triggers :
	{
		delayStart(mainDelay, 20000); 	// re-init mainDelay
		analogSend();		// send analog channels that changed
		/*printf("CS %d\n", scount);
		scount = 0;
		pinModeDigitalOut(STOUCH);
		digitalSet(STOUCH);
		pinModeDigitalIn(STOUCH);*/
	}
}

// Receiving

void fraiseReceiveChar() // receive text
{
	unsigned char c;
	
	c=fraiseGetChar();
	if(c=='L'){		//switch LED on/off 
		c=fraiseGetChar();
		digitalWrite(LED, c!='0');		
	}
	else if(c=='E') { 	// echo text (send it back to host)
		printf("C");
		c = fraiseGetLen(); 			// get length of current packet
		while(c--) printf("%c",fraiseGetChar());// send each received byte
		putchar('\n');				// end of line
	}	
}

