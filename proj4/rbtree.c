///////////////////////////////////////////////////////////////////////////////
//                   ALL STUDENTS COMPLETE THESE SECTIONS
// Main Class File:  main.c
// File:             rbtree.c
// Semester:         CS 537 Fall 2018
//
// Author:           Harsha Kodavalla kodavalla@wisc.edu
// CS Login:         harsha
// Lecturer's Name:  Miller
// Lab Section:      303
//
//////////////////// STUDENTS WHO GET HELP FROM OTHER THAN THEIR PARTNER //////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   Introduction to Algorithms by Cormen, Leiserson, Rivest
//                   and Stein
//////////////////////////// 80 columns wide //////////////////////////////////

#include <stdbool.h> 
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "rbtree.h"
#include "537malloc.h"

static Node *root = NULL;   
static Node *NIL = NULL;    // Sentinel node


/**
 * Searches for a given key. 
 *
 * @param void *addr - The key.
 * @return - Returns the node if it is found, NULL otherwise.
 */
Node *findNode(void *addr) {

	rbtVerify(root);

    if (root == NULL) {
        return NULL;
    }
    return recurSearch(root, addr);
}

/**
 * Searches for any nodes that overlap with the given interval.
 *
 * @param void *low - Low point of the interval.
 * @param void *high - High point of the interval.
 * @return - Returns an overlapping node if found, NULL otherwise.
 */
Node *intervalSearch(void *low, void *high) {
    Node *curr = root;

	rbtVerify(root);

    if (curr == NULL) {
        return NULL;
    }
    while (1) {
        if (curr == NIL) {
            return NULL;
        }
        if (curr->low < high && curr->high > low) {
            // They overlap, search is complete
            return curr;
        }
        // If current node's left child's max is greater than the
        // interval[low], any overlap would exist in the left subtree
        if (curr->left != NIL && curr->left->max >= low) {
            curr = curr->left;
        } else {
            // If the interval[low] is larger than the left child's max
            // any overlap would exist in the right tree
            curr = curr->right;
        }
    }
    return NULL;
}

/**
 * Insert a node into the tree.
 *
 * @param void *addr - The starting address of the memory block.
 * @param void *addrHigh - The ending address of the memory block.
 * @param bool freed - If the block is a freed block.
 */
void insert(void *addr, void *addrHigh, bool freed) {

	assert(addr < addrHigh);
	
	rbtVerify(root);
	int beforeSize = rbtSize(root);
	
    if (NIL == NULL) {
        NIL = createNode(0, 0, 0, 0, 0, NULL, NULL, NULL);
    }
    // Insert new node as red
    Node *new = createNode(addr, addrHigh, freed, 1, addrHigh, NULL, NIL, NIL);
    Node *temp = NULL;
    Node *curr = root;

    // Traverse the tree until a leaf is reached
    while (curr != NULL && curr != NIL) {
        temp = curr;
        // Set maxes as the tree is traversed
        if (new->low < curr->low) {
            curr = curr->left;
        } else {
			assert(new->low >= curr->high);
			if (new->max > curr->max) {
				curr->max = new->max;
			}

            curr = curr->right;
        }
		
    }
    // Set parents and children once exiting the tree
    new->parent = temp;
    if (temp == NULL) {
        root = new;
    } else if (new->low < temp->low) {
        temp->left = new;
    } else {
        temp->right = new;
    }

    // Fix integrity of tree
    insertFixup(new);

    // Ensure tree is valid
	rbtVerify(root);
	int afterSize = rbtSize(root);
	assert(beforeSize == afterSize-1);
}

/**
 * Deletes a node from the tree. 
 *
 * @param Node *curr - The node to delete.
 * @return - Returns the node being removed.
 */	
Node *delete(Node *node) {

    // Ensure rbtree integrity before deletion
	rbtVerify(root);
	int beforeSize = rbtSize(root);
	
    // Update max's prior to deletion
    updateMax(node);

    Node *y;
    Node *x;

    if (node->left == NIL || node->right == NIL) {
        y = node;
    } else {
        y = successor(node);
    }
    if (y->left != NIL) {
        x = y->left;
    } else {
        x = y->right;
    }
    x->parent = y->parent;
    if (y == root) {
        root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
        setMax(y->parent);
    }
    if (y != node) {
        node->low = y->low;
        node->high = y->high;
        node->freed = y->freed;
		setMax(node);
		
    }
    if (!isRed(y)) {
        deleteFixup(x);
    }
	rbtVerify(root);
	int afterSize = rbtSize(root);
	if (afterSize != beforeSize-1) {
		assert(beforeSize == afterSize+1);
	}
    return y;
}

