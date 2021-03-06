//
// screen.c
//
// USPi - An USB driver for Raspberry Pi written in C
// Copyright (C) 2014  R. Stange <rsta2@o2online.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include <uspienv/screen.h>
#include <uspienv/synchronize.h>
#include <uspienv/alloc.h>
#include <uspienv/util.h>



#define ROTORS		4
//sinus lookup table
#include "../../main/myfunc.h"

typedef enum TScreenState
{
	ScreenStateStart,
	ScreenStateEscape,
	ScreenStateBracket,
	ScreenStateNumber1,
	ScreenStateQuestionMark,
	ScreenStateSemicolon,
	ScreenStateNumber2,
	ScreenStateNumber3
}
TScreenState;

void ScreenDeviceWrite2 (TScreenDevice *pThis, char chChar);
void ScreenDeviceCarriageReturn (TScreenDevice *pThis);
void ScreenDeviceClearDisplayEnd (TScreenDevice *pThis) MAXOPT;
void ScreenDeviceClearLineEnd (TScreenDevice *pThis);
void ScreenDeviceCursorDown (TScreenDevice *pThis);
void ScreenDeviceCursorHome (TScreenDevice *pThis);
void ScreenDeviceCursorLeft (TScreenDevice *pThis);
void ScreenDeviceCursorMove (TScreenDevice *pThis, unsigned nRow, unsigned nColumn);
void ScreenDeviceCursorRight (TScreenDevice *pThis);
void ScreenDeviceCursorUp (TScreenDevice *pThis);
void ScreenDeviceDeleteChars (TScreenDevice *pThis, unsigned nCount);
void ScreenDeviceDeleteLines (TScreenDevice *pThis, unsigned nCount);
void ScreenDeviceDisplayChar (TScreenDevice *pThis, char chChar);
void ScreenDeviceEraseChars (TScreenDevice *pThis, unsigned nCount);
void ScreenDeviceInsertLines (TScreenDevice *pThis, unsigned nCount);
void ScreenDeviceInsertMode (TScreenDevice *pThis, boolean bBegin);
void ScreenDeviceNewLine (TScreenDevice *pThis);
void ScreenDeviceReverseScroll (TScreenDevice *pThis);
void ScreenDeviceSetCursorMode (TScreenDevice *pThis, boolean bVisible);
void ScreenDeviceSetStandoutMode (TScreenDevice *pThis, unsigned nMode);
void ScreenDeviceTabulator (TScreenDevice *pThis);
void ScreenDeviceScroll (TScreenDevice *pThis) MAXOPT;
void ScreenDeviceDisplayChar2 (TScreenDevice *pThis, char chChar, unsigned nPosX, unsigned nPosY, TScreenColor Color);
void ScreenDeviceEraseChar (TScreenDevice *pThis, unsigned nPosX, unsigned nPosY);
void ScreenDeviceInvertCursor (TScreenDevice *pThis);
extern unsigned getNumberLenght(double x);
extern int power(int base, int exp);

void ScreenDevice (TScreenDevice *pThis, unsigned nWidth, unsigned nHeight)
{
	CharGenerator (&pThis->m_CharGen);

	pThis->m_nInitWidth = nWidth;
	pThis->m_nInitHeight = nHeight;
	pThis->m_pFrameBuffer = 0;
	pThis->m_pBuffer = 0;
	pThis->m_nState = ScreenStateStart;
	pThis->m_nCursorX = 0;
	pThis->m_nCursorY = 0;
	pThis->m_bCursorOn = TRUE;
	pThis->m_Color = NORMAL_COLOR;
	pThis->m_bInsertOn = FALSE;
}

void _ScreenDevice (TScreenDevice *pThis)
{
	pThis->m_pBuffer = 0;
	
	_BcmFrameBuffer (pThis->m_pFrameBuffer);
	free (pThis->m_pFrameBuffer);
	pThis->m_pFrameBuffer = 0;

	_CharGenerator (&pThis->m_CharGen);
}

