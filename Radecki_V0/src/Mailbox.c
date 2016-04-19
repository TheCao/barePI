#include "Mailbox.h"
#include "raspi.h"
#include "SystemTimer.h"
#include "GPIO.h"

VOID MailboxWrite(UINT32 message, UINT8 channel)
{
	// validate inputs
	if(message & 0xF){
		return;
	}
	
	if(channel > 0xF){
		return;
	}

	// inputs are valid, we may continue
	// wait for MAILBOX0_STATUS to indicate not full
	while(*MAILBOX0_STATUS & MAILBOX_FULL){
	}

	MemoryBarrier;
	// write the message to the to the mailbox
	*MAILBOX0_WRITE = (message | channel);
	return;
}

UINT32 MailboxRead(UINT32 channel)
{
	UINT32 message = 0;

	// Validate input
	if(channel > 15){
		return 0x1; // can be looked for as an error
					// a good message will not have LSB set
	}

	do{
		// Wait for a message
		while(*MAILBOX0_STATUS & MAILBOX_EMPTY){
		}
		
		MemoryBarrier();
		message = *MAILBOX0_READ;
		MemoryBarrier();
	
										 // find channel message is on
	} while((message & 0xF) != channel); // check against wanted channel

	return (message & 0xFFFFFFF0); //the upper 28-bits
}
