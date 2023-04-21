// Implementation of the Set ADT using a balanced BST
// COMP2521 22T2 Assignment 1
//z5280731

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Set.h"
#include "SetStructs.h"

////////////////////////////////////////////////////////////////////////
// Basic Operations

/**
 * Creates a new empty set
 */
Set SetNew(void) {
	
	Set setNew = (Set)malloc(sizeof(struct set));
	setNew->tree = NULL;

	return setNew;
}

// helper function to create subtree rooted at this specific node
static void freeNodes(Node new) {
	if (new == NULL) {
		return;
	}

	free(new->right);
	free(new->left);
}

/**
 * Frees all memory associated with the given set
 */
void SetFree(Set s) {
	if (s == NULL) {
		return;
	}
	freeNodes(s->tree);
	free(s);

}


//creates a new node
static Node newNode(int item) {
	Node n = (Node)malloc(sizeof(struct node));
	n->item = item;
	n->right= NULL;
	n->left= NULL;
	n->height = 1; // set height to 1 (allocation of first node)
	return n;
}
//calculates height of the BST (similar naming structure to s1 and s2)
static int checkMaxHeight(Node n1, Node n2) {
	if (n1 == NULL && n2 == NULL) {
		return 0;
	}
	if (n1 == NULL) {
		return n2->height;
	} 
	if (n2 == NULL) {
		return n1->height;
	} 
	
	if (n1->height < n2->height) {
		return n2->height;
	} 
	return n1->height; //added to struct as a new element (SetStrucs.h)
}

static Node rightRotate(Node new) {
	return new;
}

static Node leftRotate(Node new) {
	return new;
}
// node rotation on left (done to reduce code line - tutor reccomenewation)
static Node leftHeightGreater(Node new, bool addLeft) {
	if (!addLeft) {
		new->left = leftRotate(new->left);
	}

	return rightRotate(new);
}
//node rotation on right (done to reduce code line - tutor reccomenewation)
static Node rightHeightGreater(Node new, bool addLeft) {
	if (addLeft) {
		new->right = rightRotate(new->right);
	}

	return leftRotate(new);
}
//rebalances node to create balanced BST -> ONLY IF NEEDED TO BE BALANCED
static Node nodeRebalance(Node new, int item) {
	int left = 0;
	int right = 0;
	if (new->left != NULL) {
		left = new->left->height;
	}
	if (new->right != NULL) {
		right = new->right->height;
	}

	int difference = left - right;

	// convention when there is no need for balancing
	if (difference <= 1 && difference >= -1) {
		return new;
	}

	if (difference > 1) {
		bool addLeft = item < new->left->item;
		return leftHeightGreater(new, addLeft);
	} else {
		bool addLeft = item < new->right->item;
		return rightHeightGreater(new, addLeft);
	}
}
// unique item (integer) is added depenewing on location - (function created as per tutor reccomenewation with tutor assistance)
static Node itemAddition(Node new, int item) {
	if (new == NULL) {
		return newNode(item);
	}

	if (new->item == item) {
		return new;
	} else if (item < new->item) {
		new->left = itemAddition(new->left, item);
	} else {
		new->right = itemAddition(new->right, item);
	}

	new->height = 1 + checkMaxHeight(new->left, new->right);

	Node rebalanceNode = nodeRebalance(new, item);
	return rebalanceNode;
}

/**
 * Inserts an item into the set (code is minimised to reduce cluster)
 */
void SetInsert(Set s, int item) {
	if (item == UNDEFINED) {
		return;
	}
	s->tree = itemAddition(s->tree, item);
}

//helper function for SetSize - calculates size of nodes (amount present in tree + head(root))
static int sizeCalculation(Node new) {
	if (new == NULL) {
		return 0;
	}

	int l = sizeCalculation(new->left);
	int r = sizeCalculation(new->right);
	return l+r+1;
}


/**
 * Returns the number of elements in the set
 */
int SetSize(Set s) {
	if (s == NULL) {
		return 0;
	}
	
	return sizeCalculation(s->tree);
}
//helper function for SetContains - compares if given item is present within the SET tree
static int containsHelper(Node new, int item) {
	if (new == NULL) {
		return false;
	}

	if (item == new->item) {
		return true;
	}

	if (item < new->item) {
		return containsHelper(new->left, item); //implementation is similar to lecture code to check for key in tree (BST.c)
	}
	return containsHelper(new->right, item);
}

/**
 * Returns true if the set containsHelper the given item, and false otherwise
 */