boolean ScreenDeviceInitialize (TScreenDevice *pThis)
{
	pThis->m_pFrameBuffer = malloc (sizeof (TBcmFrameBuffer));
	BcmFrameBuffer (pThis->m_pFrameBuffer, pThis->m_nInitWidth, pThis->m_nInitHeight, DEPTH);
#if DEPTH == 8
	BcmFrameBufferSetPalette (pThis->m_pFrameBuffer, NORMAL_COLOR, NORMAL_COLOR16);
	BcmFrameBufferSetPalette (pThis->m_pFrameBuffer, HIGH_COLOR,   HIGH_COLOR16);
	BcmFrameBufferSetPalette (pThis->m_pFrameBuffer, HALF_COLOR,   HALF_COLOR16);
#endif
	if (!BcmFrameBufferInitialize (pThis->m_pFrameBuffer))
	{
		return FALSE;
	}

	if (BcmFrameBufferGetDepth (pThis->m_pFrameBuffer) != DEPTH)
	{
		return FALSE;
	}

	pThis->m_pBuffer = (TScreenColor *) BcmFrameBufferGetBuffer (pThis->m_pFrameBuffer);
	pThis->m_nSize   = BcmFrameBufferGetSize (pThis->m_pFrameBuffer);
	pThis->m_nWidth  = BcmFrameBufferGetWidth (pThis->m_pFrameBuffer);
	pThis->m_nHeight = BcmFrameBufferGetHeight (pThis->m_pFrameBuffer);

	// Makes things easier and is normally the case
	if (BcmFrameBufferGetPitch (pThis->m_pFrameBuffer) != pThis->m_nWidth * sizeof (TScreenColor))
	{
		return FALSE;
	}

	pThis->m_nUsedHeight =   pThis->m_nHeight
			       / CharGeneratorGetCharHeight (&pThis->m_CharGen)
			       * CharGeneratorGetCharHeight (&pThis->m_CharGen);

	ScreenDeviceCursorHome (pThis);
	ScreenDeviceClearDisplayEnd (pThis);
	ScreenDeviceInvertCursor (pThis);

	return TRUE;
}

unsigned ScreenDeviceGetWidth (TScreenDevice *pThis)
{
	return pThis->m_nWidth;
}

unsigned ScreenDeviceGetHeight (TScreenDevice *pThis)
{
	return pThis->m_nHeight;
}

int ScreenDeviceWrite (TScreenDevice *pThis, const void *pBuffer, unsigned nCount)
{
	ScreenDeviceInvertCursor (pThis);

	const char *pChar = (const char *) pBuffer;
	int nResult = 0;

	while (nCount--)
	{
		ScreenDeviceWrite2 (pThis, *pChar++);

		nResult++;
	}

	ScreenDeviceInvertCursor (pThis);
	
	DataMemBarrier ();

	return nResult;
}

void ScreenDeviceSetPixel (TScreenDevice *pThis, unsigned nPosX, unsigned nPosY, TScreenColor Color)
{
	if (   nPosX < pThis->m_nWidth
	    && nPosY < pThis->m_nHeight)
	{
		pThis->m_pBuffer[pThis->m_nWidth * nPosY + nPosX] = Color;
	}
}

TScreenColor ScreenDeviceGetPixel (TScreenDevice *pThis, unsigned nPosX, unsigned nPosY)
{
	if (   nPosX < pThis->m_nWidth
	    && nPosY < pThis->m_nHeight)
	{
		return pThis->m_pBuffer[pThis->m_nWidth * nPosY + nPosX];
	}
	
	return BLACK_COLOR;
}

void ScreenDeviceRotor (TScreenDevice *pThis, unsigned nIndex, unsigned nCount)
{
	static const char chChars[] = "-\\|/";

	nIndex %= ROTORS;
	nCount &= 4-1;

	unsigned nPosX = pThis->m_nWidth - (nIndex + 1) * CharGeneratorGetCharWidth (&pThis->m_CharGen);

	ScreenDeviceDisplayChar2 (pThis, chChars[nCount], nPosX, 0, HIGH_COLOR);
}

