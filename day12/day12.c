#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define FW 'F'
#define LFT 'L'
#define RGT 'R'
#define NORTH 'N'
#define SOUTH 'S'
#define EAST 'E'
#define WEST 'W'
#define BUFFER_SIZE 6
#define NULL_MOVE {0, 0}

const char *USAGE = "Usage:\n\t day11 [MOVES_FILE]";

struct move {
	int stepx;
	int stepy;
};


struct move get_move(char dir, int steps, int *deg) {
	struct move res_move = NULL_MOVE;
	bool moving = true;
	int local_deg = 0;
	switch (dir) {
		case NORTH:
			local_deg = 1;
			break;
		case SOUTH:
			local_deg = 3;
			break;
		case EAST:
			local_deg = 0;
			break;
		case WEST:
			local_deg = 2;
			break;
		case LFT:
			*deg = (*deg + steps / 90) % 4;
			if (*deg < 0) {
				*deg += 4;
			}
			local_deg = *deg;
			moving = false;
			break;
		case RGT:
			*deg = (*deg - steps / 90) % 4;
			if (*deg < 0) {
				*deg += 4;
			}
			local_deg = *deg;
			moving = false;
			break;
		default:
			local_deg = *deg;
			break;
	}
	if (moving) {
		switch (local_deg) {
			case 1:
				res_move.stepx = 0;
				res_move.stepy = steps;
				break;
			case 2:
				res_move.stepx = -steps;
				res_move.stepy = 0;
				break;
			case 3:
				res_move.stepx = 0;
				res_move.stepy = -steps;
				break;
			case 0:
				res_move.stepx = steps;
				res_move.stepy = 0;
		}
	}
	return res_move;
}


struct move get_move_part2(char dir, int steps, int *wpx, int *wpy) {
	struct move res_move = NULL_MOVE;
	bool moving = false;
	switch (dir) {
		case FW:
			moving = true;
			break;
		case LFT:
			for (int i = 0; i < steps / 90; i++) {
				int tmp_x = *wpx;
				int tmp_y = *wpy;
				*wpx = -tmp_y;
				*wpy = tmp_x;
			}
			break;
		case RGT:
			for (int i = 0; i < steps / 90; i++) {
				int tmp_x = *wpx;
				int tmp_y = *wpy;
				*wpx = tmp_y;
				*wpy = -tmp_x;
			}
			break;
		case NORTH:
			*wpy += steps;
			break;
		case SOUTH:
			*wpy -= steps;
			break;
		case EAST:
			*wpx += steps;
			break;
		case WEST:
			*wpx -= steps;
			break;
	}
	if (moving) {
		res_move.stepx = steps * *wpx;
		res_move.stepy = steps * *wpy;
	}
	return res_move;
}


int get_manhattan_dist(struct move *movev, int movec) {
	int posx = 0;
	int posy = 0;
	for (int i = 0; i < movec; i++) {
		posx += movev[i].stepx;
		posy += movev[i].stepy;
	}
	printf("pos x: %d | pos y: %d\n", posx, posy);
	return abs(posx) + abs(posy);
}


struct move *get_move_arr(char *filename, int *arrc, bool part2) {
	FILE *fp = fopen(filename, "r");
	struct move *movev = NULL;

	if (fp) {
		*arrc = 0;
		char buffer[BUFFER_SIZE];
		while (fgets(buffer, BUFFER_SIZE, fp)) {
			(*arrc)++;
		}
		rewind(fp);
		int deg = 0;
		int wpx = 10;
		int wpy = 1;
		movev = malloc((*arrc) * sizeof(struct move));
		for (int i = 0; fgets(buffer, BUFFER_SIZE, fp); i++) {
			char dir = buffer[0];
			char num[BUFFER_SIZE];
			int s = 0;
			while (buffer[s + 1] != '\0' && buffer[s + 1] != '\n') {
				num[s] = buffer[s + 1];
				s++;
			}
			num[s] = '\0';
			if (part2) {
				movev[i] = get_move_part2(dir, atoi(num), &wpx, &wpy);
			}
			else {
				movev[i] = get_move(dir, atoi(num), &deg);
			}
		}
	}
	fclose(fp);
	return movev;
}


int main(int argc, char *argv[]) {
	if (argc == 2) {
		int movec = 0;
		struct move *movev = get_move_arr(argv[1], &movec, false);
		printf("Overall Manhattan distance: %d\n",
			   get_manhattan_dist(movev, movec));
		free(movev);
		movev = get_move_arr(argv[1], &movec, true);
		printf("Overall part 2 Manhattan distance: %d\n",
			   get_manhattan_dist(movev, movec));
		free(movev);
		return 0;
	}
	else {
		printf("%s\n", USAGE);
		return -1;
	}
}
