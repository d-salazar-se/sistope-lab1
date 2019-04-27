#include "lab.h"

// void printVisibilitiesFile(visibilityArray * va) {
// 	int i;
// 	for (i = 0; i < va->numberOfElements; ++i)
// 	{
// 		printf("(%12f,%12f) %12f %12f %12f\n", va->array[i]->u,
// 												va->array[i]->v,
// 												va->array[i]->real,
// 												va->array[i]->imaginary,
// 												va->array[i]->noise);
// 	}
// }

visibilityArray * readVisibilitiesFile(const char * filename) {
	visibilityArray * visibilities = (visibilityArray*)malloc(sizeof(visibilityArray));

	visibilities->numberOfElements = 0;
	visibilities->array = (visibility**)malloc(sizeof(visibility*));

	FILE * fp = fopen(filename, "r");

	visibility * line = (visibility*)malloc(sizeof(visibility));

	while ( ! feof(fp) && fscanf(fp, "%f,%f,%f,%f,%f", &(line->u), &(line->v), &(line->real), &(line->imaginary), &(line->noise))) {
		visibilities->array = (visibility**)realloc(visibilities->array, sizeof(visibility*) * (visibilities->numberOfElements + 1));
		visibilities->array[visibilities->numberOfElements] = (visibility*)malloc(sizeof(visibility));
		
		visibilities->array[visibilities->numberOfElements]->u = line->u;
		visibilities->array[visibilities->numberOfElements]->v = line->v;
		visibilities->array[visibilities->numberOfElements]->real = line->real;
		visibilities->array[visibilities->numberOfElements]->imaginary = line->imaginary;
		visibilities->array[visibilities->numberOfElements]->noise = line->noise;

		visibilities->numberOfElements++;
	}

	fclose(fp);

	return visibilities;
}

void createChilds(int numberOfChilds) {
	int i;

	for (i = 0; i < numberOfChilds; ++i) {
		fork();
		// command: ./vis 
	}
}

void sendToChilds() {
	// execve();
}