void ScreenDeviceWrite2 (TScreenDevice *pThis, char chChar)
{
	switch (pThis->m_nState)
	{
	case ScreenStateStart:
		switch (chChar)
		{
		case '\b':
			ScreenDeviceCursorLeft (pThis);
			break;

		case '\t':
			ScreenDeviceTabulator (pThis);
			break;

		case '\n':
			ScreenDeviceNewLine (pThis);
			break;

		case '\r':
			ScreenDeviceCarriageReturn (pThis);
			break;

		case '\x1b':
			pThis->m_nState = ScreenStateEscape;
			break;

		default:
			ScreenDeviceDisplayChar (pThis, chChar);
			break;
		}
		break;

	case ScreenStateEscape:
		switch (chChar)
		{
		case 'M':
			ScreenDeviceReverseScroll (pThis);
			pThis->m_nState = ScreenStateStart;
			break;

		case '[':
			pThis->m_nState = ScreenStateBracket;
			break;

		default:
			pThis->m_nState = ScreenStateStart;
			break;
		}
		break;

	case ScreenStateBracket:
		switch (chChar)
		{
		case '?':
			pThis->m_nState = ScreenStateQuestionMark;
			break;

		case 'A':
			ScreenDeviceCursorUp (pThis);
			pThis->m_nState = ScreenStateStart;
			break;

		case 'B':
			ScreenDeviceCursorDown (pThis);
			pThis->m_nState = ScreenStateStart;
			break;

		case 'C':
			ScreenDeviceCursorRight (pThis);
			pThis->m_nState = ScreenStateStart;
			break;

		case 'H':
			ScreenDeviceCursorHome (pThis);
			pThis->m_nState = ScreenStateStart;
			break;

		case 'J':
			ScreenDeviceClearDisplayEnd (pThis);
			pThis->m_nState = ScreenStateStart;
			break;

		case 'K':
			ScreenDeviceClearLineEnd (pThis);
			pThis->m_nState = ScreenStateStart;
			break;

		case 'L':
			ScreenDeviceInsertLines (pThis, 1);
			pThis->m_nState = ScreenStateStart;
			break;

		case 'M':
			ScreenDeviceDeleteLines (pThis, 1);
			pThis->m_nState = ScreenStateStart;
			break;

		case 'P':
			ScreenDeviceDeleteChars (pThis, 1);
			pThis->m_nState = ScreenStateStart;
			break;

		default:
			if ('0' <= chChar && chChar <= '9')
			{
				pThis->m_nParam1 = chChar - '0';
				pThis->m_nState = ScreenStateNumber1;
			}
			else
			{
				pThis->m_nState = ScreenStateStart;
			}
			break;
		}
		break;

	case ScreenStateNumber1:
		switch (chChar)
		{
		case ';':
			pThis->m_nState = ScreenStateSemicolon;
			break;

		case 'L':
			ScreenDeviceInsertLines (pThis, pThis->m_nParam1);
			pThis->m_nState = ScreenStateStart;
			break;

		case 'M':
			ScreenDeviceDeleteLines (pThis, pThis->m_nParam1);
			pThis->m_nState = ScreenStateStart;
			break;

		case 'P':
			ScreenDeviceDeleteChars (pThis, pThis->m_nParam1);
			pThis->m_nState = ScreenStateStart;
			break;

		case 'X':
			ScreenDeviceEraseChars (pThis, pThis->m_nParam1);
			pThis->m_nState = ScreenStateStart;
			break;

		case 'h':
		case 'l':
			if (pThis->m_nParam1 == 4)
			{
				ScreenDeviceInsertMode (pThis, chChar == 'h');
			}
			pThis->m_nState = ScreenStateStart;
			break;
			
		case 'm':
			ScreenDeviceSetStandoutMode (pThis, pThis->m_nParam1);
			pThis->m_nState = ScreenStateStart;
			break;

		default:
			if ('0' <= chChar && chChar <= '9')
			{
				pThis->m_nParam1 *= 10;
				pThis->m_nParam1 += chChar - '0';

				if (pThis->m_nParam1 > 99)
				{
					pThis->m_nState = ScreenStateStart;
				}
			}
			else
			{
				pThis->m_nState = ScreenStateStart;
			}
			break;
		}
		break;

	case ScreenStateSemicolon:
		if ('0' <= chChar && chChar <= '9')
		{
			pThis->m_nParam2 = chChar - '0';
			pThis->m_nState = ScreenStateNumber2;
		}
		else
		{
			pThis->m_nState = ScreenStateStart;
		}
		break;

	case ScreenStateQuestionMark:
		if ('0' <= chChar && chChar <= '9')
		{
			pThis->m_nParam1 = chChar - '0';
			pThis->m_nState = ScreenStateNumber3;
		}
		else
		{
			pThis->m_nState = ScreenStateStart;
		}
		break;

	case ScreenStateNumber2:
		switch (chChar)
		{
		case 'H':
			ScreenDeviceCursorMove (pThis, pThis->m_nParam1, pThis->m_nParam2);
			pThis->m_nState = ScreenStateStart;
			break;

		default:
			if ('0' <= chChar && chChar <= '9')
			{
				pThis->m_nParam2 *= 10;
				pThis->m_nParam2 += chChar - '0';

				if (pThis->m_nParam2 > 199)
				{
					pThis->m_nState = ScreenStateStart;
				}
			}
			else
			{
				pThis->m_nState = ScreenStateStart;
			}
			break;
		}
		break;

	case ScreenStateNumber3:
		switch (chChar)
		{
		case 'h':
		case 'l':
			if (pThis->m_nParam1 == 25)
			{
				ScreenDeviceSetCursorMode (pThis, chChar == 'h');
			}
			pThis->m_nState = ScreenStateStart;
			break;

		default:
			if ('0' <= chChar && chChar <= '9')
			{
				pThis->m_nParam1 *= 10;
				pThis->m_nParam1 += chChar - '0';

				if (pThis->m_nParam1 > 99)
				{
					pThis->m_nState = ScreenStateStart;
				}
			}
			else
			{
				pThis->m_nState = ScreenStateStart;
			}
			break;
		}
		break;

	default:
		pThis->m_nState = ScreenStateStart;
		break;
	}
}