/**
 * Safely check a node's color.
 *
 * @param Node *node - The node whose color will be checked.
 * @return - Returns true if the node is red, false otherwise.
 */
static bool isRed(Node *node) {
    // If node is null or it's color is black return false
    if (node == NULL) {
        return 0;
    } else if (!(node->red)) {
        return 0;
    }    
    return 1;
}

/**
 * Returns the number of nodes in the tree.
 *
 * @param Node *node - Node whose subtree sizes will be summed
 * @return - Returns the size of the tree.
 */
static int rbtSize(Node *node) {
	if (node == NULL || node == NIL) return 0;

	return rbtSize(node->left) + rbtSize(node->right) + 1;
}

/**
 * Verifies the integrity of the rbtree by checking the black node height
 *
 * @param Node *node - Node used to check validity
 */ 
static int rbtVerify(Node *node) {
	if (node == NULL || node == NIL) return 0;
	
	if (node->left != NIL) {
		assert(node->left->low < node->low);
		assert(node->left->parent == node);
	}

	if (node->right != NIL) {
		assert(node->right->low > node->low);
		assert(node->right->low >= node->high);
		assert(node->right->parent == node);
		assert(node->right->max == node->max);
	}

	// If a node is red, its children must be black
	if (node->red) {
		assert(!node->left->red);
		assert(!node->right->red);
	}
	
	assert(node->max >= node->high);

	int leftBH = rbtVerify(node->left);
	int rightBH = rbtVerify(node->right);
	assert(leftBH == rightBH);
	if (!isRed(node)) {
		return leftBH+1;
	} else {
		return leftBH;
	}
}

/**
 * Locate the smallest node in the subtree
 *
 * @param Node *node - The node whose subtree will be searched
 * @return - Returns the smallest node.
 */
static Node *subtreeMin(Node *node) {
    while (node->left != NIL) {
        node = node->left;
    }
    return node;
}

/**
 * Returns a node's successor
 *
 * @param Node *node - The node whose successor must be found.
 * @returns - Returns the successor node.
 */
static Node *successor(Node *node) {
    if (node->right != NIL) {
        // If a right subtree exists, the successor is the smallest node in
        // the right subtree.
        return subtreeMin(node->right);
    }
    // If no right subtree exists, search for the successor among ancestors
    Node *result = node->parent;
    while (result != NIL && node == result->right) {
        node = result;
        result = result->parent;
    }
    return result;
}

/**
 * Fixes the max's of the tree if a consequential node were to be deleted.
 * A node is consequential if its deletion will impact the validity of the 
 * interval tree. 
 * Consequential nodes are right children who have no right children of
 * their own.
 *
 * @param Node *node - The node that may be deleted.
 */
static void updateMax(Node *node) {
    if (node == root || node == node->parent->left || node->max != node->high) {
        // If a node is not consequential do nothing.
        return;
    }

    // If code has reached this point, the node must be consequential
	assert(node->right == NIL && node == node->parent->right);
	
    /* The new max will be the deleted node's left child's max if it exists.
     * If no left child exists, the new max will be the parent to the deleted
     * node's high. 
     * This new max must be propogated through the ancestors until the root
     * is reached, or the ancestor is a left child. 
     */

	void *newMax;
	if (node->left == NIL) {
        // No left child
		newMax = node->parent->high;
	} else {
		newMax = node->left->max;
	}
    Node *temp = node;

    // Propogate new max up
    do {
        temp = temp->parent;
        temp->max = newMax;
    } while (temp != root && temp == temp->parent->right);
}

/**
 * Creates and initializes a node object
 * 
 * @param void *low - The start point of the block of memory
 * @param void *high - The end point of the block of memory
 * @param bool freed - True for a freed node, false for an alloc'd.
 * @param bool red - True for a red node, false otherwise.
 * @param void *max - The node's max: the highest endpoint in the node's subtree
 *                    and the node itself.
 * @param Node *parent - The parent node.
 * @param Node *left - The left child.
 * @param Node *right - The right child.
 */
static Node *createNode(void *low, void *high, bool freed, bool red, void *max,
						Node *parent, Node *left, Node *right) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        printf("Malloc error in createNode. Terminating program.\n");
        exit(-1);
    }
    node->low = low;
    node->high = high;
    node->freed = freed;
    node->red = red;
    node->max = max;
    node->parent = parent;
    node->left = left;
    node->right = right;

    return node;
}

/**
 * Set's the max of a node based.
 *
 * @param Node *node - The node whose max will be set.
 */
