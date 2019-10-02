
EXE = program
SRC = main.cpp terrain.cpp 
OBJ = $(SRC:.cpp=.o)
CC = g++

$(EXE): $(OBJ)
	$(CC) -o $(EXE) $(OBJ) -lm -lncurses

.cpp.o:
	$(CC) -c $< -o $@

clean:
	rm $(OBJ) $(EXE)
