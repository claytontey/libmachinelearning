#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv){
	if(argc != 6){
		fprintf(stderr,"\nusage statistics <acc file name> <training time file name> <test time filename> <statistics output filename> <running times>\n");
		exit(-1);
	}

	FILE *fpIn = NULL, *fpOut = NULL;
	int i, it = atoi(argv[5]);
	size_t result;
	float Std = 0.0f, MeanAcc = 0.0f, aux, MeanTrainingTime = 0.0f, MeanTestingTime = 0.0f, *acc = NULL;

	/*Computing mean accuracy and standard deviation***/
	fpIn = fopen(argv[1],"r");
	if(!fpIn){
		fprintf(stderr,"\nunable to open file %s\n",argv[1]);
		exit(-1);
	}
	
	acc = (float *)malloc(it*sizeof(float));
	for (i = 1;i <= it ; i++){
		result = fscanf(fpIn,"%f",&aux);
		acc[i-1] = aux;
		MeanAcc+=aux;
	}
	MeanAcc/=it;
	for (i = 0; i < it; i++)
		Std+=pow(acc[i]-MeanAcc,2);
	Std=sqrt(Std/it);

	fclose(fpIn);
	free(acc);

	/*Computing mean training execution time  ***/
	fpIn = fopen(argv[2], "r");
	if(!fpIn){
		fprintf(stderr,"\nunable to open file %s\n",argv[2]);
		exit(-1);
	}

	for (i = 1;i <= it ; i++){
		result = fscanf(fpIn,"%f",&aux);
		MeanTrainingTime+=aux;
	}
	MeanTrainingTime/=it;
	fclose(fpIn);

	/*Computing mean testing execution time  ***/
	fpIn = fopen(argv[3], "r");
	if(!fpIn){
		fprintf(stderr,"\nunable to open file %s\n",argv[3]);
		exit(-1);
	}

	for (i = 1;i <= it ; i++){
		result = fscanf(fpIn,"%f",&aux);
		MeanTestingTime+=aux;
	}
	MeanTestingTime/=it;
	fclose(fpIn);

	fpOut = fopen(argv[4], "w");
	result = fprintf(fpOut,"%f %f %f %f", MeanAcc, Std, MeanTrainingTime, MeanTestingTime);
	fclose(fpOut);

	return 0;
}