#include "G4Externs.h"

#define	bufWidth 640
#define bufHeight 60
#define	vWidth bufWidth
#define	vHeight bufHeight-6
#define MysteryNumber 12
extern CGrafPtr	workSrcMap;

unsigned char * fireBuffer = nil; // [bufWidth][bufHeight];// VGA buffer, quarter resolution w/extra lines

void DrawLava(void);
void HandleLava(void);
void InitFire(void);

void DrawLava(void)
{
	CGrafPtr theGrafPtr = workSrcMap;
	PixMapHandle theMap = GetPortPixMap(theGrafPtr);
	unsigned long rowBytes = (*theMap)->rowBytes & 0x3fff;
	unsigned char *screen = (unsigned char *) (*theMap)->baseAddr;
	int			i;
	int			j;
	unsigned char * index;
   	unsigned char *currentLine = screen + (((480 + MysteryNumber) - vHeight) * rowBytes);
	
	index = fireBuffer;
	
    for(i=0; i < vHeight; i++)
    {
    	unsigned char *fireIndex = index;
    	
		for(j=0; j < vWidth; j++)
		{
			// unsigned char thisPixel = Convert(*fireIndex);
			unsigned char thisPixel = ((*fireIndex) & 0x00f0) >> 3;
			
			if (thisPixel != 0)
			{	
			unsigned char ConversionTable[32] = {	255, 255,
												255, 223, 
												222, 221,
												220, 219,
												218, 217,
												216, 215,
												35, 29,
												22, 23,
												16, 17,
												10, 11,
												5, 5, 
												4, 4,
												3, 3, 
												2, 2,
												1, 1,
												0, 0};
	
	
				thisPixel = ConversionTable[thisPixel];
				// thisPixel = 0;
				
				currentLine[j] = thisPixel;	
			}		
	
			fireIndex++;
		}
		
		currentLine += rowBytes;
		index += bufWidth;
	}

}

void HandleLava(void)
{
int		i;
int		j;
int		delta;
unsigned char FireModTable[] = { 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8 };

register unsigned char *pix;
unsigned char *left,*top,*right, *pixToSet;

	if (fireBuffer == nil)
	{
		InitFire();
	}
	
	// Transform current buffer
	for(i=1; i<bufHeight-1; i++)
	{				
		pix = fireBuffer + (bufWidth * i);
		pixToSet = pix - bufWidth; //row above pix

		for(j=0 ;j<bufWidth ;j++)
		{


			if(j==0) 
			{
				//Init these variables first time through row loop
				left = fireBuffer + (bufWidth*i);  //fireBuffer[i][0] not used until next time through for 

				right = left+1 ; //fireBuffer[i][1] 
				top = left+bufWidth; //&p1[i+1][0]

				*pixToSet = (*pix + *(pixToSet+79) + *right + *top) / 4; //p1[i-1][79]

				top++; right++; pixToSet++;
			}
			else if(j== (bufWidth-1)) 
				*pixToSet = (*pix + *left + *(pixToSet+1)+ *top ) / 4; //p1[i+1][0]
			else 
				{
				*pixToSet = (*pix + *left + *right + *top ) / 4;
				left++; right++; top++; pixToSet++;
				}
	
		if (*pix > 11)
			*pix = *pix - 12;
		else
			*pix = FireModTable[*pix];

		pix++;
		}
	}


    // Set new bottom line with random white or black
	delta = 0;

	top = fireBuffer + ((bufHeight -2) * bufWidth);
	left = top + bufHeight;

	for(j=0;j<bufWidth;j++)
	{
		//change this time?
		if (Random() < 0) 
		{
			if (Random() > 0)
				delta = 0;
			else
				delta = 255;
		}
		*top++ = delta;
		*left++ = delta;
	}
}


// workSrcMap
void InitFire(void)
{	
	fireBuffer = (unsigned char *)NewPtrClear (bufHeight*bufWidth);
	
	// NOTE: ??? screen = screen+ (52 + (52*rowBytes));
 }


