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
	#define SOUND_SPEEDUP 		1.4f
	#define SOUND_AMPLIFIER		2
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
	// k = SYSTEM_FREQUENCY / (NOTE_FREQ * 45)
	c2	= 8547,		// 065Hz	k=0000	c2
	d2	= 7610,		// 073Hz	k=0000	d2
	e2	= 6775,		// 082Hz	k=0000	e2
	f2	= 6386,		// 087Hz	k=0000	f2
	g2	= 5669,		// 098Hz	k=0000	g2
	a2b = 5351,		// 103Hz	k=5351	a2b
	b2	= 4500,		// 123Hz	k=4500	b2
	c3b = 4370,		// 127Hz	k=4370	c3b
	c3	= 4240,		// 131Hz	k=4240	c3
	c3s = 3996,		// 139Hz	k=3996	c3s
	d3	= 3779,		// 147Hz	k=3779	d3
	e3	= 3367,		// 165Hz	k=3367	e3
	f3	= 3175,		// 175Hz	k=3175	f3
	f3s	= 3003,		// 185Hz    k=3003	f3s
	g3	= 2834,		// 196Hz	k=2834	g3
	a3b	= 2670,		// 208Hz	k=2670	a3b
	a3	= 2525,		// 220Hz	k=2525	a3
	a3s	= 2384,		// 233Hz    k=2384	a3s
	b3	= 2249,		// 247Hz	k=2249	b3
	c4	= 2120,		// 262Hz	k=2120	c4
	c4s	= 2005,		// 277Hz    k=2005	c4s
	d4	= 1890,		// 294Hz	k=1890	d4
	e4	= 1684,		// 330Hz	k=1684	e4
	f4	= 1592,		// 349Hz	k=1592	f4
	g4	= 1417,		// 392Hz	k=1417	g4
	a4	= 1263,		// 440Hz	k=1263	a4
	b4	= 1125,		// 494Hz	k=1125	b4
	c5	= 1062,		// 523Hz	k=1062	c5
	d5	= 946 ,		// 587Hz	k=945	d5
	e5	= 843 ,		// 659Hz	k=842	e5
	f5	= 796 ,		// 698Hz	k=796	f5
	g5	= 709 ,		// 784Hz	k=708	g5
	a5	= 631 ,		// 880Hz	k=631	a5
	b5	= 562 ,		// 988Hz	k=562	b5
	pause = 0		// No Sound
} NoteFrequency;

typedef struct {
	NoteFrequency frequency;
	NoteDuration duration;
} Note;

typedef struct {
	SoundID id;					// ID of the Sound
	const Note* note_list;		// Pointer to the Array of Notes and Notes Durations
	unsigned char size;			// Size of the Array of Notes and Notes Durations
	unsigned char is_loop;		// Type of the Sound (to play or not in a loop)
} Sound;


/* (Private) Constants Definitions - Full Sounds */

const Note S_PACMAN_SOUNDTRACK[] = {
    {c3,	time_semicroma},
	{pause,	time_semicroma},
	{c4,	time_semicroma},
	{pause,	time_semicroma},
	{g3,	time_semicroma},
	{pause,	time_semicroma},
	{e3,	time_semicroma},
	{pause,	time_semicroma},
	{c4,	time_semicroma},
	{g3,	time_semicroma},
	{pause,	time_croma},
	{e3,	time_croma},
	{pause,	time_croma},
	{c3s,	time_semicroma},
	{pause,	time_semicroma},
	{c4s,	time_semicroma},
	{pause,	time_semicroma},
	{a3b,	time_semicroma},
	{pause,	time_semicroma},
	{f3,	time_semicroma},
	{pause,	time_semicroma},
	{c4s,	time_semicroma},
	{a3b,	time_semicroma},
	{pause,	time_croma},
	{f3,	time_croma},
	{pause,	time_croma},
	{c3,	time_semicroma},
	{pause,	time_semicroma},
	{c4,	time_semicroma},
	{g3,	time_semicroma},
	{pause,	time_semicroma},
	{e3,	time_semicroma},
	{pause,	time_semicroma},
	{c4,	time_semicroma},
	{g3,	time_semicroma},
	{pause,	time_croma},
	{e3,	time_croma},
	{pause,	time_croma},
	{e3,	time_semicroma},
	{f3,	time_semicroma},
	{f3s,	time_semicroma},
	{pause,	time_semicroma},
	{f3s,	time_semicroma},
	{g3,	time_semicroma},
	{a3b,	time_semicroma},
	{pause,	time_semicroma},
	{a3b,	time_semicroma},
	{a3,	time_semicroma},
	{a3s,	time_semicroma},
	{pause,	time_semicroma},
	{c4,	time_croma},
	{pause,	time_minima},
};

