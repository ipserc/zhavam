/*
 * list.c
 *
 *  Created on: 26 ene. 2018
 *      Author: ipserc
 */

#include "list.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Creates a new list and initialize its node.
 */
list_t * listNew(list_t * list)
{
	list = malloc(sizeof(list_t));
	list->head = NULL;
	list->tail = NULL;
	return list;
}

/**
 * Counts the number of nodes of a list. Returns 0 if the list is empty.
 */
unsigned listItemsCount(list_t * list)
{
	if (list->head == NULL) return 0;
	int i = 0;
	for(node_t * ptr = list->head; ptr; ptr = ptr->next) ++i;
	return i;
}

/**
 * Appends a new node AT THE END of the list.
 */
list_t * listAppend(list_t * list, void * item, size_t itemSize)
{
	node_t * newNode = malloc(sizeof(node_t));
	newNode->item = malloc(itemSize);
	newNode->item = memcpy(newNode->item, item, itemSize);

	if (list->head == NULL) //Empty List
	{
		list->head = newNode;
		list->tail = newNode;
		newNode->prev = NULL;
		newNode->next = NULL;
	}
	else
	{
		newNode->prev = list->tail;
		list->tail->next = newNode;
		newNode->next = NULL;
		list->tail = newNode;
	}
	return list;
}

/**
 * Removes the LAST node of the list if the list has more than one node.
 * If the list has ONE node, removes the item but keep the node for future uses.
 */
list_t * listDrop(list_t * list)
{
	if (list->head == NULL) return list; //Empty list
	if (list->head == list->tail) // One node list
	{
		free(list->head->item);
		list->head = NULL;
		list->tail = NULL;
	}
	else
	{
		node_t * dropNode = list->tail;
		list->tail = dropNode->prev;
		list->tail->next = NULL;

		free(dropNode->item);
		free(dropNode);
	}

	return list;
}

/**
 * Frees the all the nodes of the list and its items.
 */
void listDestroy(list_t * list)
{
	while (list->tail->prev != NULL) listDrop(list);
	free(list);
}

/**
 *  Inserts a new node BEHIND the one pointed by ptr.
 */
list_t * listInsert(list_t * list, void * item, size_t itemSize, node_t * ptr)
{
	if (ptr == list->tail) // Trying to insert after the last item. Do an append
	{
		return listAppend(list, item, itemSize);
	}
	else
	{
		node_t * newNode = malloc(sizeof(node_t));
		newNode->item = malloc(itemSize);
		newNode->item = memcpy(newNode->item, item, itemSize);
		newNode->next = ptr->next;
		newNode->prev = ptr;
		ptr->next = newNode;
		(newNode->next)->prev = newNode;
	}
	return list;
}

/**
 * Removes the node pointed by ptr.
 */
list_t * listRemove(list_t * list, node_t * ptr)
{
	if (ptr == list->tail ) // Trying to remove the last item. Do a drop
	{
		return listDrop(list);
	}
	else if (ptr == list->head) // Trying to remove the HEAD item
	{
		// If the list has only ONE node
		if (list->head == list->tail) return listDrop(list);
		// else
		list->head = list->head->next;
		list->head->prev = NULL;
		free(ptr->item);
		free(ptr);
	}
	else
	{
		(ptr->next)->prev = ptr->prev;
		(ptr->prev)->next = ptr->next;
		free(ptr->item);
		free(ptr);
	}
	return list;
}

/**
 * Removes the node in the position n.
 * 'n' starts from 0, so 0 is the index of the first item.
 */
list_t * listRemoveN(list_t * list, unsigned n)
{
	node_t * ptr = listNodeN(list, n);
	if (ptr) return listRemove(list,  ptr);
	return list;
}

/**
 * Prints whatever the printFunc prints traversing the list from the head to the tail.
 */
void listPrint(list_t * list, void (* printFunc)(void *))
{
	for(node_t * ptr = list->head;ptr;ptr = ptr->next)
	{
		if (ptr == NULL) break;
		(*printFunc)(ptr->item);
	}
}

/**
 * Prints whatever the printFunc prints for a specific node of the list (I guess the node belongs to a list)
 */
void listNodePrint(node_t * node, void (* printFunc)(void *))
{
	if (node == NULL) return;
	(*printFunc)(node->item);
}

/**
 * Prints a pointer map of the list.
 */
void listMapPrint(list_t * list)
{
	printf("list       %p\n", list);
	printf("list->head %p\n", list->head);
	printf("list->tail %p\n", list->tail);
	for(node_t * ptr = list->head;ptr;ptr = ptr->next)
	{
		printf("ptr       %p\n", ptr);
		printf("ptr->prev %p\n", ptr->prev);
		printf("ptr->item %p\n", ptr->item);
		printf("ptr->next %p\n", ptr->next);
	}
}

/**
 * Returns the pointer at the first node of the list.
 */
node_t * listNodeFirst(list_t * list)
{
	return list->head;
}

/**
 * Returns the pointer at the last node of the list.
 */
node_t * listNodeLast(list_t * list)
{
	return list->tail;
}

/**
 * Returns a pointer at the nth node of the list or NULL if it doesn't exist.
 * 'n' starts from 0, so 0 is the index of the first item.
 */
node_t * listNodeN(list_t * list, unsigned n)
{
	node_t * ptr = list->head;
	for(int i = 0;i < n;++i)
	{
		if (ptr == NULL) return NULL;
		ptr = ptr->next;
	}
	return ptr;
}

/**
 * Finds the first match given by findFunc by traversing the list from the head to the tail and returns the node.
 */
node_t * listFind(list_t * list, node_t * startNode, bool (* findFunc)(void *))
{
	if (!startNode) startNode = list->head;
	for(node_t * ptr = startNode;ptr;ptr = ptr->next)
	{
		if ((*findFunc)(ptr->item)) return ptr;
	}
	return NULL;
}
