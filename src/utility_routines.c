

/************************************************************************************
	Utility routines to support line drawing
	Author:             Ashis Kumar Das
	ID:                 11301002
	Email:              akd.bracu@gmail.com
*************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <list.h>
#include <math.h>



/************************************************************************************
*
*   Function Prototypes and Variable Definitions
*
*************************************************************************************/

float random_f(float lb, float ub);

int util_readFromFile(const char *fileName);

void util_generateRandomFloat(float lb, float ub, unsigned int len, List *outList);




/************************************************************************************
*
*   Implementation of utility functions, which are called by other routines
*
*************************************************************************************/


float random_f(float lb, float ub) {

	float fr, diff;
	register float rnd_f;

	fr = (float) rand();                        /* Generation of original random value */
	rnd_f = fr / (float) (RAND_MAX);            /* Make this a float for factorization */
	diff = ub - lb;                             /* Calculate factor value */
	rnd_f = rnd_f * diff;						/* Factorizing our random value */
	rnd_f = rnd_f + lb;                         /* Rotating our random value */
	return rnd_f;
}



int util_readFromFile(const char *fileName) {

	FILE *pFile;
	char data[60];
	char *res;
	char *format_rect, *format_color;
	
	extern float xmin, xmax, ymin, ymax;
	extern float insideR, insideG, insideB;
	extern float outsideR, outsideG, outsideB;

	format_rect = "%f %f %f %f";
	format_color = "%f %f %f";
	
	pFile = fopen(fileName, "r");
	if (pFile == 0)
		return -1;

	res = fgets(data, 59, pFile);
	sscanf(data, format_rect, &xmin, &xmax, &ymin, &ymax);
	res = fgets(data, 59, pFile);
	sscanf(data, format_color, &insideR, &insideG, &insideB);
	res = fgets(data, 59, pFile);
	sscanf(data, format_color, &outsideR, &outsideG, &outsideB);

	fclose(pFile);

	return 0;
}


void util_generateRandomFloat(float lb, float ub, unsigned int len, List *outList) {
	
	float *xy;                                          /* Pointer to generated XY */
	register unsigned int count;
	
	count = 0;
	while (count < len) {
		xy = (float *) malloc(sizeof(float) * 2);
		*(xy + 0) = random_f(lb, ub);                   /* Generate random X value */
		*(xy + 1) = random_f(lb, ub);                   /* Generate random Y value */
		list_ins_next(outList, list_tail(outList), (const void *) xy);
		count = count + 1;
	}
	
	return;
}
