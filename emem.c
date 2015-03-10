#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "emem.h"

/*  The type for a linked list node storing eles.  */
typedef struct ll_enode {
    ele_t *ele;
    struct ll_enode *next;
} ll_enode_t;

/*  The number of items in the list.  */
int num_ele = 0;

/*  A pointer to the first node in the list.  */
ll_enode_t *ehead = NULL;

////////////////////////////////////////////////////////////////////////////////
//  All three functions get(), insert(), delete() need to perform the same    //
//  task: move from node to node in the list, for a given number of steps     //
//  (to find the node at a particular index, or the one just before).         //
//  Instead of duplicating the code, we write a helper function to carry out  //
//  this task.                                                                //
////////////////////////////////////////////////////////////////////////////////

/* FUNCTION getElenode
 * Return a pointer to the node at index i in the list.
 * Parameters and preconditions:
 *     i: index of retrieval, -1 <= i < size()
 * Return value:
 *     The node at index i, or NULL if i == -1.
 */
ll_enode_t *getElenode(int i)
{
    // Use a temporary pointer to traverse the list, starting at ehead.
    ll_enode_t *current = ehead;
    // Special case: there is no node at index -1.
    if (i == -1)  return NULL;
    // Change the pointer to point to the next node, i times.
    while (i-- > 0)  current = current->next;
    // Return pointer.
    return current;
}


/* FUNCTION sizeEle -- see eheader file for full description */
int sizeEle(void)
{
    return num_ele;
}


/* FUNCTION get -- see eheader file for full description */
ele_t *getEle(int i)
{
    // The helper function does this for us -- all we need is to extract the
    // value stored in the node.
    return getElenode(i)->ele;
}


/* FUNCTION insert -- see eheader file for full description */
void insertEle(int i, uint *data_arr)
{
    // Call getElenode() to find the insertion point.
    ll_enode_t *previous = getElenode(i-1);
    // Create a new node and make sure memory allocation was successful.
    ll_enode_t *new_node = malloc(sizeof(ll_enode_t));
    if (new_node == NULL) {
        fputs("ERROR: Unable to allocate memory!\n", stderr);
        exit(1);
    }

    ele_t *new_ele = (ele_t *) malloc(sizeof(ele_t));
    if (new_ele == NULL) {
        fputs("ERROR: Unable to allocate memory!\n", stderr);
        exit(1);
    }

    new_ele->indx = data_arr[0];
    new_ele->tet[0] = data_arr[1];
    new_ele->tet[1] = data_arr[2];
    new_ele->tet[2] = data_arr[3];
    new_ele->tet[3] = data_arr[4];
    new_ele->rid  = 0;
     
    // Splice the new node into the list.
    new_node->ele = new_ele;
    if (previous == NULL) { // special case: inserting at index 0
        new_node->next = ehead;
        ehead = new_node;
    } else {                // general case: inserting further in the list
        new_node->next = previous->next;
        previous->next = new_node;
    }
    
    // Adjust the size of the list!
    ++num_ele;
}


/* FUNCTION deleteEle -- see eeheader file for full description */
void deleteEle(int i)
{
    // Call getElenode() to find the deletion point.
    ll_enode_t *previous = getElenode(i-1);
    ll_enode_t *old_node; // needed to free memory, since we got rid of 'current'
    
    // Update pointers to bypass 'current'.
    if (previous == NULL) { // special case: removing from index 0
        old_node = ehead;
        ehead = old_node->next;
    } else {                // general case: removing further in the list
        old_node = previous->next;
        previous->next = old_node->next;
    }
    
    // Free the memory for the bypassed node.
    free(old_node);
    
    // Adjust the size of the list!
    --num_ele;
}


/* FUNCTION emptyEle -- see eheader file for full description */
void emptyEle(void)
{
    // Traverse the entire list to free the memory for each node -- use ehead
    // directly to keep track of the next node to free (since ehead has to be
    // reset anyway), and one extra pointer to the node to free at each
    // iteration.  Update the number of items at each iteration, so the global
    // variables will have the correct values for an empty list once the loop
    // finishes.
    ll_enode_t *old;
    while (ehead != NULL) {
        old = ehead;
        ehead = ehead->next;
        free(old->ele);
        free(old);
        --num_ele;
    }
}


void printEle(void)
{
    printf("%d 4 0 1\n", sizeEle());

    // Use a temporary pointer to traverse the list, starting at ehead.
    ll_enode_t *current = ehead;

    while (current != NULL) {
        printf("%u %u %u %u %u %u\n", current->ele->indx, 
                                      current->ele->tet[0],
                                      current->ele->tet[1],
                                      current->ele->tet[2],
                                      current->ele->tet[3],
                                      current->ele->rid);
        current = current->next;
    }

}

void dumpEle(FILE *fp)
{
    // Print first line
    if (fprintf(fp, "%d 4 0 1\n", sizeEle()) == -1) {
        fputs("ERROR: Unable to write line.\n", stderr);
    };

    // Use a temporary pointer to traverse the list, starting at ehead.
    ll_enode_t *current = ehead;

    while (current != NULL) {
        if (fprintf(fp, "%u %u %u %u %u\n", current->ele->indx, 
                                        current->ele->tet[0],
                                        current->ele->tet[1],
                                        current->ele->tet[2],
                                        current->ele->tet[3]) == -1) {
            fputs("ERROR: Unable to write line.\n", stderr);
        };
        current = current->next;
    }
}
