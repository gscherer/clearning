#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node* Node;

struct node {
	int value;
	Node next;
};

Node ll_new(int value, Node next) {
	Node head = (Node)malloc(sizeof(struct node));
	head->value = value;
	head->next = next;
	return head;
}

void ll_append(Node const head, int value) {
	assert(head != NULL);	

	Node to_append = ll_new(value, NULL);
	Node iter = head;
	while (iter->next != NULL) {
		iter = iter->next;
	}
	iter->next = to_append;
}

Node ll_prepend(Node const head, int value) {
	Node to_prepend = ll_new(value, NULL);
	if (head == NULL) {
		return to_prepend;
	}
	to_prepend->next = head;
	return to_prepend;
}

Node ll_find(Node const head, int query) {
	Node iter = head;
	while (iter->next != NULL && iter->value != query) {
		iter = iter->next;
	}
	return iter;
}

Node ll_delete(Node const head, int query, int* success) {
	Node curr = head;
	Node prev = NULL;

	while (curr != NULL && curr->value != query) {
		prev = curr;
		curr = curr->next;
	}

	// Reached end of list
	if (curr == NULL) {
		*success = 0;
		return head;
	}

	// Deleting something...
	*success = 1;

	// Delete head
	if (prev == NULL) {
		Node result = curr->next;
		free(curr);	
		return result;
	}

	// Delete anything else
	prev->next = curr->next;
	free(curr);
	return head;
}

void ll_print_list(Node const head) {
	Node iter = head;
	while (iter != NULL) {
		printf("(%d)->", iter->value);
		iter = iter->next;
	}
	printf("(NULL)\n");
}

int main(int argc, char** argv) {
	Node head = ll_new(3, NULL);

	head = ll_prepend(head, 5);
	head = ll_prepend(head, 6);
	head = ll_prepend(head, 7);

	ll_print_list(head);

	int del_val = 33;
	Node result = ll_find(head, del_val);

	if (result != NULL) {
		printf("Found node that matched query %d, at: %p, with value = %d\n", 6, result, result->value);
	}

	int was_deleted = 0;
	head = ll_delete(head, del_val, &was_deleted);

	if (was_deleted > 0) {
		printf("Deleted node with value = %d\n", del_val);
	}

	ll_print_list(head);
	return 0;
}
