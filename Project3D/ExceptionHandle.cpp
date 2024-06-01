#include "ExceptionHandle.h"

ExceptionHandle::ExceptionHandle(int line, const char* file) noexcept 
				: line(line),
				  file(file) {

}

const char* ExceptionHandle::what() const noexcept {
	return GetFile().c_str();
	//return GetLine().c_str();
}

const char* ExceptionHandle::GetType() const noexcept {
	return "Window Exception";
}

int ExceptionHandle::GetLine() const noexcept {
	return line;
}

const std::string& ExceptionHandle::GetFile() const noexcept {
	return file;
}

std::string ExceptionHandle::GetOriginstring() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl << GetLine() << std::endl << GetFile() << GetOriginstring();
	return oss.str();
}