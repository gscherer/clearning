#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>
#include "entry.h"

static const char* const entry_err_msg[] = {
	"Entry created successfully",
	"Failed to parse name",
	"Failed to parse vote",
};

static void init_entry(Entry, EntryErr *);

Entry create_entry(
	char *entry_line,
	size_t entry_line_len,
	EntryErr *err)
{
	Entry entity = (Entry)malloc(sizeof(struct entry));
	entity->entry_line = entry_line;
	entity->entry_line_len = entry_line_len;
	init_entry(entity, err);
	return entity;
}

void free_entry(Entry entity) {
	assert(entity != NULL);
	free(entity->entry_line);
	free(entity);
}

void entry_err_exit(EntryErr *err, int line_num) {
	fprintf(stderr, "%s, on line %d\n", entry_err_msg[*err], line_num);
	exit(*err);
}

static long parse_votes(char *ptr, EntryErr *err) {
	char *end;
	errno = 0;
	long votes = strtol(ptr, &end, 10);
	if ((errno == ERANGE && (votes == LONG_MAX || votes == LONG_MIN))
		|| (errno != 0 && votes == 0)) {
		*err = ENTRY_PARSE_VOTE;	
		votes = 0;
	}
	return votes;
}

/**
 * Handle parsing of entry.entry_line, which is of form:
 * 	"<name> <vote count>\n"
 */
static void init_entry(Entry entity, EntryErr *err) 
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
		*err = ENTRY_PARSE_NAME;
		return;
	}

	// Check space before vote
	if (ptr != NULL && isdigit(*ptr)) {
		ptr--;
		i--;
		if (ptr != NULL && !isblank(*ptr)) {
			*err = ENTRY_PARSE_VOTE;
			return;		
		}
	}

	// Set name length
	entity->name_len = ptr - entity->name_ptr;

	// Parse votes
	if (ptr != NULL && i < n) {
		entity->votes = parse_votes(ptr, err);
		if (*err == ENTRY_PARSE_VOTE) {
			return;
		}
	}
	*err = ENTRY_SUCCESS;
	return;
}
