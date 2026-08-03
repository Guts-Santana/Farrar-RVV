CC = g++
CFLAGS = -std=c++17 -Wall -Iinclude

SRC = src
BIN = bin
INC = include

all: execute

$(BIN):
	mkdir -p $(BIN)

execute: main.cpp $(BIN) $(BIN)/NWAlgorithm.o $(BIN)/Gotoh.o $(BIN)/Farrar.o $(BIN)/Vec.o $(BIN)/tests.o $(BIN)/vectorial_call.o
	$(CC) $(CFLAGS) main.cpp $(BIN)/NWAlgorithm.o $(BIN)/Gotoh.o $(BIN)/Farrar.o $(BIN)/Vec.o $(BIN)/tests.o $(BIN)/vectorial_call.o -o execute

$(BIN)/NWAlgorithm.o: $(SRC)/NWAlgorithm.cpp $(INC)/NWAlgorithm.hpp
	$(CC) $(CFLAGS) -c $(SRC)/NWAlgorithm.cpp -o $(BIN)/NWAlgorithm.o

$(BIN)/Gotoh.o: $(SRC)/Gotoh.cpp $(INC)/Gotoh.hpp
	$(CC) $(CFLAGS) -c $(SRC)/Gotoh.cpp -o $(BIN)/Gotoh.o

$(BIN)/Farrar.o: $(SRC)/Farrar.cpp $(INC)/Farrar.hpp $(BIN)/Vec.o
	$(CC) $(CFLAGS) -c $(SRC)/Farrar.cpp -o $(BIN)/Farrar.o

$(BIN)/Vec.o: $(SRC)/Vec.cpp $(INC)/Vec.hpp
	$(CC) $(CFLAGS) -c $(SRC)/Vec.cpp -o $(BIN)/Vec.o

$(BIN)/tests.o: $(SRC)/tests.cpp $(INC)/tests.hpp
	$(CC) $(CFLAGS) -c $(SRC)/tests.cpp -o $(BIN)/tests.o

$(BIN)/vectorial_call.o: $(SRC)/vectorial_call.cpp $(INC)/vectorial_call.hpp
	$(CC) $(CFLAGS) -c $(SRC)/vectorial_call.cpp -o $(BIN)/vectorial_call.o


clean:
	rm -rf $(BIN)/*.o execute