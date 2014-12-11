/*
 *
 * BSTree.c
 *
 * Definitions of the functions declared in BSTree.h
 *
 * Copyright 2014  <Daniel Grimshaw>
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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "BSTree.h"

const void *defaultGetKey(const void *pData) {return pData;}

BST_t *newBST(CmpFunc_t *cmp, GetKeyFunc_t *getKey) {
	BST_t *pBST = NULL;
	if (cmp != NULL)
	    pBST = malloc(sizeof(BST_t));
	if (pBST != NULL) {
		pBST->pRoot = NULL;
		pBST->cmp = cmp;
		pBST->getKey = (getKey != NULL) ? getKey : defaultGetKey;
	}
	return pBST;
}

static bool insert(BST_t *pBST, Node_t **ppNode, const void *pData, size_t size);

bool BST_insert(BST_t *pBST, const void *pData, size_t size) {
	if (pBST == NULL || pData == NULL || size == 0)
	    return false;
	return insert(pBST, &(pBST->pRoot), pData, size);
}

static bool insert(BST_t *pBST, Node_t **ppNode, const void *pData, size_t size) {
	Node_t *pNode = *ppNode;		//Pointer to the root node of the subtree to insert the new node in.
	if (pNode == NULL) { 			//there is a location to place a new leaf here
	    pNode = malloc(sizeof(Node_t) + size);
	    if (pNode != NULL) {
			pNode->left = pNode->right = NULL;		//initialize the new node's members
			memcpy(pNode->data, pData, size);
			*ppNode = pNode;						//insert the new node.
			return true;
		}
		else
		    return false;
	}
	else {							//keep looking ...
	    const void *key1 = pBST->getKey(pData), *key2 = pBST->getKey(pNode->data);

	    if (pBST->cmp(key1, key2) <0)				//...in left subtree
	        return insert(pBST, &(pNode->left), pData, size);
	    //...in right subtree
	    else
	        return insert(pBST, &(pNode->right), pData, size);
	}
}

static const void *search(BST_t *pBST, const Node_t *pNode, const void *pKey);

const void *BST_search(BST_t *pBST, const void *pKey) {
	if (pBST == NULL || pKey == NULL) return NULL;
	return search(pBST, pBST->pRoot, pKey);		//search from the root
}

static const void *search(BST_t *pBST, const Node_t *pNode, const void *pKey) {
	if (pNode == NULL)
	    return NULL;					//no subtrees found, no matches

	else {
		int cmp_res = pBST->cmp(pKey, pBST->getKey(pNode->data));
		if (cmp_res == 0)				//found an exact match
		    return pNode->data;
		else if (cmp_res < 0)			//continue the search in left subtree
		    return search(pBST, pNode->left, pKey);
		else
		    return search(pBST, pNode->right, pKey); //continue im the right subtree
	}
}

static Node_t *detachMin(Node_t **ppNode) {
	Node_t *pNode = *ppNode;			//pointer to the current node
	if (pNode == NULL)
		return NULL;					//pNode is an empty subtree
	else if (pNode->left != NULL)
		return detachMin(&(pNode->left));	//the minimum is in the left subtree
	else
	{										//pNode points to the minimum node.
		*ppNode = pNode->right;				//Atach the right child to the parent.
		return pNode;
	}
}

static bool erase(BST_t *pBST, Node_t **ppNode, const void *pKey);

bool BST_erase(BST_t *pBST, const void *pKey) {
	if (pBST == NULL || pKey == NULL) return false;
	return erase(pBST, &(pBST->pRoot), pKey);
}

static bool erase(BST_t *pBST, Node_t **ppNode, const void *pKey) {
	Node_t *pNode = *ppNode;			//pointer to the current node.
	if (pNode == NULL)
		return false;					//No match found!

	int cmp_res = pBST->cmp(pKey, pBST->getKey(pNode->data));

	if (cmp_res < 0)
		return erase(pBST, &(pNode->left), pKey);
	else if (cmp_res > 0)
		return erase(pBST, &(pNode->right), pKey);

	else
	{									//found the node to be deleted
		if (pNode->left == NULL)		//if no more than one child
			*ppNode = pNode->right;		//attach the child to the parent
		else if (pNode->right == NULL)
			*ppNode = pNode->left;

		else
		{								//two children: replace the node with the minimum from the right subtree.
				Node_t *pMin = detachMin(&(pNode->right));
				*ppNode = pMin;			//Graft it onto the deleted node's parent.
				pMin->left = pNode->left;
				pMin->right = pNode->right;
		}
		free(pNode);					//release the memory that held the now deleted node
		return true;
	}
}

static void clear(Node_t *pNode);

void BST_clear(BST_t *pBST) {
	if (pBST != NULL) {
		clear(pBST->pRoot);
		pBST->pRoot = NULL;
	}
}

static void clear(Node_t *pNode) {
	if (pNode != NULL) {
		clear(pNode->left);
		clear(pNode->right);
		free(pNode);
	}
}

static int inorder(Node_t *pNode, bool (*action)(void *pData));

int BST_inorder(BST_t *pBST, bool (*action)(void *pData)) {
	if (pBST == NULL || action == NULL)
		return 0;
	else
		return inorder(pBST->pRoot, action);
}

static int inorder(Node_t *pNode, bool (*action)(void *pData)) {
	//LNR
	int count = 0;
	if (pNode == NULL)
		return 0;

	count = inorder(pNode->left, action);

	if (action(pNode->data))
		++count;

	count += inorder(pNode->right, action);

	return count;
}

static int rev_inorder(Node_t *pNode, bool (*action)(void *pData));

int BST_rev_inorder(BST_t *pBST, bool (action)(void *pData)) {
	if (pBST == NULL || action == NULL)
		return 0;
	else
		return rev_inorder(pBST->pRoot, action);
}

static int rev_inorder(Node_t *pNode, bool (*action)(void *pData)) {
	//RNL
	int count = 0;
	if (pNode == NULL)
		return 0;

	count = (rev_inorder(pNode->right, action));

	if (action(pNode->data))
		++count;

	count += rev_inorder(pNode->left, action);

	return count;
}

static int preorder(Node_t *pNode, bool (*action)(void *pData));

int BST_preorder(BST_t *pBST, bool (*action)(void *pData)) {
	if (pBST == NULL || action == NULL)
		return 0;
	else
		return preorder(pBST->pRoot, action);
}

static int preorder(Node_t *pNode, bool (*action)(void *pData)) {
	//NLR
	int count = 0;
	if (pNode == NULL)
		return 0;

	if (action(pNode->data))
		++count;

	count += (preorder(pNode->left, action));

	count += (preorder(pNode->right, action));

	return count;
}

static int postorder(Node_t *pNode, bool (*action)(void *pData));

int BST_postorder(BST_t *pBST, bool (*action)(void *pData)) {
	if (pBST == NULL || action == NULL)
		return 0;
	else
		return postorder(pBST->pRoot, action);
}

static int postorder(Node_t *pNode, bool (*action)(void *pData)) {
	//LRN
	int count = 0;
	if (pNode == NULL)
		return 0;

	count = (postorder(pNode->left, action));

	count += (postorder(pNode->right, action));

	if (action(pNode->data))
		++count;

	return count;
}
