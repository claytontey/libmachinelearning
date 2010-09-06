#include "OPF.h"

typedef struct _KNN{
	int size, *label;
	float *dist;
}KNN;

void SortNeighbours(KNN *v, int l, float d){
	int i = 0, j;

	while ((i < v->size) && (v->dist[i] < d))
		i++;

	if (i < v->size){
		for (j = v->size-1; j > i; j--){
			v->dist[j] = v->dist[j-1];
			v->label[j] = v->label[j-1];
		}
		v->dist[j] = d;
		v->label[j] = l;
	}
}

void InitializeKNN(KNN *v){
	int i;

	for (i = 0; i < v->size; i++){
		v->dist[i] = FLT_MAX;
		v->label[i] = 0;
	}
}

int getLabel(KNN *v){
	int i, maxlabel = v->label[0], *vec = NULL, l, aux;

	/* identifying max label */
	for (i = 1; i < v->size; i++){
		if(v->label[i] > maxlabel)
			maxlabel = v->label[i];
	}

	/* couting nearest labels */
	vec = (int *)calloc(maxlabel+1,sizeof(int));
	for (i = 0; i < v->size; i++)
		vec[v->label[i]]++;
	aux = 0;
	for (i = 1; i <= maxlabel; i++){
		if(vec[i] > aux){
			aux = vec[i];
			l = i;
		}
	}

	free(vec);

	return l;
}

KNN *CreateKNN(int size){
	KNN *v = (KNN *)calloc(1,sizeof(KNN));

	v->size = size;
	v->label = (int *)calloc(v->size, sizeof(int));
	v->dist = (float *)calloc(v->size, sizeof(float));

	return v;
}

void DestroyKNN(KNN **v){
	free((*v)->label);
	free((*v)->dist);
	free((*v));
}

void ClassifyKnn(Subgraph *Train, Subgraph *Test, char TrainingMode){
	int i, j;
	float dist;
	KNN *v = CreateKNN(Train->bestk);

	for (i = 0; i < Test->nnodes; i++){
		InitializeKNN(v);
		for (j = 0; j < Train->nnodes; j++){
			if (((TrainingMode) && (i != j)) || !TrainingMode){
				dist = opf_EuclDist(Test->node[i].feat, Train->node[j].feat, Test->nfeats);
				SortNeighbours(v, Train->node[j].truelabel, dist);
			}
		}
		Test->node[i].label = getLabel(v);
	}

	DestroyKNN(&v);
}

void TrainKnn(Subgraph *g){
	int k, kmax;
	float acc, maxacc = FLT_MIN;

	for (k = 1; k <= g->nnodes/5; k+=2){
		g->bestk = k;
		ClassifyKnn(g, g, 1);
		acc = opf_Accuracy(g);
		if(acc > maxacc){
			maxacc = acc;
			kmax = k;
		}
	}
	g->bestk = kmax;
}

int main(int argc, char **argv){
	if(argc != 3){
		fprintf(stderr,"\nusage knn <P1> <P2>\n");
		fprintf(stderr,"\nP1: training set (OPF format)");
		fprintf(stderr,"\nP2: test set (OPF format)");
		return -1;
	}

	Subgraph *Train = ReadSubgraph(argv[1]), *Test = ReadSubgraph(argv[2]);
	char trainingtimefilename[256], testtimefilename[256], predictfilename[256];
	double trainingtime, testingtime;
	FILE *f = NULL;
	timer tic, toc;
	int i;

	/*Training ***/
	fprintf(stdout, "\nTraining Knn ..."); fflush(stdout);
	gettimeofday(&tic,NULL); TrainKnn(Train); gettimeofday(&toc,NULL);
	trainingtime = ((toc.tv_sec-tic.tv_sec)*1000.0 + (toc.tv_usec-tic.tv_usec)*0.001)/1000.0;
	fprintf(stdout, " (best k = %d) OK", Train->bestk); fflush(stdout);

	/*Testing ***/
	fprintf(stdout, "\nTesting Knn ..."); fflush(stdout);
	gettimeofday(&tic,NULL); ClassifyKnn(Train, Test, 0); gettimeofday(&toc,NULL);
	testingtime = ((toc.tv_sec-tic.tv_sec)*1000.0 + (toc.tv_usec-tic.tv_usec)*0.001)/1000.0;
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nWriting output files ..."); fflush(stdout);
	sprintf(trainingtimefilename,"%s.time",argv[1]);
	f = fopen(trainingtimefilename,"a");
	fprintf(f,"%f\n",(float)trainingtime);
	fclose(f);

	sprintf(testtimefilename,"%s.time",argv[2]);
	f = fopen(testtimefilename,"a");
	fprintf(f,"%f\n",(float)testingtime);
	fclose(f);

	sprintf(predictfilename,"%s.predict",argv[2]);
	f = fopen(predictfilename,"w");

	for (i = 0; i < Test->nnodes; i++)
		fprintf(f,"%d\n",Test->node[i].label);
	fclose(f);
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nDeallocating memory ..."); fflush(stdout);
	DestroySubgraph(&Train);
	DestroySubgraph(&Test);
	fprintf(stdout, " OK"); fflush(stdout);

	return 0;
}
