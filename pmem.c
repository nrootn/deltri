#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "pmem.h"

/*  The type for a linked list node storing points.  */
typedef struct ll_pnode {
    point_t *point;
    struct ll_pnode *next;
} ll_pnode_t;

/*  The number of items in the list.  */
int num_pts = 0;

/*  A pointer to the first node in the list.  */
ll_pnode_t *phead = NULL;

////////////////////////////////////////////////////////////////////////////////
//  All three functions get(), insert(), delete() need to perform the same    //
//  task: move from node to node in the list, for a given number of steps     //
//  (to find the node at a particular index, or the one just before).         //
//  Instead of duplicating the code, we write a helper function to carry out  //
//  this task.                                                                //
////////////////////////////////////////////////////////////////////////////////

/* FUNCTION getPointnode
 * Return a pointer to the node at index i in the list.
 * Parameters and preconditions:
 *     i: index of retrieval, -1 <= i < size()
 * Return value:
 *     The node at index i, or NULL if i == -1.
 */
ll_pnode_t *getPointnode(int i)
{
    // Use a temporary pointer to traverse the list, starting at phead.
    ll_pnode_t *current = phead;
    // Special case: there is no node at index -1.
    if (i == -1)  return NULL;
    // Change the pointer to point to the next node, i times.
    while (i-- > 0)  current = current->next;
    // Return pointer.
    return current;
}


/* FUNCTION sizePoint -- see pheader file for full description */
int sizePoint(void)
{
    return num_pts;
}


/* FUNCTION get -- see pheader file for full description */
point_t *getPoint(int i)
{
    // The helper function does this for us -- all we need is to extract the
    // value stored in the node.
    return getPointnode(i)->point;
}


/* FUNCTION insert -- see pheader file for full description */
void insertPoint(int i, double *data_arr)
{
    // Call getPointnode() to find the insertion point.
    ll_pnode_t *previous = getPointnode(i-1);
    // Create a new node and make sure memory allocation was successful.
    ll_pnode_t *new_node = malloc(sizeof(ll_pnode_t));
    if (new_node == NULL) {
        fputs("ERROR: Unable to allocate memory!\n", stderr);
        exit(1);
    }

    point_t *new_pt = (point_t *) malloc(sizeof(point_t));
    if (new_pt == NULL) {
        fputs("ERROR: Unable to allocate memory!\n", stderr);
        exit(1);
    }

    new_pt->indx = (uint) data_arr[0];
    new_pt->coords[_X_] = data_arr[1];
    new_pt->coords[_Y_] = data_arr[2];
    new_pt->coords[_Z_] = data_arr[3];
    new_pt->rid  = 0;
     
    // Splice the new node into the list.
    new_node->point = new_pt;
    if (previous == NULL) { // special case: inserting at index 0
        new_node->next = phead;
        phead = new_node;
    } else {                // general case: inserting further in the list
        new_node->next = previous->next;
        previous->next = new_node;
    }
    
    // Adjust the size of the list!
    ++num_pts;
}


/* FUNCTION deletePoint -- see epheader file for full description */
void deletePoint(int i)
{
    // Call getPointnode() to find the deletion point.
    ll_pnode_t *previous = getPointnode(i-1);
    ll_pnode_t *old_node; // needed to free memory, since we got rid of 'current'
    
    // Update pointers to bypass 'current'.
    if (previous == NULL) { // special case: removing from index 0
        old_node = phead;
        phead = old_node->next;
    } else {                // general case: removing further in the list
        old_node = previous->next;
        previous->next = old_node->next;
    }
    
    // Free the memory for the bypassed node.
    free(old_node);
    
    // Adjust the size of the list!
    --num_pts;
}


/* FUNCTION emptyPoint -- see pheader file for full description */
void emptyPoint(void)
{
    // Traverse the entire list to free the memory for each node -- use phead
    // directly to keep track of the next node to free (since phead has to be
    // reset anyway), and one extra pointer to the node to free at each
    // iteration.  Update the number of items at each iteration, so the global
    // variables will have the correct values for an empty list once the loop
    // finishes.
    ll_pnode_t *old;
    while (phead != NULL) {
        old = phead;
        phead = phead->next;
        free(old->point);
        free(old);
        --num_pts;
    }
}


void printPoint(void)
{
    printf("%d 3 0 1\n", sizePoint());

    // Use a temporary pointer to traverse the list, starting at phead.
    ll_pnode_t *current = phead;

    while (current != NULL) {
        printf("%u %lf %lf %lf %u\n",  current->point->indx, 
                                    current->point->coords[_X_],
                                    current->point->coords[_Y_],
                                    current->point->coords[_Z_],
                                    current->point->rid);
        current = current->next;
    }

}

void dumpPoint(FILE *fp)
{
    // Print first line
    if (fprintf(fp, "%d 3 0 1\n", sizePoint()) == -1) {
        fputs("ERROR: Unable to write line.\n", stderr);
    };

    // Use a temporary pointer to traverse the list, starting at phead.
    ll_pnode_t *current = phead;

    while (current != NULL) {
        if (fprintf(fp, "%u %lf %lf %lf %u\n", current->point->indx, 
                                            current->point->coords[_X_],
                                            current->point->coords[_Y_],
                                            current->point->coords[_Z_],
                                            current->point->rid) == -1) {
            fputs("ERROR: Unable to write line.\n", stderr);
        };
        current = current->next;
    }
}
