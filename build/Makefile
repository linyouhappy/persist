$include ./Makefile.def
BIN = persist
SRC = persist.c connection.c thread.c
HEAD = persist.h connection.h queue.h thread.h

$(BIN: $(SRC) $(HEAD)
	gcc -o persist $(SRC) -lpthread

clean:
	rm -rf *.o $(BIN)