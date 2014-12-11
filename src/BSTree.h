/*
 * BSTree.h
 *
 * Copyright 2014  <Daniel Grimshaw>
 *
 * Header file for binary search trees.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */

 #include <stdlib.h>
 #include <stdbool.h>

typedef struct Node {
	struct Node *left, *right;
	size_t size;
	char data[];
} Node_t;

typedef const void *GetKeyFunc_t(const void *dData);
typedef int CmpFunc_t(const void *pKey1, const void *pKey2);

typedef struct {
	struct Node 	*pRoot;		//pointer to the root
	CmpFunc_t		*cmp;		//Compares two keys
	GetKeyFunc_t 	*getKey; 	//Converts data into a key value
} BST_t;

BST_t *newBST(CmpFunc_t *cmp, GetKeyFunc_t *getKey); //dynamically generates a new object with the type BST_t and returns a pointer to it.

bool BST_insert(BST_t *pBST, const void *pData, size_t size); //dynamically allocates a new node and copies the data referenced by pData to the node, then inserts the node into the specified tree.

const void *BST_search(BST_t *pBST, const void *pKey); //searches the tree and returns a pointer to the data item that matches the key referenced by pKey.

bool BST_erase(BST_t *pBST, const void *pKey); //deletes the first node whose data contents match the key referenced by pKey

void BST_clear(BST_t *pBST); //deletes the entire tree.

//The next four functions traverse the tree in a certain order, and call the function referenced by action to manipulate the data contents of each node
int BST_inorder(BST_t *pBST, bool (*action)(void *pData));

int BST_rev_inorder(BST_t *pBST, bool (*action)(void *pData));

int BST_preorder(BST_t *pBST, bool (*action)(void *pData));

int BST_postorder(BST_t *pBST, bool (*action)(void *pData));
