#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include "tst.h"

uint64_t rdtsc()
{
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}


/** constants insert, delete, max word(s) & stack nodes */
enum { INS, DEL, WRDMAX = 256, STKMAX = 512, LMAX = 1024 };
#define REF INS
#define CPY DEL

/* timing helper function */
static double tvgetf(void)
{
    struct timespec ts;
    double sec;

    clock_gettime(CLOCK_REALTIME, &ts);
    sec = ts.tv_nsec;
    sec /= 1e9;
    sec += ts.tv_sec;

    return sec;
}

/* simple trim '\n' from end of buffer filled by fgets */
static void rmcrlf(char *s)
{
    size_t len = strlen(s);
    if (len && s[len - 1] == '\n')
        s[--len] = 0;
}

// Change the '0' of string to a ','
static void addComma(char *s)
{
    size_t len = strlen(s);
    if (len && s[len-1] != ',')
        s[len] = ',';
}

#define IN_FILE "cities.txt"
#define OUTPUT_CYCLE "result/cpyCycle.txt"

int main(int argc, char **argv)
{
    uint64_t startCycle, endCycle;
    char word[WRDMAX] = "";
    char *sgl[LMAX] = {NULL};
    tst_node *root = NULL, *res = NULL;
    int rtn = 0, idx = 0, sidx = 0;
    FILE *fp = fopen(IN_FILE, "r");
    double t1, t2;
    FILE *selectFP, *benchFP;
    FILE *outputCycle=fopen(OUTPUT_CYCLE,"a");


    // input cmd by benchmark or manual input
    if (argc>1 && !strcmp(argv[1],"--bench"))   //by benchmark
        selectFP=benchFP=fopen(argv[2], "r");   //manual
    else
        selectFP=stdin;

    if (!fp) { /* prompt, open, validate file for reading */
        fprintf(stderr, "error: file open failed '%s'.\n", argv[1]);
        fclose(outputCycle);
        return 1;
    }

    t1 = tvgetf();
    while ((rtn = fscanf(fp, "%s", word)) != EOF) {
        char *p = word;
        if (!tst_ins_del(&root, &p, INS, CPY)) {
            fprintf(stderr, "error: memory exhausted, tst_insert.\n");
            fclose(fp);
            fclose(outputCycle);
            return 1;
        }
        idx++;
    }
    t2 = tvgetf();

    fclose(fp);
    printf("ternary_tree, loaded %d words in %.6f sec\n", idx, t2 - t1);

    for (;;) {
        char *p;
        printf(
            "\nCommands:\n"
            " a  add word to the tree\n"
            " f  find word in tree\n"
            " s  search words matching prefix\n"
            " d  delete word from the tree\n"
            " q  quit, freeing all data\n\n"
            "choice: ");
        fgets(word, sizeof word, selectFP);
        p = NULL;
        switch (*word) {
        case 'a':
            printf("enter word to add: ");
            if (!fgets(word, sizeof word, selectFP)) {
                fprintf(stderr, "error: insufficient input.\n");
                break;
            }
            rmcrlf(word);
            p = word;
            t1 = tvgetf();
            res = tst_ins_del(&root, &p, INS, CPY);
            t2 = tvgetf();
            if (res) {
                idx++;
                printf("  %s - inserted in %.6f sec. (%d words in tree)\n",
                       (char *) res, t2 - t1, idx);
            } else
                printf("  %s - already exists in list.\n", (char *) res);
            break;
        case 'f':
            printf("find word in tree: ");
            if (!fgets(word, sizeof word, selectFP)) {
                fprintf(stderr, "error: insufficient input.\n");
                break;
            }
            rmcrlf(word);
            t1 = tvgetf();
            startCycle=rdtsc();
            res = tst_search(root, word);
            endCycle=rdtsc();

            t2 = tvgetf();
            if (res)
                printf("  found %s in %.6f sec.\n", (char *) res, t2 - t1);
            else {
                addComma(word);
                t1 = tvgetf();
                startCycle=rdtsc();
                res = tst_search(root, word);
                endCycle=rdtsc();
                t2 = tvgetf();

                if (res)
                    printf("  found %s in %.6f sec.\n", (char *) res, t2 - t1);
                else
                    printf("  %s not found.\n", word);
            }
            fprintf(outputCycle,"%" PRIu64 "\n", endCycle-startCycle);

            break;
        case 's':
            printf("find words matching prefix (at least 1 char): ");
            if (!fgets(word, sizeof word, selectFP)) {
                fprintf(stderr, "error: insufficient input.\n");
                break;
            }
            rmcrlf(word);
            t1 = tvgetf();
            res = tst_search_prefix(root, word, sgl, &sidx, LMAX);
            t2 = tvgetf();
            if (res) {
                printf("  %s - searched prefix in %.6f sec\n\n", word, t2 - t1);
                for (int i = 0; i < sidx; i++)
                    printf("suggest[%d] : %s\n", i, sgl[i]);
            } else
                printf("  %s - not found\n", word);
            break;
        case 'd':
            printf("enter word to del: ");
            if (!fgets(word, sizeof word, selectFP)) {
                fprintf(stderr, "error: insufficient input.\n");
                break;
            }
            rmcrlf(word);
            p = word;
            printf("  deleting %s\n", word);
            t1 = tvgetf();
            res = tst_ins_del(&root, &p, DEL, CPY);
            t2 = tvgetf();
            if (res)
                printf("  delete failed.\n");
            else {
                printf("  deleted %s in %.6f sec\n", word, t2 - t1);
                idx--;
            }
            break;
        case 'q':
            tst_free_all(root);
            fclose(outputCycle);
            return 0;
            break;
        default:
            fprintf(stderr, "error: invalid selection.\n");
            break;
        }
    }

    fclose(outputCycle);
    return 0;
}
