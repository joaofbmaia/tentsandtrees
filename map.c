/**
 * Filename: map.c
 * 
 * Description: Implementation of data type for camping map
 */

#include "map.h"
#include <stdlib.h>
#include <string.h>

struct mapStruct {
    int lines;
    int columns;
    char **map;
    int *tentsInLine;
    int *tentsInColumn;
    int tentsNumber;
    int treesNumber;
    int uncertainCount;
    int assignedTentsCount;
};

/**
 * Function: newMap
 * 
 * Description: allocates new map
 * 
 * Arguments:
 *     int lines - number of lines
 *     int columns - number of columns
 * 
 * Return value:
 *     pointer to new map if successful
 *     NULL if error ocurred
 */
map *newMap(int lines, int columns) {
    map *mptr;

    if (lines < 0 || columns < 0) return NULL;

    mptr = (map *) malloc(sizeof(map));
    if (mptr == NULL) return NULL;

    mptr->lines = lines;
    mptr->columns = columns;
    mptr->uncertainCount = 0;
    mptr->assignedTentsCount = 0;

    mptr->map = (char **) malloc(lines * sizeof(char *));
    if (mptr->map == NULL) return NULL;

    for (int i = 0; i < lines; i++) {
        mptr->map[i] = (char *) malloc(columns + 1 * sizeof(char)); /** lines are saved as strings */
        if (mptr->map[i] == NULL) return NULL;
    }

    mptr->tentsInLine = (int *) malloc(lines * sizeof(int));
    if (mptr->tentsInLine == NULL) return NULL;

    mptr->tentsInColumn = (int *) malloc(columns * sizeof(int));
    if (mptr->tentsInColumn == NULL) return NULL;

    return mptr;
}
/**
 * Function: deleteMap
 * 
 * Description: deletes a map
 * 
 * Arguments:
 *     map *mptr - pointer to map to be deleted
 * 
 * Return value: none
 */
void deleteMap(map *mptr) {
    if (mptr == NULL) return;

    for (int i = 0; i < mptr->lines; i++) {
        free(mptr->map[i]);
    }
    free(mptr->map);

    free(mptr->tentsInLine);
    free(mptr->tentsInColumn);

    free(mptr);
}

/**
 * Function: getMapLines
 * 
 * Description: gets number of lines
 * 
 * Arguments:
 *     map *mptr - pointer to map
 * 
 * Return value:
 *     number of lines
 */
int getMapLines(map *mptr) {
    return mptr->lines;
}

/**
 * Function: getMapColumns
 * 
 * Description: gets number of columns
 * 
 * Arguments:
 *     map *mptr - pointer to map
 * 
 * Return value:
 *     number of columns
 */
int getMapColumns(map *mptr) {
    return mptr->columns;
}

/**
 * Function: setContentOfPosition
 * 
 * Description: sets content of position from map
 * 
 * Arguments:
 *     map *mptr - pointer to map
 *     int line - line of coordinate
 *     int column - column of coordinate
 *     char val - value to be set
 * 
 * Return value: none
 */
void setContentOfPosition(map *mptr, int line, int column, char val) {
    mptr->map[line][column] = val;
}

/**
 * Function: getContentOfPosition
 * 
 * Description: gets content of position from map
 * 
 * Arguments:
 *     map *mptr - pointer to map
 *     int line - line of coordinate
 *     int column - column of coordinate
 * 
 * Return value:
 *     content of position with specified coordinates of map
 *     '\0' if outside map
 */
char getContentOfPosition(map *mptr, int line, int column) {
    if (line >= mptr->lines || column >= mptr->columns) return '\0';
    if (line < 0 || column < 0) return '\0';
    return mptr->map[line][column];
}


/**
 * Function: getPosition
 * 
 * Description: gets position from map
 * 
 * Arguments:
 *     map *mptr - pointer to map
 *     int line - line of coordinate
 *     int column - column of coordinate
 * 
 * Return value:
 *     pointer to content of position with specified coordinates of map
 *     NULL if outside map
 */
char *getPosition(map *mptr, int line, int column) {
    if (line >= mptr->lines || column >= mptr->columns) return NULL;
    if (line < 0 || column < 0) return NULL;
    return &mptr->map[line][column];
}

/**
 * Function: setMapLine
 * 
 * Description: sets content of entire line as a string
 * 
 * Arguments:
 *     map *mptr - pointer to map
 *     int line - line of coordinate
 *     char *lineString - string containing entire line
 * 
 * Return value: none
 */
void setMapLine(map *mptr, int line, char *lineString) {
    strcpy(mptr->map[line], lineString);
}

