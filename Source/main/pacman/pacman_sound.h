#ifndef __PACMAN_SOUND_H
#define __PACMAN_SOUND_H


/* (Public) Types Definitions */

typedef enum {
	SOUND_ID_PACMAN_SOUNDTRACK	= 0,
	SOUND_ID_EAT_SPC_PILL		= 1,
	SOUND_ID_EAT_GHOST			= 2,
	SOUND_ID_LOSE_LIFE			= 3,
	SOUND_ID_GAME_OVER			= 4
} SoundID;


/* (Public) Functions Prototypes */

void set_sound_playing(SoundID sound_id);

void play_full_sound();
void play_single_sound();
void play_single_sound_duration();

void handle_change_volume(unsigned short value);



#endif //__PACMAN_SOUND_H