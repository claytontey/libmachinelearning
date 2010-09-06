#include "OPF.h"

int main(int argc, char **argv){
	if(argc != 4){
		fprintf(stderr, "\nusage mysvm <training set> <testing set> <0 - Linear kernel | 2 - RBF kernel | 3 - Sigmoid kernel | 4 - No kernel>\n");
		exit(-1);
	}
	char cmd[256], trainmodelpath[256], testpredictpath[256], accfilename[256], trainingtimefilename[256], testtimefilename[256];
	float trainingtime, testtime;
	timer tic, toc;
	FILE *f;
	size_t result;
	int op = atoi(argv[3]);

	if(op >= 0)
		sprintf(cmd,"./easy.py %s %d",argv[1],op);
	else sprintf(cmd,"./easy_linear.py %s",argv[1]);

	gettimeofday(&tic,NULL); system(cmd); gettimeofday(&toc,NULL);
	trainingtime = ((toc.tv_sec-tic.tv_sec)*1000.0 + (toc.tv_usec-tic.tv_usec)*0.001)/1000.0;
	fprintf(stdout, "\nTraining time: %f seconds", trainingtime); fflush(stdout);

	sprintf(trainmodelpath,"%s.model",argv[1]);
	sprintf(testpredictpath,"%s.predict",argv[2]);
	fflush(stdout); fprintf(stdout,"\nSVM testing ..."); fflush(stdout);
	if(op >= 0 )
		sprintf(cmd,"./svm-predict %s %s %s",argv[2], trainmodelpath, testpredictpath);
	else sprintf(cmd,"./svm_linear-predict %s %s %s",argv[2], trainmodelpath, testpredictpath);
	gettimeofday(&tic,NULL); system(cmd); gettimeofday(&toc,NULL);
	testtime = ((toc.tv_sec-tic.tv_sec)*1000.0 + (toc.tv_usec-tic.tv_usec)*0.001)/1000.0;
	fprintf(stdout, "\Classifying time: %f seconds", testtime); fflush(stdout);

	sprintf(trainingtimefilename,"%s.time",argv[1]);
	f = fopen(trainingtimefilename,"a");
	result = fprintf(f,"%f\n",trainingtime);
	fclose(f);

	sprintf(testtimefilename,"%s.time",argv[2]);
	f = fopen(testtimefilename,"a");
	result = fprintf(f,"%f\n",testtime);
	fclose(f);

	return 0;
}
