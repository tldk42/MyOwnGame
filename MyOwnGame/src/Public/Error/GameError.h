#pragma once

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <exception>

namespace GameErrorNS
{
	constexpr int FATAL_ERROR = -1;
	constexpr int WARNING = 1;
}

class GameError : public std::exception
{
public:
	GameError() : errorCode(GameErrorNS::FATAL_ERROR),
	              message("Undefined Error in game.")
	{
	}

	GameError(const GameError& Copy) : std::exception(Copy), errorCode(Copy.errorCode),
	                                message(Copy.message)
	{
	}

	GameError(int code, const std::string& s) : errorCode(code), message(s)
	{
	}

	GameError& operator=(const GameError& rhs)
	{
		std::exception::operator=(rhs);
		this->errorCode = rhs.errorCode;
		this->message = rhs.message;
	}

	virtual ~GameError()
	{
	};

	virtual const char* What() const { return this->GetMessage(); }

	const char* GetMessage() const { return message.c_str(); }
	int GetErrorCode() const { return errorCode; }

private:
	int errorCode;
	std::string message;
};
