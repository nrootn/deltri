#ifndef EMEM_H
#define EMEM_H

/* FUNCTION sizeEle
 * Return the number of items in this list.
 * No parameter.
 * Return value:
 *     The number of items currently in this list.
 */
int sizeEle(void);

/* FUNCTION insertEle
 * Insert x at index i in this list.
 * Parameters and preconditions:
 *     i: index of insertion, 0 <= i <= size()
 *     x: value to insert
 * No return value.
 */
void insertEle(int, uint *);

/* FUNCTION deleteEle
 * (Name changed from 'remove' to avoid conflict with function in stdio.)
 * Remove the item at index i from this list.
 * Parameters and preconditions:
 *     i: index of deletion, 0 <= i < size()
 * No return value.
 */
void deleteEle(int);

/* FUNCTION getEle
 * Return the item at index i in this list.
 * Parameters and preconditions:
 *     i: index of retrieval, 0 <= i < size()
 * Return value:
 *     The item at index i.
 */
ele_t *getEle(int);

void emptyEle(void);
void printEle(void);
void dumpEle(FILE *);

#endif//EMEM_H
