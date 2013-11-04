include ./Makefile.def

all: test $(BIN)

test:
	echo $(CFLAGS)

$(BIN): $(SRC) $(HEAD)
	gcc -o persist $(SRC) -lpthread $(CFLAGS) -I$(SRC_PATH)

clean:
	rm -f *.o $(BIN)
