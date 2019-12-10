
EXE = program
SRC = main.cpp dice.cpp astar.cpp log.cpp action.cpp terrain.cpp encounter.cpp game.cpp gamemode.cpp
OBJ = $(SRC:.cpp=.o)
CC = g++
FLAGS = -g

$(EXE): $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(FLAGS) -lm -lncurses -lpthread -lm

.cpp.o:
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm $(OBJ) $(EXE)
