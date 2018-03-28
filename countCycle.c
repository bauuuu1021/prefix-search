#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main ()
{
    FILE *cpyFP=fopen("result/cpyCycle.txt","r");
    FILE *refFP=fopen("result/refCycle.txt","r");
    FILE *output=fopen("result/countCycle.txt","w");
    int cpy[50000]= {0};
    int ref[50000]= {0};
    int temp;
    int countCpy=0, countRef=0, sumCpy=0, sumRef=0;

    while (fscanf(cpyFP,"%d",&temp)!=EOF) {
        if (temp<50000)
            cpy[temp]++;
        countCpy++;
    }

    while (fscanf(refFP,"%d",&temp)!=EOF) {
        if (temp<50000)
            ref[temp]++;
        countRef++;
    }

    int i;
    for (i=0; i<50000; i++) {
        fprintf(output,"cyclesFrequency ref cpy %d %d %d\n", i+1, ref[i],cpy[i]);

        sumCpy+=cpy[i]*i;
        sumRef+=ref[i]*i;
    }

    int meanCpy=sumCpy/countCpy;
    int meanRef=sumRef/countRef;
    double meanCpy_2=0, meanRef_2=0;

    for (i=0; i<50000; i++) {
        meanRef_2+=(i-meanRef)*(i-meanRef)*ref[i];
        meanCpy_2+=(i-meanCpy)*(i-meanCpy)*cpy[i];
    }

    meanCpy_2/=countCpy;
    meanRef_2/=countRef;

    printf("cpy mean %d sig %f\n", meanCpy, sqrt(meanCpy_2));
    printf("ref mean %d sig %f\n", meanRef, sqrt(meanRef_2));



    fclose(cpyFP);
    fclose(refFP);
    fclose(output);
}