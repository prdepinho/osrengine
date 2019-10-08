#pragma once

#include <ncurses.h>
#include <sstream>
#include <cstdlib>

#define log(...) ({std::sprintf(_log_buffer, __VA_ARGS__);})
