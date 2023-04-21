// COMP2521 22T2 Assignment 1

// DO NOT CHANGE THE NAME OF THIS STRUCT
struct node {
	int item;           // DO NOT CHANGE/REMOVE THIS FIELD
	struct node *left;  // DO NOT CHANGE/REMOVE THIS FIELD
	struct node *right; // DO NOT CHANGE/REMOVE THIS FIELD
	// height field is height of subtree from this node
	int height;
	// You may add more fields here if needed
};

// DO NOT CHANGE THE NAME OF THIS STRUCT
struct set {
	struct node *tree;  // DO NOT CHANGE/REMOVE THIS FIELD

	// You may add more fields here if needed
};


typedef struct node *Node;

// You may define more structs here if needed 

////////////////////////////////////////////////////////////////////////
// Cursors

struct cursorNode {
	// point refers to the set node it is pointing to
	struct node* point;
	// next in link list (i.e. next->next)
	struct cursorNode* next;
};

typedef struct cursorNode *CursorNode;

// Cursor is similar like a link list operation, where cursor nodes is link list node
struct cursor {
	// You may add more fields here if needed
	//next in list
	CursorNode next;
};

////////////////////////////////////////////////////////////////////////

