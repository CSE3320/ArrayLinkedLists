/** LinkedListShifted.c
*
* This code implements a sorted linked list using an array as the underlying data structure.
* The underlying implementation is to be hidden from the end user.  They should interact
* with the linked list using the insertNode(int value) and removeNode(int value) functions.
* The elements in the array are not sorted from element 0 to the end of the array and should
* not be used in that manner.  The internal previous and next elements of the nodes are used
* to traverse the linked list in order.
* 
*/

#include <stdio.h>

/* The maximum entries in our linked list / array */
#define MAX_LINKED_LIST_SIZE 10 

/* *** INTERNAL USE ONLY *** In an in-line implementation the root node 
 * is always 0
 */
#define ROOTNODE 0

/* *** INTERNAL USE ONLY *** Has the list been initialized */
static int initialized = 0;   

/* *** INTERNAL USE ONLY *** Keep track of the last item placed
* so we can optimize the shifting slightly and only shift what is needed.
*/
static int lastUsed = -1;


/**
*
* \struct Node
* 
* \brief The node in the linked list
* 
* This structure represents the node in our linked list implementation.
* Since this linked list is implemented in an array the previous and next
* members, which would be pointers in a dynamically allocated linked list,
* are integers in this implementation.  Their values correspond to the array
* index where the previous and next nodes reside.  
* 
* The value in_use is to let us track which array entries are currently used.
* If you are re-using this code in a heap or arena assignment this value does
* NOT represent whether your heap block is free or in-use.  You will need to add
* a new member to this struct to track that.
* 
* The value element is an integer to demonstrate the functionality of a sorted list.
* If you are reusing this code you will need to change that datatype to be the
* actual data type in your implementation, e.g you're creating a linked list of strings
* then you would change the dataype of value from an int to a char*.
* 
*/

struct Node
{
	/** If this array entry is being used as a node. 1 for in-use. 0 for empty */
    int  in_use;    

	/** The value that this linked list node contains */
    int  value;     
	
};

/**
* This array is the linked list we are implementing.  The linked list represented by this array
* is a sorted linked list using the "value" member to determine the sorting.  This array is
* not sorted in the same order as the list and, after multiple insertions and removals, may not
* even be close to the same order.  This is by design.  Rather than sort each element in the array
* every time a new node is added instead the next and previous links are adjusted to link 
* the linked list element in the correct spot.
* 
* This array is *** INTERNAL USE ONLY *** and should not be exposed to or used by the end user. 
* The user will interface with the linked list through insertNode() and removeNode().  From the 
* end-user perspective it's a linked list.  We hide that we have implemented the list in an array.
*/
static struct Node LinkedList[MAX_LINKED_LIST_SIZE]; 

/**
 * 
 * \fn findFreeNodeInternal()
 * 
 * \brief Find a free array entry  *** INTERNAL USE ONLY ***
 * 
 * Search the array to a free entry that we can use
 * to store a new node in the linked list.  Returns
 * the index of the free node or -1 is a node is not found.
 * This function is for  *** INTERNAL USE ONLY ***.  We do not expose
 * the user to the fact that the linked list is in the array.  
 * We're also not searching the list to find where this new node will
 * fit in the sorted list.  That is independant of this array.  The insertion
 * into the sorted spot is done in the insertNodeInternal() function after
 * the call to findFreeNodeInternal().
 * 
 * \return Array index that is free on success
 * \return -1 on failure
 */

 int findFreeNodeInternal()
{
	int i = 0;
	
	/**
	 *  Start searching the array beginning at the 0th element
	 *  and once we've found an element not in use we'll drop out 
	 *  and return the index that is free. 
	*/
	for (i = 0; i < MAX_LINKED_LIST_SIZE; i++)
	{
		if (LinkedList[i].in_use == 0)
		{
			return i;
		}
	}
	return -1;
}

/**
 *
 * \fn insertNodeInternal(int previous, int current)
 *
 * \brief Insert a new node in the list *** INTERNAL USE ONLY ***
 *
 * Insert a new node into the linked list.  This function 
 * is for *** INTERNAL USE ONLY ***.  The end user of this 
 * code should not be aware that their linked list is stored
 * in an array rather than a dynamically allocated linked list
 * with pointers.
 * 
 * End Users should call insertNode( int node ) instead
 * 
 * This function will insert the node located at "current" right
 * after the node indexed by "previous".  
 * 
 * We will shuffle every node after this spot one index along the array 
 * and make a hole.
 * 
 * 
 * \param previous  The index of the node that will be
 *                  previous to this node in the list
 *
 * 
 * \return 0 
 */
