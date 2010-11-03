#include <stdio.h>
#include "OPF.h"

void 	WriteSubgraph2SVMTorchFormat(Subgraph *cg, char *file){
	int i,j;
	FILE *fp = NULL;

	fp = fopen(file,"w");

	fprintf(fp, "%d ", cg->nnodes);
	fprintf(fp, "%d\n", cg->nfeats+1);


	for(i=0; i < cg->nnodes; i++)
	{
		for(j=0; j < cg->nfeats; j++)
			fprintf(fp,"%f ",cg->node[i].feat[j]);
		if(cg->node[i].truelabel == 1)
			fprintf(fp,"%d\n",+1); // Remember +1 or -1
		else
			fprintf(fp,"%d\n",-1);
	}

	fclose(fp);
}

//Multiclass
void    WriteSubgraph2SVMTorchFormatMC(Subgraph *cg, char *file){
        int i,j;
	FILE *fp = NULL;

	fp = fopen(file,"w");

	fprintf(fp, "%d ", cg->nnodes);
	fprintf(fp, "%d\n", cg->nfeats+1);

	for(i=0; i < cg->nnodes; i++)
	{
		for(j=0; j < cg->nfeats; j++)
			fprintf(fp,"%f ",cg->node[i].feat[j]);
		fprintf(fp,"%d\n",cg->node[i].truelabel-1); // Remember 0, 1, 2...
	}
	fclose(fp);

}

int main(int argc, char**argv){
	if(argc != 3){
		fprintf(stderr, "\nusage opf2svmtorch <input libopf file> <output svmtorch file>\n");
		exit(-1);
	}

	Subgraph *g = ReadSubgraph(argv[1]);

	if(g->nlabels == 2)
		WriteSubgraph2SVMTorchFormat(g,argv[2]);
	else
		WriteSubgraph2SVMTorchFormatMC(g,argv[2]);

	DestroySubgraph(&g);

	return 0;
}
