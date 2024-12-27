#ifndef __PACMAN_CORE_H
#define __PACMAN_CORE_H


/* (Public) Hardware Imports */

#include "../interfaces/hw_abstraction.h"


/* (Public) Constants Definitions */

//General (Game Optimization)
#define PACMAN_SPEED		5				// This #define is for Optimization purposes
#define TICK_SECOND_RATIO	PACMAN_SPEED
#define TICK_DURATION		(1.0f / TICK_SECOND_RATIO)
#define TICK_SPEED_CONV		(TICK_DURATION / TICK_SECOND_RATIO)

//General (Display Related)
#define SCREEN_HEIGTH		320
#define SCREEN_WIDTH		240

#define SCREEN_CENTER_X		(SCREEN_WIDTH / 2)
#define SCREEN_CENTER_Y		(SCREEN_HEIGTH / 2)

//Game Map
#define MAP_HEIGTH			31
#define MAP_WIDTH			28

#define SCALE				8

#define PADDING_LEFT		(SCREEN_WIDTH - (MAP_WIDTH * SCALE)) / 2
#define PADDING_RIGHT		(SCREEN_WIDTH - (MAP_WIDTH * SCALE)) / 2

#define PADDING_TOP			8
#define PADDING_BOTTOM		8

#define MAP_START_X			(0 + PADDING_LEFT)
#define MAP_START_Y			(SCREEN_HEIGTH - (MAP_HEIGTH * SCALE) - PADDING_BOTTOM)

//Player GUI
#define DELIMITER_PADDING	8

#define PLAYER_GUI_LENGHT	(SCREEN_WIDTH - (PADDING_TOP + PADDING_BOTTOM + DELIMITER_PADDING + (MAP_HEIGTH * SCALE)))
#define PLAYER_GUI_WIDTH	(SCREEN_WIDTH - (PADDING_LEFT + PADDING_RIGHT))

#define PLAYER_GUI_START_X	(0 + PADDING_LEFT)
#define PLAYER_GUI_START_Y	(0 + PADDING_TOP)

#define TIMER_AREA_START_X	(PLAYER_GUI_START_X + 0)
#define SCORE_AREA_START_X	(TIMER_AREA_START_X + (PLAYER_GUI_WIDTH / 3))
#define LIVES_AREA_START_X	(SCORE_AREA_START_X + (PLAYER_GUI_WIDTH / 3))

#define PLAYER_GUI_HEADER_Y	(PLAYER_GUI_START_Y + 0)

#define TIMER_VALUE_START_Y	(PLAYER_GUI_HEADER_Y + 20)
#define SCORE_VALUE_START_Y	(PLAYER_GUI_HEADER_Y + 20)
#define LIVES_VALUE_START_Y	(PLAYER_GUI_HEADER_Y + 24)

#define TIMER_VALUE_START_X	(TIMER_AREA_START_X + 20)
#define SCORE_VALUE_START_X	(SCORE_AREA_START_X + 16)
#define LIVES_VALUE_START_X	(LIVES_AREA_START_X + 16)


/* (Public) Types Definitions */

typedef enum {
	DIRECTION_STILL	= 0,
	DIRECTION_RIGHT	= 1,
	DIRECTION_LEFT	= 2,
	DIRECTION_UP	= 3,
	DIRECTION_DOWN	= 4
} Direction;

typedef enum {
    CELL_EMPTY 		= 0,	// Empty Space
    CELL_WALL 		= 1,	// Wall
    CELL_TP_L 		= 2,	// Teleport Area Left
	CELL_TP_R		= 3,	// Teleport Area Right
    CELL_STD_PILL 	= 4,	// Pill
    CELL_SPC_PILL 	= 5,	// Special Pill
	CELL_CAGE_GATE	= 6		// Cage Gate
} CellType;

typedef enum {
	CHAR_PACMAN		= 0,	// Pacman
	CHAR_BLINKY		= 1		// Blinky
} CharacterType;

typedef unsigned char GameMap[MAP_HEIGTH][MAP_WIDTH];

typedef struct {
	unsigned char x;
	unsigned char y;
} Position;

typedef struct {
	Position curr_pos;
	Position prev_pos;
	Direction direction;
	unsigned char animation_frame;
} Character;

typedef struct {
	unsigned char map_id;				// ID of selected Game Map
	unsigned char max_time;				// Start Time
    unsigned char start_lives;			// Number of Life at the start
	unsigned char max_lives;			// Max Number of Lives
    unsigned short pts_to_life;			// Points to gain Extra Life
	unsigned char std_pill_count;		// Number of Standard Pills
    unsigned char spc_pill_count;		// Number of Special Pills
	unsigned short std_pill_value;		// Score Points of a Standard Pill
	unsigned short spc_pill_value;		// Score Points of a Special Pill
	unsigned short ghost_eat_value;		// Score Points of eating a Ghost
	unsigned char pacman_speed;			// Pacman movement speed
	unsigned char blinky_min_speed;		// Blinky start movement speed
	unsigned char blinky_max_speed;		// Blinky max (final) movement speed
	unsigned char spc_pill_gen_ratio;	// Generation Ratio of Special Pills (in Ticks)
	unsigned char ghost_respawn_time;	// Respawn Time of Ghosts (in Seconds)
	unsigned char ghost_weak_time;		// Vulnerability Time of Ghosts (in Seconds)
} GameConfig;

typedef struct {
	unsigned int pacman_speed_timer_count;		// Timer count value to set Pacman Speed
	unsigned int blinky_speed_timer_count;		// Timer count value to set Blinky Speed
	unsigned short ghost_respawn_ticks;			// Ghosts Respawn Time (in Ticks)
	unsigned short ghost_weak_ticks;			// Ghosts Vulnerability Time (in Ticks)
	unsigned short blinky_accel_interval_ticks;	// Blinky Acceleration Interval Step Time (in Ticks)
} GameTimings;

typedef struct {
	GameMap game_map;				// Game Map
	unsigned char time;				// Current Time left
	unsigned char lives;			// Current Lives of the Player
	unsigned short score;			// Current Score of the Player
	unsigned char pills_left;		// Number of Pills Left
	Character pacman;				// PacMan Position and Attributes
	Character blinky;				// Blinky Position and Attributes
} GameRunning;

typedef struct {
	unsigned char is_init_over;		// State of the Game - Initializing or Ready
	unsigned char is_pause;			// State of the Game - Pause
	unsigned char is_fail;			// State of the Game - Fail
	unsigned char is_end;			// State of the Game - Gameover or Victory
	unsigned char spc_pills_gen;	// Number of Special Pills Generated
	unsigned char blinky_curr_speed;// State of Blinky (Current Speed)
	unsigned char is_blinky_dead;	// State of Blinky (Dead or Alive)
	unsigned char is_ghosts_weak;	// State of Ghosts (Vulnerable or Normal)
} GameStatus;


/* (Public) Functions Prototypes */


/* (Public) Global Variables Declaration */

extern const GameConfig GAME_CONFIG;
extern GameTimings GAME_TIMINGS;
extern GameRunning game_run;
extern GameStatus game_status;



#endif //__PACMAN_CORE_H