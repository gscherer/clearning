#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>

typedef struct candidate* Candidate; 

struct candidate {
	char *entry_line;
	size_t entry_line_len;
	char *name_ptr;
	size_t name_len;
	long votes;
};

typedef enum candidate_err CandidateErr;

enum candidate_err {
	CANDIDATE_SUCCESS		= 0,
	CANDIDATE_PARSE_NAME		= 1,
	CANDIDATE_PARSE_VOTE		= 2
};

static const char* const candidate_err_msg[] = {
	"Candidate created successfully",
	"Failed to parse name",
	"Failed to parse vote",
};

static void init_candidate(Candidate, CandidateErr *);

Candidate create_candidate(
	char *entry_line,
	size_t entry_line_len,
	CandidateErr *err)
{
	Candidate entity = (Candidate)malloc(sizeof(struct candidate));
	entity->entry_line = entry_line;
	entity->entry_line_len = entry_line_len;
	init_candidate(entity, err);
	return entity;
}

void free_candidate(Candidate entity) {
	assert(entity != NULL);
	free(entity->entry_line);
	free(entity);
}

void candidate_err_exit(CandidateErr *err, int line_num) {
	fprintf(stderr, "%s, on line %d\n", candidate_err_msg[*err], line_num);
	exit(*err);
}

static long parse_votes(char *ptr, CandidateErr *err) {
	char *end;
	errno = 0;
	long votes = strtol(ptr, &end, 10);
	if ((errno == ERANGE && (votes == LONG_MAX || votes == LONG_MIN))
		|| (errno != 0 && votes == 0)) {
		*err = CANDIDATE_PARSE_VOTE;	
		votes = 0;
	}
	return votes;
}

/**
 * Handle parsing of candidate.entry_line, which is of form:
 * 	"<name> <vote count>\n"
 */
static void init_candidate(Candidate entity, CandidateErr *err) 
{
	char *ptr = entity->entry_line;
	size_t n = entity->entry_line_len;
	size_t i = 0;

	// Eat whitespace
	while (ptr != NULL && i < n && isblank(*ptr)) {
		i++;
		ptr++;
	}
	
	// Eat name
	entity->name_ptr = ptr;
	while (ptr != NULL && i < n && !isdigit(*ptr) && *ptr != '\n') {
		i++;
		ptr++;
	}

	// Empty name	
	if (entity->name_ptr == ptr) {
		*err = CANDIDATE_PARSE_NAME;
		return;
	}

	// Check space before vote
	if (ptr != NULL && isdigit(*ptr)) {
		ptr--;
		i--;
		if (ptr != NULL && !isblank(*ptr)) {
			*err = CANDIDATE_PARSE_VOTE;
			return;		
		}
	}

	// Set name length
	entity->name_len = ptr - entity->name_ptr;

	// Parse votes
	if (ptr != NULL && i < n) {
		entity->votes = parse_votes(ptr, err);
		if (*err == CANDIDATE_PARSE_VOTE) {
			return;
		}
	}
	*err = CANDIDATE_SUCCESS;
	return;
}
