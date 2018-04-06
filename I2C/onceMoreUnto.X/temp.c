This is Google's cache of http://design.stanford.edu/spdl/ME218c/pastprojects/0708/flotillasaurus/Appendix/Software/Helm/UART.c. It is a snapshot of the page as it appeared on Mar 18, 2018 10:04:45 GMT.
The current page could have changed in the meantime. Learn more
Tip: To quickly find your search term on this page, press Ctrl+F or ⌘-F (Mac) and use the find bar.

#include <p30f4013.h>

#include "UART.h"

#include "pd.h"



#define NUM_OF_PER_XMIT		12 //~0.21s = (4/120Mhz)*8*2^16*12



void __attribute__((__interrupt__)) _T2Interrupt(void);

void __attribute__((__interrupt__)) _U1RXInterrupt(void);





//Receive variables

static unsigned char receivedBytes, junk, checkSum;

static unsigned char sourceAddressHi, sourceAddressLo;

static unsigned char userData0, userData1, userData2;

static unsigned char rxInProcessFlag;

static unsigned char api;



//Send variables

static unsigned char sendDestAddressHi, sendDestAddressLo;

static unsigned char sendUserData0, sendUserData1, sendUserData2;

static unsigned char txInProcessFlag;

static unsigned char numTimerOverflow;



void InitUART(void)

{

	

 	U1MODEbits.USIDL = 0;				/* Continue in Idle mode */

 	U1MODEbits.ALTIO = 0;				/* Use standard U1TX and U1RX pins */

 	U1MODEbits.LPBACK = 0;				/* Disable LoopBack */

    U1MODEbits.PDSEL = 0b00;			/* 8-bit data, no parity */

    U1MODEbits.STSEL = 0;				/* One Stop bit */

                         

    U1BRG = 172;							/* Baud rate generator value for

											9600 baud rate at Fcy=30MHz */



    //Configure receiving interrupts

    rxInProcessFlag = 0;

    txInProcessFlag = 0;

    receivedBytes = 0;					/* Initialize byte counter */

    IFS0bits.U1RXIF = 0;				/* Clear interrupt flag */

    IPC2bits.U1RXIP = 7;				/* Set to highest priority */

    IEC0bits.U1RXIE = 1;				/* Enable receive interrupts */

    

    U1MODEbits.UARTEN = 1;            	/* Enable UART module 1 */

    U1STAbits.UTXEN = 1;            	/* Enable transmission, overrides TRIS settings */    

    

    //Init timer 2

	T2CONbits.TCS 	= 0;		//Internal clock source,  120MHz

 	T2CONbits.TGATE = 0;	 	//Gated time accumulation disabled

	T2CONbits.TCKPS = 0b01; 	//1:8 prescale value

	T2CONbits.TSIDL = 0; 		//continue timer operation in idle mode

	T2CONbits.TON = 0;		//turn off TMR2

	TMR2 = 0; 				//reset(clear TMR2)

	PR2 = 0xFFFF; 			//set PR2 to basically check for overflow

	numTimerOverflow = 0;	//Initialize overflow counter

	T2CONbits.TON = 1;		//turn on TMR2

	IPC1bits.T2IP = 6;		/* Set to lower priority */

	IFS0bits.T2IF = 0;		//clear TMR2IR flag

	

	IEC0bits.T2IE = 1;		//Enable TMR2 interrupts

}



//Receive functions

unsigned char GetAddressHi(void)

{ 

	unsigned char temp = sourceAddressHi;

	sourceAddressHi = 0;  //clear buffer after data is taken

	return temp; 

}



unsigned char GetAddressLo(void)

{ 

	unsigned char temp = sourceAddressLo;

	sourceAddressLo = 0;  //clear buffer after data is taken

	return temp; 

}



unsigned char GetHeaderByte(void)

{ 

	unsigned char temp = userData0;

	userData0 = 0;  //clear buffer after data is taken

	return temp; 

}



unsigned char GetNavigationByte(void)

{ 

	unsigned char temp = userData1;

	userData1 = 0;  //clear buffer after data is taken

	return temp; 

}

unsigned char GetSpecialActionByte(void)

{ 

	unsigned char temp = userData2;

	userData2 = 0;  //clear buffer after data is taken

	return temp; 

}

unsigned char GetUserByte0(void)

{ 

	unsigned char temp = userData0;

	userData0 = 0;  //clear buffer after data is taken

	return temp; 

}

unsigned char GetUserByte1(void)

{ 

	unsigned char temp = userData1;

	userData1 = 0;  //clear buffer after data is taken

	return temp; 

}

unsigned char GetUserByte2(void)

{ 

	unsigned char temp = userData2;

	userData2 = 0;  //clear buffer after data is taken

	return temp; 

}

unsigned char RXInProcess(void){ return rxInProcessFlag; }



void __attribute__((__interrupt__)) _U1RXInterrupt(void)

