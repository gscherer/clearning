all:
	cc -std=c99 -o ./bin/votes ./votes/entry.c ./votes/votes.c
	cc -o ./bin/linked_list ./linked_list/linked_list.c
clean:
	rm bin/*

