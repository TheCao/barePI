#ifndef Mailbox_h
#define Mailbox_h

#include "DataTypes.h"

// Mailbox registers
static VUINT32 *MAILBOX0_READ	= (UINT32 *) 0x3f00B880;	//0x00
static VUINT32 *MAILBOX0_POLL	= (UINT32 *) 0x3f00B890;	//0x10
static VUINT32 *MAILBOX0_SENDER	= (UINT32 *) 0x3f00B894;	//0x14
static VUINT32 *MAILBOX0_STATUS	= (UINT32 *) 0x3f00B898;	//0x18
static VUINT32 *MAILBOX0_CONFIG	= (UINT32 *) 0x3f00B89C;	//0x1C
static VUINT32 *MAILBOX0_WRITE	= (UINT32 *) 0x3f00B8A0;	//0x20

// Useful defines
#define MAILBOX_FULL		0x80000000
#define MAILBOX_EMPTY		0x40000000

#define MAILBOX_FB			0x00000001
#define MAILBOX_TAG			0x00000008

// function prototypes
VOID 	MailboxWrite(UINT32 message, UINT8 channel);
UINT32  mbox_read(UINT32 channel);
UINT32	MailboxRead(UINT32 channel);

#endif
