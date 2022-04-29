/*********************************************************************
 *               analog example for Versa2.0
 *	Analog capture on connectors K1, K2, K3 and K5. 
 *********************************************************************/

#define BOARD Versa2

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
	analogInitTouch();	// enable capacitive touch (for K5)
	
	analogSelect(0,K1);	// assign connector K1 to analog channel 0
	analogSelect(1,K2);
	analogSelect(TOUCH1_CHAN, TOUCH1_PIN);
	analogSelectTouch(3,K5); // assign connector K5 to analog channel 3, but configure it for capacitive touch measurement.

	pinModeDigitalIn(INC1);
	pinModeDigitalIn(INC2);
	
	pinModeDigitalOut(STOUCH);
	digitalSet(STOUCH);
}

void incService()
{
	byte oldinc = inc;
	if(digitalRead(INC1) != inc1) {
		inc1 = digitalRead(INC1);
		if(inc1) { if(inc2) inc++; else inc--;}
		else { if(inc2) inc--; else inc++;}
	}
	if(digitalRead(INC2) != inc2) {
		inc2 = digitalRead(INC2);
		if(inc2) { if(!inc1) inc++; else inc--;}
		else { if(!inc1) inc--; else inc++;}
	}
	
	if(inc != oldinc) printf("Ci %d\n", inc);
}

void analogChannelConfig(unsigned char chan) {
	if(chan == TOUCH1_CHAN) {
		digitalSet(TOUCH1_PIN);
		pinModeDigitalOut(TOUCH1_PIN);
		pinModeAnalogIn(TOUCH1_PIN);
	}
}

unsigned int scount;
void stouchService()
{
	//pinModeDigitalIn(STOUCH);
	if(digitalRead(STOUCH)) scount++;
	//pinModeDigitalOut(STOUCH);
	//digitalClear(STOUCH);
	//pinModeDigitalIn(STOUCH);
}


void loop() {
// ---------- Main loop ------------
	fraiseService();	// listen to Fraise events
	analogService();	// analog management routine
	incService();
	stouchService();
	
	if(delayFinished(mainDelay)) // when mainDelay triggers :
	{
		delayStart(mainDelay, 20000); 	// re-init mainDelay
		analogSend();		// send analog channels that changed
		printf("CS %d\n", scount);
		scount = 0;
		pinModeDigitalOut(STOUCH);
		digitalSet(STOUCH);
		pinModeDigitalIn(STOUCH);
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

