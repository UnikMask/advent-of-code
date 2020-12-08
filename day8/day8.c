#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 15

#define JMP "jmp"
#define ACC "acc"
#define NOP "nop"
#define NULL_INSTRUCTION {Nop, 0}


enum code {
	Jmp, Acc, Nop
};


/**
 * Structure representing a day 8 game console instruction.
 */
struct instruction {
	enum code unitc;
	int value;
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
		char *buffer = NULL;
		for (int i = 0; sentence[i] != '\0' && sentence[i] != '\n'; i++) {
			if (sentence[i] == ' ' || sentence[i + 1] == '\n' || sentence[i + 1] == '\0') {
				// Add word to array and reset buffer for new word.
				buffer[buffer_index] = '\0';
				splitv[split_index] = buffer;
				buffer = NULL;
				buffer_index = 0;
			}
			else if (sentence[i] != ',' && sentence[i] != '.') {
				if (buffer == NULL) {
					buffer = malloc(BUFFER_SIZE * sizeof(char));
				}
				buffer[buffer_index] = sentence[i];
				buffer_index++;
			}
		}
		return splitv;
	}
	else {
		return NULL;
	}
}


/**
 * Gets instruction that given line describes.
 *
 * @param line The line that describes an instruction.
 * @return The instruction structure represented by the input line.
 */
struct instruction get_instruction(char *line) {
	struct instruction res_instruction = NULL_INSTRUCTION;
	if (line != NULL) {
		// Get line split
		int splitc;
		char **splitv = split(line, &splitc);

		// Get what instruction the line is.
		if (strcmp(splitv[0], JMP) == 0) {
			res_instruction.unitc = Jmp;
			printf("%d\n", atoi(splitv[1]));
			res_instruction.value = atoi(splitv[1]);
		}
		for (int i = 0; i < splitc; i++) {
			free(splitv[i]);
		}
		free(splitv);
	}
	return res_instruction;
}


/**
 * Main function of the program.
 *
 * @param argc The number of user arguments.
 * @param argv The array of user arguments.
 * @return The exit code of the program.
 */
int main(int argc, char *argv[]) {
	get_instruction("jmp +3");
	return 0;
}
