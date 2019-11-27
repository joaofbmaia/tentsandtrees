/**
 * Filename: io.h
 * 
 * Description: functions for interaction with files
 */

#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "map.h"

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
int readAndSolveMap(FILE *fp, map **mptr, int *lines, int *columns, int *result);


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
void writeSolution(FILE *fp, map *mptr, int lines, int columns, int result);

#endif