#include "pacman_sound.h"


/* (Private) Constants Definitions */

#define MUSIC_SPEEDUP 		1.6f
#define MUSIC_TIMERSCALER 	1
#define MUSIC_SECOND 		(0x017D7840 * MUSIC_TIMERSCALER)

#define UPTICKS				1

#define RIT_SEMIMINIMA 8 // TODO forse non serve
#define RIT_MINIMA 16 // TODO forse non serve
#define RIT_INTERA 32 // TODO forse non serve


/* (Private) Type Definitions */

typedef enum {
	time_semibiscroma 	= (unsigned int)(MUSIC_SECOND * MUSIC_SPEEDUP / 64.0f + 0.5),	// 1/128
	time_biscroma 		= (unsigned int)(MUSIC_SECOND * MUSIC_SPEEDUP / 32.0f + 0.5),	// 1/64
	time_semicroma 		= (unsigned int)(MUSIC_SECOND * MUSIC_SPEEDUP / 16.0f + 0.5),	// 1/32
	time_croma 			= (unsigned int)(MUSIC_SECOND * MUSIC_SPEEDUP / 8.0f + 0.5),	// 1/16
	time_semiminima 	= (unsigned int)(MUSIC_SECOND * MUSIC_SPEEDUP / 4.0f + 0.5),	// 1/4
	time_minima 		= (unsigned int)(MUSIC_SECOND * MUSIC_SPEEDUP / 2.0f + 0.5),	// 1/2
	time_semibreve 		= (unsigned int)(MUSIC_SECOND * MUSIC_SPEEDUP + 0.5),			// 1
} NoteDuration;

typedef enum {
	a2b = 5351,	// 103Hz	k=5351 a2b
	b2	= 4500,	// 123Hz	k=4500 b2
	c3b	= 4370,	// 127Hz	k=4370 c3b
	c3	= 4240,	// 131Hz	k=4240 c3
	d3	= 3779,	// 147Hz	k=3779 d3
	e3	= 3367,	// 165Hz	k=3367 e3
	f3	= 3175,	// 175Hz	k=3175 f3
	g3	= 2834,	// 196Hz	k=2834 g3
	a3b	= 2670, // 208Hz	k=2670 a4b
	a3	= 2525,	// 220Hz	k=2525 a3
	b3	= 2249,	// 247Hz	k=2249 b3
	c4	= 2120,	// 262Hz	k=2120 c4
	d4	= 1890,	// 294Hz	k=1890 d4
	e4	= 1684,	// 330Hz	k=1684 e4
	f4	= 1592,	// 349Hz	k=1592 f4
	g4	= 1417,	// 392Hz	k=1417 g4
	a4	= 1263,	// 440Hz	k=1263 a4
	b4	= 1125,	// 494Hz	k=1125 b4
	c5	= 1062,	// 523Hz	k=1062 c5
	pause = 0	// DO NOT SOUND
} NoteFrequency;

typedef struct {
	NoteFrequency frequency;
	NoteDuration duration;
} Note;


/* (Private) Constants Definitions - Songs */

const Note provv_song[] = {
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


/* (Private) Constants Definitions - SinTable */

const unsigned short SinTable[45] = {
	410, 467, 523, 576, 627, 673, 714, 749, 778,
	799, 813, 819, 817, 807, 789, 764, 732, 694, 
	650, 602, 550, 495, 438, 381, 324, 270, 217,
	169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
	20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};


/* (Private) Function Prototypes */

void play_note(Note note);
unsigned char is_note_playing();
unsigned char is_full_sound_over(unsigned short curr_note, Note full_sound[]);


/* Function Implementations */

void play_full_sound() {
	static unsigned short current_note = 0;
	static unsigned char ticks = 0;
	
	if(!is_note_playing()) {
		++ticks;
		if(ticks == UPTICKS) {
			ticks = 0;
			play_note(provv_song[current_note++]);
		}
	}
	
	if(current_note == (sizeof(provv_song) / sizeof(provv_song[0]))) {
		current_note = 0;
	}
}

/*
unsigned char is_full_sound_over(unsigned short curr_note, Note full_sound[]) {
	return (curr_note == sizeof(full_sound) / sizeof(full_sound[0]);
}
*/

void play_single_sound() {
	static unsigned char sineticks;
	static unsigned int current_value;
	
	current_value = SinTable[sineticks];
	current_value -= 410;
	current_value /= 1;
	current_value += 410;
	
	LPC_DAC->DACR = current_value << 6;
	
	if(++sineticks == 45)	sineticks = 0;
}

void play_single_sound_duration() {
	//disable_timer(2);
	LPC_TIM2->TCR = 0;
}

void play_note(Note note) {
	if(note.frequency != pause) {
		reset_timer(2);
		init_timer(2, 0, 0, 3, note.frequency);
		enable_timer(2);
	}
	reset_timer(3);
	init_timer(3, 0, 0, 7, note.duration);
	enable_timer(3);
}

unsigned char is_note_playing() {
	return ((LPC_TIM2->TCR != 0) || (LPC_TIM3->TCR != 0));
}



