#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 36

const unsigned long evenMask = ~(0xfffffffff);	// 0xfffffff000000000 (in case of 8 byte unsigned long)
const unsigned long oddMask  = ~(0xfffffffff0);	// 0xffffff000000000f (in case of 8 byte unsigned long)

typedef struct RGB
{
	unsigned long red;
	unsigned long green;
	unsigned long blue;
} RGB;


/*-----------------------------------------------------------------------------
README	README	README	README	README	README	README	README README	README	
-------------------------------------------------------------------------------
		§1) LED INDEXING
			leds are indexed from 0

		§2) LED ADDRESS ARITHMETIC
			* in memory model, used in this library, even led N has address = (N/2) * 9
			* odd led N has address = ( ((N+1)/2) * 9 ) - 5

				EXAMPLE - (led)address - :
					even leds have addresses: (0)0, (2)9, (4)18, (6)27, ...
					odd leds have addresses:  (1)4, (3)13, (5)22, ...
-------------------------------------------------------------------------------
README	README	README	README	README	README	README	README README	README	
/------------------------------------------------------------------------------*/


unsigned long compileRGBcolor(RGB rgbSet)
{
	unsigned long red   = rgbSet.red,
				  green = rgbSet.green, 
				  blue  = rgbSet.blue;

	red   =	  red & 0xfff;	// remove remaining bits 
	green = green &	0xfff;	// so every color may have values from 0x000 to 0xfff
	blue  =  blue & 0xfff;

	unsigned long RGBcolor = 0;
									// [00 00 .. 00]  - example schemes are given for 8 byte long int

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

	return RGBcolor;				// result is not ready for 
}

void insertLedRgb(unsigned int ledNumber, unsigned long rgbSet, char* tab)
{
	unsigned int index = -1;
	unsigned long* tabSegment = 0;
	if( ledNumber%2 == 0 ) 					// even led number
	{
		index = (ledNumber/2) * 9;			// (N/2) * 9 	(check README)
		tabSegment = (unsigned long *) &tab[index];

		*tabSegment = (*tabSegment) & evenMask;	// clean last 4.5 bytes for new RGB value
	}
	else									// odd led number
	{
		index = ( ((ledNumber+1)/2) * 9) - 5;	// ( ((N+1)/2) * 9 ) - 5 	(check README)
		tabSegment = (unsigned long *) &tab[index];

		*tabSegment = (*tabSegment) & oddMask;	// clean last 4.5 bytes for new RGB value

		rgbSet = rgbSet << 4;					// shift compiled RGB value 4 bits left
												// (necessary in case of odd leds)
	}

	*tabSegment = (*tabSegment) | rgbSet; 		// put compiled led color to array
}


int main()
{
	char tab[SIZE];
	memset(tab, 0xFF, SIZE);

	int i = 0;

	RGB rgb0;
	rgb0.red   = 0x123;
	rgb0.green = 0x789;
	rgb0.blue  = 0xABC;

	RGB rgb1;
	rgb1.red   = 0x78D;
	rgb1.green = 0x9C0;
	rgb1.blue  = 0xBAF; 

	unsigned long RGBcolor = 0;

	// printf("ledRGBcolor: [ 7|6|5|4|3|2|1|0 ]\n");


	

	// unsigned long *ledRGBcolor = 0;

	//----------------led0

	// void setLedColor(unsigned int ledNumber, )
	// {
	// 	unsigned long RGBcolor = 0;
	// 	unsigned long *ledRGBcolor = 0;

	// 	RGBcolor = compileRGBcolor(r0, g0, b0);

	// 	ledRGBcolor = (unsigned long *) &tab[0];

	// 	*ledRGBcolor = (*ledRGBcolor) & 0xfffffff000000000;			// clean last 4.5 bytes for new RGB value

	// 	*ledRGBcolor = (*ledRGBcolor) | RGBcolor; 					// put compiled led color to array

	// 	printf("RGBcolor: %#018lx\n", RGBcolor);

	// 	printf("ledRGBcolor: %#018lx\n", *ledRGBcolor);
	// }

	
	RGBcolor = compileRGBcolor(rgb0);
	insertLedRgb(0, RGBcolor, tab);

	// ledRGBcolor = (unsigned long *) &tab[0];

	// *ledRGBcolor = (*ledRGBcolor) & evenMask;			// clean last 4.5 bytes for new RGB value

	// *ledRGBcolor = (*ledRGBcolor) | RGBcolor; 					// put compiled led color to array

	// printf("RGBcolor: %#018lx\n", RGBcolor);

	// printf("ledRGBcolor: %#018lx\n", *ledRGBcolor);

	//----------------led1

	RGBcolor = compileRGBcolor(rgb1);
	insertLedRgb(1, RGBcolor, tab);
	
	// ledRGBcolor = (unsigned long *) &tab[4];

	// *ledRGBcolor = (*ledRGBcolor) & oddMask;			// clean last 4.5 bytes for new RGB value

	// RGBcolor = RGBcolor << 4;									// shift compiled RGB value 4 bits left
	// 															// (necessary in case of odd leds)

	// printf("RGBcolor: %#018lx\n", RGBcolor);

	// *ledRGBcolor = (*ledRGBcolor) | RGBcolor; 					// put compiled led color to array


	// printf("ledRGBcolor: %#018lx\n", *ledRGBcolor);

	//------------------------------------------------------------------------------------------------//


	for(i=0; i< 10 /*SIZE*/; ++i)
	{
		printf("tab[%d]: %#x\n", i, (unsigned) tab[i] & 0x000000ff );
	}	

			   
	
	printf("evenMask: %#018lx\n", evenMask);
	printf("oddMask:  %#018lx\n", oddMask);





	// printf("sizeof(long int): %lu\n", sizeof(long int));


	
	return 0;
}