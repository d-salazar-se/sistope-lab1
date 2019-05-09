#include "lab.h"

int main(int argc, char *argv[])
{
	// Obtención y validación de parametros
	param * params = getParams(argc, argv);

	if ( ! params) {
		fprintf(stderr, "%s\n", USAGE);
		exit(1);
	}

	// Leer el archivo con visibilidades
	visibilityArray * visibilities = readVisibilitiesFile(params->filenameVisibilities);

	int i;
	
	// Crear hijos/discos y pipes respectivos
	disk ** disks = createDisks(params->numberOfDisks);

	// Enviar visibilidades a cada hijo/disco 
	int selectedDisk = 0;
	for (i = 0; i < visibilities->numberOfElements; ++i) {
		selectedDisk = selectDisk(params->diskWidth, visibilities->array[i]);
		if (0 < selectedDisk && selectedDisk < params->numberOfDisks){
			writeToChild(disks[selectedDisk], visibilities->array[i]);
		}
	}

	/*
	 * Luego de enviar todas las visibilidades se
	 * envia señal de FIN a cada hijo para que estos comiencen
	 * a enviar sus resultados de vuelta
	 */
	endPipes(disks, params->numberOfDisks, ESCRITURA);

	// Obtener respuestas desde hijos
	int status;
	pid_t pid;
	disk * d;
	i = params->numberOfDisks;
	while (i > 0) {
		pid = wait(&status);
		d = findByPID(disks, params->numberOfDisks, pid);
		readFromChild(d, params->showChildVisibilities);
		i--;
	}

	// Cerrar pipes de lectura desde procesos hijos
	endPipes(disks, params->numberOfDisks, LECTURA);

	// Generar archivo salida con resultados por hijo
	writeResultsFile(params->filenameOutput, disks, params->numberOfDisks);

	// Finalmente liberar la memoria utilizada.
	freeMem(params, visibilities, disks);
	
	return 0;
}