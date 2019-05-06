#ifndef LAB_H
#define LAB_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define USAGE "USAGE:\n\
./lab1.o -i <filename> -o <filename> -n <amount of disks> -d <disk width> [-b]\n\
\t-i\tnombre de archivo de visibilidades\n\
\t-o\tnombre de archivo de salida\n\
\t-n\tcantidad de discos\n\
\t-d\tancho de cada disco\n\
OPTIONS:\n\
\t-b\tShow each child process visibilities"

#define LECTURA 0
#define ESCRITURA 1

typedef struct {
	char * filenameVisibilities;	// -i
	char * filenameOutput;			// -o
	int numberOfDisks;				// -n
	int diskWidth;					// -d
	int showChildVisibilities;		// -b
} param;

typedef struct {
	float u;
	float v;
	float real;
	float imaginary;
	float noise;
} visibility;

typedef struct {
	int numberOfElements;
	visibility ** array;
} visibilityArray;

typedef struct {
	// child process
	int pid;
	int pipe[2];

	// results
	int id;
	float realMean;
	float imaginaryMean;
	float potency;
	float totalNoise;
} disk;

param * getParams(int argc, char * argv[]);

visibilityArray * readVisibilitiesFile(const char * filename);
void printVisibilitiesFile(visibilityArray * va);

float visibilityDistance(visibility * v);
int selectDisk(int diskWidth, visibility * v);

void writeToChild(disk * d, char * message);
void readFromChild(disk * d, int bFlag);

disk ** createDisks(int numberOfDisks);

disk * findByPID(disk ** disks, int numberOfDisks, pid_t pid);

void writeResultsFile(char * filename, disk ** disks, int numberOfDisks);

#endif