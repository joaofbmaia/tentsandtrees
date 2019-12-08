/**
 * Filename: solver.c
 * 
 * Description: Solver for tents and trees games
 */

#include <stdlib.h>
#include <string.h>
#include "map.h"

struct {
    int dx;
    int dy;
} adjacents[] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

struct {
    int dx;
    int dy;
} orthogonals[] = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};

typedef struct {
    int line;
    int column;
} cell;

void countNumberOfTrees(map *mptr);
int markUncertainCells(map *mptr);
void buildUncertainAndTreeArray(map *mptr, cell **uncertainArray, cell **treeArray);
int checkHintsConsistency(map *mptr);
int backtrackingSolve(map *mptr, cell *uncertainArray, cell *treeArray, cell *links, char *visited, int current);
int validTent(map *mptr, cell Cell, cell *treeArray, cell *links, char *visited);
int validGrass(map *mptr, cell Cell);
int localInjectivity(map *mptr, cell tent, cell *treeArray, cell *links, char *visited);

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
int solveMap(map *mptr) {
    int possible;
    cell *uncertainArray = NULL;
    cell *treeArray = NULL;
    cell *links = NULL;
    char *visited = NULL;

    countNumberOfTrees(mptr);
    if (getTreesNumber(mptr) < getTentsNumber(mptr)) return -1;

    possible = markUncertainCells(mptr);
    if (!possible) return -1;

    buildUncertainAndTreeArray(mptr, &uncertainArray, &treeArray);

    possible = checkHintsConsistency(mptr);
    if (!possible) {
        free(uncertainArray);
        return -1;
    }

    links = (cell *) malloc(getTreesNumber(mptr) * sizeof(cell));
    if (links == NULL) exit(EXIT_FAILURE);
    for (int i = 0; i < getTreesNumber(mptr); i++) {
        links[i].line = -1;
        links[i].column = -1;
    }

    visited = (char *) malloc(getTreesNumber(mptr) * sizeof(char));
    if (visited == NULL) exit(EXIT_FAILURE);

    possible = backtrackingSolve(mptr, uncertainArray, treeArray, links, visited, 0);

    free(uncertainArray);
    free(treeArray);
    free(links);
    free(visited);
    if (!possible) return -1;

    return 1;
}

/**
 * Function: countNumberOfTrees
 * 
 * Description: counts number of trees inside map
 * 
 * Side-effects: writes number of trees in mptr
 * 
 * Arguments:
 *     map *mptr - map pointer
 * 
 * Return value: none
 */
void countNumberOfTrees(map *mptr) {
    int treeCount = 0;
    for (int i = 0; i < getMapLines(mptr); i++) {
        for (int j = 0; j < getMapColumns(mptr); j++) {
            if (getContentOfPosition(mptr, i, j) == 'A') treeCount++;
        }
    }
    setTreesNumber(mptr, treeCount);
}

/**
 * Function: markUncertainCells
 * 
 * Description: marks in map cells that might support tents i.e. orthogonal to tree and not in a zero row/collumn
 * 
 * Side-effects: writes uncertains in mptr
 * 
 * Arguments:
 *     map *mptr - map pointer
 * 
 * Return value:
 *     1 - if map might have solution
 *     0 - if map is impossible
 */
int markUncertainCells(map *mptr) {
    int isolatedTree;
    char c;
    for (int i = 0; i < getMapLines(mptr); i++) {
        for (int j = 0; j < getMapColumns(mptr); j++) {
            if (getContentOfPosition(mptr, i, j) == 'A') {
                isolatedTree = 1;
                for (int k = 0; k < 4; k++) {
                    if (i + orthogonals[k].dx < 0 || i + orthogonals[k].dx >= getMapLines(mptr)) continue;
                    if (j + orthogonals[k].dy < 0 || j + orthogonals[k].dy >= getMapColumns(mptr)) continue;
                    if (((c = getContentOfPosition(mptr, i + orthogonals[k].dx, j + orthogonals[k].dy)) != 'A') && getTentsInLine(mptr, i + orthogonals[k].dx) && getTentsInColumn(mptr, j + orthogonals[k].dy)) {
                        if (c != 'U') {
                            setContentOfPosition(mptr, i + orthogonals[k].dx, j + orthogonals[k].dy, 'U');
                            incrementUncertainCount(mptr);
                        }
                        isolatedTree = 0;
                    }
                }
                if (isolatedTree && getTreesNumber(mptr) == getTentsNumber(mptr)) return 0;
            }
        }
    }
    if (getUncertainCount(mptr) < getTreesNumber(mptr) && getTreesNumber(mptr) == getTentsNumber(mptr)) return 0;
    return 1;
}

