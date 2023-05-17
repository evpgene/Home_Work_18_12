#pragma once
#include <string>
#include "SaveRestor.h"
class Message
{
public:
	Message();

	Message(std::string& username, std::string& timesend, std::string& message ) : 
	_userName(username), 
	_timeSend(timesend), 
	_message(message)
	{};

	~Message();
	void userName(const std::string& userName);
	void setMessage(const std::string& message);
	void setTimeSend(const std::string& time);
	void printMessage() const;

	friend std::string SaveRestor::saveMessage(Message& message);

private:
	std::string _userName;
	std::string _timeSend;
	std::string _message;
	std::string _timeRead;
};

