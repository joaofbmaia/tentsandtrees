/**
 * Filename: io.c
 * 
 * Description: Interaction with files
 */

#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "solver.h"

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
    int ret;
    int *lineHints, *columnsHints;
    int lineSum = 0, columnSum = 0;
    char *lineString;

    ret = fscanf(fp, "%d %d", lines, columns);
    if (ret == EOF) return 0;

    lineHints = (int *) malloc(*lines * sizeof(int));
    if (lineHints == NULL) exit(EXIT_FAILURE);

    columnsHints = (int *) malloc(*columns * sizeof(int));
    if (columnsHints == NULL) exit(EXIT_FAILURE);

    lineString = (char *) malloc((*columns + 1) * sizeof(char));
    if (lineString == NULL) exit(EXIT_FAILURE);

    for (int i = 0; i < *lines; i++) {
        ret = fscanf(fp, "%d", &lineHints[i]);
        lineSum += lineHints[i];
    }

    for (int i = 0; i < *columns; i++) {
        ret = fscanf(fp, "%d", &columnsHints[i]);
        columnSum += columnsHints[i];
    }

    if (lineSum == columnSum) {
        *mptr = newMap(*lines, *columns);
        if (*mptr == NULL) exit(EXIT_FAILURE);
        setTentsInfo(*mptr, lineHints, columnsHints);
        setTentsNumber(*mptr, lineSum);
        for (int i = 0; i < *lines; i++) {
            ret = fscanf(fp, "%s", lineString);
            setMapLine(*mptr, i, lineString);
        }
        *result = solveMap(*mptr);
    } else {
        *mptr = NULL;
        *result = -1;
    }

    free(lineHints);
    free(columnsHints);
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