#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define BUFFER_SIZE 255
#define RULE_EMP_TRESH 0
#define RULE_OCC_TRESH 4
#define PART_2_OCC_TRESH 5
#define VACANT 'L'
#define FLOOR '.'
#define OCCUPIED '#'


const char *USAGE = "Usage:\n\t day11 [SEATS_FILE]";
enum seat  {
Floor, Vacant, Occupied
};


enum seat get_seat(char desc) {
	switch(desc) {
		case VACANT:
			return Vacant;
		case FLOOR:
			return Floor;
		case OCCUPIED:
			return Occupied;
		default:
			return Floor;
	}
}


char get_char_from_seat(enum seat desc) {
	switch(desc) {
		case Floor:
			return FLOOR;
		case Vacant:
			return VACANT;
		default:
			return OCCUPIED;
	}
}


enum seat **get_seat_arr(char *filename, int *arrx, int *arry) {
	FILE *fp = fopen(filename, "r");
	enum seat **seat_arr = NULL;

	if(fp) {
		// Get array size
		char buffer[BUFFER_SIZE];
		*arrx = 0;
		*arry = 0;
		while (fgets(buffer, BUFFER_SIZE, fp)) {
			if (*arrx == 0) {
				// Get x-size.
				for(int i = 0; buffer[i] != '\n'; i++) {
					(*arrx)++;
				}
			}
			(*arry)++;
		}
		rewind(fp);
		seat_arr = malloc((*arry) * sizeof(enum seat *));
		for (int i = 0; fgets(buffer, BUFFER_SIZE, fp); i++) {
			seat_arr[i] = malloc((*arrx) * sizeof(enum seat));
			for (int j = 0; buffer[j] != '\n'; j++) {
				seat_arr[i][j] = get_seat(buffer[j]);
			}
		}
	}
	return seat_arr;
}


bool equals(enum seat **seat_arr0, enum seat **seat_arr1, int arrx, int arry) {
	if (seat_arr0 != NULL && seat_arr1 != NULL) {
		for (int i = 0; i < arry; i++) {
			for (int j = 0; j < arrx; j++) {
				if (seat_arr0[i][j] != seat_arr1[i][j]) {
					return false;
				}
			}
		}
		return true;
	}
	else {
		return false;
	}
}


int get_neighbours(enum seat **seat_arr, int arrx, int arry, int posx, int posy) {
	if (posx < arrx && posx >= 0 && posy < arry && posy >= 0) {
		int res = 0;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (posx + i < arrx && posx + i >= 0 && posy + j < arry && posy + j >= 0) {
					if (seat_arr[posy + j][posx + i] == Occupied && (j != 0 || i != 0)) {
						res++;
					}
				}
			}
		}
		return res;
	}
	else {
		return -1;
	}
}


int get_neighbours_part2(enum seat **seat_arr, int arrx, int arry, int posx, int posy) {
	if (posx < arrx && posx >= 0 && posy < arry && posy >= 0) {
		int res = 0;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				int pscx = i;
				int pscy = j;
				bool loop = true;
				while (loop && (pscx != 0 || pscy != 0)) {
					if (posx + pscx < arrx && posx + pscx >= 0 && posy + pscy < arry && posy + pscy >= 0) {
						if (seat_arr[posy + pscy][posx + pscx] == Occupied) {
							res++;
							loop = false;
						}
						else if (seat_arr[posy + pscy][posx + pscx] != Floor) {
							loop = false;
						}
					}
					else {
						loop = false;
					}
					pscx += i;
					pscy += j;
				}
			}
		}
		return res;
	}
	else {
		return -1;
	}
}


int get_num_occupied(enum seat **seat_arr, int arrx, int arry) {
	int res = 0;
	for (int i = 0; i < arrx; i++) {
		for (int j = 0; j < arry; j++) {
			if (seat_arr[j][i] == Occupied) {
				res++;
			}
		}
	}
	return res;
}


enum seat **run_step(enum seat **seat_arr, int arrx, int arry, bool part2) {
	enum seat **res_arr = malloc(arry * sizeof(enum seat *));
	for (int i = 0; i < arry; i++) {
		res_arr[i] = malloc(arrx * sizeof(enum seat));
		for (int j = 0; j < arrx; j++) {
			if (!part2) {
				int neighbours = get_neighbours(seat_arr, arrx, arry, j, i);
				res_arr[i][j] = seat_arr[i][j];
				if (neighbours >= RULE_OCC_TRESH && seat_arr[i][j] == Occupied) {
					res_arr[i][j] = Vacant;
				}
				else if (neighbours <= RULE_EMP_TRESH && seat_arr[i][j] == Vacant) {
					res_arr[i][j] = Occupied;
				}
			}
			else {
				int neighbours = get_neighbours_part2(seat_arr, arrx, arry, j, i);
				res_arr[i][j] = seat_arr[i][j];
				if (neighbours >= PART_2_OCC_TRESH && seat_arr[i][j] == Occupied) {
					res_arr[i][j] = Vacant;
				}
				else if (neighbours <= RULE_EMP_TRESH && seat_arr[i][j] == Vacant) {
					res_arr[i][j] = Occupied;
				}
			}
		}
	}
	return res_arr;
}


int day11(char *filename, bool part2) {
	int arrx = 0;
	int arry = 0;
	enum seat **seat_arr = get_seat_arr(filename, &arrx, &arry);
	enum seat **prev_arr = NULL;
	int step = 1;
	while (!equals(seat_arr, prev_arr, arrx, arry)) {
		if (prev_arr != NULL) {
			for (int i = 0; i < arry; i++) {
				free(prev_arr[i]);
			}
			free(prev_arr);
		}
		prev_arr = seat_arr;
		seat_arr = run_step(prev_arr, arrx, arry, part2);
		step++;
	}
	int res = get_num_occupied(seat_arr, arrx, arry);
	for (int i = 0; i < arry; i++) {
		free(prev_arr[i]);
		free(seat_arr[i]);
	}
	free(prev_arr);
	free(seat_arr);
	return res;
}


int main(int argc, char *argv[]) {
	if (argc == 2) {
		printf("Number occupied seats after part 1 algorithm: %d\n", day11(argv[1], false));
		printf("Number occupied seats after part 2 algorithm: %d\n", day11(argv[1], true));
	}
	else {
		printf("%s\n", USAGE);
		return -1;
	}
}
