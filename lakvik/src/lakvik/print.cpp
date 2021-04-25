#include "lakvik\print.h"
namespace lakvik
{
	void print(std::ostream & stream, const std::string & value)
	{
		stream << '"' << value << '"';
	}
}