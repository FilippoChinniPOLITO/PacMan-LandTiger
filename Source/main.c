/***
 *                                                                                                                                                       
 *                                                                                                                                                       
 *    PPPPPPPPPPPPPPPPP        AAA                  CCCCCCCCCCCCCMMMMMMMM               MMMMMMMM               AAA               NNNNNNNN        NNNNNNNN
 *    P::::::::::::::::P      A:::A              CCC::::::::::::CM:::::::M             M:::::::M              A:::A              N:::::::N       N::::::N
 *    P::::::PPPPPP:::::P    A:::::A           CC:::::::::::::::CM::::::::M           M::::::::M             A:::::A             N::::::::N      N::::::N
 *    PP:::::P     P:::::P  A:::::::A         C:::::CCCCCCCC::::CM:::::::::M         M:::::::::M            A:::::::A            N:::::::::N     N::::::N
 *      P::::P     P:::::P A:::::::::A       C:::::C       CCCCCCM::::::::::M       M::::::::::M           A:::::::::A           N::::::::::N    N::::::N
 *      P::::P     P:::::PA:::::A:::::A     C:::::C              M:::::::::::M     M:::::::::::M          A:::::A:::::A          N:::::::::::N   N::::::N
 *      P::::PPPPPP:::::PA:::::A A:::::A    C:::::C              M:::::::M::::M   M::::M:::::::M         A:::::A A:::::A         N:::::::N::::N  N::::::N
 *      P:::::::::::::PPA:::::A   A:::::A   C:::::C              M::::::M M::::M M::::M M::::::M        A:::::A   A:::::A        N::::::N N::::N N::::::N
 *      P::::PPPPPPPPP A:::::A     A:::::A  C:::::C              M::::::M  M::::M::::M  M::::::M       A:::::A     A:::::A       N::::::N  N::::N:::::::N
 *      P::::P        A:::::AAAAAAAAA:::::A C:::::C              M::::::M   M:::::::M   M::::::M      A:::::AAAAAAAAA:::::A      N::::::N   N:::::::::::N
 *      P::::P       A:::::::::::::::::::::AC:::::C              M::::::M    M:::::M    M::::::M     A:::::::::::::::::::::A     N::::::N    N::::::::::N
 *      P::::P      A:::::AAAAAAAAAAAAA:::::AC:::::C       CCCCCCM::::::M     MMMMM     M::::::M    A:::::AAAAAAAAAAAAA:::::A    N::::::N     N:::::::::N
 *    PP::::::PP   A:::::A             A:::::AC:::::CCCCCCCC::::CM::::::M               M::::::M   A:::::A             A:::::A   N::::::N      N::::::::N
 *    P::::::::P  A:::::A               A:::::ACC:::::::::::::::CM::::::M               M::::::M  A:::::A               A:::::A  N::::::N       N:::::::N
 *    P::::::::P A:::::A                 A:::::A CCC::::::::::::CM::::::M               M::::::M A:::::A                 A:::::A N::::::N        N::::::N
 *    PPPPPPPPPPAAAAAAA                   AAAAAAA   CCCCCCCCCCCCCMMMMMMMM               MMMMMMMMAAAAAAA                   AAAAAAANNNNNNNN         NNNNNNN
 *                                                                                                                                                                                                                                                                                                   
 *                                                                                                                                                       
 */
 
/**
 *
 * Politecnico di Torino - A.A. 2024/2025
 * 
 * Architetture dei Sistemi di Eleborazione - ExtraPoint 2
 *
 * Filippo Chinni Carella - S345011
 *
 */


	/* System Defines */

#ifdef SIMULATOR
	extern unsigned char ScaleFlag;
#endif

	/* System Imports */
	
#include "LPC17xx.h"

	/* Hardware Imports */

//#include "main/interfaces/hw_abstraction.h"		// Transitive Import

	/* User Imports */

#include "main/pacman/pacman_init.h"



	/* Main function */

int main() {
	
	/* Area Init */
	
	// System
	SystemInit();
	
	// Display
	HW_DISPLAY_init();
	HW_DISPLAY_LCD_Clear_Black();
	
	// Buttons
	HW_BUTTONS_init();
	
	// Joystick
	HW_JOYSTICK_init();
	
	// ADC
	HW_ADC_init();
	
	// Speaker
	HW_SPEAKER_turn_on();
	
	// CAN
	HW_CAN_init();
	
	// RIT
	HW_RIT_init_RIT(0x004C4B40);			// 50ms
	HW_RIT_enable_RIT();
	
	// Timers
	HW_TIMER_turn_on();
	
	
	/* Area Program Code */
	
	init_game();

	
	
	/* <TEST TEST TEST> */

	/* </TEST TEST TEST> */	
	
	
	
	/* Area <Other> */

	//Power-Down Mode
	HW_PowerDown_Mode();
	
	
	/* Area Loop */
	
	while (1)	
	{
		__ASM("wfi");
	}
	
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
