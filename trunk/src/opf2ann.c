#include <stdio.h>
#include "OPF.h"

void 	WriteSubgraph2FANNFormat(Subgraph *cg, char *fann_file_name){
	FILE *fp = NULL;
	int i, j, k;

	fp = fopen(fann_file_name, "w");

	fprintf(fp,"%d %d %d\n",cg->nnodes,cg->nfeats ,cg->nlabels);
	
	for(i = 0; i < cg->nnodes; i++){
		for(j = 0; j < cg->nfeats; j++)
			fprintf(fp,"%f ", cg->node[i].feat[j]);
		fprintf(fp,"\n");
		for(j = 1; j < cg->node[i].truelabel; j++) 
			fprintf(fp,"0 ");
		fprintf(fp,"1 ");
		j++;
		for(k = j; k <= cg->nlabels; k++) 
			fprintf(fp,"0 ");
		fprintf(fp,"\n");
	}
	fclose(fp);
}

int main(int argc, char**argv){
	if(argc != 3){
		fprintf(stderr, "\nusage opf2ann <input libopf file> <output fann file>\n");
		exit(-1);
	}

	Subgraph *g = ReadSubgraph(argv[1]);
	WriteSubgraph2FANNFormat(g, argv[2]);
	DestroySubgraph(&g);

	return 0;
}
