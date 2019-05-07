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

	visibilityArray * visibilities = readVisibilitiesFile(params->filenameVisibilities);

	disk ** disks = createDisks(params->numberOfDisks);
	/*
	 * Crear hijos y pipes respectivos
	 */
	int i;
	// for (i = 0; i < params->numberOfDisks; ++i) {
	// 	if (fork() == 0) {
	// 		disks[i]->pid = getpid();
	// 	}
	// }

	/*
	 * Enviar visibilidades a cada hijo
	 */
	int selectedDisk = 0;
	for (i = 0; i < visibilities->numberOfElements; ++i) {
		selectedDisk = selectDisk(params->diskWidth, visibilities->array[i]);
		// writeToChild(disks[selectedDisk], "");
		printf("[%d] distancia [%f] va al disco [ID:%d] [%d, %d)\n",
									i,
									visibilityDistance(visibilities->array[i]),
									selectedDisk,
									selectedDisk * params->diskWidth,
									(selectedDisk + 1) * params->diskWidth
									);
	}

	/*
	 * Luego de enviar todas las visibilidades se
	 * envia FIN a cada hijo para que estos comiencen
	 * a enviar sus resultados de vuelta
	 */
	// for (i = 0; i < params->numberOfDisks; ++i) {
	// 	writeToChild(disks[i], "FIN");
	// }

	/*
	 * Obtener respuestas desde hijo
	 */
	// int status;
	// pid_t pid;
	// disk * d;
	// i = params->numberOfDisks;
	// while (i > 0) {
	// 	pid = wait(&status);
	// 	d = findByPID(disks, params->numberOfDisks, pid);
	// 	readFromChild(disks[i], params->showChildVisibilities);
	// 	i--;
	// }

	/*
	 * Generar archivo salida con resultados por hijo
	 */
	writeResultsFile(params->filenameOutput, disks, params->numberOfDisks);

	return 0;
}