{

	IFS0bits.U1RXIF = 0;				/* Clear interrupt flag */



	receivedBytes++;					/* Increment byte counter */

	rxInProcessFlag = 1;

	

	switch(receivedBytes)

	{

		case 1:

			junk = U1RXREG;

			if(junk != START_BYTE)		//Make sure the first byte is good before we move on.

			{

				receivedBytes = 0;

				rxInProcessFlag = 0;

			}

			break;

				

		case 2:

			junk = U1RXREG;

			break;

				

		case 3:

		 	junk = U1RXREG;

			break;

				

		case 4:

			api = U1RXREG;

			break;

				

		case 5:

			sourceAddressHi = U1RXREG;

			break;

				

		case 6:

			sourceAddressLo = U1RXREG;

			break;

				

		case 7:

			junk = U1RXREG;

			break;

				

		case 8:

			junk = U1RXREG;

			break;

				

		case 9:

			userData0 = U1RXREG;

			break;

				

		case 10:

			userData1 = U1RXREG;

			break;

				

		case 11:

			userData2 = U1RXREG;

			break;

				

		case 12:

			junk = U1RXREG;

//			checkSum = 0xFF - (api+frameID+destAddressHi+destAddressLo+userData0+userData1+userData2);

//			if(junk != checkSum)		//If checksum doesn't match

				receivedBytes = 0;		//then start over

//			else						//else

				rxInProcessFlag = 0;	//end transmission

			break;

	}

}





//Send functions

unsigned char TXInProcess(void){ return txInProcessFlag; }

void SetDestAddressHi(unsigned char data){ sendDestAddressHi = data; }

void SetDestAddressLo(unsigned char data){ sendDestAddressLo = data; }

void SetHeaderByte(unsigned char data){ sendUserData0 = data; }

void SetNavigationByte(unsigned char data){ sendUserData1 = data; }

void SetSpecialActionByte(unsigned char data){ sendUserData2 = data; }

void SetUserData0(unsigned char data){ sendUserData0 = data; }

void SetUserData1(unsigned char data){ sendUserData1 = data; }

void SetUserData2(unsigned char data){ sendUserData2 = data; }	





//At every 0.2s, we will begin a transfer

void __attribute__((__interrupt__)) _T2Interrupt(void)

{

	IFS0bits.T2IF = 0;			//clear TMR2 flag

	numTimerOverflow++;			//increment overflows

	

	if(numTimerOverflow == NUM_OF_PER_XMIT)

	{

		txInProcessFlag = 1;

		

		U1TXREG = START_BYTE;				/* Write data to lower byte of U1TXREG */

											/* Ignore the upper byte, it won't send */

		while(U1STAbits.UTXBF == 1);		//Wait until transmit is complete

		

		U1TXREG = LENGTH_MSB;				/* Write data to lower byte of U1TXREG */

											/* Ignore the upper byte, it won't send */

		while(U1STAbits.UTXBF == 1);		//Wait until transmit is complete

	

		U1TXREG = LENGTH_LSB;				/* Write data to lower byte of U1TXREG */

											/* Ignore the upper byte, it won't send */

		while(U1STAbits.UTXBF == 1);		//Wait until transmit is complete

	

		U1TXREG = API_TX;					/* Write data to lower byte of U1TXREG */

											/* Ignore the upper byte, it won't send */

		while(U1STAbits.UTXBF == 1);		//Wait until transmit is complete

	

		U1TXREG = FRAME_ID;					/* Write data to lower byte of U1TXREG */

											/* Ignore the upper byte, it won't send */

		while(U1STAbits.UTXBF == 1);		//Wait until transmit is complete

	

		U1TXREG = sendDestAddressHi;		/* Write data to lower byte of U1TXREG */

											/* Ignore the upper byte, it won't send */

		while(U1STAbits.UTXBF == 1);		//Wait until transmit is complete

	

		U1TXREG = sendDestAddressLo;		/* Write data to lower byte of U1TXREG */

											/* Ignore the upper byte, it won't send */

		while(U1STAbits.UTXBF == 1);		//Wait until transmit is complete

	

		U1TXREG = 0x00;						/* Write data to lower byte of U1TXREG */

											/* Ignore the upper byte, it won't send */

		while(U1STAbits.UTXBF == 1);		//Wait until transmit is complete

	

		U1TXREG = sendUserData0;			/* Write data to lower byte of U1TXREG */

											/* Ignore the upper byte, it won't send */

		while(U1STAbits.UTXBF == 1);		//Wait until transmit is complete

	

		U1TXREG = sendUserData1;			/* Write data to lower byte of U1TXREG */

											/* Ignore the upper byte, it won't send */

		while(U1STAbits.UTXBF == 1);		//Wait until transmit is complete

	

		U1TXREG = sendUserData2;			/* Write data to lower byte of U1TXREG */

											/* Ignore the upper byte, it won't send */

		while(U1STAbits.UTXBF == 1);		//Wait until transmit is complete

	

		checkSum = API_TX + FRAME_ID + sendDestAddressHi + sendDestAddressLo + sendUserData0 + sendUserData1 + sendUserData2;

		checkSum = 0xFF - checkSum;

		U1TXREG = checkSum;					/* Write data to lower byte of U1TXREG */

											/* Ignore the upper byte, it won't send */

		while(U1STAbits.UTXBF == 1);		//Wait until transmit is complete

	

		sendDestAddressHi = 0;				//Clear buffer after send

		sendDestAddressLo = 0;				//Clear buffer after send

		sendUserData0 = 0;					//Clear buffer after send

		sendUserData1 = 0;					//Clear buffer after send

		sendUserData2 = 0;					//Clear buffer after send

	

		txInProcessFlag = 0;

		numTimerOverflow = 0;			//reset counter

	}

}

