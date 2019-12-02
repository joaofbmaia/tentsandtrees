/**
 * Filename: io.c
 * 
 * Description: Interaction with files
 */

#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "solver.h"

#define READ_SYNC_FAILURE 5

/**
 * Function: readAndSolveMap
 * 
 * Description: reads problem from file calls apropriate solving functions
 * 
 * Arguments:
 *     FILE *fp - file pointer
 *     map **mptr - map pointer
 *     int *lines - returns number of lines
 *     int *columns - returns number of columns
 *     int *result - returns result
 * 
 * Return value: 
 *     1 - if map was read
 *     0 - if EOF
 */
int readAndSolveMap(FILE *fp, map **mptr, int *lines, int *columns, int *result) {
    int ret, negative = 0;
    int *lineHints, *columnHints;
    int lineSum = 0, columnSum = 0;
    char *lineString;

    ret = fscanf(fp, "%d %d", lines, columns);
    if (ret == EOF) return 0;
    if (ret != 2) exit(READ_SYNC_FAILURE);

    lineHints = (int *) malloc(*lines * sizeof(int));
    if (lineHints == NULL) exit(EXIT_FAILURE);

    columnHints = (int *) malloc(*columns * sizeof(int));
    if (columnHints == NULL) exit(EXIT_FAILURE);

    lineString = (char *) malloc((*columns + 1) * sizeof(char));
    if (lineString == NULL) exit(EXIT_FAILURE);

    for (int i = 0; i < *lines; i++) {
        ret = fscanf(fp, "%d", &lineHints[i]);
        if (ret != 1) exit(READ_SYNC_FAILURE);
        if (lineHints[i] < 0) negative = 1;
        lineSum += lineHints[i];
    }

    for (int i = 0; i < *columns; i++) {
        ret = fscanf(fp, "%d", &columnHints[i]);
        if (ret != 1) exit(READ_SYNC_FAILURE);
        if (columnHints[i] < 0) negative = 1;
        columnSum += columnHints[i];
    }

    if (lineSum == columnSum && !negative) {
        *mptr = newMap(*lines, *columns);
        if (*mptr == NULL) exit(EXIT_FAILURE);
        setTentsInfo(*mptr, lineHints, columnHints);
        setTentsNumber(*mptr, lineSum);
        for (int i = 0; i < *lines; i++) {
            ret = fscanf(fp, "%s", lineString);
            if (ret != 1) exit(READ_SYNC_FAILURE);
            setMapLine(*mptr, i, lineString);
        }
        *result = solveMap(*mptr);
    } else {
        for (int i = 0; i < *lines; i++) {
            ret = fscanf(fp, "%s", lineString);
            if (ret != 1) exit(READ_SYNC_FAILURE);
        }
        *mptr = NULL;
        *result = -1;
    }

    free(lineHints);
    free(columnHints);
    free(lineString);

    return 1;
}

/**
 * Function: readAndSolveMap
 * 
 * Description: writes problem output to file
 * 
 * Arguments:
 *     FILE *fp - file pointer
 *     map *mptr - map pointer
 *     int lines - number of lines
 *     int columns - number of columns
 *     int result - result
 * 
 * Return value: none
 */
void writeSolution(FILE *fp, map *mptr, int lines, int columns, int result) {
    fprintf(fp, "%d %d %d\n", lines, columns, result);

    if (result == 1) {
        for (int i = 0; i < lines; i++) {
            fprintf(fp, "%s\n", getMapLine(mptr, i));
        }
    }
    fprintf(fp, "\n");

    deleteMap(mptr);
}