/*
 * sortlines.c
 *
 * Copyright 2014  <Daniel Grimshaw>
 *
 * Basic utility to sort lines of text from an imput file.
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
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "BSTree.c"				//BST functions

#define LEN_MAX 1024			//maximum length of line
char buffer[LEN_MAX];

//Action to perform for each line:
bool printStr(void *str) {return printf("%s", str) >= 0;}

int main()
{
	BST_t *pStrTree = newBST((CmpFunc_t*)strcmp, NULL);
	int n;

	while (fgets(buffer, LEN_MAX, stdin) != NULL) {		//read each line
		size_t len = strlen(buffer);        			//length with \n character

		if (!BST_insert(pStrTree, buffer, len+1))		//insert the line in the tree.
			break;
	}
	if (!feof(stdin)) {									//if we can not read the entire text:
		fprintf(stderr, "sortlines: ",
				"Error reading or storing text input.\n");
		exit(EXIT_FAILURE);
	}
	n = BST_inorder(pStrTree, printStr);				//print each line in sorted order.

	fprintf(stderr, "\nsortlines: Printed %d lines.\n", n);

	BST_clear(pStrTree);								//Discard all nodes.
	return 0;
}

