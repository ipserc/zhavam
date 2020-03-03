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

char NAME[] = "list";
char VERSION[] = "1.2";

/**
 * Prints the name and the version of the code
 */
void listVersion(void)
{
	printf("%s version:%s\n", NAME, VERSION);
}

/**
 * Creates a new list and initializes its nodes pointers.
 * Returns NULL if memory allocation fails
 * @param list list_t**
 * @return list_t* the new list initialized or NULL
 */
list_t * listNew(list_t ** list)
{
	if ((*list = malloc(sizeof(list_t))) != NULL)
	{
		(*list)->head = NULL;
		(*list)->tail = NULL;
	}
	return *list;
}

/**
 * Counts the number of nodes of a list.
 * Returns 0 if the list is empty.
 * @param list list_t*
 * @return unsigned the number of items in the list
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
 * @param list_t* list
 * @param void* item
 * @param size_t itemSize
 * @return list_t*
 */
list_t * listAppend(list_t * list, void * item, size_t itemSize)
{
	node_t * newNode = malloc(sizeof(node_t));
	newNode->item = malloc(itemSize);
	newNode->item = memmove(newNode->item, item, itemSize);

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
 * If the list has only ONE node, removes the item but it keeps the list for future uses.
 * Pass the function required to free the dynamic memory allocated to the item. NULL if item hasn't got allocated memory
 * @param list list_t*
 * @param void (* freeItem)(void *)
 * @return list_t*
 */
list_t * listDrop(list_t * list, void (* freeItem)(void *))
{
	if (list->head == NULL) return list; //Empty list
	if (list->head == list->tail) // One node list
	{
		void * item = list->head->item;
		if (freeItem) freeItem(item);
		free(list->head->item);
		list->head = NULL;
		list->tail = NULL;
	}
	else
	{
		node_t * dropNode = list->tail;
		void * item = dropNode->item;
		if (freeItem) freeItem(item);
		list->tail = dropNode->prev;
		list->tail->next = NULL;
		free(dropNode->item);
		free(dropNode);
	}
	return list;
}

/**
 * Frees the all the nodes of the list and its items.
 * Pass the function required to free the dynamic memory allocated to the item. NULL if item has not memory allocated
 * @param list list_t*
 * @param void (* freeItem)(void *)
 */
void listDestroy(list_t * list, void (* freeItem)(void *))
{
	while (list->tail->prev != NULL) listDrop(list, freeItem);
	listDrop(list, freeItem);
	free(list);
}

/**
 *  Inserts a new node BEHIND the one pointed by ptr.
 *  @param list list_t*
 *  @param item void *
 *  @param itemSize size_t
 *  @param ptr node_t*
 *  @return list_t*
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
 * Pass the function required to free the dynamic memory allocated to the item. NULL if item has not memory allocated
 * @param list list_t*
 * @param ptr node_t*
 * @param void (* freeItem)(void *)
 * @return list_t*
 */
list_t * listRemove(list_t * list, node_t * ptr, void (* freeItem)(void *))
{
	if (ptr == list->tail ) // Trying to remove the last item. Do a drop
	{
		return listDrop(list, freeItem);
	}
	else if (ptr == list->head) // Trying to remove the HEAD item
	{
		// If the list has only ONE node
		if (list->head == list->tail) return listDrop(list, freeItem);
		// else
		list->head = list->head->next;
		list->head->prev = NULL;
		if (freeItem) freeItem(ptr->item);
		free(ptr->item);
		free(ptr);
	}
	else
	{
		(ptr->next)->prev = ptr->prev;
		(ptr->prev)->next = ptr->next;
		if (freeItem) freeItem(ptr->item);
		free(ptr->item);
		free(ptr);
	}
	return list;
}

/**
 * Removes the node in the position n.
 * 'n' starts from 0, so 0 is the index of the first item.
 * Pass the function required to free the dynamic memory allocated to the item. NULL if item has not memory allocated
 * @param list list_t*
 * @param n unsigned
 * @param void (* freeItem)(void *)
 * @return list_t*
 */
list_t * listRemoveN(list_t * list, unsigned n, void (* freeItem)(void *))
{
	node_t * ptr = listNodeN(list, n);
	if (ptr) return listRemove(list,  ptr, freeItem);
	return list;
}

/**
 * Prints whatever the printFunc would print about the item of the pointed node while traversing the list from the head to the tail.
 * @param list list_t*
 * @param void (* printFunc)(void *)
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
 * Prints whatever the printFunc would print for an item of a given node of the list (I guess the node belongs to a list)
 * @param node node_t*
 * @param void (* printFunc)(void *)
 */
void listNodePrint(node_t * node, void (* printFunc)(void *))
{
	if (node == NULL) return;
	(*printFunc)(node->item);
}

/**
 * Prints a pointer map of the list.
 * @param list list_t*
 */
void listMapPrint(list_t * list)
{
	puts("************* LIST MEMORY MAP *************");
	printf("list       %p\n", list);
	printf("list->head %p\n", list->head);
	printf("list->tail %p\n", list->tail);
	for(node_t * node = list->head; node; node = node->next)
	{
		printf("node       %p\n", node);
		printf("node->prev %p\n", node->prev);
		printf("node->item %p\n", node->item);
		printf("node->next %p\n", node->next);
	}
	puts("********** END OF LIST MEMORY MAP **********");
}

/**
 * Returns the pointer at the first node of the list.
 * @param list list_t*
 * @return node_t*
 */
node_t * listNodeFirst(list_t * list)
{
	return list->head;
}

/**
 * Returns the pointer at the last node of the list.
 * @param list list_t*
 * @return node_t*
 */
node_t * listNodeLast(list_t * list)
{
	return list->tail;
}

/**
 * Returns a pointer at the nth node of the list or NULL if it doesn't exist.
 * 'n' starts from 0, so 0 is the index of the first item.
 * @param list list_t*
 * @param n unsigned
 * @return node_t*
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
 * @param list list_t*
 * @param startNode node_t*
 * @param bool (* findFunc)(void *)
 * @return node_t*
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