int insertNodeInternal(int previous, int value )
{
	int i = 0;

	if( initialized && previous < 0 || previous >= MAX_LINKED_LIST_SIZE )
	{
		printf( "ERROR: Tried to insert a node beyond our bounds %d\n", previous);
		return -1;
	}

    /**
     * On the first node being inserted make sure that
     * the entire list has been initialized and then
     * set the initialized flag to we don't do this initialization
     * again.  
     */
	if (initialized == 0)
	{
		int i = 0;
		for( i = ROOTNODE; i < MAX_LINKED_LIST_SIZE; i++)
		{
			LinkedList[i].in_use = 0;
		}
	    initialized = 1;
	}

	/**
	 * Shift everything down and make room for the new node. But we only
	 * need to shift from lastUsed to previous + 1
	*/
printf("lastUsed %d previous %d\n", lastUsed, previous );
    if( lastUsed == -1 )
	{
        LinkedList[i].value  = value;
		LinkedList[i].in_use = 1;
		lastUsed = i;
	}
	else
	{
		printf("Starting for loop %d %d \n", lastUsed, previous);
		for( i = 10; i > previous; i-- )
		{
			printf("Shifting value %d from %d to %d\n", LinkedList[i].value, i, i+1);
			LinkedList[i+1].value  = LinkedList[i].value;
			LinkedList[i+1].in_use = LinkedList[i].in_use;
			
			printf("Inserted into %d\n",i);
		}  
		printf("Putting %d in %d\n", LinkedList[previous].value, previous);
		LinkedList[previous+1].in_use = 1;
		LinkedList[previous+1].value = value;
		lastUsed = previous+1;
	}  

	return 0;
}

/**
 *
 * \fn removeNodeInternal(int node)
 *
 * \brief Remove a node in the list *** INTERNAL USE ONLY ***
 *
 * Remove a new node from the linked list.  This function
 * is for *** INTERNAL USE ONLY ***.  The end user of this
 * code should not be aware that their linked list is stored
 * in an array rather than a dynamically allocated linked list
 * with pointers.
 * 
 * End Users should call remove( int node ) instead.
 *
 * This function will remove the node that is specified by the parameter
 * from the linked list.  This does not remove any data.  Only the next,
 * previous, and in_use flags are updated.
 *
 * \param node The index of the node that will be
 *             removed from the linked list
 * 
 * \return 0 on success
 * \return -1 on failure
 */
int removeNodeInternal(int node)
{
	#if 0
	/**
	 * Check to make sure we haven't tried to insert a node beyond the bounds of
	 * the array.  This shouldn't ever happen.
	 */
	if (node < 0 || node >= MAX_LINKED_LIST_SIZE )
	{
		printf("ERROR: Can not remove node %d because it is out of our array bounds"
			   " of 0 ... %d\n", node, MAX_LINKED_LIST_SIZE);
		return -1;
	}

    /**
	 * And make sure that the node we've been asked to remove is actually one in use
	 */
	if (LinkedList[node].in_use == 0)
	{
		printf("ERROR: Can not remove node %d.  It is not in use\n", node);
		return -1;
	}

	/** 
	 * Mark this node as not in-use so we can reuse it if we need to allocate
	 * another node.
	 */
	LinkedList[node].in_use = 0;

    /**	
	  * If we are removing the head of the list then we also need
	  * to make sure to update the ROOTNODE variable as well so that
	  * it points to the new head of the list.
	  */
	if (node == ROOTNODE)
	{
		ROOTNODE = LinkedList[ROOTNODE].next;
	}

	/**
	 * If we have a previous node then hook up the previous nodes next value 
	 * to point to our next value. That will cause our node to be snipped out
	 * of the linked list.
	 */
	if (LinkedList[node].previous >= 0)
	{
		LinkedList[LinkedList[node].previous].next = LinkedList[node].next;
	}

    /**	
	 * Return our next and previous to default values so this node is ready 
	 * be reused
	 */
    LinkedList[node].next = -1;
	LinkedList[node].previous = -1;
#endif
	return 0;
}


