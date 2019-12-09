#pragma once

#include <ncurses.h>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <vector>

// #define Log(...) ({std::sprintf(_log_buffer, __VA_ARGS__);})

class Log {
public:
	static Log &get();
	static void write(std::string str);
	static std::vector<std::string> get_lines(int quantity);
	static void scroll_lines(int quantity);
private:
	Log(){}
	Log(Log &log){}
	~Log(){}
	std::vector<std::string> lines;
	int cursor = 0;
};
