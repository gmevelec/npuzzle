#include "Exception.h"

Exception::Exception(int nb, std::string const& error, int lvl):
_nb(nb),
_error(error),
_lvl(lvl)
{
}

Exception::~Exception() throw()
{
}

const char* 	Exception::what() const throw()
{
	std::string str;
	str += this->_error;
	return (str.c_str());
}


Exception& 		Exception::operator=(Exception const & rhs)
{
	this->_nb = rhs.getNb();
	this->_error = rhs.getError();
	this->_lvl = rhs.getLvl();
	return *this;
}

