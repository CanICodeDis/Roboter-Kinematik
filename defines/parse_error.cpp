#include "parse_error.h"

const char* parse_error::a(const char* m, int a) {
	msg = (char*)malloc(strlen(m)+10);
	if (msg != NULL) {
		sprintf(msg, "%s @%i", m, a);
		return msg;
	} else
		return m;
}

const char* parse_error::what() const throw() {
	return std::runtime_error::what();
}

