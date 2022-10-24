#pragma once

#include <exception>
#include <string>

class Exception : public std::exception
{

public:
	Exception(unsigned int line, const char* file);
	const char* what() const override;
	virtual const char* GetType() const;
	int GetLine() const;
	const std::string& GetFile() const;
	std::string GetOriginString() const;

private:
	unsigned int line;
	std::string file;
protected:
	mutable std::string whatBuffer;
};

