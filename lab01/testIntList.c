// testIntList.c - a program for testing IntListInsertInOrder

// !!! DO NOT MODIFY THIS FILE !!!

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IntList.h"

#define MAX_LINE_LEN 1024

static IntList readIntList(void);

int main(void) {
	// read the list
	printf("Enter some numbers (must be in ascending order): ");
	IntList l = readIntList();
	if (!IntListIsSorted(l)) {
		fprintf(stderr, "error: the numbers are not in ascending order\n");
		IntListFree(l);
		exit(EXIT_FAILURE);
	}

	// read the number to be inserted
	printf("Enter number to insert: ");
	int num;
	if (scanf("%d", &num) != 1) {
		fprintf(stderr, "error: failed to read number");
		IntListFree(l);
		exit(EXIT_FAILURE);
	}

	printf("Original list:\n");
	IntListShow(l);

	printf("List after inserting %d:\n", num);
	IntListInsertInOrder(l, num);
	IntListShow(l);

	if (!IntListOK(l)) {
		fprintf(stderr,
		        "\n"
		        "#####  ####   ####   #####  ####\n"
		        "#      #   #  #   #  #   #  #   #\n"
		        "####   ####   ####   #   #  ####\n"
		        "#      #  #   #  #   #   #  #  #\n"
		        "#####  #   #  #   #  #####  #   #\n"
		        "\n"
		        "error: IntListOK returned false, which means the list "
		        "was not updated correctly. Please see the IntListOK "
		        "function for details.\n");
		IntListFree(l);
		exit(EXIT_FAILURE);
	}

	IntListFree(l);
}

static IntList readIntList(void) {
	char *line = NULL;
	size_t n = 0;
	if (getline(&line, &n, stdin) < 0) {
		fprintf(stderr, "error: failed to read list");
		free(line);
		exit(EXIT_FAILURE);
	}

	IntList l = IntListNew();
	char *start = line;
	char *token = strtok(line, " \t");
	while (token != NULL) {
		int num;
		if (sscanf(token, "%d", &num) == 1) {
			IntListAppend(l, num);
		} else {
			break;
		}
		token = strtok(NULL, " \t");
	}

	free(start);
	return l;
}