static void setMax(Node *node) {
    if (node->right == NIL) {
        // If no right child exists, the node's max will be its own high
        node->max = node->high;
    } else {
        // If the right child exists, the node's max will be the right child's high
        node->max = node->right->max;
    }
    return;
}

/**
 * Rotates the given node left.
 *
 * @param Node *node - The node to rotate.
 */
static void leftRotate(Node *x) {
	assert(x->right != NIL);
	
    // Y is x's right child and will be rotated such that x becomes y's 
    // left child.
    Node *y = x->right;
    x->right = y->left;
    if (y->left != NIL) {
        // Check for null
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x == root) {
        // If x was the root, y must be the new root
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    setMax(x);
    setMax(y);
}

/**
 * Rotates the given node right.
 *
 * @param Node *node - The node to rotate.
 */
static void rightRotate(Node *x) {
	assert(x->left != NIL);

    // Y is x's left child and will be rotated such that x becomes y's
    // right child.
    Node *y = x->left;
    x->left = y->right;
    if (y->right != NIL) {
        // Check for null
        y->right->parent = x;
    }
    y->parent = x->parent;
    y->right = x;
    x->parent = y;

	if (x == root) {
        // If x was the root, y must be the new root
        root = y;
    } else if (y->parent->left == x) {
		y->parent->left = y;
    } else {
		y->parent->right = y;
    }

    setMax(x);
    setMax(y);
}

/**
 * Function that fixes the integrity of the RB tree after
 * an insertion. Uses CLR algorithm. 
 *
 * @param Node *node - The node to fixup around.
 */
static void insertFixup(Node *node) {
    Node *uncle;
    while (isRed(node->parent)) {
        if (node->parent == node->parent->parent->left) {
            uncle = node->parent->parent->right;
            if (isRed(uncle)) {
                node->parent->red = 0;
                uncle->red = 0;
                node->parent->parent->red = 1;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->right) {
                    node = node->parent;
                    leftRotate(node);
                }
                node->parent->red = 0;
                node->parent->parent->red = 1;
                rightRotate(node->parent->parent);
                
            }
        } else {
			// TODO: 
			//            if (node->parent == node->parent->parent->right) {
                uncle = node->parent->parent->left;
                if (isRed(uncle)) {
                    node->parent->red = 0;
                    uncle->red = 0;
                    node->parent->parent->red = 1;
                    node = node->parent->parent;
                } else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        rightRotate(node);
                    }
                    node->parent->red = 0;
                    node->parent->parent->red = 1;
                    leftRotate(node->parent->parent);
                }
				//            }
        }
    }
    root->red = 0;
}

/**
 * Function that fixes the integrity of the RB tree after
 * a deletion. Uses CLR algorithm. 
 *
 * @param Node *node - The node to fixup around.
 */
static void deleteFixup(Node *node) {
    Node *w;
    while (node != root && !isRed(node)) {
        if (node == node->parent->left) {
            w = node->parent->right;
			assert(w != NIL);
            if (isRed(w)) {
                w->red = 0;
                node->parent->red = 1;
                leftRotate(node->parent);
                w = node->parent->right;
            }
            if (!isRed(w->left) && !isRed(w->right)) {
                w->red = 1;
                node = node->parent;
            } else {
                if (!isRed(w->right)) {
                    w->left->red = 0;
                    w->red = 1;
                    rightRotate(w);
                    w = node->parent->right;
                }
                w->red = node->parent->red;
                node->parent->red = 0;
                w->right->red = 0;
                leftRotate(node->parent);
                node = root;
            }
        } else {
            w = node->parent->left;
			assert(w != NIL);
            if (isRed(w)) {
                w->red = 0;
                node->parent->red = 1;
                rightRotate(node->parent);
                w = node->parent->left;
            }
            if (!isRed(w->right) && !isRed(w->left)) {
                w->red = 1;
                node = node->parent;
            } else {
                if (!isRed(w->left)) {
                    w->right->red = 0;
                    w->red = 1;
                    leftRotate(w);
                    w = node->parent->left;
                }
                w->red = node->parent->red;
                node->parent->red = 0;
                w->left->red = 0;
                rightRotate(node->parent);
                node = root;
            }
        }
    }
    node->red = 0;
}


/**
 * Helper function that recursively searches for a key. 
 *
 * @param Node *curr - The node currently being checked.
 * @param void *addr - The key.
 * @return - Returns the node if it is found, NULL otherwise.
 */
static Node *recurSearch(Node *curr, void *addr) {
    if (curr == NIL) {
        return NULL;
    }
    if (curr->low == addr) {
        return curr;
    }
    if (curr->low < addr) {
        return recurSearch(curr->right, addr);
    } else {
        return recurSearch(curr->left, addr);
    }
}

	
	
