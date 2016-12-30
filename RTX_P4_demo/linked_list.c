#include "linked_list.h"
#include "types.h"

void *add_list_node(linked_list_t *list, void *data_p) {

	list_node_t *old_head, *new_head;

	// Obtain the old head of the list
	old_head = list->head;

	// Create a new node to carry the data
	new_head = (list_node_t*)malloc(sizeof(list_node_t));
	new_head->prev = NULL;
	new_head->next = old_head;
	new_head->data_p = data_p;

	// Attach the bullet to the head of the list
	if (old_head) {
		old_head->prev = new_head;
	}

	// Point the head of the list to the new head
	list->head = new_head;
	list->size++;
}

void remove_list_node(linked_list_t *list, list_node_t *node) {

	list_node_t *prev_node, *next_node;

	prev_node = node->prev;
	next_node = node->next;

	// If the node was the head note, point the list to the new head
	if (list->head == node) {
		list->head = next_node;
	}

	// If the previous node exists, point it to the next node
	if (prev_node) {
		prev_node->next = next_node;
	}
	// If the next node exists, point it to the previous node
	if (next_node) {
		next_node->prev = prev_node;
	}

	// Free memory allocated for the bullet
	free(node);

	// Decrease the list size
	list->size--;
}