#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>
#include <pcre2.h>


char valid[1][4] = {
	"cid"
};
const int valid_size = 1;


char required[7][4] =  {
	"byr",
	"iyr",
	"eyr",
	"hgt",
	"hcl",
	"ecl",
	"pid"
};
const int required_size = 7;
char req_regex[5][25] = {
	"^[:digit:]\{4\\}$",
	"^[:digit:]\{3\\}$",
	"^[:digit:]\{2\\}$",
	"^[:xdigit:]\{6\\}$",
	"^[:digit:]\{9\\}$"
};

char ecl_req[7][4] =  {
	"amb",
	"blu",
	"brn",
	"gry",
	"grn",
	"hzl",
	"oth"
};
int ecl_reqc = 7;


struct field {
	char *key;
	char *value;
};


bool matches(char *sentence, char *regex) {
	regex_t regexcmp;
	if (regcomp(&regexcmp, sentence, 0) == 0) {
		if (regexec(&regexcmp, sentence, 0, NULL, 0) == 0) {
			regfree(&regexcmp);
			return true;
		}
		regfree(&regexcmp);
	}
	regfree(&regexcmp);
	return false;
}


bool checkppvalidity(struct field *fieldv, int fieldc, char required[][4], char valid[][4], int requiredc, int validc) {
	// Check if all required elements are in.
	for (int i  = 0; i < requiredc; i++) {
		bool has_req = false;
		for (int j = 0; j < fieldc; j++) {
			if (strcmp(fieldv[j].key, required[i]) == 0) {
				has_req = true;
				break;
			}
		}
		if (!has_req) {
			return false;
		}
	}
	return true;
}


bool checkpart2val(struct field *fieldv, int fieldc) {
	if (checkppvalidity(fieldv, fieldc, required, valid, required_size, valid_size)) {
		bool total_match = true;
		for(int i = 0; i < fieldc; i++) {
			bool does_match = true;
			if (strcmp(fieldv[i].key, required[0]) == 0) {
				// Match byr
				if (!matches(fieldv[i].value, req_regex[0])) {
					does_match =  false;
				}
				else {
					int num_year = atoi(fieldv[i].value);
					if (!(num_year >= 1920 && num_year <= 2002)) {
						does_match = false;
					}
				}
			}
			else if (strcmp(fieldv[i].key, required[1]) == 0) {
				// Match iyr
				if (!matches(fieldv[i].value, req_regex[0])) {
					does_match = false;
				}
				else {
					int num_year = atoi(fieldv[i].value);
					if (!(num_year >= 2010 && num_year <= 2020)) {
						does_match = false;
					}
				}

			}
			else if (strcmp(fieldv[i].key, required[2]) == 0) {
				// Match eyr
				if (!matches(fieldv[i].value, req_regex[0])) {
					does_match = false;
				}
				else {
					int num_year = atoi(fieldv[i].value);
					if (!(num_year >= 2020 && num_year <= 2030)) {
						does_match = false;
					}
				}

			}
			else if (strcmp(fieldv[i].key, required[3]) == 0) {
				// Match hgt
				does_match = false;
				if (matches(fieldv[i].value, req_regex[1])) {
					char num[4] =  {
						fieldv[i].value[0],
						fieldv[i].value[1],
						fieldv[i].value[2],
						'\0'
					};
					int cm = atoi(num);
					does_match = (cm >= 150 && cm <= 193);
				}
				else if (matches(fieldv[i].value, req_regex[2])){
					char num[3] =  {
						fieldv[i].value[0],
						fieldv[i].value[1],
						'\0'
					};
					int in = atoi(num);
					does_match = (in >= 59 && in <= 76);
				}

			}
			else if (strcmp(fieldv[i].key, required[4]) == 0) {
				// Match hcl
				if (!matches(fieldv[i].value, req_regex[3])) {
					does_match = false;
				}

			}
			else if (strcmp(fieldv[i].key, required[5]) == 0) {
				does_match = false;
				for(int j = 0; j < ecl_reqc; j++) {
					if (strcmp(fieldv[i].value, ecl_req[j]) == 0) {
						does_match = true;
					}
				}
			}
			else if (strcmp(fieldv[i].key, required[6]) == 0) {
				if (!matches(fieldv[i].value, req_regex[4])) {
					does_match = false;
				}
			}
			printf("%s - %s: %s\n",
				   fieldv[i].key, fieldv[i].value,
				   does_match?"true":"false");
			if (does_match == false) {
				total_match = false;
			}
		}
		return total_match;
	}
	else {
		return false;
	}
}

char *get_field_str(struct field print_field) {
	char * print_str = malloc((4 + 24) * sizeof(char));
	print_str[0] = '[';
	int index = 1;
	for (int i = 0; i < 4; i++) {
		print_str[index] = print_field.key[i];
		index++;
	}
	print_str[index] = ',';
	index++;
	for (int i = 0; print_field.value[i] == '\0'; i++)  {
		print_str[index] = print_field.value[i];
		index++;
	}
	print_str[index] = ']';
	print_str[index + 1] = '\0';
	return print_str;
}

/**
 * Returns number of valid passports based on day 4 part 1 of advent of code.
 * @param filepath The path to the file containing the passport fields.
 */
int day4part1(char *filepath) {
	FILE *manip = fopen(filepath, "r");
	// Do file manipulation
	if (manip) {
		int res = 0;
		int m_res = 0;
		char buffer[255];
		// Loop over each line of file
		while (fgets(buffer, 255, manip)) {
			// Allocate memory to field array.
			struct field *fieldv = malloc(20 * sizeof(struct field));
			int fields = 0;
			fieldv[fields].key = malloc(4 * sizeof(char));
			fieldv[fields].value = malloc(24 * sizeof(char));

			char *editw = fieldv[fields].key;
			int windex = 0;

			int buffi = 0; // Buffer index.
			// Loop over buffer
			while(buffer[buffi] != '\n') {
				// Loop through line buffer
				if (buffer[buffi] == ':') {
					// Complete key of field and get to value.
					editw[windex] = '\0';
					editw = fieldv[fields].value;
					windex = 0;
				}
				else if (buffer[buffi] == ' ') {
					// Complete value and get to next field.
					editw[windex] = '\0';
					fields++;
					fieldv[fields].key = malloc(4 * sizeof(char));
					fieldv[fields].value = malloc(24 * sizeof(char));
					editw = fieldv[fields].key;
					windex = 0;
				}
				else {
					// Fill current string with next character.
					editw[windex] = buffer[buffi];
					windex++;
				}
				buffi++;
			}
			// Complete last value of last element.
			printf("\n");
			editw[windex] = '\0';

			// Check if passport is valid.
			if (checkppvalidity(fieldv, fields + 1, required, valid, required_size, valid_size)) {
				res++;
				if (checkpart2val(fieldv, fields + 1)) {
					m_res++;
				}
			}

		}
		fclose(manip);
		printf("Number of very valid passports: %d\n", m_res);
		return res;
	}
	else {
		fclose(manip);
		return 0;
	}
}


int main(int argc, char *argv[]) {
	if (argc == 2) {
		printf("Number of valid passwords: %d\n", day4part1(argv[1]));
		return 0;
	}
	else {
		return -1;
	}
}
