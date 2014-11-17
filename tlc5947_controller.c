#include "tlc5947_controller.h"

static const uint64_t evenCleanMask = ~(0xF0FFFFFFFF);	// 0xFFFFFF0F00000000 
static const uint64_t oddCleanMask  = ~(0xFFFFFFFF0F);	// 0xFFFFFF00000000F0

static uint64_t compileEvenLedPattern(RGB rgbSet)
{
	uint64_t pattern = 0;
	uint64_t hex = 0;					

	uint64_t red   = rgbSet.red   & 0xFFF,	// remove remaining bits
	    	 green = rgbSet.green & 0xFFF, 	// so every color may have values from 0x000 to 0xfff
		     blue  = rgbSet.blue  & 0xFFF;	// (one and a half bytes value)
			
	uint64_t hex1Mask = 0xF00;				// masks for extracting corresponding hex value
	uint64_t hex2Mask = 0x0F0;
	uint64_t hex3Mask = 0x00F;


		// EXAMPLE (Even LED) :
		// 
		// for RGB rgbSet.red   = 0x123,
		// 	  	   rgbSet.green = 0x456,
		// 	       rgbSet.blue  = 0x789;
		//
		// function should compile following pattern:
		// 		pattern = [00 00 00 90 78 56 34 12]

							    
	//compiling red color		
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = red & hex1Mask;		//     hex = [00 00 00 00 00 00 01 00]
	hex = hex >> 4;				//     hex = [00 00 00 00 00 00 00 10]
	pattern = pattern | hex; 	// pattern = [00 00 00 00 00 00 00 10]
								
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = red & hex2Mask;		//     hex = [00 00 00 00 00 00 00 20]
	hex = hex >> 4;				//     hex = [00 00 00 00 00 00 00 02]
	pattern = pattern | hex; 	// pattern = [00 00 00 00 00 00 00 12]
								
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = red & hex3Mask;		//     hex = [00 00 00 00 00 00 00 03]
	hex = hex << 12;			//     hex = [00 00 00 00 00 00 30 00]
	pattern = pattern | hex; 	// pattern = [00 00 00 00 00 00 30 12]
								
	//compiling green color    	
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = green & hex1Mask;		//     hex = [00 00 00 00 00 00 04 00]
	pattern = pattern | hex; 	// pattern = [00 00 00 00 00 00 34 12]
								
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = green & hex2Mask;		//     hex = [00 00 00 00 00 00 00 50]
	hex = hex << 16;			//     hex = [00 00 00 00 00 50 00 00]
	pattern = pattern | hex; 	// pattern = [00 00 00 00 00 50 34 12]
								
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = green & hex3Mask;		//     hex = [00 00 00 00 00 00 00 06]
	hex = hex << 16;			//     hex = [00 00 00 00 00 06 00 00]
	pattern = pattern | hex; 	// pattern = [00 00 00 00 00 56 34 12]
								
	//compiling blue color 		
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = blue & hex1Mask;		//     hex = [00 00 00 00 00 00 07 00]
	hex = hex << 20;			//     hex = [00 00 00 00 70 00 00 00]
	pattern = pattern | hex; 	// pattern = [00 00 00 00 70 56 34 12]
								
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = blue & hex2Mask;		//     hex = [00 00 00 00 00 00 00 80]
	hex = hex << 20;			//     hex = [00 00 00 00 08 00 00 00]
	pattern = pattern | hex; 	// pattern = [00 00 00 00 78 56 34 12]
								
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = blue & hex3Mask;		//     hex = [00 00 00 00 00 00 00 09]
	hex = hex << 36;			//     hex = [00 00 00 90 00 00 00 00]
	pattern = pattern | hex; 	// pattern = [00 00 00 90 78 56 34 12]
	
	return pattern;
}