void ScreenDeviceCarriageReturn (TScreenDevice *pThis)
{
	pThis->m_nCursorX = 0;
}

void ScreenDeviceClearDisplayEnd (TScreenDevice *pThis)
{
	ScreenDeviceClearLineEnd (pThis);
	
	unsigned nPosY = pThis->m_nCursorY + CharGeneratorGetCharHeight (&pThis->m_CharGen);
	unsigned nOffset = nPosY * pThis->m_nWidth;
	
	TScreenColor *pBuffer = pThis->m_pBuffer + nOffset;
	unsigned nSize = pThis->m_nSize / sizeof (TScreenColor) - nOffset;
	
	while (nSize--)
	{
		*pBuffer++ = BLACK_COLOR;
	}
}

void ScreenDeviceClearLineEnd (TScreenDevice *pThis)
{
	for (unsigned nPosX = pThis->m_nCursorX; nPosX < pThis->m_nWidth; nPosX += CharGeneratorGetCharWidth (&pThis->m_CharGen))
	{
		ScreenDeviceEraseChar (pThis, nPosX, pThis->m_nCursorY);
	}
}

void ScreenDeviceCursorDown (TScreenDevice *pThis)
{
	pThis->m_nCursorY += CharGeneratorGetCharHeight (&pThis->m_CharGen);
	if (pThis->m_nCursorY >= pThis->m_nUsedHeight)
	{
		ScreenDeviceScroll (pThis);

		pThis->m_nCursorY -= CharGeneratorGetCharHeight (&pThis->m_CharGen);
	}
}

void ScreenDeviceCursorHome (TScreenDevice *pThis)
{
	pThis->m_nCursorX = 0;
	pThis->m_nCursorY = 0;
}

void ScreenDeviceCursorLeft (TScreenDevice *pThis)
{
	if (pThis->m_nCursorX > 0)
	{
		pThis->m_nCursorX -= CharGeneratorGetCharWidth (&pThis->m_CharGen);
	}
	else
	{
		if (pThis->m_nCursorY > 0)
		{
			pThis->m_nCursorX = pThis->m_nWidth - CharGeneratorGetCharWidth (&pThis->m_CharGen);
			pThis->m_nCursorY -= CharGeneratorGetCharHeight (&pThis->m_CharGen);
		}
	}
}

void ScreenDeviceCursorMove (TScreenDevice *pThis, unsigned nRow, unsigned nColumn)
{
	unsigned nPosX = (nColumn - 1) * CharGeneratorGetCharWidth (&pThis->m_CharGen);
	unsigned nPosY = (nRow - 1) * CharGeneratorGetCharHeight (&pThis->m_CharGen);

	if (   nPosX < pThis->m_nWidth
	    && nPosY < pThis->m_nUsedHeight)
	{
		pThis->m_nCursorX = nPosX;
		pThis->m_nCursorY = nPosY;
	}
}

