/*
 * main.c
 *
 *  Created on: 2022 Oct 12 12:45:44
 *  Author: mt21001
 */



#include "xmc_common.h"


uint32_t i = 0;
int main(void)
{
  PORT0->IOCR0 = 0x80808080;
  PORT0->IOCR4 = 0x80808080;
  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
  while(1U)
  {
	  for (i = 0; i < 1005500; i++);
	  PORT0->OUT++;
  }
}
