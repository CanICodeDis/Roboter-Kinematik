#include <stdexcept>
#ifndef __PARSE_ERROR_H__
#define __PARSE_ERROR_H__

#include <cstring>

class parse_error : public std::runtime_error {
	int line;
	char* msg;
	const char* a(const char* m, int a);
public:
	inline parse_error( const char* message, int at )
			:line(at), std::runtime_error(a(message,at)) 
	{
	}
  inline int getLine() {
		return line;
	}
	
	virtual const char* what() const throw();

};

#endif