/**
 * Function: buildUncertainAndTreeArray
 * 
 * Description: writes arrays with all uncertain cells and tree cells
 * 
 * Arguments:
 *     map *mptr - map pointer
 *     cell **uncertainArray - returns uncertain array
 *     cell **treeArray - returns tree array
 * 
 * Return value: none
 */
void buildUncertainAndTreeArray(map *mptr, cell **uncertainArray, cell **treeArray) {
    int u = 0, t = 0;

    *uncertainArray = (cell *) malloc(getUncertainCount(mptr) * sizeof(cell));
    if (*uncertainArray == NULL) exit(EXIT_FAILURE);
    *treeArray = (cell *) malloc(getTreesNumber(mptr) * sizeof(cell));
    if (*treeArray == NULL) exit(EXIT_FAILURE);

    for (int i = 0; i < getMapLines(mptr); i++) {
        for (int j = 0; j < getMapColumns(mptr); j++) {
            if (getContentOfPosition(mptr, i, j) == 'U') {
                (*uncertainArray)[u].line = i;
                (*uncertainArray)[u].column = j;
                u++;
            } else if (getContentOfPosition(mptr, i, j) == 'A') {
                (*treeArray)[t].line = i;
                (*treeArray)[t].column = j;
                t++;
            }
        }
    }
}

/**
 * Function: checkHintsConsistency
 * 
 * Description: checks that all tents in row/column requirements are possible
 * 
 * 
 * Arguments:
 *     map *mptr - map pointer
 * 
 * Return value:
 *     1 - if map might have solution
 *     0 - if map is impossible
 */
int checkHintsConsistency(map *mptr) {
    int uncertainCount;

    for (int i = 0; i < getMapLines(mptr); i++) {
        uncertainCount = 0;
        for (int j = 0; j < getMapColumns(mptr); j++) {
            if (getContentOfPosition(mptr, i, j) == 'U') uncertainCount++;
        }
        if (uncertainCount < getTentsInLine(mptr, i)) return 0;
    }

    for (int i = 0; i < getMapColumns(mptr); i++) {
        uncertainCount = 0;
        for (int j = 0; j < getMapLines(mptr); j++) {
            if (getContentOfPosition(mptr, j, i) == 'U') uncertainCount++;
        }
        if (uncertainCount < getTentsInColumn(mptr, i)) return 0;
    }

    return 1;
}

/**
 * Function: backtrackingSolve
 * 
 * Description: recursively try to solve map using backtracking
 * 
 * Side-effects: writes solution to mptr
 * 
 * Arguments:
 *     map *mptr - map pointer
 *     cell *uncertainArray - array with cells that might support a tent
 *     cell *treeArray - array with all trees
 *     cell *links - array with linked tent coordinates for every tree (should be init to -1)
 *     char *visited - array with boolean values for visited trees
 *     int current - current position of uncertainArray that backtracking is working (should be called with 0)
 * 
 * Return value:
 *     1 - if map is possible
 *     0 - if map is impossible
 */
int backtrackingSolve(map *mptr, cell *uncertainArray, cell *treeArray, cell *links, char *visited, int current) {
    int line, column;

    if (current == getUncertainCount(mptr)) return 1;

    line = uncertainArray[current].line;
    column = uncertainArray[current].column;

    setContentOfPosition(mptr, line, column, 'T');
    if (validTent(mptr, uncertainArray[current], treeArray, links, visited)) {
        if (backtrackingSolve(mptr, uncertainArray, treeArray, links, visited, current + 1)) return 1;
    }
    setContentOfPosition(mptr, line, column, '.');
    if (validGrass(mptr, uncertainArray[current])) {
        if (backtrackingSolve(mptr, uncertainArray, treeArray, links, visited, current + 1)) return 1;
    }
    setContentOfPosition(mptr, line, column, 'U');
    return 0;
}

/**
 * Function: validTent
 * 
 * Description: checks if Tent is valid
 * 
 * Arguments:
 *     map *mptr - map pointer
 *     cell Cell - tent to be validated
 *     cell *treeArray - array with all trees
 *     cell *links - array with linked tent coordinates for every tree (should be init to -1)
 *     char *visited - array with boolean values for visited trees
 * 
 * Return value:
 *     1 - if tent is valid
 *     0 - if tent is invalid
 */
