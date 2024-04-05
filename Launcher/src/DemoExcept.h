#pragma once
#include <exception>
#include <source_location>
#include <string>
#include <sstream>


class DemoException : public std::exception
{
public:
	DemoException(std::string reason, std::source_location loc = std::source_location::current());
	const char* what() const noexcept override;

private:
	mutable std::string wBuffer;
	std::string file, func, reason;
	int line;
};