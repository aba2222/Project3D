#pragma once
#include <exception>
#include<string>
#include <sstream>

class ExceptionHandle : public std::exception {
public:
	ExceptionHandle(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginstring() const noexcept;

private:
	int line;
	std::string file;
};

