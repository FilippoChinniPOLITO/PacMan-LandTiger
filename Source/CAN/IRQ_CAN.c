/*----------------------------------------------------------------------------
 * Name:    Can.c
 * Purpose: CAN interface for for LPC17xx with MCB1700
 * Note(s): see also http://www.port.de/engl/canprod/sv_req_form.html
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "CAN.h"                      /* LPC17xx CAN adaption layer */

//#include "../GLCD/GLCD.h"


#include "../main/interfaces/user_CAN.h"


extern uint8_t icr ; 				//icr and result must be global in order to work with both real and simulated landtiger.
extern uint32_t result;

extern CAN_msg CAN_TxMsg;    /* CAN message for sending */
extern CAN_msg CAN_RxMsg;    /* CAN message for receiving */                                


/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_IRQHandler (void)  {

	/* CAN Controller 1 */
	
	icr = 0;
	icr = (LPC_CAN1->ICR | icr) & 0xFF;               		/* clear interrupts */
	
	/* CAN Controller 1 - Message is Received */
	if (icr & (1 << 0)) {
		CAN_rdMsg (1, &CAN_RxMsg);	                		/* Read the message */
		LPC_CAN1->CMR = (1 << 2);                    		/* Release receive buffer */
		
		//your code
	}
	
	/* CAN Controller 1 - Message is Transmitted */
	if (icr & (1 << 1)) {
		//your code
	}
	
	
	/* CAN Controller 2 */
	
	icr = 0;
	icr = (LPC_CAN2->ICR | icr) & 0xFF;             		/* clear interrupts */

	/* CAN Controller 2 - Message is Received */
	if (icr & (1 << 0)) {
		CAN_rdMsg (2, &CAN_RxMsg);	                		/* Read the message */
		LPC_CAN2->CMR = (1 << 2);                    		/* Release receive buffer */
		
		
		interrupt_CAN_write_stat(CAN_RxMsg.data);
		
		
	}
	
	/* CAN Controller 2 - Message is Transmitted */
	if (icr & (1 << 1)) {
		//your code
	}
}

