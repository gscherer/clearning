#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "entry.h"

#define ENTRY_N 5
#define ENTRY_ENTRY_LINE_SIZE 1024

static void print_election(Entry entries[ENTRY_N], long total_votes, Entry winner) {
	assert(winner != NULL);
	char *header_fmt = "%-40s%-20s%-20s\n";
	printf(header_fmt, "Entry", "Votes Received", "% of Total Votes");
	for (int i = 0; i < ENTRY_N; i++) {
		Entry c = entries[i];
		printf("%-40.*s", (int)c->name_len, c->name_ptr);
		printf("%-20ld", c->votes);
		printf("%-20.2f", 100 * ((double)(c->votes) / total_votes));
		printf("\n");
	}
	printf("%-40s%-20ld\n", "Total", total_votes);
	printf("The winner of the election is %.*s\n", 
		(int)winner->name_len, winner->name_ptr);
} 

static void get_entries(Entry entries[ENTRY_N], long *total_votes, Entry *winner) {
	EntryErr *err;
	char *buffer;
	size_t buff_size = ENTRY_ENTRY_LINE_SIZE;
	long votes = 0;
	*winner = NULL;
	for (size_t i = 0; i < ENTRY_N; i++) {
		size_t len;
		buffer = NULL;
		len = getline(&buffer, &buff_size, stdin);
		Entry c = create_entry(buffer, len, err);
		entries[i] = c;
		if (*err != ENTRY_SUCCESS) {
			entry_err_exit(err, i);
		}
		votes += c->votes;
		if (*winner == NULL || c->votes > (*winner)->votes) {
			*winner = c;
		}
	}
	*total_votes = votes;
}

static void cleanup(Entry entries[ENTRY_N]) {
	for (int i = 0; i < ENTRY_N; i++) {
		free_entry(entries[i]);
	}
}

int main(int argc, char* argv[]) {
	Entry entries[ENTRY_N];
	long total_votes;
	Entry winner;
	get_entries(entries, &total_votes, &winner);
	print_election(entries, total_votes, winner);
	cleanup(entries);
	exit(EXIT_SUCCESS);
}
