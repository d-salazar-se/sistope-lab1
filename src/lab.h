#ifndef LAB_H
#define LAB_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

//===================================================+
// DEFINICION DE CONSTANTES
//===================================================+

/**
 * Modo de uso del programa, especificando parametros obligatorios y opcionales
 */
#define USAGE "USAGE:\n\
./lab1.o -i <filename> -o <filename> -n <amount of disks> -d <disk width> [-b]\n\
\t-i\tNombre de archivo de visibilidades.\n\
\t-o\tNombre de archivo de salida.\n\
\t-n\tCantidad de discos.\n\
\t-d\tAncho de cada disco.\n\
OPTIONS:\n\
\t-b\tMuestra registros realizados por cada hijo."

/**
 * Streams de identificacion para pipes
 */
#define LECTURA 0
#define ESCRITURA 1

/**
 * Maximo tamano de buffer para transmision de
 * datos en pipes
 */
#define MAX_STR_BUFF 256

//===================================================+
// DEFINICION DE TIPOS DE DATOS
//===================================================+

/**
 * Representa parametros de entrada del programa
 */
typedef struct {
	char * filenameVisibilities;	//< -i: Nombre de archivo de visibilidades 
	char * filenameOutput;			//< -o: Nombre de archivo de salida (resultados)
	int numberOfDisks;				//< -n: Numero de discos
	int diskWidth;					//< -d: Ancho de cada disco
	int showChildVisibilities;		//< -b: Mostrar Log
} param;

/**
 * Representa 1 registro de visibilidad,
 * es decir, una linea del archivo de entrada.
 */
typedef struct {
	float u;						//< Posicion en eje X
	float v;						//< Posicion en eje Y
	float real;						//< Valor Real
	float imaginary;				//< Valor Imaginario
	float noise;					//< Ruido
} visibility;

/**
 * Representa el archivo completo de entrada.
 */
typedef struct {
	int numberOfElements;			//< Numero de elementos del array
	visibility ** array;			//< Registros de visibilidad
} visibilityArray;

/**
 * Representa un disco/proceso hijo
 */
typedef struct {
	// child process
	int pid;						//< Process ID
	int pipe[2][2];					//< Pipe bi-direccional

	// results
	int id;							//< ID del disco
	float realMean;					//< Promedio real
	float imaginaryMean;			//< Promedio imaginario
	float potency;					//< Potencia
	float totalNoise;				//< Ruido total
} disk;

//===================================================+
// DEFINICION DE FIRMAS DE FUNCIONES
//===================================================+

/**
 * Obtiene los parametros desde argc y argv de la funcion main().
 *
 * @param 	int 		argc  	Número de variables en array argv.
 * @param 	char** 		argv  	Parametros como arreglo de strings.
 * @return 	param*				Estructura con los parametros asignados.
 */
param * getParams(int argc, char * argv[]);

/**
 * Lee el archivo de visibilidades y los almacena en un array.
 *
 * @param 	const char* 		filename 	Nombre del archivo con visibilidades.
 * @return 	visibilityArray*				Estructura con los registros del archivo.
 */
visibilityArray * readVisibilitiesFile(const char * filename);

/**
 * Calcula la distancia desde el centro del plano hasta la visibilidad.
 *
 * @param 	visibility* 	v 	Visibilidad a la que se calculara la distancia desde el centro
 * @return 	float				Distancia desde el centro.
 */
float visibilityDistance(visibility * v);

/**
 * Selecciona un disco/proceso de acuerdo a la distancia que se encuentra la visibilidad.
 *
 * @param 	int 			diskWidth 	Ancho de los discos.
 * @param 	visibility* 	v 			Visibilidad a la que se calculara el disco al que pertenece.
 * @return 	int							Indice del disco basado en la distancia.
 */
int selectDisk(int diskWidth, visibility * v);

/**
 * Escribe hacia un proceso hijo los datos de una visibilidad.
 *
 * @param 	disk* 			d 			Disco/Proceso al cual escribir.
 * @param 	visibility* 	v 			Visibilidad que se enviara como data.
 * @return 	void
 */
void writeToChild(disk * d, visibility * v);

/**
 * Lee desde un proceso hijo los datos resultantes del disco.
 *
 * @param 	disk* 			d 			Disco/Proceso del cual leer.
 * @param 	int 			bFlag		Determina si se muestra o no la cantidad de visibilidades procesadas
 * 										0: No se muestra, 1: Si se muestra.
 * @return 	void
 */
void readFromChild(disk * d, int bFlag);

/**
 * Crea tantos procesos hijos/discos como "numberOfDisks"
 *
 * @param 	int 			numberOfDisks 			Numero de procesos/discos a crear.
 * @return 	disk** 									Arreglo con informacion de los procesos creados.
 */
disk ** createDisks(int numberOfDisks);

/**
 * Cierra los pipes de cierto stream (IN/OUT) desde el proceso padre.
 *
 * @param 	disk** 			disks 					Arreglo con informacion de los procesos hijos/discos.
 * @param 	int 			numberOfDisks 			Cantidad de discos/procesos corriendo.
 * @param 	int 			stream 					0: LECTURA, 1: ESCRITURA.
 * @return 	void
 */
void endPipes(disk ** disks, int numberOfDisks, int stream);

/**
 * Encuentra y retorna un disco desde el arreglo en base a su PID.
 *
 * @param 	disk** 			disks 					Arreglo con informacion de los procesos hijos/discos.
 * @param 	int 			numberOfDisks 			Cantidad de discos/procesos corriendo.
 * @param 	pid_t 			pid 					PID del proceso buscado.
 * @return 	disk*									Estructura disk si se encuentra, NULL en caso contrario.
 */
disk * findByPID(disk ** disks, int numberOfDisks, pid_t pid);

/**
 * Escribe los resultados en el archivo de salida.
 *
 * @param 	char* 			filename 				Nombre del archivo de salida.
 * @param 	disk** 			disks 					Arreglo con informacion de los procesos hijos/discos.
 * @param 	int 			numberOfDisks 			Cantidad de discos/procesos corriendo.
 * @return 	void
 */
void writeResultsFile(char * filename, disk ** disks, int numberOfDisks);

/**
 * Libera la memoria utilizada por las estructuras.
 *
 * @param 	param* 				p 				Parametros de entrada del programa
 * @param 	visibilityArray* 	v 				Arreglo de visibilidades, representa el archivo de entrada..
 * @param 	disk** 				d 				Arreglo con informacion de los procesos hijos/discos.
 * @return 	void
 */
void freeMem(param * p, visibilityArray * va, disk ** d);

#endif