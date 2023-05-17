#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
class Chat;
class User;
class Message;

namespace fs = std::filesystem;

class SaveRestor
{
private:

    fs::path const Path{"/tmp"};
    fs::path const savePath{"/tmp/Chat_Yevgeniy"};
    fs::path const savePathChats{savePath / "Chats"};
    const std::string ID{"ID:"};             // identifier
    const std::string log{"login:"};         // login string
    const std::string pas{"password:"};      // password string
    const std::string name{"username:"};     // user name string
    const std::string timesend{"timesend:"}; // timesend string
    const std::string mess{"message:"};      // message string
    const std::string sep{" "};              // separator



public:
    SaveRestor();
    ~SaveRestor();
    void createPath(const fs::path &Path, const fs::path &savePath);

    std::string saveUser(User &user);
    std::shared_ptr<User> restorUser(std::string &str);

    void saveChat(std::shared_ptr<Chat> chat);
    std::shared_ptr<Chat> restorChat(fs::path path);

    std::string saveMessage(Message &message);

    void saveUsers(std::vector<User> &users);
    void restorUsers(std::vector<User> &users);

    void restorChats(std::vector<std::shared_ptr<Chat>>& chats);
    void saveChats(std::vector<std::shared_ptr<Chat>> chats);
};
