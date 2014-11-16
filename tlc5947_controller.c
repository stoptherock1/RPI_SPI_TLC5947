#include "tlc5947_controller.h"

#define SIZE 36

static const unsigned long long evenMask = ~(0xFFFFFFFFF);	// EXAMPLE: 0xFFFFFFF000000000 
															// (in case of 8 byte unsigned long long)

static const unsigned long long oddMask  = ~(0xFFFFFFFFF0);	// EXAMPLE: 0xFFFFFF000000000F 
															//(in case of 8 byte unsigned long long)


unsigned long long compileRGBpattern(RGB rgbSet)
{
	unsigned long long red   = rgbSet.red,
				  	   green = rgbSet.green, 
				       blue  = rgbSet.blue;

	red   =	  red & 0xFFF;			// remove remaining bits
	green = green &	0xFFF;			// so every color may have values from 0x000 to 0xfff
	blue  =  blue & 0xFFF;			// (one and a half bytes value)

	unsigned long long RGBcolor = 0;
									// [00 00 .. 00] - example schemes of 8 byte unsigned long long

	RGBcolor =  RGBcolor | blue;	// adding 'blue' value to RGBcolor
									// [00 00 00 00 00 00 00 00] -> [00 00 00 00 00 00 0b bb]

	RGBcolor =  RGBcolor << 12;		// moving 'blue' value left for 1.5 bytes
									// [00 00 00 00 00 00 0b bb] -> [00 00 00 00 00 bb b0 00]

	RGBcolor =  RGBcolor | green;	// adding 'green' value to RGBcolor
									// [00 00 00 00 00 bb b0 00] -> [00 00 00 00 00 bb bg gg]

	RGBcolor =  RGBcolor << 12;		// moving 'blue' and 'green' values left for 1.5 bytes
									// [00 00 00 00 00 bb bg gg] -> [00 00 00 0b bb gg g0 00]

	RGBcolor =  RGBcolor | red;		// adding 'blue' value to RGBcolor
									// [00 00 00 0b bb gg g0 00] -> [00 00 00 0b bb gg gr rr]

	return RGBcolor;
}

int insertLedRgb(unsigned int ledNumber, unsigned long long rgbSet, uint8_t* tab)
{
	if(tab <= 0)
		return -1;

	unsigned int index = -1;
	unsigned long long* tabSegment = 0;

	if( 0 == ledNumber%2 ) 					// even led number
	{
		index = (ledNumber/2) * 9;						// (N/2) * 9 	(check README)
		tabSegment = (unsigned long long*) &tab[index];

		*tabSegment = (*tabSegment) & evenMask;			// clean last 4.5 bytes for new RGB value
	}
	else									// odd led number
	{
		index = ( ((ledNumber-1)/2) * 9) + 4;			// ( ((N-1)/2) * 9 ) + 4 	(check README)
		tabSegment = (unsigned long long*) &tab[index];

		*tabSegment = (*tabSegment) & oddMask;			// clean last 4.5 bytes for new RGB value

		rgbSet = rgbSet << 4;							// shift compiled RGB value 4 bits left
														// (necessary in case of odd leds)
	}

	*tabSegment = (*tabSegment) | rgbSet; 				// put compiled led color to array

	return 0;
}

int setLedColor(unsigned int ledNumber, RGB rgbSet, uint8_t* tab)
{
	unsigned long long RGBpattern = 0;

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



int main()
{
	// printf("ledRGBcolor: [ 7|6|5|4|3|2|1|0 ]\n");
	printf("sizeof(long int): %u\n", sizeof(unsigned long long));
	// printf("evenMask: %#018lx\n", evenMask);
	// printf("oddMask:  %#018lx\n", oddMask);

	uint8_t tab[SIZE];
	memset(tab, 0, SIZE);

	// int i = 0;

	RGB rgb0;
	rgb0.red   = 0x123;
	rgb0.green = 0x789;
	rgb0.blue  = 0xABC;

	RGB rgb1;
	rgb1.red   = 0x78D;
	rgb1.green = 0x9C0;
	rgb1.blue  = 0xBAF; 

	RGB rgb2;
	rgb2.red   = 0x578;
	rgb2.green = 0x892;
	rgb2.blue  = 0x7CD; 

	RGB rgb3;
	rgb3.red   = 0x25F;
	rgb3.green = 0x7EC;
	rgb3.blue  = 0xFC2; 

		
	setLedColor(4, rgb0, tab);
	setLedColor(5, rgb1, tab);
	setLedColor(6, rgb2, tab);
	setLedColor(7, rgb3, tab);

	printRegister(tab);

	
	

	// // printf("tab: ");
	// for(i=0; i<20 /*SIZE*/; ++i)
	// {
	// 	printf("tab[%d]: %#x\n", i, (unsigned) tab[i] & 0x000000FF );
	// 	// printf("%x", (unsigned) tab[i] & 0x000000ff );
	// }	
	// printf("\n");
	
	return 0;
}