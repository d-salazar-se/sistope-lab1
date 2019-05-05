#include "lab.h"

#define USAGE "USAGE:\n\
./lab1.o -i <visibilities filename> -o <output filename> -n
	-o nombre de archivo de salida
	-n cantidad de discos
	-d ancho de cada disco\n
OPTIONS:\n
\t-b\tShow each child process visibilities
"

void printVisibilitiesFile(visibilityArray * va) {
	int i;
	for (i = 0; i < va->numberOfElements; ++i)
	{
		printf("(%12f,%12f) %12f %12f %12f\n", va->array[i]->u,
												va->array[i]->v,
												va->array[i]->real,
												va->array[i]->imaginary,
												va->array[i]->noise);
	}
}

void printParams(param * p) {
	printf("-i: %s\n-o: %s\n-n: %d\n-d: %d\n-b: %d\n",
				p->filenameVisibilities,
				p->filenameOutput,
				p->numberOfDisks, 
				p->diskWidth,
				p->showChildVisibilities
			);
}

param * getParams(int argc, char * argv[]) {
	param * params = (param*)malloc(sizeof(param));

	params->numberOfDisks = 0;
	params->diskWidth = 0;
	params->showChildVisibilities = 0;

	int option;
	opterr = 0;
	while((option = getopt(argc, argv, "i:o:n:d:b")) != -1) {
		switch (option) {
			case 'i':
				params->filenameVisibilities = (char*)malloc(sizeof(char) * (strlen(optarg) + 1));
				strcpy(params->filenameVisibilities, optarg);
				params->filenameVisibilities[strlen(params->filenameVisibilities)] = '\0';
				break;
			case 'o':
				params->filenameOutput = (char*)malloc(sizeof(char) * (strlen(optarg) + 1));
				strcpy(params->filenameOutput, optarg);
				params->filenameOutput[strlen(params->filenameOutput)] = '\0';
				break;
			case 'n':
				params->numberOfDisks = atoi(optarg);
				break;
			case 'd':
				params->diskWidth = atoi(optarg);
				break;
			case 'b':
				params->showChildVisibilities = 1;
				break;
			default:
				abort();
		}
	}

	opterr = 0;
	// revisar parametros obligatorios
	if ( ! params->filenameVisibilities) {
		fprintf(stderr, "Parametro incorrecto para -i <filename>\n");
		opterr = 1;
	}
	if ( ! params->filenameOutput) {
		fprintf(stderr, "Parametro incorrecto para -o <filename>\n");
		opterr = 1;
	}
	if (params->numberOfDisks <= 0) {
		fprintf(stderr, "Parametro incorrecto para -n <number of disks>\n");
		opterr = 1;
	}
	if (params->diskWidth <= 0) {
		fprintf(stderr, "Parametro incorrecto para -d <disk width>\n");
		opterr = 1;
	}

	if (opterr == 1) {
		fprintf(stderr, "%s\n", USAGE);
	}

	return params;
}

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

void sendToChild(child * c, char * message) {
	char * argv[] = { message, NULL };
	char * envp[] = { NULL };

	execve("./vis.o", argv, envp);
}
