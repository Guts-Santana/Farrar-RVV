CC =/usr/bin/riscv64-linux-gnu-g++
CFLAGS = -std=c++17 -Wall -Iinclude \
          -march=rv64gcv \
	  -mabi=lp64d

SRC = src
BIN = bin
INC = include

all: execute

$(BIN):
	mkdir -p $(BIN)

execute: main.cpp $(BIN)  $(BIN)/Gotoh.o $(BIN)/Farrar.o $(BIN)/ScalarVec.o $(BIN)/RvvVec.o $(BIN)/tests.o $(BIN)/vectorial_call.o $(BIN)/RvvReg.o
	$(CC) $(CFLAGS) main.cpp $(BIN)/Gotoh.o $(BIN)/Farrar.o $(BIN)/ScalarVec.o $(BIN)/RvvVec.o $(BIN)/tests.o $(BIN)/vectorial_call.o $(BIN)/RvvReg.o -o execute

$(BIN)/Gotoh.o: $(SRC)/Gotoh.cpp $(INC)/Gotoh.hpp ${INC}/constants.hpp
	$(CC) $(CFLAGS) -c $(SRC)/Gotoh.cpp -o $(BIN)/Gotoh.o

$(BIN)/Farrar.o: $(SRC)/Farrar.cpp $(INC)/Farrar.hpp ${INC}/constants.hpp $(BIN)/ScalarVec.o $(BIN)/RvvVec.o
	$(CC) $(CFLAGS) -c $(SRC)/Farrar.cpp -o $(BIN)/Farrar.o

$(BIN)/ScalarVec.o: $(SRC)/ScalarVec.cpp $(INC)/ScalarVec.hpp ${INC}/constants.hpp
	$(CC) $(CFLAGS) -c $(SRC)/ScalarVec.cpp -o $(BIN)/ScalarVec.o

$(BIN)/RvvVec.o: $(SRC)/RvvVec.cpp $(INC)/RvvVec.hpp ${INC}/constants.hpp
	$(CC) $(CFLAGS) -c $(SRC)/RvvVec.cpp -o $(BIN)/RvvVec.o

$(BIN)/tests.o: $(SRC)/tests.cpp $(INC)/tests.hpp
	$(CC) $(CFLAGS) -c $(SRC)/tests.cpp -o $(BIN)/tests.o

$(BIN)/vectorial_call.o: $(SRC)/vectorial_call.cpp $(INC)/vectorial_call.hpp
	$(CC) $(CFLAGS) -c $(SRC)/vectorial_call.cpp -o $(BIN)/vectorial_call.o

$(BIN)/RvvReg.o: $(SRC)/RvvReg.cpp $(INC)/RvvReg.hpp
	$(CC) $(CFLAGS) -c $(SRC)/RvvReg.cpp -o $(BIN)/RvvReg.o

$(BIN)/FarrarRvv.o: $(SRC)/FarrarRvv.cpp $(INC)/FarrarRvv.hpp ${INC}/constants.hpp $(BIN)/RvvVec.o $(BIN)/RvvReg.o
	$(CC) $(CFLAGS) -c $(SRC)/FarrarRvv.cpp -o $(BIN)/FarrarRvv.o

clean:
	rm -rf $(BIN)/*.o execute
