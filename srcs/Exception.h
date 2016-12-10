#pragma once

#include <iostream>
#include <sstream>
#include <exception>

class Exception : public std::exception
{
public:
	Exception(int nb = 0, std::string const& error="", int lvl = 0);
	Exception(Exception const & src);
	Exception(std::string const & msg) throw();
	virtual ~Exception() throw();
	virtual const char	*what() const throw();
	Exception& 			operator=(Exception const & src);

	int					getNb() const {return (_nb);};
	std::string			getError() const {return (_error);};
	int					getLvl() const {return (_lvl);};

private:
	int				_nb;
	std::string		_error;
	int				_lvl;
};