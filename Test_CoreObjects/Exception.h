#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class Exception
{
private:
	std::string message;

public:

	const std::string& msg(){return message;};

	Exception(const std::string& msg):message(msg){}
};

class ASSERTionExcetion:public Exception
{public:
ASSERTionExcetion():Exception("ASSERTion failed!"){}
};

#define EXCEPTION(msg) throw new Exception(msg)
#define ASSERT(test) if(!(test))throw new ASSERTionExcetion()

#endif