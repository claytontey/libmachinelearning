#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
    if(argc != 2){
        fprintf(stderr,"\nusage generatetxtfile4gnu <input file name>");
        exit(-1);
    }

    int percentage, pi, pf, size, i, j;
    char tmp[64], *classifier = NULL;
    float acc, std, trainingtime, testingtime;
    FILE *fp = NULL;
    size_t result;

    sprintf(tmp,"%c",argv[1][strlen(argv[1])-5]);
    percentage = atoi(tmp)*10;

    fp = fopen(argv[1], "r");
    if(!fp){
        fprintf(stderr,"\nunable to open file %s",argv[1]);
        exit(-1);
    }
    result = fscanf(fp,"%f %f %f %f",&acc,&std,&trainingtime,&testingtime);
    fclose(fp);

    /*retrieving the classifier's name*/
    pf = strlen(argv[1])-7; pi = pf;
    while(argv[1][pi] != '.')
        pi--;
    pi++; size = pf-pi+1; j = 0;
    classifier = (char *)malloc((size+1)*sizeof(char));
    for(i = pi; i <= pf; i++)
        classifier[j++] = argv[1][i];
    classifier[j] = '\0';
    printf("\nclassifier's name: %s",classifier);

    sprintf(tmp,"%s.accuracy.gnudat",classifier);
    fp = fopen(tmp,"a");
    result = fprintf(fp,"%d %f\n",percentage,acc);
    fclose(fp);

    sprintf(tmp,"%s.training-time.gnudat",classifier);
    fp = fopen(tmp,"a");
    result = fprintf(fp,"%d %f\n",percentage,trainingtime);
    fclose(fp);

    sprintf(tmp,"%s.testing-time.gnudat",classifier);
    fp = fopen(tmp,"a");
    result = fprintf(fp,"%d %f\n",percentage,testingtime);
    fclose(fp);

    free(classifier);

    return 0;
}
