#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 20
#define BAG "bag"
#define BAGS "bags"
#define MAX_CONTAINED_BAGS 5
#define CONTAIN "contain"
#define INVALID_SIZE 640
#define TARGET_BAG "shiny gold"
#define NULL_BAG {NULL, NULL, NULL, -1}


/* Structure for a bag. */
struct bag {
	char *color; /* Color of the bag. */
	char **supported_bags; /* array of supported bags. */
	int *num_bags; /* Array corresponding each supported bag to number it can support. */
	int sbagc; /* Size of array of supported bag. */
};



/**
 * Splits a sentence ended by a termination or newline into array of words.
 *
 * @param sentence The sentence to split.
 * @param splitc_ptr the pointer to assign the size of the array to.
 * @return The array of words from the sentence, removing punctuation (e.g. [.,]).
 */
char **split(char *sentence, int *splitc_ptr) {
	if (sentence != NULL) {
		// Calculate the size of the sentence.
		if (splitc_ptr == NULL) {
			splitc_ptr = malloc(sizeof(int));
		}
		*splitc_ptr = 0;
		for(int i = 0; sentence[i] != '\0' && sentence[i] != '\n'; i++) {
			if (sentence[i] == ' ' || sentence[i + 1] == '\n' || sentence[i + 1] == '\0') {
				(*splitc_ptr)++;
			}
		}

		// Split sentence into words.
		char **splitv = malloc(*splitc_ptr * sizeof(char *));
		int split_index = 0;
		int buffer_index = 0;
		char *buffer = malloc(BUFFER_SIZE * sizeof(char));
		for (int i = 0; sentence[i] != '\0' && sentence[i] != '\n'; i++) {
			if (sentence[i] == ' ' || sentence[i + 1] == '\n' || sentence[i + 1] == '\0') {
				// Add word to array and reset buffer for new word.
				buffer[buffer_index] = '\0';
				splitv[split_index] = buffer;
				buffer = malloc(BUFFER_SIZE * sizeof(char));
				split_index++;
				buffer_index = 0;
			}
			else if (sentence[i] != ',' && sentence[i] != '.') {
				buffer[buffer_index] = sentence[i];
				buffer_index++;
			}
		}
		free(buffer);
		return splitv;
	}
	else {
		return NULL;
	}
}


/**
 * Getter for next color in line.
 *
 * @param splitv The array of words in line to check in.
 * @param splitc The size of the array of words.
 * @param stop_index The index to read the array of words from, and pointer to where
 * end stop index is.
 * @return The next color in line from stop index.
 */
char *get_next_color(char **splitv, int splitc, int* stop_index) {
	char *res_str = malloc(BUFFER_SIZE * sizeof(char));
	strcpy(res_str, "");
	if (splitv != NULL) {
		if (stop_index == NULL) {
			stop_index = malloc(sizeof(int));
			(*stop_index) = 0;
		}
		if ((*stop_index) < splitc) {
			for (int i = (*stop_index); strcmp(splitv[i], BAG)
					&& strcmp(splitv[i], BAGS); i++) {
				if (strcmp(res_str, "")) {
					strcat(res_str, " ");
				}
				strcat(res_str, splitv[i]);
				(*stop_index)++;
			}
			(*stop_index)++;
		}
	}
	return res_str;
}


/**
 * Defines a bag based on the line describing it.
 *
 * @param The line defining the bag.
 * @return The bag structure representing that line.
 */
struct bag define_bag(char *df_line) {
	struct bag res_bag = NULL_BAG;
	if (df_line != NULL) {
		// Get word split
		int splitc = 0;
		char **splitv = split(df_line, &splitc);

		// Get bag color
		int stop_index = 0;
		int bag_index = 0;
		res_bag.color = get_next_color(splitv, splitc, &stop_index);

		// Check if next string is contain. Else return null res_bag.
		if (strcmp(splitv[stop_index], CONTAIN) == 0) {
			stop_index++;
			// Get contained colored bags.
			res_bag.supported_bags = malloc(MAX_CONTAINED_BAGS * sizeof(char *));
			res_bag.num_bags = malloc(MAX_CONTAINED_BAGS * sizeof(int));

			// Loop until end of file
			if (strcmp(splitv[stop_index], "no")) {
				while (stop_index < splitc) {
					int num_contains = atoi(splitv[stop_index]);
					stop_index++;
					char *curr_bag = get_next_color(splitv, splitc, &stop_index);
					if (strcmp(curr_bag, "")) {
						res_bag.num_bags[bag_index] = num_contains;
						res_bag.supported_bags[bag_index] = curr_bag;
						bag_index++;
					}
				}
			}
		}
		for (int i = 0; i < splitc; i++) {
			free(splitv[i]);
		}
		free(splitv);
		res_bag.sbagc = bag_index;
	}
	return res_bag;
}


/**
 * Creates a bag array from file.
 *
 * @param filename The name/path of the file containing the bag list.
 * @param bagc The pointer on which the size of the bag array is assigned.
 * @return The array in which all bags in file are put in.
 */
struct bag* get_bag_arr(char *filename, int* bagc) {
	struct bag* res_arrv = NULL;
	FILE *fp = fopen(filename, "r");
	if (fp) {
		// Calculate the size the array should be at
		char buffer[255];
		int index = 0;
		while (fgets(buffer, 255, fp)) {
			index++;
		}
		res_arrv = malloc(index * sizeof(struct bag));
		index = 0;
		rewind(fp);
		while (fgets(buffer, 255, fp)) {
			struct bag curr_bag = define_bag(buffer);
			res_arrv[index] = curr_bag;
			index++;
		}
		if (bagc == NULL) {
			bagc = malloc(sizeof(int));
		}
		*bagc = index;
	}
	fclose(fp);
	return res_arrv;

}


