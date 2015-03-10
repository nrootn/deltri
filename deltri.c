#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "defs.h"
#include "emem.h"
#include "pmem.h"

void readFile(FILE *, int);
void dumpFile(FILE *, int);
void myPrint(int);
void swap(int*, int*);

/*	take mesh and stitch them together

	file i/o
	read file
	store .ele into structs
		structs: 
			index
			x, y, z
			regionID
	build a node dictionary
	build a ele dictionary
	linked list of structs


	build point dictionary
	offset points when added to ele
	for point dictionary, offset indices
	look into face dictionary

	build point/ele dict for roi5/6 and view on TetView

	C loop through list of files and stitch

*/

int main()
{	
	/* File IO */
	FILE *in_fp, *out_fp;

	// Open input file.
	if ((in_fp = fopen("./curves/roi5.1.node", "r")) == NULL) {
		fputs("Couldn't open input file.\n", stderr);
		exit(-1);
	}
	// Open output file.
	if ((out_fp = fopen("out.node", "w")) == NULL) {
		fputs("Couldn't open output file.\n", stderr);
		exit(-1);
	}

	readFile(in_fp, POINT);
	// myPrint(POINT);

	// Open input file.
	if ((in_fp = fopen("./curves/roi6.1.node", "r")) == NULL) {
		fputs("Couldn't open input file.\n", stderr);
		exit(-1);
	}

	readFile(in_fp, POINT);
	myPrint(POINT);
	dumpFile(out_fp, POINT);

	// fclose(in_fp);
	// fclose(out_fp);

	// // Open input file.
	// if ((in_fp = fopen("./curves/roi5.1.ele", "r")) == NULL) {
	// 	fputs("Couldn't open input file.\n", stderr);
	// 	exit(-1);
	// }
	// // Open output file.
	// if ((out_fp = fopen("out.ele", "w")) == NULL) {
	// 	fputs("Couldn't open output file.\n", stderr);
	// 	exit(-1);
	// }

	// readFile(in_fp, ELE);
	// // myPrint(ELE);

	// // Open input file.
	// if ((in_fp = fopen("./curves/roi6.1.ele", "r")) == NULL) {
	// 	fputs("Couldn't open input file.\n", stderr);
	// 	exit(-1);
	// }

	// readFile(in_fp, ELE);
	// myPrint(ELE);	
	// dumpFile(out_fp, ELE);

	/* GARBAGE COLLECTION */
	// printf("%d\n", fclose(in_fp));
	// fclose(out_fp);
	emptyPoint();
	emptyEle();

	return 0;
}


void readFile(FILE *fp, int type)
{
	char *line = NULL;
    char *tail;
	size_t len = 0;
	ssize_t read;
    int i, vld;

    if (type == ELE) {
    	int tmp;
	    uint tmp_arr[5] = {0, 0, 0, 0, 0};
	    uint offset = (uint) sizeEle();
	    uint pt_offset = (uint) sizePoint();
		
		// Skip first line.
		getline(&line, &len, fp);

		while ((read = getline(&line, &len, fp)) != -1) {
			// Parse line into integers
			i = 0; tmp = -1; vld = 0;
			while (*line != 0) {
				// Skip whitespace manually.
				while (isspace(*line)) line++;
						
				// Parse new number.
				if ((tmp = strtol(line, &tail, 0)) == 0) break;
				// Check if line provides new data.
				if (i == 0 && tmp_arr[0] != tmp) vld = 1;
				if (i < 5) {
					tmp_arr[i++] = (uint) tmp;
				}

				// Advance past it.
				// free(line);
				line = tail;
		  	}

		  	/* If a new line of data is available, 
		  	   offset the index and add to LL. */
		  	if (vld) {
		  		// Shift indices.
		  		tmp_arr[0] += offset;
		  		tmp_arr[1] += pt_offset;
		  		tmp_arr[2] += pt_offset;
		  		tmp_arr[3] += pt_offset;
		  		tmp_arr[4] += pt_offset;
		  		insertEle(sizeEle(), tmp_arr);
		  	}
		}
		printf("New ele added.\n");

	} else if (type == POINT) {
		double tmp;
	    double tmp_arr[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
	    uint offset = (uint) sizePoint();
		
		// Skip first line.
		getline(&line, &len, fp);

		while ((read = getline(&line, &len, fp)) != -1) {
			// Parse line into integers
			i = 0; tmp = -1.0; vld = 0;
			while (*line != 0) {
				// Skip whitespace manually.
				while (isspace(*line)) line++;
						
				// Parse new number.
				if ((tmp = strtod(line, &tail)) == 0) break;
				// Check if line provides new data.
				if (i == 0 && tmp_arr[0] != tmp) vld = 1;
				if (i < 5) {
					tmp_arr[i++] = (double) tmp;
				}

				// Advance past it.
				// free(line);
				line = tail;
		  	}

		  	/* If a new line of data is available, 
		  	   offset the index and add to LL. */
		  	if (vld) {
		  		tmp_arr[0] += offset;
		  		insertPoint(sizePoint(), tmp_arr);
		  	}
		}
		printf("New point added.\n");

	}
}

void dumpFile(FILE *fp, int type)
{
	if 	(type == ELE) 		dumpEle(fp);
	else if (type == POINT) dumpPoint(fp);
}

void myPrint(int type)
{
	if 	(type == ELE) 		printEle();
	else if (type == POINT) printPoint();
}

// Swap values in given pointers
void swap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}
