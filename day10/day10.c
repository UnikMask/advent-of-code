#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SIZE 5
const char *USAGE = "Usage:\n\t day10 [ADAPTERS_FILE]";


/**
 * Partitions array between bounds in quicksort manner.
 *
 * @param arrv The array to partition.
 * @param arrc The size of that array.
 * @param low The lower index for swapping.
 * @param high The higher index for swapping.
 * @return The index of the pivot's position after partition, or negative if an error occured.
 */
int partition(long **arrv, int arrc, int low, int high) {
	if (arrv != NULL) {
		int rhigh = high > arrc - 1 ? arrc - 1: high;
		int rlow = low < 0 ? 0: low;
		long pivot = (*arrv)[rhigh]; // Select rightmost element as pivot

		int i = low - 1; // Index smaller elements.
		for (int j = low; j <= rhigh - 1; j++) {
			if ((*arrv)[j] < pivot) {
				i++;
				long tmp = (*arrv)[j];
				(*arrv)[j] = (*arrv)[i];
				(*arrv)[i] = tmp;
			}
		}
		// Swap pivot to new area
		long tmp = (*arrv)[rhigh];
		(*arrv)[rhigh] = (*arrv)[i + 1];
		(*arrv)[i + 1] = tmp;
		return i + 1;
	}
	else {
		return -1;
	}

}


/**
 * Sorts an array of integers using the quicksort algorithm.
 *
 * @param arrv The pointer to the array to sort.
 * @param arrc The size of that array.
 * @param begin The index of the first element to sort.
 * @param end The index of the last element to sort.
 */
void quicksort(long **arrv, int arrc, int begin, int end) {
	if (arrv != NULL && begin < end) {
		int pivot = partition(arrv, arrc, begin, end);
		quicksort(arrv, arrc, begin, pivot - 1);
		quicksort(arrv, arrc, pivot + 1, end);
	}
}


long *load_adapters(char *filename, int* adaptc) {
	FILE *fp = fopen(filename, "r");
	long *adapt_arr = NULL;
	if (fp) {
		char buffer[BUFFER_SIZE];
		while (fgets(buffer, BUFFER_SIZE, fp)) {
			(*adaptc)++;
		}
		rewind(fp);
		adapt_arr = malloc((*adaptc + 2) * sizeof(long));

		for(int i = 0; fgets(buffer, BUFFER_SIZE, fp); i++) {
			adapt_arr[i] = atoll(buffer);
		}
		adapt_arr[*adaptc] = 0;
		(*adaptc)++;
	}
	fclose(fp);
	return adapt_arr;
}


void print_num_diffs(char *filename) {
	int adaptc = 0;
	int num[3] = {0, 0, 0};
	long *adapter_arr = load_adapters(filename, &adaptc);
	quicksort(&adapter_arr, adaptc, 0, adaptc - 1);
	adapter_arr[adaptc] = adapter_arr[adaptc - 1] + 3;
	for (int i = 0; i < adaptc; i++) {
		num[adapter_arr[i + 1] - adapter_arr[i] - 1]++;
	}
	printf("Number of 1-jolt diffs: %d\n Number of 3-jolt diffs: %d\n",
		   num[0], num[2]);
	printf("Num 1-jolt diffs multiplied my num 3-jolt diffs: %d\n", num[0] * num[2]);
	free(adapter_arr);
}


long get_arrangements(long *adapt_arr, int adaptc, int index, int **memory, long **resultv, int *memoryc) {
	for(int i = 0; i < *memoryc; i++) {
		if ((*memory)[i] == index) {
			return (*resultv)[i];
		}
	}
	long arrangements = 0;
	if (index == adaptc) {
		arrangements = 1;
	}
	else {
		for(int i = index + 1; i <= adaptc; i++) {
			if (adapt_arr[i] - adapt_arr[index] <= 3) {
				arrangements += get_arrangements(adapt_arr, adaptc, i, memory, resultv, memoryc);
			}
			else {
				break;
			}
		}
	}
	(*memory)[*memoryc] = index;
	(*resultv)[*memoryc] = arrangements;
	(*memoryc)++;
	return arrangements;
}


void get_total_num_arrangements(char *filename) {
	int adaptc = 0;
	long *adapt_arr = load_adapters(filename, &adaptc);
	quicksort(&adapt_arr, adaptc, 0, adaptc - 1);
	int memoryc = 0;
	int *memory = malloc(adaptc * sizeof(int));
	long *resultv = malloc(adaptc * sizeof(long));
	printf("Number of total arrangements: %ld\n", get_arrangements(adapt_arr, adaptc, 0, &memory,
																   &resultv, &memoryc));
	free(memory);
	free(resultv);
	free(adapt_arr);
}


int main (int argc, char *argv[]) {
	if (argc == 2) {
		print_num_diffs(argv[1]);
		get_total_num_arrangements(argv[1]);
		return 0;
	}
	else {
		printf("%s\n", USAGE);
		return -1;
	}
}
