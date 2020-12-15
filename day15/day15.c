#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 32
#define MAP_SIZE 16

struct arr {
	long *value;
	long mapc;
};


bool arr_increase(struct arr *mp, int newsize) {
	long *tmp_value = mp->value;

	if (newsize > mp->mapc) {
		mp->value = malloc(newsize * sizeof(long));

		for(long i = 0; i < mp->mapc; i++) {
			mp->value[i] = tmp_value[i];
		}
		for (long i = mp->mapc; i < newsize; i++) {
			mp->value[i] = -1;
		}
		free(tmp_value);
		mp->mapc = newsize;
		return true;
	}
	else {
		return false;
	}

}


bool arr_contains(struct arr val_map, long key) {
	if (val_map.mapc > key) {
		if (val_map.value[key] >= 0) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}


struct arr arr_new(long max_c) {
	struct arr res_map;
	res_map.value = malloc(max_c * sizeof(long));
	res_map.mapc = max_c;
	for (int i = 0; i < max_c; i++) {
		res_map.value[i] = -1;
	}
	return res_map;
}


void arr_add(struct arr *val_map, long key, long value) {
	if (key >= val_map->mapc) {
		arr_increase(val_map, key * 2);
	}
	val_map->value[key] = value;
}


void arr_free(struct arr mapv) {
	free(mapv.value);
	mapv.mapc = 0;
}


/**
 * Splits a sentence ended by a termination or newline into array of words.
 *
 * @param sentence The sentence to split.
 * @param splitc_ptr the pointer to assign the size of the array to.
 * @param schar Character to split string with.
 * @return The array of words from the sentence, removing punctuation (e.g. [.,]).
 */
char **split(char *sentence, long *splitc_ptr, char *scharv, long scharc) {
	if (sentence != NULL) {
		// Calculate the size of the sentence.
		if (splitc_ptr == NULL) {
			splitc_ptr = malloc(sizeof(long));
		}
		*splitc_ptr = 0;
		bool non_empty = false;
		for(long i = 0; sentence[i] != '\0' && sentence[i] != '\n'; i++) {
			bool check = false;
			for (long j = 0; j < scharc; j++) {
				if (sentence[i] == scharv[j]) {
					check = true;
					break;
				}
			}
			if (!check) {
				non_empty = true;
			}
			if ((check || sentence[i + 1] == '\n' || sentence[i + 1] == '\0') && non_empty) {
				(*splitc_ptr)++;
				non_empty = false;
			}
		}

		// Split sentence into words.
		char **splitv = malloc(*splitc_ptr * sizeof(char *));
		long split_index = 0;
		long buffer_index = 0;
		char *buffer = NULL;
		// Get word split size.
		for (long i = 0; sentence[i] != '\0' && sentence[i] != '\n'; i++) {
			bool check = false;
			for (long j = 0; j < scharc; j++) {
				if (sentence[i] == scharv[j]) {
					check = true;
					break;
				}
			}
			if (!check && sentence[i] != '.') {
				if (buffer == NULL) {
					buffer = malloc(BUFFER_SIZE * sizeof(char));
					buffer_index = 0;
				}
				buffer[buffer_index] = sentence[i];
				buffer_index++;

			}
			if (check || sentence[i + 1] == '\n' || sentence[i + 1] == '\0') {
				// Add word to array and reset buffer for new word.
				if (buffer != NULL) {
					buffer[buffer_index] = '\0';
					splitv[split_index] = buffer;
					buffer = NULL;
					buffer_index = 0;
					split_index++;
				}
			}
		}
		return splitv;
	}
	else {
		return NULL;
	}
}


long get_next_num(struct arr *numv, long lastw, long *turnn) {
	long res = 0;
	if (arr_contains(*numv, lastw)) {
		res = *turnn - numv->value[lastw];
		numv->value[lastw] = *turnn;
	}
	else {
		arr_add(numv, lastw, *turnn);
	}
	(*turnn)++;
	return res;
}


long get_num_at_step(long lastt, struct arr *numv, long lastw, long *turnn) {
	while(*turnn < lastt) {
		lastw = get_next_num(numv, lastw, turnn);
	}
	return lastw;
}


struct arr load_game(char *filename, long *turnn) {
	FILE *fp = fopen(filename, "r");
	struct arr numv = arr_new(MAP_SIZE);

	if (fp) {
		char buffer[BUFFER_SIZE];
		fgets(buffer, BUFFER_SIZE, fp);
		long splitc = 0;
		char **splitw = split(buffer, &splitc, ", ", 2);
		*turnn = 0;
		for (long i = 0; i < splitc; i++) {
			arr_add(&numv, atoi(splitw[i]), i + 1);
			(*turnn)++;
		}
		for (long i = 0; i < splitc; i++) {
			printf("%s - ", splitw[i]);
		}
		printf("\n");
	}
	return numv;
}


int main(int argc, char *argv[]) {
	if (argc == 2) {
		long turnn = 0;
		struct arr numv = load_game(argv[1], &turnn);
		turnn++;
		long part1 = get_num_at_step(2020, &numv, 0, &turnn);
		printf("2020th number spoken is %ld\n", part1);
		arr_free(numv);

		turnn = 0;
		numv = load_game(argv[1], &turnn);
		turnn++;
		long part2 = get_num_at_step(30000000, &numv, 0, &turnn);
		printf("30000000th number spoken is %ld\n", part2);
		arr_free(numv);
	}
	else {
		return -1;
	}
}
