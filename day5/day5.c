#include <stdio.h>
#include <stdlib.h>

#define N_ROWS 128
#define N_COL 8
#define FILE_SIZE 816

/* Characters used for binary space partitioning of seats. */
#define FRONT 'F'
#define BACK 'B'
#define LEFT 'L'
#define RIGHT 'R'

/* Struct representing a seat */
struct seat {
	u_int32_t row;
	u_int32_t col;
	u_int32_t id;
};


/**
 * Returns a seat from the binary space partition representation of that seat.
 *
 * @param bsp The binary space partition representation of a seat.
 * @return The seat struct representing it.
 */
struct seat get_seat(char *bsp) {
	struct seat res_seat = {0, 0};
	if (bsp != NULL) {
		// Prepare row and column
		int index = 0;
		while (bsp[index] != '\0' && bsp[index] != '\n') {
			switch(bsp[index]) {
				case BACK:
					res_seat.row = res_seat.row << 1;
					res_seat.row = (res_seat.row | 1);
					break;
				case FRONT:
					res_seat.row = res_seat.row << 1;
					break;
				case RIGHT:
					res_seat.col = res_seat.col << 1;
					res_seat.col = (res_seat.col | 1);
					break;
				case LEFT:
					res_seat.col = res_seat.col << 1;
					break;
				default:
					break;
			}
			index++;
		}
		res_seat.id = res_seat.row * 8 + res_seat.col;
		return res_seat;
	}
	else {
		return res_seat;
	}
}


/**
 * Returns an array of seats from a file containing binary space partitions of seats.
 *
 * @param filename The file containing the binary space partitions of seats.
 * @param seatc_ptr Pointer which will be storing the size of the seats array.
 * @returns An array of seats representing that file.
 */
struct seat *compile_seatv(char *filename, int* seatc_ptr) {
	FILE *fp = fopen(filename, "r");

	// Check the file
	if (fp) {
		char buffer[255];
		// Count number of lines in the file.
		int num_lines = 0;
		while (fgets(buffer, 255, fp)) {
			num_lines++;
		}
		rewind(fp); // Go back to beginning.
		struct seat *resv = malloc(num_lines * sizeof(struct seat));
		for (int i = 0; fgets(buffer, 255, fp); i++) {
			resv[i] = get_seat(buffer);
		}
		*seatc_ptr = num_lines;
		fclose(fp);
		return resv;
	}
	else {
		fclose(fp);
		return NULL;
	}
}


/**
 * Finds the highest seat in the aircraft.
 *
 * @param seatv The array of seats in the aircraft.
 * @param seatc The size of the array of seats.
 */
int day5part1(struct seat *seatv, int seatc) {
	int highest_id = 0;
	if (seatv != NULL) {
		for (int i = 0; i < seatc; i++) {
			highest_id =
				(seatv[i].id > highest_id? seatv[i].id : highest_id);
		}
	}
	return highest_id;
}




/**
 * Partitions array between bounds in quicksort manner.
 *
 * @param arrv The array to partition.
 * @param arrc The size of that array.
 * @param low The lower index for swapping.
 * @param high The higher index for swapping.
 * @return The index of the pivot's position after partition, or negative if an error occured.
 */
int partition(u_int32_t **arrv, int arrc, int low, int high) {
	if (arrv != NULL) {
		int rhigh = high > arrc - 1 ? arrc - 1: high;
		int rlow = low < 0 ? 0: low;
		u_int32_t pivot = (*arrv)[rhigh]; // Select rightmost element as pivot

		int i = low - 1; // Index smaller elements.
		for (int j = low; j <= rhigh - 1; j++) {
			if ((*arrv)[j] < pivot) {
				i++;
				u_int32_t tmp = (*arrv)[j];
				(*arrv)[j] = (*arrv)[i];
				(*arrv)[i] = tmp;
			}
		}
		// Swap pivot to new area
		u_int32_t tmp = (*arrv)[rhigh];
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
void quicksort(u_int32_t **arrv, int arrc, int begin, int end) {
	if (arrv != NULL && begin < end) {
		int pivot = partition(arrv, arrc, begin, end);
		quicksort(arrv, arrc, begin, pivot - 1);
		quicksort(arrv, arrc, pivot + 1, end);
	}
}


/**
 * Finds your seat in the aircraft based on day 5 instructions.
 *
 * @param seatv The list of taken seats.
 * @param seatc The size of the list of taken seats.
 * @return The ID of your seat.
 */
int day5part2(struct seat *seatv, int seatc) {
	// Copy seats IDs to new int array.
	u_int32_t *id_arr = malloc(seatc * sizeof(u_int32_t));
	for (int i = 0; i < seatc; i++) {
		id_arr[i] = seatv[i].id;
	}
	// Sort array for easy iteration through IDs.
	quicksort(&id_arr, seatc, 0, seatc - 1);

	// Find first seat where ID + 1 and ID - 1 exists and not first or last row.
	u_int32_t res = 0;
	for (int i = 0; seatc; i++) {
		if (id_arr[i] >= N_COL && id_arr[i] < (N_ROWS - 1) * N_COL) {
			if (i != 0 && i != seatc - 2) {
				if (id_arr[i - 1] == id_arr[i] - 1 && id_arr[i + 1] == id_arr[i] + 2) {
					res = id_arr[i] + 1;
					break;
				}
			}
		}
	}
	free(id_arr);
	return res;
}


/**
 * Main function of the day 5 program.
 *
 * @param argc The number of user-given arguments.
 * @param argv The user-given arguments array.
 * @returns The exit code of the program.
 */
int main(int argc, char *argv[]) {
	if (argc == 2) {
		int seatc = 0;
		struct seat *seatv = compile_seatv(argv[1], &seatc);
		// Complete part 1 of day 5.
		printf("The highest seat ID on the list of given array passes is %d.\n",
			   day5part1(seatv, seatc));
		printf("Your seat ID is %d\n", day5part2(seatv, seatc));
		free(seatv);
		return 0;
	}
	else {
		return -1;
	}
}
