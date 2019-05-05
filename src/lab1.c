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
	param * params = getParams(argc, argv);

	printParams(params);

	visibilityArray * va = readVisibilitiesFile(params->filenameVisibilities);

	// char buff[50];
	// int * pipes[2] = (int*)malloc(sizeof(int[2]) * params->numberOfDisks);
	// int i;
	// for (i = 0; i < params->numberOfDisks; ++i) {
	// 	// c = createChild();
	// 	sprintf(buff, "message[%d]\n", i);
	// 	sendToChild((child*)NULL, buff);
	// 	printf("buff->%s\n", buff);
	// }

	return 0;
}