const Note S_EAT_SPC_PILL[] = {
	{c4,	time_croma},
	{e4,	time_croma},
	{c4,	time_croma},
	{pause,	time_croma},
	{c4,	time_croma},
	{e4,	time_croma},
	{c4,	time_croma},
	{pause,	time_croma},
	{c4,	time_croma},
	{e4,	time_croma},
	{c4,	time_croma},
	{pause,	time_croma},
	{pause,	time_minima},
};

const Note S_EAT_GHOST[] = {
	{e4,	time_semiminima},
	{d4,	time_semiminima},
	{c4,	time_semiminima},
	{b3,	time_semiminima},
	{a3,	time_semiminima},
	{g3,	time_semiminima},
	{pause,	time_semiminima},
	{e4,	time_semiminima},
	{d4,	time_semiminima},
	{c4,	time_semiminima},
	{b3,	time_semiminima},
	{a3,	time_semiminima},
	{g3,	time_semiminima},
	{pause,	time_semiminima},
	{pause,	time_minima},
};

const Note S_LOSE_LIFE[] = {
	{c3,	time_semiminima},
	{g2,	time_semiminima},
	{e2,	time_semiminima},
	{c2,	time_minima},
	{pause,	time_croma},
	{c3,	time_semiminima},
	{g2,	time_semiminima},
	{e2,	time_semiminima},
	{c2,	time_minima},
	{pause,	time_croma},
	{pause,	time_minima},
};

const Note S_GAME_OVER[] = {
	{c3,	time_minima},
	{g2,	time_minima},
	{e2,	time_semiminima},
	{pause,	time_semiminima},
	{c2,	time_semibreve},
	{c3,	time_minima},
	{g2,	time_minima},
	{e2,	time_semiminima},
	{pause,	time_semiminima},
	{c2,	time_semibreve},
	{pause,	time_minima},
};

const unsigned char SOUNDS_LIST_SIZE = 5;
const static Sound SOUNDS_LIST[] = {
	{
		.id 		= SOUND_ID_PACMAN_SOUNDTRACK,
		.note_list 	= S_PACMAN_SOUNDTRACK,
		.size		= 52,
		.is_loop	= 1
	},
	{
		.id 		= SOUND_ID_EAT_SPC_PILL,
		.note_list 	= S_EAT_SPC_PILL,
		.size		= 13,
		.is_loop	= 0
	},
	{
		.id 		= SOUND_ID_EAT_GHOST,
		.note_list 	= S_EAT_GHOST,
		.size		= 15,
		.is_loop	= 0
	},
	{
		.id 		= SOUND_ID_LOSE_LIFE,
		.note_list 	= S_LOSE_LIFE,
		.size		= 11,
		.is_loop	= 0
	},
	{
		.id 		= SOUND_ID_GAME_OVER,
		.note_list 	= S_GAME_OVER,
		.size		= 11,
		.is_loop	= 1
	}
};


/* (Private) Functions Prototypes */

const Sound* get_sound(SoundID sound_id);
void play_note(Note note);
unsigned char is_note_playing();
unsigned char is_full_sound_over(unsigned short curr_note, Note full_sound[]);


/* (Private) File-Scope Global Variables */

static unsigned char volume_mult = 8;
static unsigned short current_note = 0;
static SoundID curr_sound_playing = SOUND_ID_PACMAN_SOUNDTRACK;
static SoundID prev_sound_playing = SOUND_ID_PACMAN_SOUNDTRACK;


/* Functions Implementations */

const Sound* get_sound(SoundID sound_id) {
	unsigned char i;
	for(i=0; i < SOUNDS_LIST_SIZE; i++) {
		if(SOUNDS_LIST[i].id == sound_id) {
			return &SOUNDS_LIST[i];
		}
	}
	return &SOUNDS_LIST[0];
}

void set_sound_playing(SoundID sound_id) {
	//prev_sound_playing = curr_sound_playing;
	curr_sound_playing = sound_id;
	current_note = 0;
}

void play_full_sound() {
	static unsigned char ticks = 0;
	
	const Sound* sound_playing = get_sound(curr_sound_playing);
	
	if(!is_note_playing()) {
		++ticks;
		if(ticks == SOUND_UPTICKS) {
			ticks = 0;
			play_note(sound_playing->note_list[current_note++]);
		}
	}
	
	if(current_note == sound_playing->size) {
		if(sound_playing->is_loop) {
			current_note = 0;
		}
		else {
			set_sound_playing(prev_sound_playing);
		}
	}
}

void play_single_sound() {
	static unsigned char sineticks;
	static unsigned int current_value;
	
	current_value = SIN_TABLE[sineticks];
	
	//current_value -= 410;		//????
	//current_value /= 1;		//????
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
		HW_TIMER_init_timer(2, 0, 0, 3, note.frequency * volume_mult * SOUND_AMPLIFIER);
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

