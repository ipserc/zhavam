/*
 * list.h
 *
 *  Created on: 27 ene. 2018
 *      Author: ipserc
 */

#ifndef SOURCE_LIST_H_
#define SOURCE_LIST_H_

#include <stdbool.h>
#include <stddef.h>

/**
 * A node is the unit with which the linked lists are mounted.
 * Each node has a reference to its predecessor and its successor.
 * The head and tail nodes are special because their predecessor and successor are set to NULL respectively.
 * A node also has a void pointer that serves to point to the information structure it transports so that
 * the list of nodes can organize the information.
 */
typedef struct node
{
	void * item;
	struct node * prev;
	struct node * next;
} node_t;

/**
 * A list is a structure of chained (linked) nodes that can be specified by its head node (and optionally with its tail node too).
 * Between these two nodes is the complete list and the link between the nodes allows you to traverse it
 * in an orderly manner, to recover each of its elements.
 * The operations necessary to manage lists include the creation and destruction of the list.
 * the remaining operations are operations that involve the nodes as they are. the appendix, the drop,
 * the elimination and the insertion of nodes.
 */
typedef struct list {
	node_t * head;
	node_t * tail;
} list_t;

/*
 * Prototypes
 */
/* list.c */
void listVersion(void);
list_t *listNew(list_t **list);
unsigned listItemsCount(list_t *list);
list_t *listAppend(list_t *list, void *item, size_t itemSize);
list_t *listDrop(list_t *list, void (*freeItem)(void *));
void listDestroy(list_t *list, void (*freeItem)(void *));
list_t *listInsert(list_t *list, void *item, size_t itemSize, node_t *ptr);
list_t *listRemove(list_t *list, node_t *ptr, void (*freeItem)(void *));
list_t *listRemoveN(list_t *list, unsigned n, void (*freeItem)(void *));
void listPrint(list_t *list, void (*printFunc)(void *));
void listNodePrint(node_t *node, void (*printFunc)(void *));
void listMapPrint(list_t *list);
node_t *listNodeFirst(list_t *list);
node_t *listNodeLast(list_t *list);
node_t *listNodeN(list_t *list, unsigned n);
node_t *listFind(list_t *list, node_t *startNode, _Bool (*findFunc)(void *));

#endif /* SOURCE_LIST_H_ */
