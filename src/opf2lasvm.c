#include <stdio.h>
#include "OPF.h"

void 	WriteSubgraph2LASVMFormat(Subgraph *cg, char *file){
	int i,j,tmp;
	FILE *fp = NULL;

	fp = fopen(file,"w");

	for(i=0; i < cg->nnodes; i++){
	    if (cg->node[i].truelabel == 1) tmp = -1;
	    else tmp = 1;
		fprintf(fp,"%d ",tmp);
		for(j=0; j < cg->nfeats; j++)
			fprintf(fp,"%d:%f ",j+1,cg->node[i].feat[j]);
		fprintf(fp,"\n");
	}

	fclose(fp);
}

int main(int argc, char**argv){
	if(argc != 3){
		fprintf(stderr, "\nusage opf2lasvm <input libopf file> <output libsvm file>\n");
		exit(-1);
	}

	Subgraph *g = ReadSubgraph(argv[1]);
	WriteSubgraph2LASVMFormat(g, argv[2]);
	DestroySubgraph(&g);

	return 0;
}
