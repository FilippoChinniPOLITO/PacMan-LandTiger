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


	/* System Defines */
	
#include "LPC17xx.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
const int is_simulator = 1;
#endif


	/* Hardware Imports */

#include "GLCD/GLCD.h" 
//#include "TouchPanel/TouchPanel.h"
#include "led/led.h"
#include "timer/timer.h"


	/* User Imports */

#include "main/pacman/pacman_init.h"
	
	

int main(void)
{
	/* Area Init */
	
	// System
	SystemInit();
	
	// Display
	LCD_Initialization();
	LCD_Clear(Black);
	
	/*
	// TouchScreen
	TP_Init();
	if (!is_simulator)
		TouchPanel_Calibrate();
	*/
	
	// Led
	LED_init();
	
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
	
	/*
	// ADC
	//ADC_init();
	*/
	
	/* Area Timers */
	
	// t * f = count
	// f = 25Mhz	=> 25'000'000 Hz
	// t = richiesta del problema
	
	//init_timer(timer_num, Prescaler, MatchReg, SRImatchReg, TimerInterval);
	
		//TIMER0
	//init_timer(0, 0, 0, 3, 0x);
	//enable_timer(0);
	
		//TIMER1
	//init_timer(1, 0, 0, 3, 0x017D7840);		//1s
	//init_timer(1, 0, 0, 3, 0x001D7840);		//test
	init_timer(1, 0, 0, 3, 0x00017840);		//test
	//init_timer(1, 0, 1, 3, 0x59682F00);	//60s
	//enable_timer(1);
	
		//TIMER2
	//init_timer(2, 0, 0, 3, 0x);
	//enable_timer(2);
	
		//TIMER3
	//init_timer(3, 0, 0, 3, 0x);
	//enable_timer(3);
	
	
	/* Area Code */
	
	//scrivere codice qui
	init_game();
	enable_timer(1);
	
	
	
	/* <TEST TEST TEST> */
	
	/*
	GUI_Text(50, 280, (uint8_t *) "  TEST TEST TEST  ", Blue, Green);
	LCD_DrawLine(0, 0, 200, 200, White);
	LED_Out(0b00110011); //Test LED
	
	//Quando si usa il touch bisogna utilizzare un Timer per gestire il Refresh. Il count in questo caso è il t di refresh
	init_timer(0, 0, 0, 1, 0xC8);
	enable_timer(0);
	*/
		
	//CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	//DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	//GUI_Text(TIMER_VALUE_START_X, TIMER_VALUE_START_Y, (uint8_t *) convert_int_to_string(DWT->CYCCNT % 1024), Blue, Green);

	
	/* </TEST TEST TEST> */	
	
	
	
	/* Area Loop */
	
	LPC_SC->PCON |= 0x1;					/* power-down mode */
	LPC_SC->PCON &= ~(0x2);						
	
	while (1)	
	{
		__ASM("wfi");
	}
	
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
