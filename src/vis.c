#include <stdio.h>
#include <math.h>

int main(int argc, char const *argv[])
{
	float realMean = 0.0,
			imaginaryMean = 0.0,
			potency = 0.0,
			totalNoise = 0.0,
			quantity = 0.0;
	float u, v, real, imaginary, noise;

	// Lee mientras no se cierre el pipe desde el proceso padre
	while(fscanf(stdin, "%f,%f,%f,%f,%f\n", &u, &v, &real, &imaginary, &noise) != EOF) {
		realMean += real;
		imaginaryMean += imaginary;
		potency += sqrt(pow(real, 2) + pow(imaginary, 2));
		totalNoise += noise;

		quantity++;
	}

	// Previene division por 0.
	if (quantity > 0) {
		realMean = realMean / quantity;
		imaginaryMean = imaginaryMean / quantity;
	} else {
		realMean = 0.0;
		imaginaryMean = 0.0;
	}

	// Utiliza el cambio en el pipe de dub2
	// para enviar el resultado al proceso padre.
	printf("%f,%f,%f,%f,%d\n", realMean, imaginaryMean, potency, totalNoise, (int)quantity);

	return 0;
}