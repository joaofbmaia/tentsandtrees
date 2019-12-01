#include <stdlib.h>
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
cell *buildUncertainArray(map *mptr);
int checkHintsConsistency(map *mptr);
int backtrackingSolve(map *mptr, cell *workingArray, int current);
int validTent(map *mptr, int line, int column);
int validGrass(map *mptr, int line, int column);

int solveMap(map *mptr) {
    int possible;
    cell *uncertainArray = NULL;

    countNumberOfTrees(mptr);
    if (getTreesNumber(mptr) < getTentsNumber(mptr)) return -1;

    possible = markUncertainCels(mptr);
    if (!possible) return -1;

    uncertainArray = buildUncertainArray(mptr);

    possible = checkHintsConsistency(mptr);
    if (!possible) {
        free(uncertainArray);
        return -1;
    }

    possible = backtrackingSolve(mptr, uncertainArray, 0);
    free(uncertainArray);
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

cell *buildUncertainArray(map *mptr) {
    cell *array;
    int c;

    array = (cell *) malloc(getUncertainCount(mptr) * sizeof(cell));
    if (array == NULL) exit(EXIT_FAILURE);

    c = 0;
    for (int i = 0; i < getMapLines(mptr); i++) {
        for (int j = 0; j < getMapColumns(mptr); j++) {
            if (getContentOfPosition(mptr, i, j) == 'U') {
                array[c].line = i;
                array[c].column = j;
                c++;
            }
        }
    }
    return array;
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

int backtrackingSolve(map *mptr, cell *workingArray, int current) {
    int line, column;

    if (current == getUncertainCount(mptr)) return 1;

    line = workingArray[current].line;
    column = workingArray[current].column;

    setContentOfPosition(mptr, line, column, 'T');
    if (validTent(mptr, line, column)) {
        if (backtrackingSolve(mptr, workingArray, current + 1)) return 1;
    }
    setContentOfPosition(mptr, line, column, '.');
    if (validGrass(mptr, line, column)) {
        if (backtrackingSolve(mptr, workingArray, current + 1)) return 1;
    }
    setContentOfPosition(mptr, line, column, 'U');
    return 0;
}

int validTent(map *mptr, int line, int column) {
    int tentSum;

    for (int i = 0; i < 8; i++) {
        if (getContentOfPosition(mptr, line + adjacents[i].dx, column + adjacents[i].dy) == 'T')
            return 0;
    }

    tentSum = 0;
    for (int i = 0; i < getMapColumns(mptr); i++) {
        if (getContentOfPosition(mptr, line, i) == 'T') tentSum++;
    }
    if (tentSum > getTentsInLine(mptr, line)) return 0;

    tentSum = 0;
    for (int i = 0; i < getMapLines(mptr); i++) {
        if (getContentOfPosition(mptr, i, column) == 'T') tentSum++;
    }
    if (tentSum > getTentsInColumn(mptr, column)) return 0;

    return 1;
}

int validGrass(map *mptr, int line, int column) {
    int tentSum, uncertainSum;
    char c;
    int isolated;

    /** When in high season check if theres an isolated tree */
    if (getTentsNumber(mptr) == getTreesNumber(mptr)) {
        for (int i = 0; i < 4; i++) {
            if (getContentOfPosition(mptr, line + ortogonals[i].dx, column + ortogonals[i].dy) == 'A') {
                isolated = 1;
                for (int j = 0; j < 4; j++) {
                    if ((c = getContentOfPosition(mptr, line + ortogonals[i].dx + ortogonals[j].dx, column + ortogonals[i].dy + ortogonals[j].dy)) == 'T' || c == 'U') {
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
        if (getContentOfPosition(mptr, line, i) == 'T')
            tentSum++;
        else if (getContentOfPosition(mptr, line, i) == 'U')
            uncertainSum++;
    }
    if (uncertainSum < getTentsInLine(mptr, line) - tentSum) return 0;

    tentSum = 0;
    uncertainSum = 0;
    for (int i = 0; i < getMapLines(mptr); i++) {
        if (getContentOfPosition(mptr, i, column) == 'T')
            tentSum++;
        else if (getContentOfPosition(mptr, i, column) == 'U')
            uncertainSum++;
    }
    if (uncertainSum < getTentsInColumn(mptr, column) - tentSum) return 0;

    return 1;
}