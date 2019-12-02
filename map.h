/**
 * Filename: map.h
 * 
 * Description: functions for interactions with data type map
 */

#ifndef MAP_H
#define MAP_H

typedef struct mapStruct map;

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
map *newMap(int lines, int columns);

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
void deleteMap(map *mptr);

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
int getMapLines(map *mptr);

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
int getMapColumns(map *mptr);

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
void setContentOfPosition(map *mptr, int line, int column, char val);

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
char getContentOfPosition(map *mptr, int line, int column);

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
void setMapLine(map *mptr, int line, char *lineString);

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
char *getMapLine(map *mptr, int line);

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
void setTentsInfo(map *mptr, int tentsInLine[], int tentsInColumn[]);

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
int getTentsInLine(map *mptr, int line);

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
int getTentsInColumn(map *mptr, int column);

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
int getTentsNumber(map *mptr);

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
void setTentsNumber(map *mptr, int tentsNumber);

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
int getTreesNumber(map *mptr);

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
void setTreesNumber(map *mptr, int treesNumber);

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
int getUncertainCount(map *mptr);

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
void incrementUncertainCount(map *mptr);

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
void decrementUncertainCount(map *mptr);

#endif