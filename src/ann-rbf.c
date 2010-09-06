#include "OPF.h"

void K_Means(Subgraph *g){
	int i,j = 0, kbest, nclusters = g->nlabels;
	unsigned int *k = NULL;
	srandom((unsigned int)time(NULL));
	float dist, mindist;

	/* finding cluster centers and variances */
	k = (unsigned int *)malloc(sizeof(unsigned int)*(unsigned int)nclusters);
	i = nclusters;
	while(i > 0){
		j = RandomInteger(0, g->nnodes-1);
		if(!g->node[j].status){
			g->node[j].status = 1;
			k[i-1] = (unsigned int)j;
			i--;
		}
	}

	for (i = 0; i < g->nnodes; i++){
		mindist = FLT_MAX;
		if(!g->node[i].status){
			for (j = 0; j < nclusters; j++)	{
				dist = EuclDist(g->node[i].feat, g->node[k[j]].feat, g->nfeats);
				if(dist < mindist){
					mindist  = dist;
					kbest = j;
				}
			}
		}
		g->node[j].label = j;
	}

	for (i = 0; i < nclusters; i++)
		fprintf(stderr,"\ncluster %d choosen as %d",i+1,k[i]);

	free(k);
}

void TrainANN(Subgraph *Train){

	/* Finding clusters' centers */
	K_Means(Train);
}

int main(int argc, char **argv){
	if(argc != 3){
		fprintf(stderr, "\nusage ann-rbf <training set> <testing set>\n");
		exit(-1);
	}

	timer tic, toc;
	double trainingtime;
	Subgraph *Train = NULL, *Test = NULL;
	
	Train = ReadSubgraph(argv[1]);
	Test = ReadSubgraph(argv[2]);

	/*Training ***/
	fprintf(stdout, "\nTraining ANN-RBF ..."); fflush(stdout);
	gettimeofday(&tic,NULL); TrainANN(Train); gettimeofday(&toc,NULL);
	trainingtime = ((toc.tv_sec-tic.tv_sec)*1000.0 + (toc.tv_usec-tic.tv_usec)*0.001)/1000.0;
	fprintf(stdout, " OK"); fflush(stdout);

	fprintf(stdout, "\nDeallocating memory ..."); fflush(stdout); 
	DestroySubgraph(&Train);
	DestroySubgraph(&Test);
	fprintf(stdout, " OK\n"); fflush(stdout);

	return 0;
}