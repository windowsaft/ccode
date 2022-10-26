/*
 * main.c
 *
 *  Created on: 2022 Oct 12 13:25:27
 *  Author: mt21001
 */



#include "xmc_common.h"

/*
 * gruen			1
 * gelb				2
 * gelb - rot		3
 * rot				4
 *
 *
 */

uint16_t i;
uint16_t ampel_phasen[] = {0x406, 0x406, 0x406, 0x406, 0x406, 0x40a, 0x412, 0x411, 0x031, 0x031, 0x031, 0x031, 0x031, 0x052, 0x41a, 0x406};
//uint16_t ampel_phasen[] = {0x86, 0x86, 0x86, 0x86, 0x86, 0x8a, 0x92, 0xd1, 0x31, 0x31, 0x31, 0x31, 0x31, 0x52, 0x9a, 0x86};

//sleep 1 sec
void warte1(uint32_t multi) {
	uint32_t a;
	for (a = 0; a < (1205500 * multi); a++);
}

int main(void)
{
	/* Placeholder for user application code. The while loop below can be replaced with user application code. */
	PORT0->IOCR0 = 0x80808080;
	PORT0->IOCR4 = 0x80808080;
	PORT0->IOCR8 = 0x80808080;

	while(1U)
	{
		if ((PORT1->IN & 0x0100) == 0) {
			  for (i = 0; i < 16; i++) {
				  PORT0->OUT = ampel_phasen[i];
				  warte1(1);

				  if ((PORT1->IN & 0x0100) > 0) break;
			  }
		}

		else {
			PORT0->OUT = 0x48;
			warte1(1);
			PORT0->OUT = 0x0;
			warte1(1);
		}

	}
}
