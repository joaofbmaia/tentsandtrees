/**
 * Filename: main.c
 * 
 * Authors: Group 20 AED
 *     Gonçalo Fernandes - 93070 MEEC
 *     João Maia - 93091 MEEC
 * 
 * Synopsis: This is a tents and trees puzzle solver, made for the final-course
 *           evaluation of the Algorithms and Data Structures course from
 *           Electrical and Computer Engineering degree, at Instituto Superior Técnico, Portugal.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "map.h"

int main(int argc, char *argv[]) {
    char *resultFilename;
    FILE *fpIn, *fpOut;
    map *currentMap;
    int lines, columns, result;

    if (argc != 2) return 0;
    if (strcmp(strrchr(argv[1], '.'), ".camp")) return 0;

    resultFilename = (char *) malloc((strlen(argv[1]) + 2) * sizeof(char));
    if (resultFilename == NULL) return EXIT_FAILURE;

    strcpy(resultFilename, argv[1]);
    *(strrchr(resultFilename, '.')) = '\0';
    strcat(resultFilename, ".tents");

    fpIn = fopen(argv[1], "r");
    if (fpIn == NULL) return 0;

    fpOut = fopen(resultFilename, "w");
    if (fpOut == NULL) return EXIT_FAILURE;

    while (readAndSolveMap(fpIn, &currentMap, &lines, &columns, &result)) {
        writeSolution(fpOut, currentMap, lines, columns, result);
    }

    fclose(fpIn);
    fclose(fpOut);
    free(resultFilename);

    return 0;
}