#include "lab.h"

param * getParams(int argc, char * argv[]) {
	param * params = (param*)malloc(sizeof(param));

	params->numberOfDisks = 1;
	params->diskWidth = 1;
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

	// revisar parametros obligatorios
	if ( ! params->filenameVisibilities
		|| ! params->filenameOutput
		|| params->numberOfDisks <= 0
		|| params->diskWidth <= 0) {
		return (param*)NULL;
	}

	return params;
}

visibilityArray * readVisibilitiesFile(const char * filename) {
	visibilityArray * visibilities = (visibilityArray*)malloc(sizeof(visibilityArray));

	visibilities->numberOfElements = 0;
	visibilities->array = (visibility**)malloc(sizeof(visibility*));

	FILE * fp = fopen(filename, "r");

	if ( ! fp) {
		perror(filename);
	}

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

float visibilityDistance(visibility * v) {
	return sqrt(pow(v->u, 2) + pow(v->v, 2));
}

int selectDisk(int diskWidth, visibility * v) {
	return (visibilityDistance(v) / diskWidth);
}

disk ** createDisks(int numberOfDisks) {
	disk ** disks = (disk**)malloc(numberOfDisks * sizeof(disk*));

	int i;
	for (i = 0; i < numberOfDisks; ++i) {
		disks[i] = (disk*)malloc(sizeof(disk));

		pipe(disks[i]->pipe[0]);
		pipe(disks[i]->pipe[1]);

		disks[i]->pid = fork();
		
		disks[i]->id 			= i + 1;

		disks[i]->realMean 		= 0.0;
		disks[i]->imaginaryMean = 0.0;
		disks[i]->potency 		= 0.0;
		disks[i]->totalNoise 	= 0.0;

		// error
		if (disks[i]->pid < 0) {
			return (disk**)NULL;
		}
		// padre
		else if (disks[i]->pid > 0) {
			close(disks[i]->pipe[0][1]);
			close(disks[i]->pipe[1][0]);
		}
		// hijo(s)
		else {
			// cierra pipes que no usan los hijos
			close(disks[i]->pipe[1][1]);
			close(disks[i]->pipe[0][0]);
			// los que si usa, los copia a la salida standard
			dup2(disks[i]->pipe[1][0], STDIN_FILENO);
			dup2(disks[i]->pipe[0][1], STDOUT_FILENO);
			// ejecuta "vis"
			execve("./vis.o", NULL, NULL);
			return (disk**)NULL;
		}

	}

	return disks;
}

void writeToChild(disk * d, visibility * v) {
	char buffer[MAX_STR_BUFF];
	
	sprintf(buffer, "%f,%f,%f,%f,%f\n", v->u, v->v, v->real, v->imaginary, v->noise);
	
	write(d->pipe[1][1], buffer, strlen(buffer));
}

void readFromChild(disk * d, int bFlag) {
	char buffer[MAX_STR_BUFF];
	int quantity;
	
	read(d->pipe[0][0], buffer, MAX_STR_BUFF * sizeof(char));
	sscanf(buffer, "%f,%f,%f,%f,%d\n", &d->realMean, &d->imaginaryMean, &d->potency, &d->totalNoise, &quantity);
	
	if (bFlag) {
		printf("Soy el hijo de pid %d, procesé %d visibilidades\n", d->pid, quantity);
	}
}

void endPipes(disk ** disks, int numberOfDisks, int stream) {
	int i;
	for (i = 0; i < numberOfDisks; ++i) {
		close(disks[i]->pipe[stream][stream]);
	}
}

disk * findByPID(disk ** disks, int numberOfDisks, pid_t pid) {
	int i;

	for (i = 0; i < numberOfDisks; ++i) {
		if (disks[i]->pid == (long)pid) {
			return disks[i];
		}
	}

	return (disk*)NULL;
}

void writeResultsFile(char * filename, disk ** disks, int numberOfDisks) {
	FILE * fp = fopen(filename, "w");

	if ( ! fp ){
		perror(filename);
		return;
	}

	int i;
	for (i = 0; i < numberOfDisks; ++i) {
		fprintf(fp, "Disco %d:\nMedia real: %f\nMedia imaginaria: %f\nPotencia: %f\nRuido total: %f\n",
						disks[i]->id,
						disks[i]->realMean,
						disks[i]->imaginaryMean,
						disks[i]->potency,
						disks[i]->totalNoise);
	}

	fclose(fp);
}

void freeMem(param * p, visibilityArray * va, disk ** d) {
	int i;

	// visibilities
	for (i = 0; i < va->numberOfElements; ++i) {
		free(va->array[i]);
	}
	free(va);

	// disks
	for (i = 0; i < p->numberOfDisks; ++i) {
		free(d[i]);
	}
	free(d);

	// params
	free(p->filenameVisibilities);
	free(p->filenameOutput);
	free(p);
}