bool SetContains(Set s, int item) {
	if (s == NULL) {
		return false;
	}
	return containsHelper(s->tree, item);
}

//helper function to show each node present in tree (can only be accesed via struct node)
//this was done as per tutor recommendation - to create pointer variable
static void showNode(Node new, int *p) {
	if (new == NULL) {
		return;
	}
	showNode (new->left, p);
	if(*p == 0) {
		*p = 1;
		printf("%d",new->item);
	} else {
		printf(", %d",new->item);
	}
	showNode(new->right, p);
}

/**
 * Prints the given set in the format
 * {elem1, elem2, elem3}
 */
void SetShow(Set s) {
	if (s == NULL) {
		printf("{}\n");
		return;
	}

	printf("{");
	int p = 0;
	showNode(s->tree, &p);
	printf("}\n");
}


////////////////////////////////////////////////////////////////////////
// Further Operations

//helper function for SetUnion - copies** nodes to new (new)
static Node copyNodes(Node n) {
	if (n == NULL) {
		return NULL;
	}
	Node new = newNode(n->item);
	new->right = copyNodes(n->right);
	new->left = copyNodes(n->left);
	
	return new;
}

//helper function for SetUnion  - proccess & compilation format of node (n)
//insertion function from earlier is also called here to insert new item given
static void unionHelp(Set s , Node n) {
	if (n == NULL) {
		return;
	}
	unionHelp(s, n->left);
	unionHelp(s, n->right);
	SetInsert(s, n->item);
	
}

/**
 * Returns a new set representing the union of the two sets
 */
Set SetUnion(Set s1, Set s2) {
	Set s = SetNew();
	s->tree = copyNodes(s1->tree);
	unionHelp(s, s2->tree);
	return s;
}
//helper function for SetIntersection - calls on set contains, insert functions from earlier as aid
//two sets are utilised to add nodes and to check for presence. (increases time complexity)
//this can be reduced further - according to tutor
static void intersectHelper(Set toAdd, Set toCheck , Node n) {
	if (n == NULL) {
		return;
	}
	intersectHelper(toAdd, toCheck, n->left);
	if (SetContains(toCheck, n->item)) {
		SetInsert(toAdd, n->item);

	}
	intersectHelper(toAdd, toCheck, n->right);
}

/**
 * Returns a new set representing the intersection of the two sets
 */
Set SetIntersection(Set s1, Set s2) {
	Set s = SetNew();
	intersectHelper(s, s1, s2->tree); //call on above function
	return s;
}
//helper function for SetDifference - same methodology used here as intersectHelper function
//only change is a check method for true/false is implemented
static void differenceHelp(Set toAdd, Set toCheck , Node n) {
	if (n == NULL) {
		return;
	}
	differenceHelp(toAdd,toCheck, n->left);
	if (SetContains(toCheck, n->item) == false) {
		SetInsert(toAdd, n->item);

	}
	differenceHelp(toAdd,toCheck, n->right);
}

/**
 * Returns a new set representing the set differenceerence s1 - s2
 */
Set SetDifference(Set s1, Set s2) {
	Set s = SetNew();
	differenceHelp(s,s2, s1->tree); //call on above function
	return s;
}

//helper function for SetEquals - boolean operation is used
//set contains is function is also called to check for item similar
// 
static bool equalsHelper(Set toCheck, Node n) {
	if (n == NULL) {
		return true;
	}

	if (SetContains(toCheck, n->item) == false) {
		return false;
	}
	
	return equalsHelper(toCheck, n->left) && equalsHelper(toCheck, n->right);
}

/**
 * Returns true if the two sets are equal, and false otherwise
 */
bool SetEquals(Set s1, Set s2) {
	if (s1 == NULL && s2 == NULL) {
		return true;
	}
	if (s1 == NULL || s2 == NULL) {
		return false;
	}
	int size1 = SetSize(s1);
	int size2 = SetSize(s2);
	if (size1 != size2) {
		return false;
	}
	return equalsHelper(s1, s2->tree);
}

//helper function for SetSubset - similar structure to equalsHelper
//calls upon Setcontains function to check for similar
static bool subsetHelper(Set toCheck , Node n) {
	if (n == NULL) {
		return true;
	}
	if (SetContains(toCheck, n->item) == false) {
		return false;

	}
	return subsetHelper(toCheck, n->left) && subsetHelper(toCheck, n->right);
}

/**
 * Returns true if set s1 is a subset of set s2, anew false otherwise
 */
