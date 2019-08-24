#include <stdio.h>
#include <stdlib.h>

struct node {
	int value;
	struct node* next;
};

struct node* ll_new(int value, struct node* next) {
	struct node* head = (struct node*)malloc(sizeof(struct node));
	head->value = value;
	head->next = next;
	return head;
}

int main(int argc, char** argv) {
	struct node* my_ll = ll_new(3, NULL);
	printf("my_ll->value = %d\n", my_ll->value);
	printf("my_ll->next= %x\n", my_ll->next);

	struct node my_node;
	struct node my_other_node;

	my_other_node.value = 6;
	my_other_node.next = NULL;

	my_node.value = 5;
	my_node.next = &my_other_node;

	printf("my_node.value = %d\n", my_node.value);
	printf("my_node.next = 0x%x\n", my_node.next);

	printf("my_other_node.value = %d\n", my_other_node.value);
	printf("my_other_node.next = %x\n", my_other_node.next);

	return 0;
}
