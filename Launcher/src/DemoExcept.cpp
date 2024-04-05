#include "DemoExcept.h"

DemoException::DemoException(std::string reason, std::source_location loc) :
	reason(reason), file(loc.file_name()), func(loc.function_name()), line(loc.line())
{}

const char* DemoException::what() const noexcept
{

	std::ostringstream oss;
	oss << "Exception caught!\n"
#ifdef _DEBUG
		<< "File: " << file << "\n"
		<< "Func: " << func << "\n"
		<< "Line: " << line << "\n"
#endif
		<< "Reason: " << reason << "\n";

	wBuffer = oss.str();
	return wBuffer.c_str();
}
