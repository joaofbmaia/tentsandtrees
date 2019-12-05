/**
 * Filename: solver.c
 * 
 * Description: Solver for tents and trees games
 */

#ifndef SOLVER_H
#define SOLVER_H

#include "map.h"

/**
 * Function: solveMap
 * 
 * Description: solves tents and trees map
 * 
 * Side-effects: writes solution for mptr
 * 
 * Arguments:
 *     map *mptr - map pointer
 * 
 * Return value:
 *     1 - if map has solution
 *     -1 - if map is impossible
 */
int solveMap(map *mptr);

#endif