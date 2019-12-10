#ifndef VOTES_ENTRY_H
#define VOTES_ENTRY_H

typedef struct entry* Entry; 

struct entry {
	char *entry_line;
	size_t entry_line_len;
	char *name_ptr;
	size_t name_len;
	long votes;
};

typedef enum entry_err EntryErr;

enum entry_err {
	ENTRY_SUCCESS		= 0,
	ENTRY_PARSE_NAME	= 1,
	ENTRY_PARSE_VOTE	= 2
};

Entry create_entry(char * /* entry_line */, size_t /* entry_line_len */, EntryErr * /* err */);
void free_entry(Entry entity);
void entry_err_exit(EntryErr * /* err */, int /* line_num */);

#endif
