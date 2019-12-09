#include "log.h"

// char _log_buffer[256];

Log &Log::get() {
	static Log log;
	return log;
}

void Log::write(std::string str) {
	get().lines.push_back(str);
}

std::vector<std::string> Log::get_lines(int quantity=-1) {
	if (quantity < 0)
		return get().lines;
	else {
		Log &log = get();
		return std::vector<std::string>(
				log.lines.end() - std::min((int)quantity, (int)log.lines.size()),
			   	log.lines.end()
				);
	}
}

void Log::scroll_lines(int quantity) {
	Log &log = get();
	log.cursor += quantity;
}
