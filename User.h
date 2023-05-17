#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Chat.h"
#include "SaveRestor.h"

using namespace std;


class User
{
public:
	User();
	explicit User(std::string);
	User(unsigned long long ID, const std::string& login, const std::string& pass); 
	~User();
	void setID(const unsigned long long);
	void setLogin(const std::string&);
	void setPass(const std::string&);
	unsigned long long getID() const;
	std::string getLogin() const;
	std::string getPass() const;
	void printUser() const;
	void addChat(shared_ptr<Chat> chat);
	void printChatNames();

friend std::string SaveRestor::saveUser(User& user);
friend std::shared_ptr<User> restorUser(std::string& str);

private:
	std::string _login;
	std::string _pass;
	unsigned long long _ID{ 0 };

	std::vector<shared_ptr<Chat>> _chats;
	std::vector<int> _readPos; // array of chats read positions

};
