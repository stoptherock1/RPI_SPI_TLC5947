#include "tlc5947_controller.h"

static const uint64_t evenMask = ~(0xFFFFFFFFF);	// 0xFFFFFFF000000000 
static const uint64_t oddMask  = ~(0xFFFFFFFFF0);	// 0xFFFFFF000000000F

uint64_t compileRGBpattern(RGB rgbSet)
{
	uint64_t RGBpattern = 0;
	uint64_t hex = 0;					

	uint64_t red   = rgbSet.red   & 0xFFF,	// remove remaining bits
	    	 green = rgbSet.green & 0xFFF, 	// so every color may have values from 0x000 to 0xfff
		     blue  = rgbSet.blue  & 0xFFF;	// (one and a half bytes value)
			
	uint64_t hex1Mask = 0xF00;				// masks for extracting corresponding hex
	uint64_t hex2Mask = 0x0F0;
	uint64_t hex3Mask = 0x00F;


	//EVEN
		// EXAMPLE:
		//
		// for RGB rgbSet.red   = 0x123,
		// 	  	   rgbSet.green = 0x789,
		// 	       rgbSet.blue  = 0xABC;
		//
		// function should compile following RGBpattern:
		// RGBpattern = [00 00 00 C0 AB 89 37 12]

	//compiling red color
	hex = 0;						//        hex = [00 00 00 00 00 00 00 00]
	hex = red & hex1Mask;			//        hex = [00 00 00 00 00 00 01 00]
	hex = hex >> 4;					//        hex = [00 00 00 00 00 00 00 10]
	RGBpattern = RGBpattern | hex; 	// RGBpattern = [00 00 00 00 00 00 00 10]

	hex = 0;						//        hex = [00 00 00 00 00 00 00 00]
	hex = red & hex2Mask;			//        hex = [00 00 00 00 00 00 00 20]
	hex = hex >> 4;					//        hex = [00 00 00 00 00 00 00 02]
	RGBpattern = RGBpattern | hex; 	// RGBpattern = [00 00 00 00 00 00 00 12]

	hex = 0;						//        hex = [00 00 00 00 00 00 00 00]
	hex = red & hex3Mask;			//        hex = [00 00 00 00 00 00 00 03]
	hex = hex << 12;				//        hex = [00 00 00 00 00 00 30 00]
	RGBpattern = RGBpattern | hex; 	// RGBpattern = [00 00 00 00 00 00 30 12]

	//compiling green color
	hex = 0;						//        hex = [00 00 00 00 00 00 00 00]
	hex = green & hex1Mask;			//        hex = [00 00 00 00 00 00 07 00]
	RGBpattern = RGBpattern | hex; 	// RGBpattern = [00 00 00 00 00 00 37 12]

	hex = 0;						//        hex = [00 00 00 00 00 00 00 00]
	hex = green & hex2Mask;			//        hex = [00 00 00 00 00 00 00 80]
	hex = hex << 16;				//        hex = [00 00 00 00 00 80 00 00]
	RGBpattern = RGBpattern | hex; 	// RGBpattern = [00 00 00 00 00 80 37 12]

	hex = 0;						//        hex = [00 00 00 00 00 00 00 00]
	hex = green & hex3Mask;			//        hex = [00 00 00 00 00 00 00 09]
	hex = hex << 16;				//        hex = [00 00 00 00 00 09 00 00]
	RGBpattern = RGBpattern | hex; 	// RGBpattern = [00 00 00 00 00 89 37 12]

	//compiling blue color
	hex = 0;						//        hex = [00 00 00 00 00 00 00 00]
	hex = blue & hex1Mask;			//        hex = [00 00 00 00 00 00 0A 00]
	hex = hex << 20;				//        hex = [00 00 00 00 A0 00 00 00]
	RGBpattern = RGBpattern | hex; 	// RGBpattern = [00 00 00 00 A0 89 37 12]

	hex = 0;						//        hex = [00 00 00 00 00 00 00 00]
	hex = blue & hex2Mask;			//        hex = [00 00 00 00 00 00 00 B0]
	hex = hex << 20;				//        hex = [00 00 00 00 0B 00 00 00]
	RGBpattern = RGBpattern | hex; 	// RGBpattern = [00 00 00 00 AB 89 37 12]

	hex = 0;						//        hex = [00 00 00 00 00 00 00 00]
	hex = blue & hex3Mask;			//        hex = [00 00 00 00 00 00 00 0C]
	hex = hex << 36;				//        hex = [00 00 00 C0 00 00 00 00]
	RGBpattern = RGBpattern | hex; 	// RGBpattern = [00 00 00 C0 A0 89 37 12]

	//ODD
	//000BC9A78230100000000]
	
	return RGBpattern;
}

int insertLedRgb(uint32_t ledNumber, uint64_t rgbSet, uint8_t* tab)
{
	if(tab <= 0)
		return -1;

	uint32_t index = -1;
	uint64_t* tabSegment = 0;

	if( 0 == ledNumber%2 ) 					// even led number
	{
		index = (ledNumber/2) * 9;				// (N/2) * 9 	(check README)
		tabSegment = (uint64_t*) &tab[index];

		*tabSegment = (*tabSegment) & evenMask;	// clean last 4.5 bytes for new RGB value
	}
	else									// odd led number
	{
		index = ( ((ledNumber-1)/2) * 9) + 4;	// ( ((N-1)/2) * 9 ) + 4 	(check README)
		tabSegment = (uint64_t*) &tab[index];

		*tabSegment = (*tabSegment) & oddMask;	// clean last 4.5 bytes for new RGB value

		rgbSet = rgbSet << 4;					// shift compiled RGB value 4 bits left
												// (necessary in case of odd leds)
	}

	*tabSegment = (*tabSegment) | rgbSet; 		// put compiled led color to array

	return 0;
}

int setLedColor(uint32_t ledNumber, RGB rgbSet, uint8_t* tab)
{
	uint64_t RGBpattern = 0;

	RGBpattern   = compileRGBpattern(rgbSet);
	int result = insertLedRgb(ledNumber, RGBpattern, tab);

	return result;
}

void printRegister(uint8_t *tab)
{
	int i=SIZE-1;
	
	printf("tab: [");

	for(i=SIZE-1; i>=0; --i)
	{
		printf("%02X", tab[i]);
	}

	printf("]\n");
}



// int main()
// {
// 	// printf("ledRGBcolor: [ 7|6|5|4|3|2|1|0 ]\n");
// 	printf("sizeof(long int): %lu\n", sizeof(uint64_t));
// 	// printf("evenMask: %#018lx\n", evenMask);
// 	// printf("oddMask:  %#018lx\n", oddMask);

// 	uint8_t tab[SIZE];
// 	memset(tab, 0, SIZE);

// 	// int i = 0;

// 	RGB rgb0;
// 	rgb0.red   = 0x123;
// 	rgb0.green = 0x789;
// 	rgb0.blue  = 0xABC;

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
// 	// setLedColor(5, rgb1, tab);
// 	// setLedColor(6, rgb2, tab);
// 	// setLedColor(7, rgb3, tab);

// 	printRegister(tab);
	
// 	return 0;
// }