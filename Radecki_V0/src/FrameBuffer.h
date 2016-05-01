#ifndef FrameBuffer_h
#define FrameBuffer_h

#include "DataTypes.h"
#include "kolory.h"
#include "periph.h"



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

typedef enum lineDirection
{
	HORIZONTAL,
	VERTICAL,
} lineDirection_t;

// function prototypes
UINT32	InitialiseFrameBuffer(VOID);
VOID	TestPattern();
VOID	DrawPixel(UINT32 x, UINT32 y, UINT16 colour);
VOID	DrawLine(struct pixel Start, struct pixel End);
int		DrawPixelK(struct FrameBufferInfo *fb_info,UINT32 x, UINT32 y, colour_t colour );
int	 	DrawLineK(struct FrameBufferInfo *fb_info,UINT32 x, UINT32 y, UINT32 lenght, lineDirection_t direction, colour_t colour );
int	 	DrawRectK(struct FrameBufferInfo *fb_info, UINT32 x0, UINT32 y0, UINT32 dx, UINT32 dy, colour_t colour);
int	 	FillScreen(struct FrameBufferInfo *fb_info,colour_t colour);
int		ClearScreen(struct FrameBufferInfo *fb_info);
int 	DrawRectKTesting(struct FrameBufferInfo *fb_info, UINT32 x0, UINT32 y0, UINT32 dx, UINT32 dy, colour_t colour);
#endif
