#ifndef TLC5947_CONTROLLER_H
#define TLC5947_CONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdint.h>
#include "RGB.h"

#define REAL_SIZE 39		// real size is bigger by 3 bytes, to prevent reading/writing out of scope
#define USED_SIZE 36		// it is caused by implemented here color compilation mechanism

#define SIZE USED_SIZE 		// constant value SIZE is wide used here

/*----------------------------------------------------------------------------------------------------
README	README	README	README	README	README	README	README README	README	README	README	README
------------------------------------------------------------------------------------------------------
		§1) LED INDEXING
			leds are indexed from 0

		§2) LED ADDRESS ARITHMETIC
			* in memory model, used in this library, even led N has address = (N/2) * 9
			* odd led N has address = ( ((N-1)/2) * 9 ) + 4

				EXAMPLE - (led)address - :
					even leds have addresses: (0)0, (2)9, (4)18, (6)27, ...
					odd leds have addresses:  (1)4, (3)13, (5)22, ...
------------------------------------------------------------------------------------------------------
README	README	README	README	README	README	README	README README	README	README	README	README
/---------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

int setLedRGB(uint32_t ledNumber, RGB rgbSet, uint8_t* tab);
RGB getLedRGB(uint8_t ledNumber, uint8_t* tab);
void printLedDataArray(uint8_t* tab);

#ifdef __cplusplus
}
#endif

#endif /*TLC5947_CONTROLLER_H*/
