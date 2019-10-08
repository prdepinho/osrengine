
EXE = program
SRC = main.cpp log.cpp terrain.cpp encounter.cpp
OBJ = $(SRC:.cpp=.o)
CC = g++

$(EXE): $(OBJ)
	$(CC) -o $(EXE) $(OBJ) -lm -lncurses -lpthread

.cpp.o:
	$(CC) -c $< -o $@

clean:
	rm $(OBJ) $(EXE)
