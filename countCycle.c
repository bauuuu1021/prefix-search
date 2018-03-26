#include <stdio.h>
int main ()
{
    FILE *cpyFP=fopen("result/cpyCycle.txt","r");
    FILE *refFP=fopen("result/refCycle.txt","r");
    FILE *output=fopen("result/countCycle.txt","w");
    int cpy[50000]= {0};
    int ref[50000]= {0};
    int temp;

    while (fscanf(cpyFP,"%d",&temp)!=EOF) {
        if (temp<50000)
            cpy[temp]++;
        else
            printf("%d\n",temp);
    }

    while (fscanf(refFP,"%d",&temp)!=EOF) {
        if (temp<50000)
            ref[temp]++;
        else
            printf("%d\n",temp);
    }

    int i;
    for (i=0; i<50000; i++)
        fprintf(output,"cyclesFrequency ref cpy %d %d %d\n", i+1, ref[i],cpy[i]);

    fclose(cpyFP);
    fclose(refFP);
    fclose(output);
}