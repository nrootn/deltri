#ifndef PMEM_H
#define PMEM_H

/* FUNCTION sizePoint
 * Return the number of items in this list.
 * No parameter.
 * Return value:
 *     The number of items currently in this list.
 */
int sizePoint(void);

/* FUNCTION insertPoint
 * Insert x at index i in this list.
 * Parameters and preconditions:
 *     i: index of insertion, 0 <= i <= size()
 *     x: value to insert
 * No return value.
 */
void insertPoint(int, double *);

/* FUNCTION deletePoint
 * (Name changed from 'remove' to avoid conflict with function in stdio.)
 * Remove the item at index i from this list.
 * Parameters and preconditions:
 *     i: index of deletion, 0 <= i < size()
 * No return value.
 */
void deletePoint(int);

/* FUNCTION getPoint
 * Return the item at index i in this list.
 * Parameters and preconditions:
 *     i: index of retrieval, 0 <= i < size()
 * Return value:
 *     The item at index i.
 */
point_t *getPoint(int);

void emptyPoint(void);
void printPoint(void);
void dumpPoint(FILE *);

#endif//PMEM_H