void ScreenDeviceCursorRight (TScreenDevice *pThis)
{
	pThis->m_nCursorX += CharGeneratorGetCharWidth (&pThis->m_CharGen);
	if (pThis->m_nCursorX >= pThis->m_nWidth)
	{
		ScreenDeviceNewLine (pThis);
	}
}

void ScreenDeviceCursorUp (TScreenDevice *pThis)
{
	if (pThis->m_nCursorY >= CharGeneratorGetCharHeight (&pThis->m_CharGen))
	{
		pThis->m_nCursorY -= CharGeneratorGetCharHeight (&pThis->m_CharGen);
	}
}

void ScreenDeviceDeleteChars (TScreenDevice *pThis, unsigned nCount)	// TODOx
{
}

void ScreenDeviceDeleteLines (TScreenDevice *pThis, unsigned nCount)	// TODOx
{
}

void ScreenDeviceDisplayChar (TScreenDevice *pThis, char chChar)
{
	// TODOx: Insert mode
	
	if (' ' <= chChar && chChar <= '~')
	{
		ScreenDeviceDisplayChar2 (pThis, chChar, pThis->m_nCursorX, pThis->m_nCursorY, pThis->m_Color);

		ScreenDeviceCursorRight (pThis);
	}
}

void ScreenDeviceEraseChars (TScreenDevice *pThis, unsigned nCount)
{
	if (nCount == 0)
	{
		return;
	}

	unsigned nEndX = pThis->m_nCursorX + nCount * CharGeneratorGetCharWidth (&pThis->m_CharGen);
	if (nEndX > pThis->m_nWidth)
	{
		nEndX = pThis->m_nWidth;
	}

	for (unsigned nPosX = pThis->m_nCursorX; nPosX < nEndX; nPosX += CharGeneratorGetCharWidth (&pThis->m_CharGen))
	{
		ScreenDeviceEraseChar (pThis, nPosX, pThis->m_nCursorY);
	}
}

void ScreenDeviceInsertLines (TScreenDevice *pThis, unsigned nCount)	// TODOx
{
}

void ScreenDeviceInsertMode (TScreenDevice *pThis, boolean bBegin)
{
	pThis->m_bInsertOn = bBegin;
}

void ScreenDeviceNewLine (TScreenDevice *pThis)
{
	ScreenDeviceCarriageReturn (pThis);
	ScreenDeviceCursorDown (pThis);
}

void ScreenDeviceReverseScroll (TScreenDevice *pThis)
{
	if (   pThis->m_nCursorX == 0
	    && pThis->m_nCursorY == 0
	)
	{
		ScreenDeviceInsertLines (pThis, 1);
	}
}

void ScreenDeviceSetCursorMode (TScreenDevice *pThis, boolean bVisible)
{
	pThis->m_bCursorOn = bVisible;
}


void ScreenDeviceSetStandoutMode (TScreenDevice *pThis, unsigned nMode)
{
	switch (nMode)
	{
	case 0:
	case 27:
		pThis->m_Color = NORMAL_COLOR;
		break;
		
	case 1:
		pThis->m_Color = HIGH_COLOR;
		break;

	case 2:
		pThis->m_Color = HALF_COLOR;
		break;

	case 7:				// TODOx: reverse mode
	default:
		break;
	}
}

void ScreenDeviceTabulator (TScreenDevice *pThis)
{
	unsigned nTabWidth = CharGeneratorGetCharWidth (&pThis->m_CharGen) * 8;
	
	pThis->m_nCursorX = ((pThis->m_nCursorX + nTabWidth) / nTabWidth) * nTabWidth;
	if (pThis->m_nCursorX >= pThis->m_nWidth)
	{
		ScreenDeviceNewLine (pThis);
	}
}

