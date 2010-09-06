#include "OPF.h"

typedef struct _Map{
	float ***W;
	int **L;
	int dimension, size;
}Map;

Map *CreateMap(int size, int dimension){
	Map *M = NULL;
	int i, j;

	M = (Map *)calloc(1,sizeof(Map));
	M->size = size;
	M->dimension = dimension;

	M->W = (float ***)calloc(size,sizeof(float ***));
	M->L = (int **)calloc(size,sizeof(int **));
	for (i  =0; i < size; i++){
		M->L[i] = (int *)calloc(size,sizeof(int));
		M->W[i] = (float **)calloc(size,sizeof(float **));
		for (j = 0; j < size; j++)
			M->W[i][j] = (float *)calloc(M->dimension,sizeof(float));
	}

	return M;
}

DestroyMap(Map **M){
	int i, j;

	for (i = 0; i < (*M)->size; i++)	{
		for (j = 0; j < (*M)->size; j++)
			free((*M)->W[i][j]);
		free((*M)->W[i]);
		free((*M)->L[i]);
	}

	free((*M)->W);
	free((*M)->L);
	free(*M);
}

void InitializeMap(Map *M){
	int i, j, z;

	for (i = 0; i < M->size; i++)
		for (j = 0; j < M->size; j++)
			for (z = 0; z < M->dimension; z++)
				M->W[i][j][z] = RandomInteger(0,100)/100.0;
}

void TrainKohonen(Map *M, Subgraph *g, int T){
	int i, j, n, winner_i, winner_j, t, k, N = M->size/2;
	float dist, mindist, h, alpha, sigma;

	InitializeMap(M);

	while (N >= 0){
		for (t = 0; t <= T-1; t++){
			/*Calculating winner neuron for each trainining node*/
			for (n = 0; n < g->nnodes; n++){
				mindist = FLT_MAX;
				for (i = 0; i < M->size; i++){
					for (j = 0; j < M->size; j++){
						dist = opf_EuclDist(g->node[n].feat, M->W[i][j],M->dimension);
						if(dist < mindist){
							mindist = dist;
							winner_i = i;
							winner_j = j;
						}
					}
				}

				/*updating winner neuron weights neighborhood*/
				sigma = 2-(t/(float)T);
				alpha = 1-(t/(float)T);

				for (i = winner_i-N; i<= winner_i-N+N; i++){
					for (j = winner_j-N; j <= winner_j+N; j++){
						if  ((i >= 0) && (i <= M->size-1) && (j >= 0) && (j <= M->size-1)){
							dist = opf_EuclDist(M->W[i][j], M->W[winner_i][winner_j],M->dimension);;
							h = alpha*exp(-(dist/(sigma*sigma)));
							for (k = 0; k < M->dimension; k++)
								M->W[i][j][k]+= (g->node[n].feat[k]-M->W[i][j][k])*h;
						}
					}
				}
			}
		}
		N-=1;
	}

	/* labeling neurons */
	for (i = 0; i < M->size; i++){
		for (j = 0; j < M->size; j++){
			mindist = FLT_MAX;
			for (n = 0; n < g->nnodes; n++){
				dist = opf_EuclDist(M->W[i][j], g->node[n].feat, M->dimension);
				if(dist < mindist){
					mindist = dist;
					k = g->node[n].truelabel;
				}
			}
			M->L[i][j] = k;
		}
	}

	/* tunning map (Learning Vector Quantization) */
	for (t = 0; t < T-1; t++){
		for (n = 0; n < g->nnodes; n++){
			mindist = FLT_MAX;
			for (i = 0; i < M->size; i++){
				for (j = 0; j < M->size; j++){
					dist = opf_EuclDist(g->node[n].feat, M->W[i][j],M->dimension);
					if(dist < mindist){
						mindist = dist;
						winner_i = i;
						winner_j = j;
					}
				}
			}

			alpha = 1-(t/(float)T);
			if(M->L[winner_i][winner_j] == g->node[n].truelabel){
				for (k = 0; k < M->dimension; k++)
					M->W[winner_i][winner_j][k]+= alpha*(g->node[n].feat[k]-M->W[winner_i][winner_j][k]);
			}
			else{
				for (k = 0; k < M->dimension; k++)
					M->W[winner_i][winner_j][k]-= alpha*(g->node[n].feat[k]-M->W[winner_i][winner_j][k]);
			}
		}
	}
}

void ClassifyKohonen(Map *M, Subgraph *g){
	int n,i,j, winner_i, winner_j;
	float dist, mindist;

	for (n = 0; n < g->nnodes; n++){
		mindist = FLT_MAX;
		for (i = 0; i < M->size; i++){
			for (j = 0; j < M->size; j++){
				dist = opf_EuclDist(M->W[i][j], g->node[n].feat, M->dimension);
				if(dist < mindist){
					mindist = dist;
					winner_i = i;
					winner_j = j;
				}
			}
		}
		g->node[n].label = M->L[winner_i][winner_j];
	}
}

void WriteKohonen(Map *M, char *filename){
	int i,j, k;
	FILE *f = fopen(filename, "w");

	for (i = 0; i < M->size; i++){
		for (j = 0; j < M->size; j++){
			fprintf(f,"%d	",M->L[i][j]);
		}
		fprintf(f,"\n");
	}
	fclose(f);
}

int main(int argc, char **argv){
	if(argc != 5){
		fprintf(stderr,"\nusage kohonen <P1> <P2> <P3> <P4>\n");
		fprintf(stderr,"\nP1: training set (OPF format)");
		fprintf(stderr,"\nP2: test set (OPF format)");
		fprintf(stderr,"\nP3: number of neurons");
		fprintf(stderr,"\nP4: learning iterations\n");
		return -1;
	}

	Subgraph *Train = ReadSubgraph(argv[1]), *Test = ReadSubgraph(argv[2]);
	int neurons = atoi(argv[3]), iterations = atoi(argv[4]), i;
	Map *M = CreateMap(neurons,Train->nfeats);
	char trainingtimefilename[256], testtimefilename[256], predictfilename[256];
	double trainingtime, testingtime;
	FILE *f = NULL;
	timer tic, toc;

	/*Training ***/
	fprintf(stdout, "\nTraining Kohonen ..."); fflush(stdout);
	gettimeofday(&tic,NULL); TrainKohonen(M, Train, iterations); gettimeofday(&toc,NULL);
	trainingtime = ((toc.tv_sec-tic.tv_sec)*1000.0 + (toc.tv_usec-tic.tv_usec)*0.001)/1000.0;
	fprintf(stdout, " OK"); fflush(stdout);

	/*Testing ***/
	fprintf(stdout, "\nTesting Kohonen ..."); fflush(stdout);
	gettimeofday(&tic,NULL); ClassifyKohonen(M, Test); gettimeofday(&toc,NULL);
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
	DestroyMap(&M);
	fprintf(stdout, " OK\n"); fflush(stdout);

	return 0;
}