/**
 * Getter for bag based on kind in the bag array.
 *
 * @param color The color identifying the bag.
 * @param bagv The bag array containing the bag to check for.
 * @param bagc The size of the bag array.
 * @return The bag structure from the bag array corresponding to the color.
 */
struct bag get_bag(char *color, struct bag* bagv, int bagc) {
	struct bag null_bag = NULL_BAG;
	for (int i = 0; i < bagc; i++) {
		if (strcmp(bagv[i].color, color) == 0) {
			return bagv[i];
		}
	}
	return null_bag;
}


/**
 * Check if the given bag array contains source bag.
 *
 * @param src The bag to check for existance in bag array.
 * @param bagv The bag array to check in.
 * @param bagc The size of the bag array.
 * @returns Whether the given bag is contained in the bag array or not.
 */
bool contains(struct bag src, struct bag *bagv, int bagc) {
	for (int i = 0; i < bagc; i++) {
		if(strcmp(src.color, bagv[i].color) == 0) {
			return true;
		}
	}
	return false;
}


/**
 * Check if a bag can contain a target bag after recursion.
 *
 * @param dest The destination bag, to check if it contains target bag.
 * @param target The target bag color to check for.
 * @param bagv The bag array containing all kinds of bags.
 * @param bagc The size of the bag array.
 * @param validv The array of all bags containing target.
 * @param valids The size of the array of valid arrays.
 * @returns Whether the bag contains target bag.
 */
bool bag_can_contain(struct bag dest, char* target, struct bag* bagv, int bagc, struct bag** validv, int *valids) {
	bool res = false;
	if (!contains(dest, *validv, *valids)) {
		if (strcmp(dest.color, target) == 0) {
			res = true;
		}
		else {
			for (int i = 0; i < dest.sbagc; i++) {
				struct bag next = get_bag(dest.supported_bags[i], bagv, bagc);
				res = bag_can_contain(next, target, bagv, bagc, validv, valids);
				if (res == true) {
					break;
				}
			}
		}
		if (res == true) {
			(*validv)[*valids] = dest;
			(*valids)++;
		}
		return res;
	}
	 else {
		 return true;
	}
}


/**
 * Gets number of bags in given bag, this bag included.
 *
 * @param given_bag The bag to check.
 * @param bagv The array of bags for rule checking.
 * @param bagc The size of the array of bags.
 * @return The number of bags in given bag, this bag included.
 */
int get_num_in_bag(struct bag given_bag, struct bag* bagv, int bagc) {
	int num = 1;
	for (int i = 0; i < given_bag.sbagc; i++) {
		struct bag next_bag = get_bag(given_bag.supported_bags[i], bagv, bagc);
		num += given_bag.num_bags[i] * get_num_in_bag(next_bag, bagv, bagc);
	}
	return num;
}


/**
 * Completes part 1 of day 7: Number of kinds of bags that contain at least
 * 1 shiny gold bag in the end.
 *
 * @param filename The name of the file where rules for all bags are contained.
 * @return The number of kinds of bags containing at least one target bag.
 */
int get_num_gold_bags(char *filename) {
	int res = 0;
	int bagc = 0;
	struct bag* bagv = get_bag_arr(filename, &bagc);
	struct bag *validv = malloc(bagc * sizeof(struct bag));
	int valids = 0;
	for (int i = 0; i < bagc; i++) {
		if (bag_can_contain(bagv[i], TARGET_BAG, bagv, bagc, &validv, &valids)
		&& strcmp(bagv[i].color, TARGET_BAG) != 0) {
			res++;
		}
	}

	// Free array of bags.
	for (int i = 0; i < bagc; i++) {
		free(bagv[i].color);
		free(bagv[i].num_bags);
		for (int j = 0; j < bagv[i].sbagc; j++) {
			free(bagv[i].supported_bags[j]);
		}
		free(bagv[i].supported_bags);
	}
	free(validv);
	free(bagv);
	return res;
}


/**
 * Completes part 2 of day 7: Number of bags inside a gold bag.
 *
 * @param filename The name/path of the file containing bag descriptions.
 * @return The number of bags in the target bag.
 */
int get_num_bags_in_gold_bag(char *filename) {
	int num = -1; // Take gold bag itself into account.
	int bagc = 0;
	struct bag *bagv = get_bag_arr(filename, &bagc);
	num += get_num_in_bag(get_bag(TARGET_BAG, bagv, bagc), bagv, bagc);

	// Free array of bags.
	for (int i = 0; i < bagc; i++) {
		free(bagv[i].color);
		free(bagv[i].num_bags);
		for (int j = 0; j < bagv[i].sbagc; j++) {
			free(bagv[i].supported_bags[j]);
		}
		free(bagv[i].supported_bags);
	}
	free(bagv);
	return num;
}

/**
 * Main function of the program.
 * @param argc The number of user arguments.
 *
 * @param argv The arrray of user arguments.
 * @return The exit code of the program.
 */
int main(int argc, char *argv[]) {
	if (argc == 2) {
		printf("Number of bag able to contain at least 1 gold bag: %d\n",
			   get_num_gold_bags(argv[1]));
		printf("Number of bags in gold bag: %d\n", get_num_bags_in_gold_bag(argv[1]));
	}
	else {
		return -1;
	}
}
