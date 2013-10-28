include ./Makefile.def

all: test $(BIN)

test:
	echo $(SRC_PATH)

$(BIN): $(SRC) $(HEAD)
	gcc -o persist $(SRC) -lpthread

clean:
	rm -f *.o $(BIN)
