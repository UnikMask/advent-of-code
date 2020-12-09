#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define BUFFER_SIZE 32
#define NULL_XMAS -1

int PREAMBLE_SIZE = -1;
const char *USAGE = "Usage:\n\t day9.out [XMAS_FILE] [SUM_QUEUE_SIZE]";

bool is_valid_xmas(long num, long* preamble, int preamblec) {
	for (int i = 0; i < preamblec - 1; i++) {
		for (int j = i + 1; j < preamblec; j++) {
			if (preamble[i] + preamble[j] == num) {
				return true;
			}
		}
	}
	return false;
}


void shift(long **arr, int arrc, int shiftc) {
	for (int i = arrc - 1; i >= shiftc ; i--) {
		(*arr)[i] = (*arr)[i - shiftc];
	}
	for (int i =0; i < shiftc; i++) {
		(*arr)[i] = NULL_XMAS;
	}
}


void fill(int **arr, int arrc, int fill_n) {
	for (int i = 0; i < arrc; i++) {
		(*arr)[i] = fill_n;
	}
}


long *load_xmas(char *filename, int* xmasc) {
	FILE *fp = fopen(filename, "r");
	long *xmas_arr = NULL;
	if (fp) {
		char buffer[BUFFER_SIZE];
		while (fgets(buffer, BUFFER_SIZE, fp)) {
			(*xmasc)++;
		}
		rewind(fp);
		xmas_arr = malloc((*xmasc) * sizeof(long));

		for(int i = 0; fgets(buffer, BUFFER_SIZE, fp); i++) {
			xmas_arr[i] = atoll(buffer);
		}
	}
	fclose(fp);
	return xmas_arr;
}


/**
 * Completes part 1 of day 9 - Find the first non-valid number
 * in the given XMAS file.
 *
 * @param filename The name/path of the file encrypted in XMAS.
 * @return The first non-valid number in that file.
 */
long day9part1(char *filename) {
	int xmasc = 0;
	long *xmasv = load_xmas(filename, &xmasc);
	long *preamble = malloc(PREAMBLE_SIZE * sizeof(long));
	int preamblec = 0;
	long res = NULL_XMAS;
	for (int i = 0; i < xmasc; i++) {
		if(preamblec == PREAMBLE_SIZE) {
			if (!is_valid_xmas(xmasv[i], preamble, preamblec)) {
				res = xmasv[i];
				break;
			}
		}
		shift(&preamble, PREAMBLE_SIZE, 1);
		preamble[0] = xmasv[i];
		if (preamblec < PREAMBLE_SIZE) {
			preamblec++;
		}
	}
	free(xmasv);
	free(preamble);
	return res;
}


/**
 * Completes part 2 of day 9 - Find contiguous sum adding up to invalid
 * number and return the encryption weakness.
 *
 * @param filename The name/path of the file encrypted in XMAS.
 * @param invalid_n The number in the list that is invalid.
 * @return The encryption weakness of that list.
 */
long day9part2(char *filename, long invalid_n) {
	int xmasc = 0;
	long *xmasv = load_xmas(filename, &xmasc);
	long res = NULL_XMAS;
	for (int i = 1; i < xmasc; i++) {
		for(int j = 0; j < xmasc - i; j++) {
			long largest = NULL_XMAS;
			long smallest = INT32_MAX;
			long sum = 0;
			for (int k = j; k <= j + i; k++) {
				sum += xmasv[k];
				if (xmasv[k] < smallest) {
					smallest = xmasv[k];
				}
				if (xmasv[k] > largest) {
					largest = xmasv[k];
				}
			}
			if (sum == invalid_n) {
				res = smallest + largest;
			}
		}
		if (res != NULL_XMAS) {
			break;
		}
	}
	free(xmasv);
	return res;
}


int main(int argc, char *argv[]) {
	if (argc == 3) {
		PREAMBLE_SIZE = atoi(argv[2]);
		long invalid_n = day9part1(argv[1]);
		printf("First invalid number in XMAS file: %ld\n",
			   invalid_n);
		printf("The encryption weakness of this XMAS-encrypted list is: %ld\n",
			   day9part2(argv[1], invalid_n));
		return 0;
	}
	else {
		printf("%s", USAGE);
		return -1;
	}
}