void ScreenDeviceScroll (TScreenDevice *pThis)
{
	unsigned nLines = CharGeneratorGetCharHeight (&pThis->m_CharGen);

	u32 *pTo = (u32 *) pThis->m_pBuffer;
	u32 *pFrom = (u32 *) (pThis->m_pBuffer + nLines * pThis->m_nWidth);

	unsigned nSize = pThis->m_nWidth * (pThis->m_nUsedHeight - nLines) * sizeof (TScreenColor);

	memcpyblk (pTo, pFrom, nSize);
	pTo += nSize / sizeof (u32);

	nSize = pThis->m_nWidth * nLines * sizeof (TScreenColor) / sizeof (u32);
	while (nSize--)
	{
		*pTo++ = BLACK_COLOR;
	}
}

void ScreenDeviceDisplayChar2 (TScreenDevice *pThis, char chChar, unsigned nPosX, unsigned nPosY, TScreenColor Color)
{
	for (unsigned y = 0; y < CharGeneratorGetCharHeight (&pThis->m_CharGen); y++)
	{
		for (unsigned x = 0; x < CharGeneratorGetCharWidth (&pThis->m_CharGen); x++)
		{
			ScreenDeviceSetPixel (pThis, nPosX + x, nPosY + y,
				  CharGeneratorGetPixel (&pThis->m_CharGen, chChar, x, y) ? Color : BLACK_COLOR);
		}
	}
}

void ScreenDeviceEraseChar (TScreenDevice *pThis, unsigned nPosX, unsigned nPosY)
{
	for (unsigned y = 0; y < CharGeneratorGetCharHeight (&pThis->m_CharGen); y++)
	{
		for (unsigned x = 0; x < CharGeneratorGetCharWidth (&pThis->m_CharGen); x++)
		{
			ScreenDeviceSetPixel (pThis, nPosX + x, nPosY + y, BLACK_COLOR);
		}
	}
}

void ScreenDeviceInvertCursor (TScreenDevice *pThis)
{
	if (!pThis->m_bCursorOn)
	{
		return;
	}
	
	for (unsigned y = CharGeneratorGetUnderline (&pThis->m_CharGen); y < CharGeneratorGetCharHeight (&pThis->m_CharGen); y++)
	{
		for (unsigned x = 0; x < CharGeneratorGetCharWidth (&pThis->m_CharGen); x++)
		{
			if (ScreenDeviceGetPixel (pThis, pThis->m_nCursorX + x, pThis->m_nCursorY + y) == BLACK_COLOR)
			{
				ScreenDeviceSetPixel (pThis, pThis->m_nCursorX + x, pThis->m_nCursorY + y, pThis->m_Color);
			}
			else
			{
				ScreenDeviceSetPixel (pThis, pThis->m_nCursorX + x, pThis->m_nCursorY + y, BLACK_COLOR);
			}
		}
	}
}

void ScreenDeviceDrawLine(TScreenDevice *pThis,unsigned nPosX, unsigned nPosY, unsigned lenght, TScreenColor color, lineDirection_t direction)
{
	switch(direction)
		{
		case HORIZONTAL:
			for(unsigned i=0;i<lenght;i++)
			{
				ScreenDeviceSetPixel (pThis, nPosX + i, nPosY, color);
			}
			break;
		case VERTICAL:
			for(unsigned i=0;i<=lenght;i++)
				{
				ScreenDeviceSetPixel (pThis, nPosX, nPosY-i, color);
				}
			break;
		}

}

void ScreenDeviceDrawDottedLine(TScreenDevice *pThis,unsigned nPosX, unsigned nPosY, unsigned lenght, unsigned dotBreak, TScreenColor color, lineDirection_t direction)
{
	// draw Dotted Line with break in px specified by dotBreak
	switch(direction)
		{
		case HORIZONTAL:
			for(unsigned i=0;i<lenght;i+=dotBreak)
			{
				ScreenDeviceSetPixel (pThis, nPosX + i, nPosY, color);
			}
			break;
		case VERTICAL:
			for(unsigned i=0;i<=lenght;i+=dotBreak)
				{
				ScreenDeviceSetPixel (pThis, nPosX, nPosY-i, color);
				}
			break;
		}

}

void ScreenDeviceDrawRect(TScreenDevice *pThis,unsigned nPosX, unsigned nPosY,unsigned dx, unsigned dy, TScreenColor color)
{
	for(unsigned i = 0; i <dy;i++)
	{
		ScreenDeviceDrawLine(pThis,nPosX,nPosY+i,dx,color,HORIZONTAL);
	}
}

