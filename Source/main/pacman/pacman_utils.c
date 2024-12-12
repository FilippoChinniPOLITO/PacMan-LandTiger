#include "pacman_utils.h"


/* (Private) Function Prototypes */


/* (Private) Constants Definitions */


/* Function Implementations */

void parse_map_file(const char *filename, GameMap game_map, short id) {
    FILE *file = fopen(filename, "r");
	
    if (!file) {
        exit(EXIT_FAILURE);
    }
	
	int read_char;
	char c;
	unsigned char i;
	unsigned char j;
	unsigned char found;
	
    while ((read_char = fgetc(file)) != EOF) {
		c = (char) read_char;
		
		if (c == ' ' || c == ',' || c == '\t' || c == '\n') {
			continue;
		}
		
		if (c == '{') {
			j = 0;
			continue;
		}
		
		if (c == '}') {
			i++;
			continue;
		}
		
		if (isdigit(c)) {
			game_map[i][j] = (uint8_t) c;
			j++;
			continue;
		}
		
        if (c == '#') {
			if (found) {
				break;
			}
			found = 1;
        }
	}
	fclose(file);
}

