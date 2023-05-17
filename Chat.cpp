#include "Chat.h"
#include <iostream>
#include <fstream>
Chat::Chat()
{
}

Chat::Chat(const std::string& chatname) :_chatname(chatname)
{
}


Chat::~Chat()
{
}

void Chat::setID(unsigned long long ID)
{
	_ID = ID;
}

void Chat::printMessage()
{
	for(auto m : _messages)
       m.printMessage();
}

void Chat::addMessage(const Message& message)
{
	_messages.push_back(message);
}

unsigned long long Chat::getID() const
{
	return _ID;
}

void Chat::printChatName() const
{
	std::cout << " " << _ID;
	std::cout << " " << _chatname << std::endl;
}


std::string Chat::getChatName() const
{
	return _chatname;
}