void ScreenDeviceClearDisplay(TScreenDevice *pThis)
{
	ScreenDeviceDrawRect(pThis,0,0,pThis->m_nWidth, pThis->m_nHeight,BLACK_COLOR);
	ScreenDeviceCursorHome (pThis);
}

void ScreenDeviceFillDisplay(TScreenDevice *pThis,TScreenColor color)
{
	ScreenDeviceDrawRect(pThis,0,0,pThis->m_nWidth, pThis->m_nHeight,color);
}

unsigned int ScreenDeviceDrawChartCaption(TScreenDevice *pThis, double number, unsigned startPointX, unsigned startPointY,TScreenColor color)
{
	unsigned length = getNumberLenght(number);
	boolean isFloatingPoint = FALSE;
	char temp;
	int temp1digt;
	if((int)number<number)
	{
		isFloatingPoint = TRUE;
		number*=10;
		length+=1;
	}
	for(unsigned i = length; i>0;i--)
	{
		temp1digt = (number/power(10,i-1));
		number-= temp1digt*power(10,i-1);
		temp = (temp1digt + 0x30);
		if(isFloatingPoint == TRUE && i ==1)
		{
			// set comma
			ScreenDeviceDisplayChar2(pThis,',',startPointX+10*(length-i),startPointY,color);
			ScreenDeviceDisplayChar2(pThis,temp,startPointX+10*(length-i)+10,startPointY,color);
		}
		else
		{
			ScreenDeviceDisplayChar2(pThis,temp,startPointX+10*(length-i),startPointY,color);
		}
	}

	return 0;
}
unsigned ScreenDeviceDrawChartCaptionOXAll(TScreenDevice *pThis, unsigned startPointX, unsigned startPointY, unsigned lenX,unsigned lenY, boolean isFirstDraw, double dt, unsigned resolution,double actualTime)
{

	double maxValueFirstDrawOX = (lenX * dt)*resolution; //max time OX value in first draw of chart
	double actValueDrawOX = maxValueFirstDrawOX/10;

	if(isFirstDraw == TRUE)
	{
		for(unsigned i = startPointX+lenX/10;i<=startPointX+lenX;i+=lenX/10) //dotted lines vertical
			{
		ScreenDeviceDrawChartCaption(pThis,actValueDrawOX,i-15,startPointY+20,WHITE_COLOR); // dolne
		actValueDrawOX+=maxValueFirstDrawOX/10;
			}
	}
	else
	{
		ScreenDeviceDrawChartCaption(pThis,actualTime,startPointX+lenX-15,startPointY+20,WHITE_COLOR); // ostatni podpis z prawej
		for(unsigned i = 1; i<=9;i++)
		{
			ScreenDeviceDrawChartCaption(pThis,actualTime-(i*(maxValueFirstDrawOX/10)),startPointX+((lenX/10)*(10-i))-15,startPointY+20,WHITE_COLOR);
		}
	}
	return 0;
}
unsigned ScreenDeviceDrawChartCaptionOYAll(TScreenDevice *pThis, unsigned startPointX, unsigned startPointY,unsigned lenY, boolean isFirstDraw, double actualValue,TScreenColor color)
{
	// printing actual values of OY
	double counterOY = 0.201;
	double maxValueFirstDrawOY = 2.0;

		if(isFirstDraw == TRUE)
		{
			for(unsigned j=startPointY-lenY/10;j>=startPointY-lenY;j-=lenY/10, counterOY+=0.2)
			{
				ScreenDeviceDrawChartCaption(pThis,counterOY,startPointX-50,j-5,color);
			}
		}
		else
		{
			ScreenDeviceDrawChartCaption(pThis,actualValue,startPointX-50,startPointY-lenY-5,color); // g�rny podpis
			for(unsigned i = 1; i<=9;i++)
			{
				ScreenDeviceDrawChartCaption(pThis,actualValue-(i*maxValueFirstDrawOY/10),startPointX-50,startPointY-5-lenY+(i*lenY/10),color);
			}
		}
		return 0;
}
unsigned ScreenDeviceDrawChartCaptionOYAllCounter(TScreenDevice *pThis, unsigned startPointX, unsigned startPointY,unsigned lenY, double counter,TScreenColor color)
{
	// printing actual values of OY
	double actualCounter = counter+0.01;
	for(unsigned j=startPointY-lenY/10;j>=startPointY-lenY;j-=lenY/10, actualCounter+=counter)
	{
		ScreenDeviceDrawChartCaption(pThis,actualCounter,startPointX-50,j-5,color);
	}
	return 0;
}

