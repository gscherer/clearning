#include <stdio.h>
#include "candidate.h"

#define CANDIDATE_N 5
#define CANDIDATE_ENTRY_LINE_SIZE 1024

static void print_election(Candidate entries[CANDIDATE_N], long total_votes, Candidate winner) {
	assert(winner != NULL);
	char *header_fmt = "%-40s%-20s%-20s\n";
	printf(header_fmt, "Candidate", "Votes Received", "% of Total Votes");
	for (int i = 0; i < CANDIDATE_N; i++) {
		Candidate c = entries[i];
		printf("%-40.*s", (int)c->name_len, c->name_ptr);
		printf("%-20ld", c->votes);
		printf("%-20.2f", 100 * ((double)(c->votes) / total_votes));
		printf("\n");
	}
	printf("%-40s%-20ld\n", "Total", total_votes);
	printf("The winner of the election is %.*s\n", 
		(int)winner->name_len, winner->name_ptr);
} 

static void get_candidates(Candidate entries[CANDIDATE_N], long *total_votes, Candidate *winner) {
	CandidateErr *err;
	char *buffer;
	size_t buff_size = CANDIDATE_ENTRY_LINE_SIZE;
	long votes = 0;
	*winner = NULL;
	for (size_t i = 0; i < CANDIDATE_N; i++) {
		size_t len;
		buffer = NULL;
		len = getline(&buffer, &buff_size, stdin);
		Candidate c = create_candidate(buffer, len, err);
		entries[i] = c;
		if (*err != CANDIDATE_SUCCESS) {
			candidate_err_exit(err, i);
		}
		votes += c->votes;
		if (*winner == NULL || c->votes > (*winner)->votes) {
			*winner = c;
		}
	}
	*total_votes = votes;
}

static void cleanup(Candidate entries[CANDIDATE_N]) {
	for (int i = 0; i < CANDIDATE_N; i++) {
		free_candidate(entries[i]);
	}
}

int main(int argc, char* argv[]) {
	Candidate entries[CANDIDATE_N];
	long total_votes;
	Candidate winner;
	get_candidates(entries, &total_votes, &winner);
	print_election(entries, total_votes, winner);
	cleanup(entries);
	exit(EXIT_SUCCESS);
}
