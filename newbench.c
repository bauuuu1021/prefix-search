#include <stdio.h>

int main ()
{
    FILE * fp1=fopen("cities.txt","r");
    FILE * fp2=fopen("benchmark_all.txt","w");
    char word[100];

    while ((fscanf(fp1, "%s", word)) != EOF) {
        fprintf(fp2,"f\n%s\n", word);
    }
    fprintf(fp2,"q\n");

    fclose(fp1);
    fclose(fp2);
}