unsigned ScreenDeviceDrawDottedBackground(TScreenDevice *pThis, TScreenColor color, unsigned startPointX, unsigned startPointY, unsigned lenX, unsigned lenY,chartAddLines_t linesOption)
{
	for(unsigned i = startPointX+lenX/10;i<=startPointX+lenX;i+=lenX/10) //dotted lines vertical
	{
		if(linesOption == VERTICALLINES || linesOption == BOTH)
			{
				if(i == startPointX+lenX) ScreenDeviceDrawLine(pThis,i,startPointY,lenY,BLACK_COLOR,VERTICAL); // last vertical dotted line need to cleared before drawing dots
				ScreenDeviceDrawDottedLine(pThis,i,startPointY,lenY,5,color,VERTICAL);
			}
		ScreenDeviceDrawLine(pThis,i,startPointY+10,10,color,VERTICAL);

	}

	for(unsigned j=startPointY-lenY/10;j>=startPointY-lenY;j-=lenY/10) // dotted lines horizontal
	{
		if(linesOption == HORIZONTALLINES || linesOption == BOTH)
			{
				ScreenDeviceDrawDottedLine(pThis,startPointX,j,lenX,3,color,HORIZONTAL);
			}
		ScreenDeviceDrawLine(pThis,startPointX-10,j,10,color,HORIZONTAL);

	}
	return 0;
}

unsigned ScreenDeviceDrawChart(TScreenDevice *pThis, TScreenColor color,chartAddLines_t linesOption)
{
	// draw chart axis
	unsigned startPointX = ((pThis->m_nWidth)/10); //10% of whole Screen Width
	unsigned startPointY = ((pThis->m_nHeight)*9)/10;//90% of whole screen Height
	unsigned lenX = ((pThis->m_nWidth)-2*startPointX)/10;
	lenX*=10;
	unsigned lenY = (startPointY-(pThis->m_nHeight/10))/10;
	lenY*=10;
	static const char czas[] = "Czas [s]";
	static const char predkosc[] = "Predkosc";
	static const char predkosc2[] = "[rad/s]";
	static const char moment[] = "Moment [Nm]";
	static const char napiecie[] = "Napiecie [V]";

	// chart lines drawing
	for(unsigned i = 0;i<2;i++)
	{
		ScreenDeviceDrawLine(pThis,startPointX,startPointY+i,lenX,color,HORIZONTAL);
	}
	for(unsigned i = 0;i<2;i++)
	{
		ScreenDeviceDrawLine(pThis,startPointX-i,startPointY,lenY,color,VERTICAL);
	}
	// dotted lines
	ScreenDeviceDrawDottedBackground(pThis,color,startPointX,startPointY,lenX, lenY,BOTH);
	for(unsigned int i=0;i <sizeof(czas);i++)
	{
		ScreenDeviceDisplayChar2(pThis,czas[i],startPointX+10*i+(lenX/2),startPointY+50,WHITE_COLOR);
	}
	for(unsigned int i=0;i <sizeof(predkosc);i++)
	{
		ScreenDeviceDisplayChar2(pThis,predkosc[i],startPointX-120+i*10,startPointY-(lenY/2)-60,WHITE_COLOR);
	}
	for(unsigned int i=0;i <sizeof(predkosc2);i++)
	{
		ScreenDeviceDisplayChar2(pThis,predkosc2[i],startPointX-115+i*10,startPointY-(lenY/2)-42,WHITE_COLOR);
	}
	for(unsigned int i=0;i <sizeof(moment);i++)
	{
		ScreenDeviceDisplayChar2(pThis,moment[i],startPointX-120+i*10,startPointY-(lenY/2)+20,RED_COLOR);
	}

	for(unsigned int i=0;i <sizeof(napiecie);i++)
	{
		ScreenDeviceDisplayChar2(pThis,napiecie[i],startPointX+lenX+10+i*10,startPointY-(lenY/2)-42,BLUE_COLOR);
	}



	return 0;
}