int validTent(map *mptr, cell Cell, cell *treeArray, cell *links, char *visited) {
    int tentSum;

    for (int i = 0; i < 8; i++) {
        if (getContentOfPosition(mptr, Cell.line + adjacents[i].dx, Cell.column + adjacents[i].dy) == 'T')
            return 0;
    }

    tentSum = 0;
    for (int i = 0; i < getMapColumns(mptr); i++) {
        if (getContentOfPosition(mptr, Cell.line, i) == 'T') tentSum++;
    }
    if (tentSum > getTentsInLine(mptr, Cell.line)) return 0;

    tentSum = 0;
    for (int i = 0; i < getMapLines(mptr); i++) {
        if (getContentOfPosition(mptr, i, Cell.column) == 'T') tentSum++;
    }
    if (tentSum > getTentsInColumn(mptr, Cell.column)) return 0;

    memset(visited, 0, getTreesNumber(mptr));
    if (!localInjectivity(mptr, Cell, treeArray, links, visited)) return 0;

    return 1;
}

/**
 * Function: validGrass
 * 
 * Description: checks if grass is valid
 * 
 * Arguments:
 *     map *mptr - map pointer
 *     cell Cell - tent to be validated
 * 
 * Return value:
 *     1 - if tent is valid
 *     0 - if tent is invalid
 */
int validGrass(map *mptr, cell Cell) {
    int tentSum, uncertainSum;
    char c;
    int isolated;

    /** When in high season check if theres an isolated tree */
    if (getTentsNumber(mptr) == getTreesNumber(mptr)) {
        for (int i = 0; i < 4; i++) {
            if (getContentOfPosition(mptr, Cell.line + orthogonals[i].dx, Cell.column + orthogonals[i].dy) == 'A') {
                isolated = 1;
                for (int j = 0; j < 4; j++) {
                    if ((c = getContentOfPosition(mptr, Cell.line + orthogonals[i].dx + orthogonals[j].dx, Cell.column + orthogonals[i].dy + orthogonals[j].dy)) == 'T' || c == 'U') {
                        isolated = 0;
                        break;
                    }
                }
                if (isolated) return 0;
            }
        }
    }

    tentSum = 0;
    uncertainSum = 0;
    for (int i = 0; i < getMapColumns(mptr); i++) {
        if (getContentOfPosition(mptr, Cell.line, i) == 'T')
            tentSum++;
        else if (getContentOfPosition(mptr, Cell.line, i) == 'U')
            uncertainSum++;
    }
    if (uncertainSum < getTentsInLine(mptr, Cell.line) - tentSum) return 0;

    tentSum = 0;
    uncertainSum = 0;
    for (int i = 0; i < getMapLines(mptr); i++) {
        if (getContentOfPosition(mptr, i, Cell.column) == 'T')
            tentSum++;
        else if (getContentOfPosition(mptr, i, Cell.column) == 'U')
            uncertainSum++;
    }
    if (uncertainSum < getTentsInColumn(mptr, Cell.column) - tentSum) return 0;

    return 1;
}

/**
 * Function: localInjectivity
 * 
 * Description: checks tent-tree injectivity i.e. theres a unique tree for a tent (locally)
 * 
 * Arguments:
 *     map *mptr - map pointer
 *     cell tent - tent to be validated
 *     cell *treeArray - array with all trees
 *     cell *links - array with linked tent coordinates for every tree (should be init to -1)
 *     char *visited - array with boolean values for visited trees
 * 
 * Return value:
 *     1 - if tent is valid
 *     0 - if tent is invalid
 */
int localInjectivity(map *mptr, cell tent, cell *treeArray, cell *links, char *visited) {
    int adjacent;
    char c;
    for (int i = 0; i < getTreesNumber(mptr); i++) {
        adjacent = 0;
        for (int k = 0; k < 4; k++) {
            if (treeArray[i].line + orthogonals[k].dx == tent.line && treeArray[i].column + orthogonals[k].dy == tent.column) adjacent = 1;
        }
        if (!adjacent) continue;
        if (!visited[i]) {
            visited[i] = 1;

            if (links[i].line == tent.line && links[i].column == tent.column) return 1;

            if (links[i].line == -1 || (c = getContentOfPosition(mptr, links[i].line, links[i].column)) == 'U' || c == '.' || localInjectivity(mptr, links[i], treeArray, links, visited)) {
                links[i] = tent;
                return 1;
            }
        }
    }
    return 0;
}