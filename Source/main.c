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
 * Architetture dei Sistemi di Eleborazione - ExtraPoint 1
 *
 * Filippo Chinni Carella - S345011
 *
 */


	/* System Defines */
	
#include "LPC17xx.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag;
#endif


	/* Hardware Imports */

#include "GLCD/GLCD.h" 
#include "timer/timer.h"


	/* User Imports */

#include "main/pacman/pacman_init.h"



	/* Main function */

int main(void)
{
	/* Area Init */
	
	// System
	SystemInit();
	
	// CAN
	CAN_Init();
	
	// Display
	LCD_Initialization();
	LCD_Clear(Black);
	
	// Buttons
	BUTTON_init();
	
	// Joystick
	joystick_init();
	
	// RIT
	init_RIT(0x004C4B40);					// 50ms
	enable_RIT();
	
	// Timers
	LPC_SC -> PCONP |= (1 << 22);  			// Turn ON TIMER2 (anche da Wizard del System)
	LPC_SC -> PCONP |= (1 << 23);  			// Turn ON TIMER3 (anche da Wizard del System)
	
	
	/* Area Timers */
	
	// t * f = count
	// f = 25Mhz	=> 25'000'000 Hz
	// t = richiesta del problema
	
	//init_timer(timer_num, Prescaler, MatchReg, SRImatchReg, TimerInterval);


	// Timers are Initilized during the Game
	
	
	/* Area Program Code */
	
	init_game();

	
	
	/* <TEST TEST TEST> */

	/* </TEST TEST TEST> */	
	
	
	
	/* Area <Other> */

	//Power-Down Mode
	LPC_SC->PCON |= 0x1;
	LPC_SC->PCON &= ~(0x2);	
	
	
	/* Area Loop */
	
	while (1)	
	{
		__ASM("wfi");
	}
	
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
