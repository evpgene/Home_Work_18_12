#include "SaveRestor.h"
#include "User.h"
#include "Message.h"

SaveRestor::SaveRestor()
{
}

SaveRestor::~SaveRestor()
{
}

void SaveRestor::createPath(const fs::path &Path, const fs::path &savePath)
{
	if (!fs::exists(Path))
		fs::create_directory(Path);
	if (!fs::exists(savePath))
		fs::create_directory(savePath);
	if (fs::exists(savePath))
	{
		fs::permissions(
			savePath,
			fs::perms::owner_all,
			fs::perm_options::add);
		fs::permissions(
			savePath,
			fs::perms::owner_exec | fs::perms::others_all | fs::perms::group_all,
			fs::perm_options::add);
	}
	else
	{
	}
};

std::string SaveRestor::saveUser(User &user)
{
	return ID + sep + to_string(user._ID) + sep + log + sep + user._login + sep + pas + sep + user._pass;
};

void SaveRestor::saveUsers(std::vector<User> &users)
{
	// write
	std::string filename{savePath / "Users"};
	std::fstream s{filename, s.binary | s.trunc | s.in | s.out};

	s.clear();

	if (!s.is_open())
		std::cout << "failed to open " << filename << '\n';
	else
	{
		for (auto user : users)
		{
			s << saveUser(user) << endl;
		}
	}
	s.close();
}

std::shared_ptr<User> SaveRestor::restorUser(std::string &str)
{

	bool nextIsID{false}, doneID{false}, nextIsLogin{false}, doneLogin{false}, nextIsPass{false}, donePass{false};
	std::string word;
	shared_ptr<User> user = make_shared<User>(User());
	std::istringstream iss(str);
	while (iss >> word)
	{
		std::cout << word << std::endl;
		if (nextIsID)
		{
			user->setID(stoll(word));
			nextIsID = false;
			doneID = true;
		};
		if (nextIsLogin)
		{
			user->setLogin(word);
			nextIsLogin = false;
			doneLogin = true;
		};
		if (nextIsPass)
		{
			user->setPass(word);
			nextIsPass = false;
			donePass = true;
		};
		if (word == ID)
		{
			nextIsID = true;
		}
		if (word == log)
		{
			nextIsLogin = true;
		}
		if (word == pas)
		{
			nextIsPass = true;
		}
	}
	if (doneID && doneLogin /*  && donePass */) // donePass может отсутствовать для общего чата
	{
		return user;
	}
	return nullptr;
}

void SaveRestor::restorUsers(std::vector<User> &users)
{
	// read
	std::string filename{savePath / "Users"};
	std::fstream s{filename};

	if (!s.is_open())
		std::cout << "failed to open " << filename << '\n';
	else
	{
		s.seekg(0);
		std::cout << "opened " << filename << '\n';
		for (std::string line; std::getline(s, line);)
		{
			std::cout << line << '\n'; // для диагностики - можно убрать
			shared_ptr<User> restorUser_ptr = restorUser(line);
			if (restorUser_ptr)
				users.push_back(*restorUser_ptr);
			else
				std::cout << "User one not restored";
		}
	}
	s.close();
}

std::string SaveRestor::saveMessage(Message &message)
{
	return name + sep + message._userName + '\n' + timesend + sep + message._timeSend + '\n' + mess + sep + message._message;
};

void SaveRestor::saveChat(std::shared_ptr<Chat> chat)
{
	// write
	std::cout << savePathChats / chat->getChatName() << std::endl;
	std::string filename{savePathChats / chat->getChatName()};
	std::fstream s{filename, s.binary | s.trunc | s.out};

	s.clear();

	if (!s.is_open())
		std::cout << "failed to open " << filename << '\n';
	else
	{
		for (auto message : chat->_messages)
		{
			std::cout << saveMessage(message) << std::endl;
			s << saveMessage(message) << endl;
		}
	}
	s.close();
};

std::shared_ptr<Chat> SaveRestor::restorChat(fs::path path)
{

	std::string filename{path};
	std::fstream t{filename};

	if (!t.is_open())
		{std::cout << "failed to open " << filename << '\n';
		return nullptr;}
	else
	{
		t.seekg(0);
		std::cout << "opened " << filename << '\n';		   // для диагностики - можно убрать
		std::cout << "opened " << path.filename() << '\n'; // для диагностики - можно убрать
		shared_ptr<Chat> restor = make_shared<Chat>(Chat(path.filename()));

		for (std::string line; std::getline(t, line);)
		{
			std::string isName, isTime, isMessage;
			bool doneName{false}, doneTime{false}, doneMessage{false};
			std::size_t pos = line.find(name);
			if (pos != std::string::npos)
			{
				isName = line.substr(name.size() + sep.size());
				doneName = true;
			};
			pos = line.find(timesend);
			if (pos != std::string::npos)
			{
				isTime = line.substr(timesend.size() + sep.size());
				doneTime = true;
			};
			pos = line.find(mess);
			if (pos != std::string::npos)
			{
				isMessage = line.substr(mess.size() + sep.size());
				doneMessage = true;
			};
			if (doneName && doneTime && doneMessage)
			{
				restor->_messages.push_back(Message(isName, isTime, isMessage));
			}
		}
		t.close();
		return restor;
	}
}

void SaveRestor::restorChats(std::vector<std::shared_ptr<Chat>>& chats)
{
	for (auto const &dir_entry : std::filesystem::directory_iterator{savePathChats})
	{
	fs::path filename = dir_entry;	
	//std::string filename{file};
	std::fstream t{filename};

	if (!t.is_open())
		std::cout << "failed to open " << filename << '\n';
	else
	{
		t.seekg(0);
		std::cout << "opened " << filename << '\n';		   // для диагностики - можно убрать
		//std::cout << "opened " << path.filename() << '\n'; // для диагностики - можно убрать
		std::string isName, isTime, isMessage;

		shared_ptr<Chat> restoringChat = make_shared<Chat>(Chat(dir_entry.path().filename()));
		bool doneName{false}, doneTime{false}, doneMessage{false};
		for (std::string line; std::getline(t, line);)
		{
			
			std::size_t pos = line.find(name);
			if (pos != std::string::npos)
			{
				isName = line.substr(name.size() + sep.size());
				doneName = true;
				std::cout << "isName: " << isName << std::endl;
			};
			pos = line.find(timesend);
			if (pos != std::string::npos)
			{
				isTime = line.substr(timesend.size() + sep.size());
				doneTime = true;
				std::cout << "isTime: " << isTime << std::endl;
			};
			pos = line.find(mess);
			if (pos != std::string::npos)
			{
				isMessage = line.substr(mess.size() + sep.size());
				doneMessage = true;
				std::cout << "isMessage: " << isMessage << std::endl;
			};
			if (doneName && doneTime && doneMessage)
			{
				restoringChat->addMessage((Message(isName, isTime, isMessage)));
				chats.push_back(restoringChat);
				doneName = false;
				doneTime = false;
				doneMessage = false;

			}
		}
		t.close();
	}
	}
}

void SaveRestor::saveChats(std::vector<std::shared_ptr<Chat>> chats)
{
	for (auto chat : chats)
		saveChat(chat);
};