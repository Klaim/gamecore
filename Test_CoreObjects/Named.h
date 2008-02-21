#ifndef NAMED_H
#define NAMED_H

#include <string>

class Named
{
private:

	std::string m_name;

public:

	const std::string& getName() const {return m_name;}

	Named(const std::string& name):m_name(name)
	{}

	virtual ~Named(){}

	
};

#endif