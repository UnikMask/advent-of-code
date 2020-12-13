#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 255
#define NULL_LINE "x"

const char *USAGE = "Usage:\n\t day12 [BUSES_FILE]";

struct map {
	int *key;
	int *value;
	int mapc;
};

/**
 * Splits a sentence ended by a termination or newline longo array of words.
 *
 * @param sentence The sentence to split.
 * @param splitc_ptr the polonger to assign the size of the array to.
 * @param schar Character to split string with.
 * @return The array of words from the sentence, removing punctuation (e.g. [.,]).
 */
char **split(char *sentence, long *splitc_ptr, char schar) {
	if (sentence != NULL) {
		// Calculate the size of the sentence.
		if (splitc_ptr == NULL) {
			splitc_ptr = malloc(sizeof(long));
		}
		*splitc_ptr = 0;
		for(long i = 0; sentence[i] != '\0' && sentence[i] != '\n'; i++) {
			if (sentence[i] == schar || sentence[i + 1] == '\n' || sentence[i + 1] == '\0') {
				(*splitc_ptr)++;
			}
		}

		// Split sentence longo words.
		char **splitv = malloc(*splitc_ptr * sizeof(char *));
		long split_index = 0;
		long buffer_index = 0;
		char *buffer = NULL;
		for (long i = 0; sentence[i] != '\0' && sentence[i] != '\n'; i++) {
			if (sentence[i] != schar && sentence[i] != '.' && sentence[i] != ' ') {
				if (buffer == NULL) {
					buffer = malloc(BUFFER_SIZE * sizeof(char));
					buffer_index = 0;
				}
				buffer[buffer_index] = sentence[i];
				buffer_index++;

			}
			if (sentence[i] == schar || sentence[i + 1] == '\n' || sentence[i + 1] == '\0') {
				// Add word to array and reset buffer for new word.
				buffer[buffer_index] = '\0';
				splitv[split_index] = buffer;
				buffer = NULL;
				buffer_index = 0;
				split_index++;
			}
		}
		return splitv;
	}
	else {
		return NULL;
	}
}


int map_get(struct map mapv, int value) {
	int res = 0;
	for (int i = 0; i < mapv.mapc; i++) {
		if (mapv.key[i] == value) {
			res = mapv.value[i];
		}
	}
	return res;
}


struct map map_get_map(int **keys, int **values, int arrc) {
	struct map mapv;
	mapv.mapc = arrc;
	mapv.key = *keys;
	mapv.value = *values;
	return mapv;
}


void map_free(struct map mapv) {
	free(mapv.key);
	free(mapv.value);
	mapv.mapc = 0;
}


int *get_bus_id_arr(char *filename, int *timestamp, int *busc) {
	FILE *fp = fopen(filename, "r");
	int *id_arr = NULL;
	if (fp) {
		char buffer[255];
		fgets(buffer, BUFFER_SIZE, fp);
		*timestamp = atoi(buffer);
		long splitc = 0;
		fgets(buffer, BUFFER_SIZE, fp);
		char **wsplit = split(buffer, &splitc, ',');
		id_arr = malloc(splitc * sizeof(int));
		*busc = 0;
		for (int i = 0; i < splitc; i++) {
			if (strcmp(wsplit[i], NULL_LINE)) {
				id_arr[*busc] = atoi(wsplit[i]);
				(*busc)++;
			}
		}
	}
	return id_arr;
}


struct map get_bus_id_map(char *filename) {
	FILE *fp = fopen(filename, "r");
	struct map id_map;
	if (fp) {
		char buffer[BUFFER_SIZE];
		fgets(buffer, BUFFER_SIZE, fp);
		long splitc = 0;
		fgets(buffer, BUFFER_SIZE, fp);
		char **wsplit = split(buffer, &splitc, ',');
		id_map.key = malloc(splitc * sizeof(int));
		id_map.value = malloc(splitc * sizeof(int));
		id_map.mapc = 0;
		for (int i = 0; i < splitc; i++) {
			if (strcmp(wsplit[i], NULL_LINE)) {
				id_map.value[id_map.mapc] = atoi(wsplit[i]);
				id_map.key[id_map.mapc] = i;
				id_map.mapc++;
			}
		}
	}
	return id_map;
}


int day13_part1(char *filename) {
	int timestamp = 0;
	int busc = 0;
	int *busv = get_bus_id_arr(filename, &timestamp, &busc);
	int timestamp_o = timestamp;
	int res = -1;

	while (res < 0) {
		for (int i = 0; i < busc; i++) {
			if (timestamp % busv[i] == 0) {
				res = busv[i];
				break;
			}
		}
		if (res < 0) {
			timestamp++;
		}
	}
	free(busv);
	return (timestamp - timestamp_o) * res;
}


long long day13_part2(char *filename) {
	long long timestamp = 0;
	struct map bus_map = get_bus_id_map(filename);
	bool res = false;
	long long step = bus_map.value[0];
	while (!res) {
		res = true;
		for (int i = 0; i < bus_map.mapc; i++) {
			int time_offset = (bus_map.key[i]);
			if ((timestamp + time_offset) % bus_map.value[i] != 0) {
				res = false;
				break;
			}
			else {
				if (step % bus_map.value[i] != 0) {
					step *= bus_map.value[i];
				}
			}
		}
		if (!res) {
			timestamp += step;
		}
	}
	map_free(bus_map);
	return timestamp;
}


int main(int argc, char *argv[]) {
	if (argc == 2) {
		printf("Number minutes I need to wait multiplied by bus ID: %d\n",
			   day13_part1(argv[1]));
		printf("Earliest timestamp for 1-by-1 offset: %lld\n",
			   day13_part2(argv[1]));
		return 0;
	}
	else {
		printf("%s\n", USAGE);
		return -1;
	}
}
