#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    if(argc != 3){
        fprintf(stderr,"\nusage assemblegnufile temp.tmp~ <option>");
        fprintf(stderr,"<option> = <1 - Accuracy | 2 - Training time | 3 - Testing time");
        exit(-1);
    }

    FILE *fpIn = NULL, *fpOut = NULL;
    int op = atoi(argv[2]), i, j;
    size_t result;
    char filename[256], tmp[64];

    fpIn = fopen("temp.tmp~","r");
    if(!fpIn){
        fprintf(stderr,"\nunable to open file temp.tmp~");
        exit(-1);
    }

    switch(op){
        case 1: fpOut = fopen("accuracy.gnu","w");
                break;
        case 2: fpOut = fopen("training-time.gnu","w");
                break;
        case 3: fpOut = fopen("testing-time.gnu","w");
                break;
        default: break;
    }

    result = fprintf(fpOut,"set title \"Robustness of the classifiers with respect to variations on the training set size\"\n");
    result = fprintf(fpOut,"set xlabel \"Training set size percentage\"\n");

    switch(op){
        case 1: result = fprintf(fpOut,"set ylabel \"Accuracy\"\n");
                result = fprintf(fpOut,"set output \"accuracy.eps\"\n");
                break;
        case 2: result = fprintf(fpOut,"set ylabel \"Training time [s]\"\n");
                result = fprintf(fpOut,"set output \"training-time.eps\"\n");
                break;
        case 3: result = fprintf(fpOut,"set ylabel \"Testing time [s]\"\n");
                result = fprintf(fpOut,"set output \"testing-time.eps\"\n");
                break;
        default: break;
    }
    result = fprintf(fpOut,"set terminal postscript eps \"Helvetica\" 17\n");

    j = 0;
    while(!feof(fpIn)){
        result = fscanf(fpIn,"%s",filename);
        i = 0;
        while(filename[i] != '.')
            tmp[i++] = filename[i];
        tmp[i] = '\0';

        if(!j && !feof(fpIn)){
            result = fprintf(fpOut," plot \"%s\" using 1:2 title '%s' with linespoints, \\\n", filename, tmp);
            j++;
        }else{ if(!feof(fpIn))
            result = fprintf(fpOut,"\"%s\" using 1:2 title '%s' with linespoints, \\\n", filename, tmp);
            else result = fprintf(fpOut,"\"%s\" using 1:2 title '%s' with linespoints\n", filename, tmp);
        }
    }

    fclose(fpIn);
    fclose(fpOut);

    return 0;
}
