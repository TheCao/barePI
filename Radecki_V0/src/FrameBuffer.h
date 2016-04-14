#ifndef FrameBuffer_h
#define FrameBuffer_h

#include "Datatypes.h"

// Useful defines
// Framebuffer settings
#define SCREEN_WIDTH	1920 	//  0
#define SCREEN_HEIGHT	1080 	//  4
#define V_WIDTH			1920 	//  8
#define V_HEIGHT		1080	// 12
#define GPU_DEPTH		32   	// 20

// Framebuffer address
#define BUFFER_ADDRESS	0x1000

// Structure definitions
struct FrameBufferInfo
{
	UINT32 Width;
	UINT32 Height;
	UINT32 Depth;
	UINT32 Pointer;
	UINT32 ScreenSize;
	UINT32 Pitch;
};

struct point
{
	UINT32 x;
	UINT32 y;
};

struct pixel
{
	UINT32 x;
	UINT32 y;
	UINT16 colour;
};

// function prototypes
UINT32	InitialiseFrameBuffer(VOID);
VOID	TestPattern();
VOID	DrawPixel(UINT32 x, UINT32 y, UINT16 colour);
VOID	DrawLine(struct pixel Start, struct pixel End);

#endif