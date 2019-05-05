#ifndef LAB_H
#define LAB_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
	int pid;
	int pipe;
	int fatherPid;
} child;

param * getParams(int argc, char * argv[]);
void printParams(param * p);

visibilityArray * readVisibilitiesFile(const char * filename);
void printVisibilitiesFile(visibilityArray * va);

void sendToChild(child * c, char * message);

#endif