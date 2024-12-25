#ifndef __HW_ABSTRACTION_H
#define __HW_ABSTRACTION_H


/* (Public) Constants Definitions */

#ifdef SIMULATOR
	#define IS_SIMULATOR		1
#else
	#define IS_SIMULATOR		0
#endif
#define SYSTEM_FREQUENCY	25000000


/* (Public) Functions Prototypes */

//Timer
void HW_TIMER_turn_on();
void HW_TIMER_init_timer(unsigned char timer_num, unsigned int Prescaler, unsigned char MatchReg, unsigned char SRImatchReg, unsigned int timer_interval);
void HW_TIMER_enable_timer(unsigned char timer_num);
void HW_TIMER_disable_timer(unsigned char timer_num);
void HW_TIMER_reset_timer(unsigned char timer_num);
void HW_TIMER_toggle_timer(unsigned char timer_num);
void HW_TIMER_toggle_timers(unsigned char timer0, unsigned char timer1, unsigned char timer2, unsigned char timer3);
void HW_TIMER_toggle_all_timers();
void HW_TIMER_set_priority_timer(unsigned char timer_num, unsigned int priority_value);
unsigned int HW_TIMER_get_timer_value(unsigned char timer_num);
unsigned char HW_TIMER_is_timer_enabled(unsigned char timer_num);

//RIT
void HW_RIT_init_RIT(unsigned int RIT_interval);
void HW_RIT_enable_RIT();
void HW_RIT_disable_RIT();
void HW_RIT_reset_RIT();
unsigned int HW_RIT_get_RIT_value();

//Display
void HW_DISPLAY_init();
void HW_DISPLAY_LCD_Clear(unsigned short color);
void HW_DISPLAY_LCD_Clear_Black();
void HW_DISPLAY_LCD_DrawLine(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, unsigned short color);
void HW_DISPLAY_LCD_SetPoint(unsigned short Xpos,unsigned short Ypos,unsigned short point);
void HW_DISPLAY_GUI_Text(unsigned short Xpos, unsigned short Ypos, unsigned char* str,unsigned short Color, unsigned short bkColor);

//Buttons
void HW_BUTTONS_init();
void HW_BUTTONS_set_priority_INT0(unsigned int priority_value);
void HW_BUTTONS_set_priority_KEY1(unsigned int priority_value);
void HW_BUTTONS_set_priority_KEY2(unsigned int priority_value);

//Joystick
void HW_JOYSTICK_init();

//ADC
void HW_ADC_init();
void HW_ADC_start_conversion();
void HW_ADC_set_priority_ADC(unsigned int priority_value);

//Speaker
void HW_SPEAKER_turn_on();
void HW_SPEAKER_emit_sound(unsigned int value);

//CAN
void HW_CAN_init();
void HW_CAN_send_pacman_stats(unsigned char time, unsigned short score, unsigned char lives);

//Power-Down Mode
void HW_PowerDown_Mode();



#endif //__HW_ABSTRACTION_H