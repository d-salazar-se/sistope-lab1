#include <stdio.h>
#include <math.h>

int main(int argc, char const *argv[])
{
	float realMean, imaginaryMean, potency, totalNoise;
	float u, v, real, imaginary, noise;
	int records = 0;

	while(scanf("%f,%f,%f,%f,%f", u, v, real, imaginary, noise)) {
		realMean += real;
		imaginaryMean += imaginary;
		potency += sqrt(pow(real, 2) + pow(imaginary, 2));
		totalNoise += noise;

		records++;
	}

	realMean = realMean / (float)records ;
	imaginaryMean = imaginaryMean / (float)records ;

	printf("%f,%f,%f,%f,%f\n", realMean, imaginaryMean, potency, totalNoise);

	return 0;
}