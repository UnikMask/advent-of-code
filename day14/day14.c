#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define MAX_BIT_SIZE 128
#define BUFFER_SIZE 64
#define NULL_MAP {NULL, NULL, -1}
#define MAX_MAP_SIZE 2048

const char *USAGE = "Usage: \n\t day14 [DAY_14_INPUT]";

struct mask {
	int64_t mask_val;
	int64_t mask_hide;
	int bit_size;
};


struct map {
	int64_t *key;
	int64_t *value;
	int mapc;
	int max_c;
};


void map_doublesize(struct map * mp) {
	int64_t *tmp_key = mp->key;
	int64_t *tmp_value = mp->value;

	mp->key = malloc(2 * mp->max_c * sizeof(int64_t));
	mp->value = malloc(2 * mp->max_c * sizeof(int64_t));

	for(int i = 0; i < mp->mapc; i++) {
		mp->key[i] = tmp_key[i];
		mp->value[i] = tmp_value[i];
	}
	free(tmp_key);
	free(tmp_value);
	mp->max_c *= 2;
}


int map_contains(struct map val_map, int64_t key) {
	int null_contains =  -1;
	for (int i = 0; i < val_map.mapc; i++) {
		if (val_map.key[i] == key) {
			return i;
		}
	}
	return null_contains;
}


void map_add(struct map *val_map, int64_t key, int64_t value) {
	if (val_map->mapc == val_map->max_c) {
		map_doublesize(val_map);
	}
	val_map->key[val_map->mapc] = key;
	val_map->value[val_map->mapc] = value;
	val_map->mapc++;
}


void map_free(struct map mapv) {
	free(mapv.key);
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
char **split(char *sentence, long *splitc_ptr, char *scharv, int scharc) {
	if (sentence != NULL) {
		// Calculate the size of the sentence.
		if (splitc_ptr == NULL) {
			splitc_ptr = malloc(sizeof(long));
		}
		*splitc_ptr = 0;
		bool non_empty = false;
		for(long i = 0; sentence[i] != '\0' && sentence[i] != '\n'; i++) {
			bool check = false;
			for (int j = 0; j < scharc; j++) {
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

		// Split sentence longo words.
		char **splitv = malloc(*splitc_ptr * sizeof(char *));
		long split_index = 0;
		long buffer_index = 0;
		char *buffer = NULL;
		// Get word split size.
		for (long i = 0; sentence[i] != '\0' && sentence[i] != '\n'; i++) {
			bool check = false;
			for (int j = 0; j < scharc; j++) {
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


struct mask get_mask(char **splitv, int splitc) {
	struct mask res_mask = {0, 0, 0};
	for (int i = 0; splitv[2][i] != '\0'; i++) {
		res_mask.bit_size++;
		res_mask.mask_val <<= 1;
		res_mask.mask_hide <<= 1;
		switch (splitv[2][i]) {
			case 'X':
				res_mask.mask_val |= 0;
				res_mask.mask_hide |= 1;
				break;
			case '1':
				res_mask.mask_val |= 1;
				res_mask.mask_hide |= 0;
				break;
			case '2':
				res_mask.mask_val |= 0;
				res_mask.mask_hide |= 0;
				break;
		}
	}
	return res_mask;
}


int64_t get_masked(int64_t num, struct mask used_mask) {
	int64_t res_val = 0;

	// Mask into result value.
	for (int i = 0; i < used_mask.bit_size; i++) {
		if (!((used_mask.mask_hide >> i) & 1)) {
			int64_t curr_unit = ((used_mask.mask_val >> i) & 1) << i;
			res_val |= curr_unit;
		}
		else {
			int64_t  curr_unit = ((num >> i) & 1) << i ;
			res_val |= curr_unit;
		}
	}
	return res_val;
}


int64_t get_num_from_float(int64_t floatn, struct mask used_mask) {
	int64_t res = used_mask.mask_val;
	for (int i = 0; i < used_mask.bit_size; i++) {
		if ((used_mask.mask_hide >> i) & 1) {
			if ((res >> i) & 1) {
				res -= (1 << i);
			}
			res |= (floatn & 1) << i;
			floatn >>= 1;
		}
	}
	return res;
}


int64_t* get_masked_part2(int64_t num, struct mask used_mask, int64_t *numc) {
	*numc = 1;
	for (int i = 0; i < used_mask.bit_size; i++) {
		// Is an X.
		if (used_mask.mask_hide >> i & 1) {
			(*numc) *= 2;
		}
	}
	used_mask.mask_val |= num;
	int64_t *num_arr = malloc(*numc * sizeof(int64_t));
	for (int i = 0; i < *numc; i++) {
		num_arr[i] = get_num_from_float(i, used_mask);
	}
	return num_arr;
}


void add_to_arr(char **splitv, int splitc, struct map *val_map, struct mask curr_mask) {
	int mem_address = atoi(splitv[1]);
	int correspondant = map_contains(*val_map, mem_address);
	int64_t mem_val = get_masked(atoi(splitv[3]), curr_mask);
	if (correspondant < 0) {
		map_add(val_map, atoi(splitv[1]), mem_val);
	}
	else {
		val_map->value[correspondant] = mem_val;
	}
}


void add_to_arr_part2(char **splitv, int splitc, struct map *val_map, struct mask curr_mask) {
	int64_t memc = 0;
	int64_t *mem_address = get_masked_part2(atoi(splitv[1]), curr_mask, &memc);
	int64_t mem_val = atoi(splitv[3]);
	for (int i = 0; i < memc; i++) {
		int correspondant = map_contains(*val_map, mem_address[i]);
		if (correspondant < 0) {
			map_add(val_map, mem_address[i], mem_val);
		}
		else {
			val_map->value[correspondant] = mem_val;
		}
	}
	free(mem_address);

}


struct map load_arr(char *filename, bool part2) {
	FILE *fp = fopen(filename, "r");
	struct map res_map = NULL_MAP;

	if (fp) {
		res_map.key = malloc(MAX_MAP_SIZE * sizeof(int64_t));
		res_map.value = malloc(MAX_MAP_SIZE * sizeof(int64_t));
		res_map.mapc = 0;
		res_map.max_c = MAX_MAP_SIZE;
		char buffer[BUFFER_SIZE];
		char **splitw = NULL;
		int64_t res = 0;
		struct mask curr_mask;
		while (fgets(buffer, BUFFER_SIZE, fp)) {
			long splitc = 0;
			splitw = split(buffer, &splitc, "[ ]", 3);
			if (splitc == 3) {
				curr_mask = get_mask(splitw, splitc);
			}
			else if (part2) {
				add_to_arr_part2(splitw, splitc, &res_map, curr_mask);
			}
			else {
				add_to_arr(splitw, splitc, &res_map, curr_mask);
			}
			for (int i = 0; i < splitc; i++) {
				free(splitw[i]);
			}
			free(splitw);
		}
	}
	fclose(fp);
	return res_map;
}


int64_t day14_part1(struct map arr_map) {
	int64_t res = 0;
	for (int i = 0; i < arr_map.mapc; i++) {
		res += arr_map.value[i];
	}
	return res;
}




int main(int argc, char *argv[]) {
	if (argc == 2) {
		struct map arr_map = load_arr(argv[1],false);
		printf("Sum of all values - %"PRId64"\n", day14_part1(arr_map));
		map_free(arr_map);
		arr_map = load_arr(argv[1], true);
		printf("Sum of all values in part 2 - %"PRId64"\n", day14_part1(arr_map));
		map_free(arr_map);
		return 0;
	}
	else {
		printf("%s\n", USAGE);
		return -1;
	}
}
