all:
	cc -o ./bin/votes ./votes/votes.c
	cc -o ./bin/linked_list ./linked_list/linked_list.c
clean:
	rm votes/*.o linked_list/*.o
	rm bin/*

