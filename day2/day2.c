#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct strarr {
	char **arrv;
	int arrs;
};


/**
 * Checks the validity of passwrods described in part 1 of day 2.
 */
bool checkpart1val(int min, int max, char letter, char *password) {
	int passptr = 0;
	int numc = 0;
	while (password[passptr] != '\0') {
		if (password[passptr] == letter) {
			numc++;
		}
		passptr++;
	}
	return (numc >= min && numc <= max);
}


/**
 * Checks the validity of passwords described in part 2 of day 2.
 */
bool checkpart2val(int first, int second, char letter, char *password) {
	int passptr = 0;
	bool res = false;

	// Loop over password
	while (password[passptr] != '\0') {
		if (password[passptr] == letter && (passptr == first - 1 || passptr == second - 1)) {
			res = !res;
		}
		passptr++;
	}
	return res;
}


/**
 * Function to complete day 2 - Loop over passwords and letters, numbers,
 * then check their validity.
 */
void day2complete(char *fn) {
	FILE *fp = fopen(fn, "r");
	struct strarr resv = {
		.arrs = 0,
		.arrv = malloc(1000 * sizeof(char[255]))
	};

	// Initialise integers for part 1 and 2.
	int part1num = 0;
	int part2num = 0;

	if (fp != NULL) {
		// Loop through file strings and declare line buffer.
		char *lbuffer = malloc(255 * sizeof(char));
		for(int i = 0; i < 1000; i++) {
			// Reset buffer character pointer position, and initialise used buffer.
			fgets(lbuffer, 255, fp);
			int lbuffptr = 0;
			char *buffer = malloc(32 * sizeof(char));
			int buffs = 0;

			// Get minimum number
			while(lbuffer[lbuffptr] != '-') {
				buffer[buffs] = lbuffer[lbuffptr];
				buffs++;
				lbuffptr++;
			}
			buffer[buffs] = '\0';
			int min = atoi(buffer);

			// Get maximum number
			buffs = 0;
			lbuffptr++;
			while(lbuffer[lbuffptr] != ' ') {
				buffer[buffs] = lbuffer[lbuffptr];
				lbuffptr++;
				buffs++;
			}
			buffer[buffs] = '\0';
			int max = atoi(buffer);

			// Get letter
			char allowc;
			lbuffptr++;
			while(lbuffer[lbuffptr] != ':') {
				allowc = lbuffer[lbuffptr];
				lbuffptr++;
			}

			// Get password in buffer
			int numc = 0;
			buffs = 0;
			lbuffptr += 2;
			while(lbuffer[lbuffptr] != '\n' && lbuffer[lbuffptr] != '\0') {
				buffer[buffs] = lbuffer[lbuffptr];
				if(lbuffer[lbuffptr] == allowc) {
					numc++;
				}
				buffs++;
				lbuffptr++;
			}
			buffer[buffs] = '\0';

			// Check validity for part 1
			if (checkpart1val(min, max, allowc, buffer)) {
				part1num++;
			}
			if (checkpart2val(min, max, allowc, buffer)) {
				part2num++;
			}
			free(buffer);
		}
		free(lbuffer);
		printf("%d\n", part1num);
		printf("%d\n", part2num);
	}
	fclose(fp);
}


/**
 * Main function of day 2.
 */
int main(int argc, char *argv[]) {
	if(argc == 2) {
		day2complete(argv[1]);
		return 0;
	}
	else {
		return -1;
	}
}
