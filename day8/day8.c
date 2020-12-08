#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
	long value;
};


/**
 * Splits a sentence ended by a termination or newline longo array of words.
 *
 * @param sentence The sentence to split.
 * @param splitc_ptr the polonger to assign the size of the array to.
 * @return The array of words from the sentence, removing punctuation (e.g. [.,]).
 */
char **split(char *sentence, long *splitc_ptr) {
	if (sentence != NULL) {
		// Calculate the size of the sentence.
		if (splitc_ptr == NULL) {
			splitc_ptr = malloc(sizeof(long));
		}
		*splitc_ptr = 0;
		for(long i = 0; sentence[i] != '\0' && sentence[i] != '\n'; i++) {
			if (sentence[i] == ' ' || sentence[i + 1] == '\n' || sentence[i + 1] == '\0') {
				(*splitc_ptr)++;
			}
		}

		// Split sentence longo words.
		char **splitv = malloc(*splitc_ptr * sizeof(char *));
		long split_index = 0;
		long buffer_index = 0;
		char *buffer = NULL;
		for (long i = 0; sentence[i] != '\0' && sentence[i] != '\n'; i++) {
			if (sentence[i] != ',' && sentence[i] != '.' && sentence[i] != ' ') {
				if (buffer == NULL) {
					buffer = malloc(BUFFER_SIZE * sizeof(char));
					buffer_index = 0;
				}
				buffer[buffer_index] = sentence[i];
				buffer_index++;

			}
			if (sentence[i] == ' ' || sentence[i + 1] == '\n' || sentence[i + 1] == '\0') {
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


/**
 * Gets an instruction code from a string describing said code.
 *
 * @param The string describing a code.
 * @return The code describe by the string.
 */
enum code get_code(char *codec) {
	if (strcmp(codec, JMP) == 0) {
		return Jmp;
	}
	else if (strcmp(codec, ACC) == 0) {
		return Acc;
	}
	else {
		return Nop;
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
		long splitc;
		char **splitv = split(line, &splitc);

		// Get what instruction the line is.
		res_instruction.unitc = get_code(splitv[0]);
		res_instruction.value = atoi(splitv[1]);

		// Free the split.
		for (long i = 0; i < splitc; i++) {
			free(splitv[i]);
		}
		free(splitv);
	}
	return res_instruction;
}


/**
 * Gets program/instruction array from file.
 *
 * @param filename The path/name of the file containing the source code.
 * @param arrc The polonger to the size of the file.
 */
struct instruction* get_program(char *filename, long *arrc) {
	FILE *fp = fopen(filename, "r");
	struct instruction* arrv = NULL;
	if (fp) {
		if (arrc == NULL) {
			arrc = malloc(sizeof(long));
		}
		*arrc = 0;

		// Calculate program size.
		char buffer[BUFFER_SIZE];
		while(fgets(buffer, BUFFER_SIZE, fp)) {
			(*arrc)++;
		}
		rewind(fp);
		arrv = malloc(*arrc * sizeof(struct instruction));

		for(long i = 0; fgets(buffer, BUFFER_SIZE, fp); i++) {
			arrv[i] = get_instruction(buffer);
		}
	}
	fclose(fp);
	return arrv;
}


/**
 * Runs given instruction in program.
 *
 * @param instruct The instruction to run.
 * @param acc Polonger to accumulator - global variable modified by program.
 * @param index The index of the instruction in the program.
 * @return null if instruction was successful, else instruction generated an error.
 */
long run_instruction(struct instruction instruct, long *acc, long *index) {
	switch(instruct.unitc) {
		case Jmp:
			if (*index + instruct.value < 0) {
				printf("ERROR: line %ld - JMP to negative bound %ld", *index, *index - instruct.value);
				return -1;
			}
			else {
				(*index) += instruct.value;
			}
			break;
		case Acc:
			(*acc) += instruct.value;
		case Nop:
			(*index)++;
			break;
	}
	return 0;
}


bool modify(struct instruction *mod, long index) {
	if (mod->unitc == Jmp) {
		mod->unitc = Nop;
		return true;
	}
	else if (mod->unitc == Nop) {
		if (index + mod->value >= 0) {
			mod->unitc = Jmp;
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

bool modify_program(struct instruction **program_ptr, long programc) {
	// Prepare for backtracking and line modding
	struct instruction *program = *program_ptr;
	long acc = 0;

	long *i_backlog = malloc(programc * sizeof(long));
	long *invalid = malloc(programc * sizeof(long));
	long blc = -1;
	long invalidc = 0;

	// Reset repeat
	bool *repeat = malloc(programc * sizeof(bool));
	for (int i = 0; i < programc; i++) {
		repeat[i] = false;
	}

	long curr_mod_l = -1;
	long i = 0;
	while (i < programc) {
		if (!repeat[i]) {
			repeat[i] = true;
			blc++;
			i_backlog[blc] = i;
			if (run_instruction(program[i], &acc, &i) != 0) {
				free(repeat);
				free(invalid);
				free(i_backlog);
				return false;
			}
		}
		else {
			// Backtrack and modify new instruction.
			if (curr_mod_l != -1) {
				while (i_backlog[blc] != curr_mod_l) {
					repeat[i_backlog[blc]] = false;
					blc--;
				}
				modify(&program[curr_mod_l], curr_mod_l);
				invalid[invalidc] = curr_mod_l;
				invalidc++;
				curr_mod_l = -1;
			}
			while (curr_mod_l == -1) {
				bool tried = false;
				for (long i = 0; i <= invalidc; i++) {
					if (i_backlog[blc] == invalid[i]) {
						tried = true;
					}
				}
				// Modify new line.
				if (!tried) {
					if(!modify(&program[i_backlog[blc]], i_backlog[blc])) {
						invalid[invalidc] = i_backlog[blc];
						invalidc++;
						repeat[i_backlog[blc]]--;
					}
					else {
						curr_mod_l = i_backlog[blc];
						i = curr_mod_l;
					}
				}
				repeat[i_backlog[blc]] = false;
				blc--;
			}
		}
	}
	free(repeat);
	free(i_backlog);
	free(invalid);
	return true;
}

/**
 * Run given code to get the final accumulator value.
 *
 * @param filename The name/path of the file containing the source code.
 * @param stop_at_repeat Whether the program should stop if it makes the same instruction
 * a second time.
 * @return The value of the accumulator when the program stops.
 */
long run_program(char *filename, bool stop_at_repeat, bool fix_repeat) {
	long programc;
	struct instruction *program = get_program(filename, &programc);
	struct instruction *programd = get_program(filename, &programc);
	if (program != NULL) {
		// Run program
		if (fix_repeat) {
			modify_program(&program, programc);
		}
		long acc = 0;
		long i = 0;

		// Set repeat array all to false for repetition checking.
		bool *repeat = malloc(programc * sizeof(bool));
		for (long i = 0; i < programc; i++) {
			repeat[i] = false;
		}
		int diff = 0;
		while (i < programc && (!repeat[i] || !stop_at_repeat)) {
			repeat[i] = true;
			if (i == 616) {
				printf("Debug mode!\n");
			}
			if (program[i].unitc != programd[i].unitc) {
				diff++;
			}
			if (diff >= 2) {
				printf("WHAAT!?\n");
			}
			if (run_instruction(program[i], &acc, &i) != 0) {
				free(repeat);
				free(program);
				printf("OOPS!\n");
				return acc;
			}
		}
		free(repeat);
		free(programd);
		free(program);
		return acc;
	}
	else {
		return 0;
	}
}


/**
 * Main function of the program.
 *
 * @param argc The number of user arguments.
 * @param argv The array of user arguments.
 * @return The exit code of the program.
 */
int main(int argc, char *argv[]) {
	if (argc == 2) {
		printf("Value of accumulator before infinite cycle: %ld\n",
			   run_program(argv[1], true, false));
		printf("Value of accumulator after nop/jmp cycle fix: %ld\n",
			   run_program(argv[1], true, true));
	}
	else {
		return -1;
	}
}
