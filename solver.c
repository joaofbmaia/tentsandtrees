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
} ortogonals[] = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};

typedef struct {
    int line;
    int column;
} cell;

void countNumberOfTrees(map *mptr);
int markUncertainCels(map *mptr);
void buildUncertainAndTreeArray(map *mptr, cell **uncertainArray, cell **treeArray);
int checkHintsConsistency(map *mptr);
int backtrackingSolve(map *mptr, cell *uncertainArray, cell *treeArray, cell *links, char *visited, int current);
int validTent(map *mptr, cell Cell, cell *treeArray, cell *links, char *visited);
int validGrass(map *mptr, cell Cell);
int localInjectivity(map *mptr, cell tent, cell *treeArray, cell *links, char *visited);

int solveMap(map *mptr) {
    int possible;
    cell *uncertainArray = NULL;
    cell *treeArray = NULL;
    cell *links = NULL;
    char *visited = NULL;

    countNumberOfTrees(mptr);
    if (getTreesNumber(mptr) < getTentsNumber(mptr)) return -1;

    possible = markUncertainCels(mptr);
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

void countNumberOfTrees(map *mptr) {
    int treeCount = 0;
    for (int i = 0; i < getMapLines(mptr); i++) {
        for (int j = 0; j < getMapColumns(mptr); j++) {
            if (getContentOfPosition(mptr, i, j) == 'A') treeCount++;
        }
    }
    setTreesNumber(mptr, treeCount);
}

int markUncertainCels(map *mptr) {
    int isolatedTree;
    char c;
    for (int i = 0; i < getMapLines(mptr); i++) {
        for (int j = 0; j < getMapColumns(mptr); j++) {
            if (getContentOfPosition(mptr, i, j) == 'A') {
                isolatedTree = 1;
                for (int k = 0; k < 4; k++) {
                    if (i + ortogonals[k].dx < 0 || i + ortogonals[k].dx >= getMapLines(mptr)) continue;
                    if (j + ortogonals[k].dy < 0 || j + ortogonals[k].dy >= getMapColumns(mptr)) continue;
                    if (((c = getContentOfPosition(mptr, i + ortogonals[k].dx, j + ortogonals[k].dy)) != 'A') && getTentsInLine(mptr, i + ortogonals[k].dx) && getTentsInColumn(mptr, j + ortogonals[k].dy)) {
                        if (c != 'U') {
                            setContentOfPosition(mptr, i + ortogonals[k].dx, j + ortogonals[k].dy, 'U');
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

int validGrass(map *mptr, cell Cell) {
    int tentSum, uncertainSum;
    char c;
    int isolated;

    /** When in high season check if theres an isolated tree */
    if (getTentsNumber(mptr) == getTreesNumber(mptr)) {
        for (int i = 0; i < 4; i++) {
            if (getContentOfPosition(mptr, Cell.line + ortogonals[i].dx, Cell.column + ortogonals[i].dy) == 'A') {
                isolated = 1;
                for (int j = 0; j < 4; j++) {
                    if ((c = getContentOfPosition(mptr, Cell.line + ortogonals[i].dx + ortogonals[j].dx, Cell.column + ortogonals[i].dy + ortogonals[j].dy)) == 'T' || c == 'U') {
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

int localInjectivity(map *mptr, cell tent, cell *treeArray, cell *links, char *visited) {
    int adjacent;
    char c;
    for (int i = 0; i < getTreesNumber(mptr); i++) {
        adjacent = 0;
        for (int k = 0; k < 4; k++) {
            if (treeArray[i].line + ortogonals[k].dx == tent.line && treeArray[i].column + ortogonals[k].dy == tent.column) adjacent = 1;
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