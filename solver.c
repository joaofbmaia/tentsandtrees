#include "map.h"

struct {
    int dx;
    int dy;
} ortogonals[] = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};

int markUncertainCels(map *mptr);
int checkAndMarkOrtogonals(map *mptr, int line, int column);

int solveMap(map *mptr) {
    markUncertainCels(mptr);
    return 1;
}

void markUncertainCels(map *mptr) {
    for (int i = 0; i < getMapLines(mptr); i++) {
        for (int j = 0; j < getMapColumns(mptr); j++) {
            if (getContentOfPosition(mptr, i, j) == 'A') {
                checkAndMarkOrtogonals(mptr, i, j);
            }
        }
    }
}

void checkAndMarkOrtogonals(map *mptr, int line, int column) {
    for (int i = 0; i < 4; i++) {
        if ((getContentOfPosition(mptr, line + ortogonals[i].dx, column + ortogonals[i].dy) != 'A') && getTentsInLine(mptr, line) && getTentsInColumn(mptr, column)) {
            setContentOfPosition(mptr, line + ortogonals[i].dx, column + ortogonals[i].dy, 'U');
        }
    }
}
