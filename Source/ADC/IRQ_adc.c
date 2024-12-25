/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "adc.h"
//#include "../led/led.h"
//#include "../timer/timer.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

void ADC_IRQHandler(void) {
	static unsigned short AD_last = 0x0FF;     			/* Last converted value               */
	static unsigned short AD_current;
  	
	AD_current = ((LPC_ADC -> ADGDR >> 4) & 0xFFF);		/* Read Conversion Result             */
	
	if(AD_current != AD_last) {

		
		interrupt_ADC_change_volume(AD_current);
		
		
		AD_last = AD_current;
	}
}
