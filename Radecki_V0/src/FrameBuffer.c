#include "raspi.h"
#include "FrameBuffer.h"
#include "Mailbox.h"
#include "GPIO.h"

// Declarations
struct FrameBufferInfo fb_info;

// Functions
UINT32 InitialiseFrameBuffer()
{
	VUINT32 *mailbuffer = (UINT32 *) BUFFER_ADDRESS;// Buffer
	UINT32  bp;										// Buffer position
	UINT32	position;								// Position
	UINT32  result = 0;								// Read result
	
	// Get the display size (Monitor/TV resolution??)
	bp = 1;
	mailbuffer[bp++] = 0;			// 1 - Request
	mailbuffer[bp++] = 0x40003;		// 2 - Get display size
	mailbuffer[bp++] = 8;			// 3 - Buffer size
	mailbuffer[bp++] = 0;			// 4 - Request size
	mailbuffer[bp++] = 0;			// 5 - Width
	mailbuffer[bp++] = 0;			// 6 - Height
	mailbuffer[bp++] = 0;			// 7 - End tag
	mailbuffer[0]    = bp * 4;		// 0 - Total size

	MailboxWrite((UINT32) mailbuffer, MAILBOX_TAG);
	result = MailboxRead(MAILBOX_TAG);
	
	if(mailbuffer[1] != 0x80000000){
		return 1;
	}
	
	fb_info.Width  = SCREEN_WIDTH;
	fb_info.Height = SCREEN_HEIGHT;
	fb_info.Depth  = GPU_DEPTH;

	if(fb_info.Width == 0 || fb_info.Height == 0){
		return 2;
	}

	bp = 1;
	mailbuffer[bp++] = 0;				// 1 - Request
	
	mailbuffer[bp++] = 0x00048003;		// 2 - Set screen size
	mailbuffer[bp++] = 8;				// 3 - Buffer size
	mailbuffer[bp++] = 8;				// 4 - Size of request
	mailbuffer[bp++] = fb_info.Width;	// 5 - Width
	mailbuffer[bp++] = fb_info.Height;	// 6 - Height

	mailbuffer[bp++] = 0x00048004;		// 7 - Set virtual screen size
	mailbuffer[bp++] = 8;				// 8 - Buffer size
	mailbuffer[bp++] = 8;				// 9 - Size of request
	mailbuffer[bp++] = fb_info.Width;	//10 - Width
	mailbuffer[bp++] = fb_info.Height;	//11 - Width

	mailbuffer[bp++] = 0x00048005;		//12 - Set depth
	mailbuffer[bp++] = 4;				//13 - Buffer size
	mailbuffer[bp++] = 4;				//14 - Size of request
	mailbuffer[bp++] = GPU_DEPTH;				//15 - 32 bpp

	mailbuffer[bp++] = 0x00040001;		//16 - Allocate frame buffer
	mailbuffer[bp++] = 8;				//17 - Buffer size
	mailbuffer[bp++] = 4;				//18 - Size of request
	mailbuffer[bp++] = 32;				//19 - Alignment = 16
	mailbuffer[bp++] = 0;				//20 - FrameBuffer address	
	mailbuffer[bp++] = 0;				//21 - Terminating tag
	
	mailbuffer[0] = bp * 4;				// 0 - Size of buffer

	MailboxWrite((UINT32) mailbuffer, MAILBOX_TAG);
	result = MailboxRead(MAILBOX_TAG);

	if(mailbuffer[1] != 0x80000000){
		return 3;
	}

	position = 2;						// First tag (byte 2)
	while((result = mailbuffer[position])){
		if(result == 0x00040001){		// Last tag
			break;
		}

		// Skip to next tag
		position += 3 + (mailbuffer[position + 1] >> 2);
		// 2 + 3 + (8 / 4) = 7
		// 7 + 3 + (8 / 4) = 12
		//12 + 3 + (4 / 4) = 16
		
		if(position > bp){
			return 4;
		}
	}

	if(mailbuffer[position + 2] != 0x80000008){
		return 5;
	}
	
	fb_info.Pointer    = mailbuffer[position + 3];
	fb_info.ScreenSize = mailbuffer[position + 4];

	if(fb_info.Pointer == 0 || fb_info.ScreenSize == 0){
		return 6;
	}
	
	// Get framebuffer pitch (bytes per line)
	bp = 1;
	mailbuffer[bp++] = 0;
	mailbuffer[bp++] = 0x40008;
	mailbuffer[bp++] = 4;
	mailbuffer[bp++] = 0;
	mailbuffer[bp++] = 0;
	mailbuffer[bp++] = 0;
	mailbuffer[0] = bp * 4;

	MailboxWrite((UINT32) mailbuffer, MAILBOX_TAG);
	result = MailboxRead(MAILBOX_TAG);

	if(mailbuffer[4] != 0x80000004){
		return 7;
	}

	fb_info.Pitch = mailbuffer[5];
	if(fb_info.Pitch == 0){
		return 8;
	}
	
	return 0;
}

VOID TestPattern()
{
	VUINT16 *ptr;
	UINT32 offset;
	UINT16 colour = 0x0000;
	
	for(offset = 0; offset < (fb_info.Width * fb_info.Height); offset++){
		ptr = (UINT16 *)(fb_info.Pointer + offset);
		*ptr = colour++;
	}
	return;
}

VOID DrawPixel(UINT32 x, UINT32 y, UINT16 colour)
{
	VUINT16 *ptr;
	UINT32 offset;
	
	offset = (y * fb_info.Pitch) + x;
	ptr = (UINT16 *)(fb_info.Pointer + offset);
	*ptr = colour;
	
	return;
}

VOID DrawLine(struct pixel Start,struct pixel End)
{
	UINT32 delta_x;
	UINT32 delta_y;
	UINT32 step_x;
	UINT32 step_y;
	UINT32 error;

	if(End.x > Start.x){
		delta_x = End.x - Start.x;
		step_x = 1;
	}
	else{
		delta_x = Start.x - End.x;
		step_x = -1;
	}
	
	if(End.y > Start.y){
		delta_y = End.y - Start.y;
		step_y = 1;
	}
	else{
		delta_y = Start.y - End.y;
		step_y = -1;
	}
	
	error = delta_x - delta_y;
	while((Start.x == End.x + step_x) || (Start.y == End.y + step_y)){
		DrawPixel(Start.x, Start.y, Start.colour);
		if((error * 2) >= -delta_y){
			Start.x += step_x;
			error -= delta_y;
		}
		if((error * 2) <= delta_x){
			Start.y += step_y;
			error += delta_x;
		}
	}
	return;
}

// Primitives to add...
// DrawPixel
// DrawLine
// DrawRect
// DrawTriangle
// DrawCircle

// Features to add...
// FloodFill
