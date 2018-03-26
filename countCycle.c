#include <stdio.h>
int main ()
{
    FILE *cpyFP=fopen("result/cpyCycle.txt","r");
    FILE *refFP=fopen("result/refCycle.txt","r");
    FILE *output=fopen("result/countCycle.txt","w");
    int cpy[20000]= {0};
    int ref[20000]= {0};
    int temp;

    while (fscanf(cpyFP,"%d",&temp)!=EOF) {
        if (temp<20000)
            cpy[temp]++;
    }

    while (fscanf(refFP,"%d",&temp)!=EOF) {
        if (temp<20000)
            ref[temp]++;
    }

    int i;
    for (i=0; i<20001; i++)
        fprintf(output,"cyclesFrequency ref cpy %d %d %d\n", i, ref[i],cpy[i]);

    fclose(cpyFP);
    fclose(refFP);
    fclose(output);
}