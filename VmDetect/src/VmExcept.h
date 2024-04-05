#pragma once
#include <exception>
#include <source_location>
#include <string>
#include <sstream>

namespace VmDetect
{
	class VmException : public std::exception
	{
	public:
		VmException(std::string reason, std::source_location loc = std::source_location::current());
		const char* what() const noexcept override;

	private:
		mutable std::string wBuffer;
		std::string file, func, reason;
		int line;
	};
}