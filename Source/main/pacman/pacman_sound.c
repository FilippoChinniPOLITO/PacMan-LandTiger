#include "pacman_sound.h"


/* (Private) Hardware Imports */

//#include "../interfaces/hw_abstraction.h"		// Transitive Import


/* (Private) User Imports */

//#include "pacman_core.h"


/* (Private) Constants Definitions */

#define SOUND_TIMERSCALER 		1
#define SOUND_SECOND 			(0x017D7840 * SOUND_TIMERSCALER)

#define SOUND_UPTICKS			1

#if (IS_SIMULATOR == 1)
	#define SOUND_SPEEDUP 		1.6f
	#define SOUND_AMPLIFIER		3
#else
	#define SOUND_SPEEDUP 		1.0f
	#define SOUND_AMPLIFIER		1
#endif

const unsigned short SIN_TABLE[45] = {
	410, 467, 523, 576, 627, 673, 714, 749, 778,
	799, 813, 819, 817, 807, 789, 764, 732, 694, 
	650, 602, 550, 495, 438, 381, 324, 270, 217,
	169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
	20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};


/* (Private) Types Definitions */

typedef enum {
	time_semibiscroma 	= (unsigned int)(SOUND_SECOND * SOUND_SPEEDUP / 64.0f + 0.5),	// 1/128
	time_biscroma 		= (unsigned int)(SOUND_SECOND * SOUND_SPEEDUP / 32.0f + 0.5),	// 1/64
	time_semicroma 		= (unsigned int)(SOUND_SECOND * SOUND_SPEEDUP / 16.0f + 0.5),	// 1/32
	time_croma 			= (unsigned int)(SOUND_SECOND * SOUND_SPEEDUP / 8.0f + 0.5),	// 1/16
	time_semiminima 	= (unsigned int)(SOUND_SECOND * SOUND_SPEEDUP / 4.0f + 0.5),	// 1/4
	time_minima 		= (unsigned int)(SOUND_SECOND * SOUND_SPEEDUP / 2.0f + 0.5),	// 1/2
	time_semibreve 		= (unsigned int)(SOUND_SECOND * SOUND_SPEEDUP + 0.5),			// 1
} NoteDuration;

typedef enum {
	a2b = 5351 * SOUND_AMPLIFIER,	// 103Hz	k=5351 a2b
	b2	= 4500 * SOUND_AMPLIFIER,	// 123Hz	k=4500 b2
	c3b	= 4370 * SOUND_AMPLIFIER,	// 127Hz	k=4370 c3b
	c3	= 4240 * SOUND_AMPLIFIER,	// 131Hz	k=4240 c3
	d3	= 3779 * SOUND_AMPLIFIER,	// 147Hz	k=3779 d3
	e3	= 3367 * SOUND_AMPLIFIER,	// 165Hz	k=3367 e3
	f3	= 3175 * SOUND_AMPLIFIER,	// 175Hz	k=3175 f3
	g3	= 2834 * SOUND_AMPLIFIER,	// 196Hz	k=2834 g3
	a3b	= 2670 * SOUND_AMPLIFIER,	// 208Hz	k=2670 a4b
	a3	= 2525 * SOUND_AMPLIFIER,	// 220Hz	k=2525 a3
	b3	= 2249 * SOUND_AMPLIFIER,	// 247Hz	k=2249 b3
	c4	= 2120 * SOUND_AMPLIFIER,	// 262Hz	k=2120 c4
	d4	= 1890 * SOUND_AMPLIFIER,	// 294Hz	k=1890 d4
	e4	= 1684 * SOUND_AMPLIFIER,	// 330Hz	k=1684 e4
	f4	= 1592 * SOUND_AMPLIFIER,	// 349Hz	k=1592 f4
	g4	= 1417 * SOUND_AMPLIFIER,	// 392Hz	k=1417 g4
	a4	= 1263 * SOUND_AMPLIFIER,	// 440Hz	k=1263 a4
	b4	= 1125 * SOUND_AMPLIFIER,	// 494Hz	k=1125 b4
	c5	= 1062 * SOUND_AMPLIFIER,	// 523Hz	k=1062 c5
	pause = 0						// DO NOT SOUND
} NoteFrequency;

typedef struct {
	NoteFrequency frequency;
	NoteDuration duration;
} Note;


/* (Private) Constants Definitions - Full Sounds */

const unsigned char PACMAN_SOUNDTRACK_SIZE = 48;	// for Optimization
const Note PACMAN_SOUNDTRACK[] = {
	// 1
	{d3, time_semicroma},
	{d3, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 2
	{c3, time_semicroma},
	{c3, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 3
	{c3b, time_semicroma},
	{c3b, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 4
	{a2b, time_semicroma},
	{a2b, time_semicroma},
	{d4, time_croma},
	{a3, time_croma},
	{pause, time_semicroma},
	{a3b, time_semicroma},
	{pause, time_semicroma},
	{g3, time_croma},
	{f3, time_semicroma*2},
	{d3, time_semicroma},
	{f3, time_semicroma},
	{g3, time_semicroma},
	// 5	
};


/* (Private) Functions Prototypes */

void play_note(Note note);
unsigned char is_note_playing();
unsigned char is_full_sound_over(unsigned short curr_note, Note full_sound[]);


/* (Private) File-Scope Global Variables */

static unsigned char volume_mult = 8;


/* Functions Implementations */

void play_full_sound() {
	static unsigned short current_note = 0;
	static unsigned char ticks = 0;
	
	if(!is_note_playing()) {
		++ticks;
		if(ticks == SOUND_UPTICKS) {
			ticks = 0;
			play_note(PACMAN_SOUNDTRACK[current_note++]);
		}
	}
	
	//if(current_note == (sizeof(PACMAN_SOUNDTRACK) / sizeof(PACMAN_SOUNDTRACK[0]))) {
	if(current_note == PACMAN_SOUNDTRACK_SIZE) {
		current_note = 0;
	}
}


void play_single_sound() {
	static unsigned char sineticks;
	static unsigned int current_value;
	
	current_value = SIN_TABLE[sineticks];
	
	//current_value -= 410;		//????
	//current_value /= 1;			//????
	//current_value += 410;		//????
	
	HW_SPEAKER_emit_sound(current_value);
	
	if(++sineticks >= 45)	sineticks = 0;
}

void play_single_sound_duration() {
	HW_TIMER_disable_timer(2);
}

void play_note(Note note) {
	if((note.frequency != pause) && (volume_mult != 0)) {
		HW_TIMER_reset_timer(2);
		HW_TIMER_init_timer(2, 0, 0, 3, note.frequency * volume_mult);
		HW_TIMER_enable_timer(2);
	}
	HW_TIMER_reset_timer(3);
	HW_TIMER_init_timer(3, 0, 0, 7, note.duration);
	HW_TIMER_enable_timer(3);
}

unsigned char is_note_playing() {
	return (HW_TIMER_is_timer_enabled(2) || HW_TIMER_is_timer_enabled(3));
}

void handle_change_volume(unsigned short value) {
	const unsigned short ADC_MAX = 0xFFF;	// ADC max value (12 bit)
	const unsigned char NUM_SPLIT = 10;		// Number of different Volume values
	
	unsigned char temp = (value * (NUM_SPLIT-1)) / ADC_MAX;
	volume_mult = temp;
}

