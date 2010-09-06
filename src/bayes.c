#include "OPF.h"

void TrainingBayes(Subgraph *sg){
    int     i,j;
    double  dist, radius;

    // Compute maximum arc-weight in the graph between distinct classes
    sg->df = -1.0;
    for (i=0; i < sg->nnodes; i++) {
      radius=FLT_MAX;
      for (j=0; j < sg->nnodes; j++) {
	if ((i != j)&&(sg->node[i].truelabel != sg->node[j].truelabel)){
	  if (!opf_PrecomputedDistance)
	    dist = opf_EuclDist(sg->node[i].feat,sg->node[j].feat,sg->nfeats);
	  else
	    dist = opf_DistanceValue[sg->node[i].position][sg->node[j].position];
	  if (radius > dist)
	    radius = dist;
	}
      }
      if (radius > sg->df)
	sg->df = radius;
    }

    // Compute constant for exponential kernel
    sg->K     = (2.0*(float)sg->df/9.0);
}

void ClassifyBayes(Subgraph *sgTrain, Subgraph *sg){
  int    p,q,i,nelems;
  float  dist,Pmax,rho,radius,*Pi=AllocFloatArray(sgTrain->nlabels+1);

  // Compute a priori probabilities
  for (p=0; p < sgTrain->nnodes; p++)
    Pi[sgTrain->node[p].truelabel]++;
  for (i=1; i <= sgTrain->nlabels; i++)
    Pi[i]/=sgTrain->nlabels;


  // Apply classification
  for (p=0; p < sg->nnodes; p++){
      // For each class, find the radius, the conditional probability
      // density and the maximum a posteriori probability

      Pmax=-1.0;

      for (i=1; i <= sgTrain->nlabels; i++) {
	radius = FLT_MAX;
	for (q=0; q < sgTrain->nnodes; q++){
	  if (sgTrain->node[q].truelabel!=i){
	    if(!opf_PrecomputedDistance)
	      dist = opf_EuclDist(sgTrain->node[q].feat,sg->node[p].feat,sg->nfeats);
	    else
	      dist = opf_DistanceValue[sgTrain->node[q].position][sg->node[p].position];
	    if (dist < radius)
	      radius = dist;
	  }
	}

	rho = 0.0; nelems=1;
	for (q=0; q < sgTrain->nnodes; q++){
	  if (sgTrain->node[q].truelabel==i){
	    if(!opf_PrecomputedDistance)
	      dist = opf_EuclDist(sgTrain->node[q].feat,sg->node[p].feat,sg->nfeats);
	    else
	      dist = opf_DistanceValue[sgTrain->node[q].position][sg->node[p].position];
	    if (dist <= radius){
	      rho += exp(-dist/sgTrain->K);
	      nelems++;
	    }
	  }
	}
	rho = Pi[i]*(rho/nelems);

	if (rho > Pmax){
	  Pmax = rho;
	  sg->node[p].label = i;
	}
      }
    }
  free(Pi);
}


int main(int argc, char **argv){
	if(argc != 3){
		fprintf(stderr,"\nusage bayes <P1> <P2> <P3> <P4>\n");
		fprintf(stderr,"\nP1: training set (OPF format)");
		fprintf(stderr,"\nP2: test set (OPF format)\n");
		return -1;
	}

	Subgraph *Train = ReadSubgraph(argv[1]), *Test = ReadSubgraph(argv[2]);
	char trainingtimefilename[256], testtimefilename[256], predictfilename[256];
	double trainingtime, testingtime;
	int i;
	FILE *f = NULL;
	timer tic, toc;

	/*Training ***/
	fprintf(stdout, "\nTraining Bayes classifier ..."); fflush(stdout);
	gettimeofday(&tic,NULL); TrainingBayes(Train); gettimeofday(&toc,NULL);
	trainingtime = ((toc.tv_sec-tic.tv_sec)*1000.0 + (toc.tv_usec-tic.tv_usec)*0.001)/1000.0;
	fprintf(stdout, " OK"); fflush(stdout);

	/*Testing ***/
	fprintf(stdout, "\nTesting Bayes classifier ..."); fflush(stdout);
	gettimeofday(&tic,NULL); ClassifyBayes(Train, Test); gettimeofday(&toc,NULL);
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
	fprintf(stdout, " OK\n"); fflush(stdout);

	return 0;
}