/**
 *
 * \fn removeNode(int node)
 *
 * \brief Remove a node from the linked list that contains the given value
 *
 * This function will remove the node that is specified by the parameter
 * from the linked list.  This does not remove any data.  Only the next,
 * previous, and in_use flags are updated.
 *
 * \param value The value of the node that will be
 *              removed from the linked list
 *
 * \return 0 on success
 * \return -1 on failure
 */
int removeNode(int value)
{
	#if 0
	/**
	  * We start searching for the node to remove at the root of the linked list
      */
	int index = ROOTNODE;

   /**
    * Iterate over the linked list and find the node
    * containing the value we are looking to remove
    */
	while (index != -1)
	{
    /**    
	 * Once we've found the node to delete then
     * remove it.
     */
		if (LinkedList[index].value == value)
		{
			return removeNodeInternal(index);
		}
	}
	#endif
	return -1;
}


/**
 *
 * \fn insertNode(int value)
 *
 * \brief Insert a node into the linked list that contains the given value
 *
 * This function will insert a node with a value that is specified by the parameter
 * into the linked list. 
 *
 * \param value The value of the node that will be
 *              inserted into the linked list
 *
 * \return 0 on success
 * \return -1 on failure
 */
int insertNode(int value)
{
	/*  Hold the index of the node we will insert behind */
	int previous = -1;

	/* Return value 0 for success. -1 for failure. */
	int ret = -1;

	/* Loop variable */
	int i;

    /**
     * Since this list is sorted, iterate over the linked list and find which node we  would
     * fit behind with our value.  Once we have found a spot then the while loop will exit
     * and previous will have the index of the node we will insert behind.
     */
	for( i = ROOTNODE; i < MAX_LINKED_LIST_SIZE; i++ )
	{
		if( ( LinkedList[i].in_use && LinkedList[i].value > value ) ||
		    ( LinkedList[i].in_use == 0 ) )
		{
			previous = i;
			break;
		}
	}
	
	printf("*** Inserting %d in %d\n", value, previous);
    /** If we found a free node and we haven't run off the end of the list */
	if (previous >= -1)
	{
		ret = insertNodeInternal(previous, value);
	}
    else if ( previous >= MAX_LINKED_LIST_SIZE || previous < 0 )
	{
		printf("Error: Tried to insert beyond the bounds of the allocated list.\n");

	}
	return ret;
}

/**
 *
 * \fn printList()
 *
 * \brief Print the linked list
 *
 * This function iterates over the list and prints the values in list order.
 * If you need a function to iterate over the linked list in order, this is 
 * how you would do that.
 * 
 * Since the array is kept in sorted order, iterating in order over the array
 * iterates in order.
 *
 */
void printList()
{
    /** Start at the root of the linked list */
	int i = 0;
	
    /** Iterate over the linked list in node order and print the nodes. */

    for( i = ROOTNODE; i < MAX_LINKED_LIST_SIZE; i++ )
	{
		if( LinkedList[i].in_use == 0 )
		{
			/** This array is not sparse so the first entry which has a 0 value for 
			 *  in-use means we've reached the end so no need to continue the loop 
			 */
			break;
		}
		printf("LinkedList[%d]: %d\n", i, LinkedList[i].value);
	}
}

int main()
{
	insertNode(85);
	insertNode(87);

	insertNode(500);
    insertNode(100);
	printf("\n\nPrint the sorted Linked List\n\n");
	printList();
#if 0
	removeNode(84);
	
	printf("\n\nTrying to remove an array element that is out of bounds\n");
	printf("This should print two errors.\n");
	removeNodeInternal(-1);
	removeNodeInternal(80000);

	printf("\n\nUpdated Linked List with Node with value 84 deleted\n\n");
	printList();

	removeNode(500);
	insertNode(600);
	printf("\n\nUpdated Linked List with the last Node removed and replaced with a value of 600 \n\n");
	printList();

	removeNode(10);
	printf("\n\nUpdated Linked List with the first Node removed \n\n");
	printList();

	insertNode(2);
	printf("\n\nUpdated Linked List with a new root node added with a value of 2 \n\n");
	printList();
#endif
	return 0; 
}





