#pragma once

#include <ncurses.h>
#include <sstream>
#include <cstdlib>

char buffer[256];

#define log(...) ({std::sprintf(buffer, __VA_ARGS__); mvprintw(0, 0, buffer);})
