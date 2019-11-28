#include "map.h"

struct {
    int dx;
    int dy;
} ortogonals[] = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};

int markUncertainCels(map *mptr);
int checkandMarkOrtogonals(map *mptr, int line, int column);

int solveMap(map *mptr) {
    if (markUncertainCels(mptr) == -1) {
        return -1;
    }
    return 1;
}

int markUncertainCels(map *mptr) {
    for (int i; i < getMapLines; i++) {
        for (int j = 0; j < getMapColumns; j++) {
            if (getContentOfPosition(mptr, i, j) == 'A') {
                if (checkandMarkOrtogonals(mptr, i, j) == -1) {
                    return -1;
                } else
                    continue;
            }
        }
    }
    return 1;
}

int checkandMarkOrtogonals(map *mptr, int line, int column) {
    int gangBang = 0;
    for (int i; i < 4; i++) {
        if ((getContentOfPosition(mptr, line + ortogonals[i].dx, column + ortogonals[i].dy) != 'A') && getTentsInLine(mptr, line) && getTentsInColumn(mptr, column)) {
            setContentOfPosition(mptr, line + ortogonals[i].dx, column + ortogonals[i].dy, 'U');
            gangBang++;
        }
    }

    if (gangBang == 0) {
        return -1;
    } else
        return 1;
}