bool SetSubset(Set s1, Set s2) {
	
	return subsetHelper(s2,s1->tree);
}
//helper function for SetFloor (node is named 'n' as per style guideline)
//finds the largest item (in node) that is less than or equal to the given data (int item)
static int floorHelper(Node n, int item) {
	if (n == NULL) {
		return UNDEFINED; //undefined used here as per Set.h
	}
	if (item == n->item) { //same item comparison made
		return item;
	}

	if (item < n->item) 
	{
		return floorHelper(n->left, item);
	} 
	else  
	{
		if (n->right != NULL && n->right->item <= item) {
			return floorHelper(n->right, item);
		} else if (n->right != NULL) {
			int floorValue = n->item;
			Node temp = n->right->left; //node 
			Node newNodeFound = NULL; //node
			while(temp != NULL) { //while loop implemented - can be reduced/simplified with lines (time constraint)
				if (temp->item < item) {
					newNodeFound = temp;
					break;
				}
				temp = temp->left;
			}
			if (newNodeFound != NULL) {
				return floorHelper(newNodeFound, item);
			} else {
				return floorValue;
			}

		} else {
			return n->item;
		}
	}

}
/**
 * finds the floor of a given item in a set
 */
int SetFloor(Set s, int item) {
	
	return floorHelper(s->tree,item); //calls upon above function as aid
}


//helper function for Setceiling 
//finds smallest element in set that is greater than or equal to data (int item)
//this is a similar operation to the floor function (excpt. greater than or equal (>=) item is key)
static int ceilingHelp(Node n, int item) {
	if (n == NULL) {
		return UNDEFINED;
	}
	if (item == n->item) {
		return item;
	}

	if (item > n->item) 
	{
		return ceilingHelp(n->right, item);
	} 
	else  
	{
		if (n->left != NULL && n->left->item >= item) {
			return ceilingHelp(n->left, item);
		} else if (n->right != NULL) {
			int floorValue = n->item;
			Node temp = n->left->right;
			Node newNodeFound = NULL;
			while(temp != NULL) {
				if (temp->item > item) {
					newNodeFound = temp;
					break;
				}
				temp = temp->right;
			}
			if (newNodeFound != NULL) {
				return ceilingHelp(newNodeFound, item);
			} else {
				return floorValue;
			}

		} else {
			return n->item;
		}
	}


}

/** 
 * Returns the ceiling of a given item in a set
 */
int SetCeiling(Set s, int item) {
	return ceilingHelp(s->tree, item);
}

////////////////////////////////////////////////////////////////////////
// Cursor Operations
//helper function for SetCursorNew
//creates a new cursor for the set in relation and similarly it is positioned at the smallest
//element of the given set (i.e. {1,3,4,5,56} pointing to '1')
static void SetCursorNewhelper(CursorNode* root, CursorNode* previous, Node n) {
	if (n == NULL) {
		return;
	}

	SetCursorNewhelper(root, previous, n->left);

	CursorNode cursor_node = (CursorNode)malloc(sizeof(struct cursorNode));
	cursor_node->point = n;
	cursor_node->next = NULL;

	if (*root == NULL) {
		*root = cursor_node;
	}

	if (*previous != NULL) {
		(*previous)->next = cursor_node;
	}

	*previous = cursor_node;

	SetCursorNewhelper(root, previous, n->right);
}

/**
 * Creates a new cursor positioned at the smallest element of the set
 */
SetCursor SetCursorNew(Set s) {
	SetCursor cursor_node = (SetCursor)malloc(sizeof(struct cursor));
	CursorNode root = NULL;
	CursorNode previous = NULL;

	SetCursorNewhelper(&root, &previous, s->tree); //call on above function

	cursor_node->next = root;
	return cursor_node;
}

/**
 * Frees all memory associated with the given cursor
 */
void SetCursorFree(SetCursor cur) {
	CursorNode current = cur->next; 
	while (current != NULL) {
		
		CursorNode temp = current->next;
		free(current);
		current = temp;
	}

	free(cur);
}

/**
 * Returns the element at the cursor's currentent position, anew then moves
 * the cursor to the next greatest element. If there is no next greatest
 * element, then all subsequent calls to SetCursorNext on this cursor
 * should return UNDEFINED.
 */
int SetCursorNext(SetCursor cur) {

	if (cur == NULL || cur->next == NULL) {		
		return UNDEFINED;
	}
	int data = cur->next->point->item;
	cur->next = cur->next->next;
	return data;

}



