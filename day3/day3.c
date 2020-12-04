#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

struct map {
	char ** mapm;
	int xs;
	int ys;
	int posx;
	int posy;
};


/**
 * Loads map from file into a map struct.
 * @param filename The path of the file containing the map.
 * @param distm The map struct to load the map to.
 * @returns whether loading was successful or not.
 */
bool loadmap(char *filename, struct map* distm) {
	FILE *fp = fopen(filename, "r");
	bool success = false;
	if (fp) {
		printf("%s", "allocating memory to map...");
		distm->mapm = malloc(distm->ys * sizeof(char*));
		printf("%s\n", "Successful!");

		// Loop through file and register buffer to next value in mapm
		char buffer[255];
		int yptr = 0;
		while (fgets(buffer, 255, fp)) {
			distm->mapm[yptr] = malloc((distm->xs + 1) * sizeof(char));
			int lagi = 0;
			for(int i = 0; i < distm->xs + 2; i++) {
				if (buffer[i] == '.' || buffer[i] == '#') {
					distm->mapm[yptr][lagi] = buffer[i];
					lagi++;
				}
			}
			distm->mapm[yptr][lagi] = '\0';
			yptr++;
			if (feof(fp)) {
				break;
			}
		}
		success = true;
	}
	printf("Closing file...");
	fclose(fp);
	printf("%s\n", "Successful");
	return success;
}


/**
 * Get number of trees encountered
 * when starting slope from given pos.
 *
 * @param distm The map to traverse the slope with.
 * @param mvx How much in the x direction does the toboggan move at each step.
 * @param mvy How much in the y direction does the toboggan move at each step.
 * @returns the number of trees encountered in that
 */
long getnumtrees(struct map* distm, int mvx, int mvy) {
	long numtrees = 0;
	if (distm->posy + mvy < distm->ys) {
		distm->posx = (distm->posx + mvx) % distm->xs;
		distm->posy += mvy;
		if (distm->mapm[distm->posy][distm->posx] == '#') {
			numtrees++;
		}
		numtrees += getnumtrees(distm, mvx, mvy);
	}
	distm->posx = 0;
	distm->posy = 0;
	return numtrees;
}

/**
 * Main function of day3 program.
 * @param argc the number of arguments in user arguments.
 * @param argv the user argument array.
 */
int main(int argc, char *argv[]) {
	if (argc == 2) {
		struct map day3m =  {
			.xs = 31,
			.ys = 323,
			.posx = 0,
			.posy = 0
		};

		if (loadmap(argv[1], &day3m)) {
			// Complete part 1
			printf("Part 1 -Number trees encountered for r3, d1: %ld\n", getnumtrees(&day3m, 3, 1));


			// Complete part 2
			long sum = getnumtrees(&day3m, 1, 1) * getnumtrees(&day3m, 3, 1) * getnumtrees(&day3m, 5, 1)
				* getnumtrees(&day3m, 7, 1) * getnumtrees(&day3m, 1, 2);
			printf("Part 2 - Product of trees encountered on each listed slopes: %ld\n", sum);
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
}