static uint64_t compileOddLedPattern(RGB rgbSet)
{
	uint64_t pattern = 0;
	uint64_t hex = 0;					

	uint64_t red   = rgbSet.red   & 0xFFF,	// remove remaining bits
	    	 green = rgbSet.green & 0xFFF, 	// so every color may have values from 0x000 to 0xfff
		     blue  = rgbSet.blue  & 0xFFF;	// (one and a half bytes value)
			
	uint64_t hex1Mask = 0xF00;				// masks for extracting corresponding hex value
	uint64_t hex2Mask = 0x0F0;
	uint64_t hex3Mask = 0x00F;


		// EXAMPLE (Odd LED) :
		// 
		// for RGB rgbSet.red   = 0x123,
		// 	  	   rgbSet.green = 0x456,
		// 	       rgbSet.blue  = 0x789;
		//
		// function should compile following pattern:
		// 		pattern = [00 00 00 89 67 45 23 01]
							    
	//compiling red color					 [00 00 00 89 67 45 23 01]
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = red & hex1Mask;		//     hex = [00 00 00 00 00 00 01 00]
	hex = hex >> 8;				//     hex = [00 00 00 00 00 00 00 01]
	pattern = pattern | hex; 	// pattern = [00 00 00 00 00 00 00 01]
								
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = red & hex2Mask;		//     hex = [00 00 00 00 00 00 00 20]
	hex = hex << 8;				//     hex = [00 00 00 00 00 00 20 00]
	pattern = pattern | hex; 	// pattern = [00 00 00 00 00 00 20 01]
								
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = red & hex3Mask;		//     hex = [00 00 00 00 00 00 00 03]
	hex = hex << 8;				//     hex = [00 00 00 00 00 00 03 00]
	pattern = pattern | hex; 	// pattern = [00 00 00 00 00 00 23 01]

	//compiling green color    	
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = green & hex1Mask;		//     hex = [00 00 00 00 00 00 04 00]
	hex = hex << 12;			//     hex = [00 00 00 00 00 40 00 00]
	pattern = pattern | hex; 	// pattern = [00 00 00 00 00 40 23 01]
								
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = green & hex2Mask;		//     hex = [00 00 00 00 00 00 00 50]
	hex = hex << 12;			//     hex = [00 00 00 00 00 05 00 00]
	pattern = pattern | hex; 	// pattern = [00 00 00 00 00 45 23 01]
								
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = green & hex3Mask;		//     hex = [00 00 00 00 00 00 00 06]
	hex = hex << 28;			//     hex = [00 00 00 00 60 00 00 00]
	pattern = pattern | hex; 	// pattern = [00 00 00 00 60 45 23 01]
		
	//compiling blue color 		
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = blue & hex1Mask;		//     hex = [00 00 00 00 00 00 07 00]
	hex = hex << 16;			//     hex = [00 00 00 00 07 00 00 00]
	pattern = pattern | hex; 	// pattern = [00 00 00 00 67 45 23 01]
								
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = blue & hex2Mask;		//     hex = [00 00 00 00 00 00 00 80]
	hex = hex << 32;			//     hex = [00 00 00 80 00 00 00 00]
	pattern = pattern | hex; 	// pattern = [00 00 00 80 67 45 23 01]
								
	hex = 0;					//     hex = [00 00 00 00 00 00 00 00]
	hex = blue & hex3Mask;		//     hex = [00 00 00 00 00 00 00 09]
	hex = hex << 32;			//     hex = [00 00 00 09 00 00 00 00]
	pattern = pattern | hex; 	// pattern = [00 00 00 89 67 45 23 01]
				
	return pattern;
}



static int insertLedRgb(uint32_t ledNumber, uint64_t rgbSet, uint8_t* tab)
{
	if(tab <= 0)
		return -1;

	uint32_t index = -1;
	uint64_t* tabSegment = 0;

	if( 0 == ledNumber%2 ) 					// even led number
	{
		index = (ledNumber/2) * 9;					 // (N/2) * 9 	(check README)
		tabSegment = (uint64_t*) &tab[index];
		*tabSegment = (*tabSegment) & evenCleanMask; // clean corresponding 4.5 bytes for new RGB value
	}
	else									// odd led number
	{
		index = ( ((ledNumber-1)/2) * 9) + 4;	 	 // ( ((N-1)/2) * 9 ) + 4 	(check README)
		tabSegment = (uint64_t*) &tab[index];
		*tabSegment = (*tabSegment) & oddCleanMask;	 // clean last 4.5 bytes for new RGB value
	}

	*tabSegment = (*tabSegment) | rgbSet; 		// put compiled led color to array

	return 0;
}

int setLedColor(uint32_t ledNumber, RGB rgbSet, uint8_t* tab)
{
	uint64_t pattern = 0;

	if(0 == ledNumber%2)
		pattern = compileEvenLedPattern(rgbSet);
	else
		pattern = compileOddLedPattern(rgbSet);

	int result = insertLedRgb(ledNumber, pattern, tab);

	return result;
}

void printRegister(uint8_t *tab)
{
	int i=SIZE-1;
	
	printf("tab: [");

	for(i=SIZE-1; i>=0; --i)
	{
		printf("%02X ", tab[i]);
	}

	printf("\b]\n");
}



// int main()
// {
// 	printf("sizeof(uint64_t): %lu\n", sizeof(uint64_t));

// 	uint8_t tab[SIZE];
// 	memset(tab, 0, SIZE);

// 	// int i = 0;

// 	RGB rgb0;
// 	rgb0.red   = 0x123;
// 	rgb0.green = 0x456;
// 	rgb0.blue  = 0x789;

// 	// RGB rgb1;
// 	// rgb1.red   = 0x78D;
// 	// rgb1.green = 0x9C0;
// 	// rgb1.blue  = 0xBAF; 

// 	// RGB rgb2;
// 	// rgb2.red   = 0x578;
// 	// rgb2.green = 0x892;
// 	// rgb2.blue  = 0x7CD; 

// 	// RGB rgb3;
// 	// rgb3.red   = 0x25F;
// 	// rgb3.green = 0x7EC;
// 	// rgb3.blue  = 0xFC2; 

		
// 	setLedColor(0, rgb0, tab);
// 	setLedColor(1, rgb0, tab);
// 	setLedColor(2, rgb0, tab);
// 	setLedColor(3, rgb0, tab);
// 	setLedColor(4, rgb0, tab);
// 	setLedColor(5, rgb0, tab);
// 	setLedColor(6, rgb0, tab);
// 	setLedColor(7, rgb0, tab);

// 	printRegister(tab);
	
// 	return 0;
// }