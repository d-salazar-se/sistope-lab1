#include "lab.h"

/**
	Como usar:

	-i nombre del archivo con visibilidades
	-o nombre de archivo de salida
	-n cantidad de discos
	-d ancho de cada disco
	-b flag => ver POR CONSOLA cantidad de visibilidades por cada proceso hijo
**/

int main(int argc, char *argv[])
{
	int option;

	// -i
	char * filenameVisibilities;
	// -o
	char * filenameOutput;
	// -n
	int numberOfDisks = 1;
	// -d
	int diskWidth = 1;
	// -b
	int showChildVisibilities = 1;

	opterr = 0;
	while((option = getopt(argc, argv, "i:o:n:d:b")) != -1) {
		switch (option) {
			case 'i':
				filenameVisibilities = (char*)malloc(sizeof(char) * (strlen(optarg) + 1));
				strcpy(filenameVisibilities, optarg);
				filenameVisibilities[strlen(filenameVisibilities)] = '\0';
				break;
			case 'o':
				filenameOutput = (char*)malloc(sizeof(char) * (strlen(optarg) + 1));
				strcpy(filenameOutput, optarg);
				filenameOutput[strlen(filenameOutput)] = '\0';
				break;
			case 'n':
				numberOfDisks = atoi(optarg);
				break;
			case 'd':
				diskWidth = atoi(optarg);
				break;
			case 'b':
				showChildVisibilities = 1;
			default:
				abort();
		}
	}

	printf("i: %s\no: %s\nn: %d\nd: %d\nb: %d\n", filenameVisibilities, filenameOutput, numberOfDisks, diskWidth, showChildVisibilities);

	visibilityArray * va = readVisibilitiesFile("visibilities.csv");
	// printVisibilitiesFile(va);

	int numberOfchilds = 0;

	// createChilds(numberOfchilds);

	// sendToChilds();

	return 0;
}