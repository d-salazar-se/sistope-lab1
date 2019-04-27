#ifndef LAB_H
#define LAB_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
} child;

// void printVisibilitiesFile(visibilityArray * va);


visibilityArray * readVisibilitiesFile(const char * filename);

#endif