/**
 * Function: getMapLine
 * 
 * Description: gets content of entire line as a string
 * 
 * Arguments:
 *     map *mptr - pointer to map
 *     int line - line of coordinate
 * 
 * Return value: string containing entire line
 */
char *getMapLine(map *mptr, int line) {
    return mptr->map[line];
}

/**
 * Function: setTentsInfo
 * 
 * Description: sets hints about number of tents per line and column
 * 
 * Arguments:
 *     map *mptr - pointer to map
 *     int tentsInLine[] - array with number of tents per line
 *     int tentsInColumn[] - array with number of tents per column
 * 
 * Return value: none
 */
void setTentsInfo(map *mptr, int tentsInLine[], int tentsInColumn[]) {
    memcpy(mptr->tentsInLine, tentsInLine, mptr->lines * sizeof(int));
    memcpy(mptr->tentsInColumn, tentsInColumn, mptr->columns * sizeof(int));
}

/**
 * Function: getTentsInLine
 * 
 * Description: gets number of tents in line
 * 
 * Arguments:
 *     map *mptr - pointer to map
 *     int line - number of line to get number of tents of
 * 
 * Return value:
 *     number of tents in line "line" of map
 */
int getTentsInLine(map *mptr, int line) {
    return mptr->tentsInLine[line];
}

/**
 * Function: getTentsInColumn
 * 
 * Description: gets number of tents in column
 * 
 * Arguments:
 *     map *mptr - pointer to map
 *     int column - number of column to get number of tents of
 * 
 * Return value:
 *     number of tents in column "column" of map
 */
int getTentsInColumn(map *mptr, int column) {
    return mptr->tentsInColumn[column];
}

/**
 * Function: getTentsNumber
 * 
 * Description: gets number of tents
 * 
 * Arguments:
 *     map *mptr - pointer to map
 * 
 * Return value:
 *     number of tents
 */
int getTentsNumber(map *mptr) {
    return mptr->tentsNumber;
}

/**
 * Function: setTentsNumber
 * 
 * Description: sets number of tents
 * 
 * Arguments:
 *     map *mptr - pointer to map
 *     int tentsNumber - number of tents
 * 
 * Return value: none
 */
void setTentsNumber(map *mptr, int tentsNumber) {
    mptr->tentsNumber = tentsNumber;
}

/**
 * Function: getTreesNumber
 * 
 * Description: gets number of trees
 * 
 * Arguments:
 *     map *mptr - pointer to map
 * 
 * Return value:
 *     number of trees
 */
int getTreesNumber(map *mptr) {
    return mptr->treesNumber;
}

/**
 * Function: setTreesNumber
 * 
 * Description: sets number of trees
 * 
 * Arguments:
 *     map *mptr - pointer to map
 *     int treesNumber - number of trees
 * 
 * Return value: none
 */
void setTreesNumber(map *mptr, int treesNumber) {
    mptr->treesNumber = treesNumber;
}

/**
 * Function: getUncertainCount
 * 
 * Description: gets uncertain count
 * 
 * Arguments:
 *     map *mptr - pointer to map
 * 
 * Return value:
 *     uncertain count
 */
int getUncertainCount(map *mptr) {
    return mptr->uncertainCount;
}

/**
 * Function: incrementUncertainCount
 * 
 * Description: increments uncertain count
 * 
 * Arguments:
 *     map *mptr - pointer to map
 * 
 * Return value: none
 */
void incrementUncertainCount(map *mptr) {
    mptr->uncertainCount++;
}

/**
 * Function: decrementUncertainCount
 * 
 * Description: decrements uncertain count
 * 
 * Arguments:
 *     map *mptr - pointer to map
 * 
 * Return value: none
 */
void decrementUncertainCount(map *mptr) {
    mptr->uncertainCount--;
}

/**
 * Function: getAssignedTentsCount
 * 
 * Description: gets assigned tents count
 * 
 * Arguments:
 *     map *mptr - pointer to map
 * 
 * Return value:
 *     assigned tents count
 */
int getAssignedTentsCount(map *mptr) {
    return mptr->assignedTentsCount;
}

/**
 * Function: incrementAssignedTentsCount
 * 
 * Description: increments assigned tents count
 * 
 * Arguments:
 *     map *mptr - pointer to map
 * 
 * Return value: none
 */
void incrementAssignedTentsCount(map *mptr) {
    mptr->assignedTentsCount++;
}

/**
 * Function: decrementAssignedTentsCount
 * 
 * Description: decrements assigned tents count
 * 
 * Arguments:
 *     map *mptr - pointer to map
 * 
 * Return value: none
 */
void decrementAssignedTentsCount(map *mptr) {
    mptr->assignedTentsCount--;
}