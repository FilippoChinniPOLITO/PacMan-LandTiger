#include "hw_abstraction.h"


/* (Private) System Imports */

#include "LPC17xx.h"


/* (Private) Hardware Imports */

#include "../../timer/timer.h"
#include "../../RIT/RIT.h"
#include "../../GLCD/GLCD.h"
#include "../../button/button.h"
#include "../../joystick/joystick.h"
#include "../../ADC/adc.h"
#include "../../CAN/CAN.h"


/* (Private) Functions Prototypes */


/* Functions Implementations */

	/* TIMER */

void HW_TIMER_turn_on() {
	LPC_SC->PCONP |= (1 << 22);  	// Turn ON TIMER2
	LPC_SC->PCONP |= (1 << 23);  	// Turn ON TIMER3
}

void HW_TIMER_init_timer(unsigned char timer_num, unsigned int Prescaler, unsigned char MatchReg, unsigned char SRImatchReg, unsigned int timer_interval) {
	init_timer(timer_num, Prescaler, MatchReg, SRImatchReg, timer_interval);
}

void HW_TIMER_enable_timer(unsigned char timer_num) {
	enable_timer(timer_num);
}

void HW_TIMER_disable_timer(unsigned char timer_num) {
	disable_timer(timer_num);
}

void HW_TIMER_reset_timer(unsigned char timer_num) {
	reset_timer(timer_num);
}

void HW_TIMER_toggle_timer(unsigned char timer_num) {
	switch(timer_num) {
		case 0:
			LPC_TIM0->TCR = !LPC_TIM0->TCR;
			return;
		case 1:
			LPC_TIM1->TCR = !LPC_TIM1->TCR;
			return;
		case 2:
			LPC_TIM2->TCR = !LPC_TIM2->TCR;
			return;
		case 3:
			LPC_TIM3->TCR = !LPC_TIM3->TCR;
			return;
		default:
			return;
	}
}

void HW_TIMER_toggle_timers(unsigned char timer0, unsigned char timer1, unsigned char timer2, unsigned char timer3) {
	if(timer0)
		HW_TIMER_toggle_timer(0);
	
	if(timer1)
		HW_TIMER_toggle_timer(1);
	
	if(timer2)
		HW_TIMER_toggle_timer(2);
	
	if(timer3)
		HW_TIMER_toggle_timer(3);
}

void HW_TIMER_toggle_all_timers() {
	HW_TIMER_toggle_timer(0);
	HW_TIMER_toggle_timer(1);
	HW_TIMER_toggle_timer(2);
	HW_TIMER_toggle_timer(3);
}

void HW_TIMER_set_priority_timer(unsigned char timer_num, unsigned int priority_value) {
	switch(timer_num) {
		case 0:
			NVIC_SetPriority(TIMER0_IRQn, priority_value);
			return;
		case 1:
			NVIC_SetPriority(TIMER1_IRQn, priority_value);
			return;
		case 2:
			NVIC_SetPriority(TIMER2_IRQn, priority_value);
			return;
		case 3:
			NVIC_SetPriority(TIMER3_IRQn, priority_value);
			return;
		default:
			return;
	}
}

unsigned int HW_TIMER_get_timer_value(unsigned char timer_num) {
	switch(timer_num) {
		case 0:
			return LPC_TIM0->TC;
		case 1:
			return LPC_TIM1->TC;
		case 2:
			return LPC_TIM2->TC;
		case 3:
			return LPC_TIM3->TC;
		default:
			return 0;
	}
}

unsigned char HW_TIMER_is_timer_enabled(unsigned char timer_num) {
	switch(timer_num) {
		case 0:
			return (LPC_TIM0->TCR == 1);
		case 1:
			return (LPC_TIM1->TCR == 1);
		case 2:
			return (LPC_TIM2->TCR == 1);
		case 3:
			return (LPC_TIM3->TCR == 1);
		default:
			return 0;
	}
}


	/* RIT */

void HW_RIT_init_RIT(unsigned int RIT_interval) {
	init_RIT(RIT_interval);
}

void HW_RIT_enable_RIT() {
	enable_RIT();
}

void HW_RIT_disable_RIT() {
	disable_RIT();
}

void HW_RIT_reset_RIT() {
	reset_RIT();
}

unsigned int HW_RIT_get_RIT_value() {
	return LPC_RIT->RICOUNTER;
}


	/* DISPLAY*/

void HW_DISPLAY_init() {
	LCD_Initialization();
}

void HW_DISPLAY_LCD_Clear(unsigned short color) {
	LCD_Clear(color);
}

void HW_DISPLAY_LCD_Clear_Black() {
	HW_DISPLAY_LCD_Clear(Black);
}

void HW_DISPLAY_LCD_DrawLine(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, unsigned short color) {
	LCD_DrawLine(x0, y0, x1, y1, color);
}

void HW_DISPLAY_LCD_SetPoint(unsigned short Xpos, unsigned short Ypos, unsigned short point) {
	LCD_SetPoint(Xpos, Ypos, point);
}

void HW_DISPLAY_GUI_Text(unsigned short Xpos, unsigned short Ypos, unsigned char* str,unsigned short Color, unsigned short bkColor) {
	GUI_Text(Xpos, Ypos, str, Color, bkColor);
}


	/* BUTTONS */

void HW_BUTTONS_init() {
	BUTTON_init();
}

void HW_BUTTONS_set_priority_INT0(unsigned int priority_value) {
	NVIC_SetPriority(EINT0_IRQn, priority_value);
}

void HW_BUTTONS_set_priority_KEY1(unsigned int priority_value) {
	NVIC_SetPriority(EINT1_IRQn, priority_value);
}

void HW_BUTTONS_set_priority_KEY2(unsigned int priority_value) {
	NVIC_SetPriority(EINT2_IRQn, priority_value);
}


	/* JOYSTICK */

void HW_JOYSTICK_init() {
	joystick_init();
}


	/* ADC */

void HW_ADC_init() {
	ADC_init();
}

void HW_ADC_start_conversion() {
	ADC_start_conversion();
}

void HW_ADC_set_priority_ADC(unsigned int priority_value) {
	NVIC_SetPriority(ADC_IRQn, priority_value);
}


	/* Speaker */

void HW_SPEAKER_turn_on() {
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
}

void HW_SPEAKER_emit_sound(unsigned int value) {
	LPC_DAC->DACR = value << 6;
}


	/* CAN */

void HW_CAN_init() {
	CAN_Init();
}

void HW_CAN_send_pacman_stats(unsigned char time, unsigned short score, unsigned char lives) {
	CAN_TxMsg.data[0] = time;
	CAN_TxMsg.data[1] = lives;
	CAN_TxMsg.data[2] = (score & 0xFF00) >> 8;
	CAN_TxMsg.data[3] = score & 0xFF;
	
	CAN_TxMsg.len = 4;
	CAN_TxMsg.id = 2;
	CAN_TxMsg.format = STANDARD_FORMAT;
	CAN_TxMsg.type = DATA_FRAME;
	
	CAN_wrMsg(1, &CAN_TxMsg);
}


	/* Power-Down Mode*/

void HW_PowerDown_Mode() {
	LPC_SC->PCON |= 0x1;
	LPC_SC->PCON &= ~(0x2);	
}


