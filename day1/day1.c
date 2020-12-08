#include <stdio.h>
#include <stdlib.h>


int *loadArr(char *filepath, int lim, int* ress, int inits) {
	FILE *currFile;
	currFile = fopen(filepath, "r");
	int *resv;

	if (currFile != NULL) {
		// Reinitialise resv and ress for new array loading
		resv = malloc(inits * sizeof(int));
		int sptr = 0;

		char buffer[255];
		while (fgets(buffer, 255, currFile) != NULL) {
			int currNum = atoi(buffer);
			if (currNum >= lim) {
				resv[sptr] = currNum;
				sptr++;
			}
		}
		*ress = sptr;
	}
	fclose(currFile);
	return resv;
}

/**
 * Function completing part 1 of day 1: find which numbers
 */
int part1(char *filepath) {
	// Initialise file and result variables.
	FILE *currFile;
	currFile = fopen("/home/alexa/gitrepos/advent-of-code/day1/day1.txt", "r");
	int res = 0;

	// Go through file and place content inside integer array
	if (currFile != NULL) {
		// Initialise arrays of integer under 1010 and over 1010 along with their size
		int unders = 0;
		int * underv = malloc(200 * sizeof(int));
		int overs = 0;
		int * overv = malloc(200 * sizeof(int));

		// Loop over file
		char buffer[255];
		while (fgets(buffer, 255, currFile) != NULL) {
			int currNum = atoi(buffer);
			// Either add number to under nums or over nums
			if (currNum <= 1010) {
				underv[unders] = currNum;
				unders++;
			}
			else {
				overv[overs] = currNum;
				overs++;
			}
		}

		// Find numbers that add up to 2020
		int undern = 2021;
		int overn = 2021;
		for (int i = 0; i < unders && undern + overn != 2020; i++) {
			undern = underv[i];
			for (int j = 0; j < overs && undern + overn != 2020; j++) {
				overn = overv[j];
			}
		}
		res = undern * overn;

		// Close all pointers and arrays
		free(underv);
		free(overv);
	}
	fclose(currFile);
	return res;
}


/**
 * Function that completes part 2 of the day 1 of advent of code.
 */
int part2(char *filepath) {
	// Initialise array and it's size
	int res = 0;
	int ress = 0;
	int* resv = loadArr(filepath, 0, &ress, 200);

	// Loop through all elements 3 times
	int elem0 = 0;
	int elem1 = 0;
	int elem2 = 0;

	for (int i = 0; i < ress && elem0 + elem1 + elem2 != 2020; i++) {
		elem0 = resv[i];
		for (int j = i + 1; j < ress && elem0 + elem1 + elem2 != 2020; j++) {
			elem1 = resv[j];
			for (int k = j + 1; k < ress && elem0 + elem1 + elem2 != 2020; k++) {
				elem2 = resv[k];
			}
		}
	}
	free(resv);
	return elem0 * elem1 * elem2;
}


// Main function for the day 1 christmas program.
int main(int argc, char *argv[]) {
	if (argc == 2) {
		printf("%d\n", part1(argv[1]));
		printf("%d\n", part2(argv[1]));
	}
	else  {
		return 1;
	}
}
