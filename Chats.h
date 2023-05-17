#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "User.h"
#include "Message.h"
#include "SaveRestor.h"



using namespace std;

class Chats
{
	// переменные
	bool Q{false};
	string userName;
	string pass;
	int cmd{0};
	std::string cmd_input;
	char choice{'n'};
	vector<User> users;						  // массив пользователей
	vector<shared_ptr<Chat>> chats;			  // массив указателей на чаты
	Message messageTmp;						  // сообщение
	User userTmp;							  // временный пользователь
	shared_ptr<User> currentUserPtr{nullptr}; // указатель на текущего пользователя
	shared_ptr<Chat> currentChatPtr{nullptr}; // указатель на текущий чат

public:
	Chats()
	{

	};

	~Chats(){};

	friend void SaveRestor::saveUsers(std::vector<User>& users);
	
	// Restore from files
	void function10()
	{
		SaveRestor restor;
		restor.createPath("/tmp", "/tmp/Chat_Yevgeniy");
		restor.createPath("/tmp", "/tmp/Chat_Yevgeniy/Chats");
		restor.restorUsers(users);
		restor.restorChats(chats);
	}

	//Save to files
	void function11()
	{
		SaveRestor save;
		save.createPath("/tmp", "/tmp/Chat_Yevgeniy");
		save.saveUsers(users);
		save.saveChats(chats);
	}

	// Main menu
	void function0()
	{
		bool commonUserExist {false}; // Пользователь общий уже есть
		for (auto user : users)
		{
			if(user.getLogin() == "Общий")
				commonUserExist = true;
		}

		if (!commonUserExist)
		{
			currentChatPtr = make_shared<Chat>(Chat("Общий"));

			// создаём пользователя для общего чата
			userTmp.setID(static_cast<unsigned long long>(users.size()) + 1); // формируем ID
			userTmp.setLogin("Общий");
			userTmp.setPass("_");
			users.push_back(userTmp);
		}

		while (!Q) // цикл
		{
			// если есть текущий пользователь - то распечатываем его
			if (currentUserPtr)
			{
				std::cout << "Активный пользователь: ";
				currentUserPtr->printUser();
			}

			std::cout << "Введите комманду (0 - помощ)" << std::endl;
			std::cin >> cmd_input;

			// проверяем корректность ввода
			try
			{
				cmd = std::stoi(cmd_input);
			}
			catch (exception &except)
			{
				cout << endl
					 << except.what() << endl;
				cmd = 0;
			}

			try // обработка исключений
			{
				switch (cmd)
				{
				case 0:
					std::cout << "0 - помощь" << std::endl;
					std::cout << "1 - вывести данные текущего пользователя" << std::endl;
					std::cout << "2 - авторизоваться" << std::endl;
					std::cout << "3 - написать сообщение" << std::endl;
					std::cout << "8 - выйти из учётной записи" << std::endl;
					std::cout << "9 - выйти из программы" << std::endl;
					std::cout << "Имя пользователя должно состоять из одного слова" << std::endl;
					break;

				case 1: // выводим  данные текущего пользователя
					function1();
					break;
				case 2: // register user (check user)
					function2();
					break;
				case 3: // Написать пользователю
					function3();
					break;
				case 8: // logoff
					function8();
					break;
				case 9: // Выход из программы
					function9();
					break;
				default:
					std::cout << "Нет такой команды: " << cmd << std::endl;
					break;
				}
			}
			catch (exception &except)
			{
				cout << endl
					 << except.what() << endl;
			}
		}
	}
	// выводим  данные текущего пользовател
	void function1()
	{
		if (currentUserPtr)
		{
			std::cout << "Активный пользователь: ";
			currentUserPtr->printUser();
			std::cout << endl;
			std::cout << "Чаты активного пользователя: " << std::endl;
			currentUserPtr->printChatNames();
		}
		// распечатываем имена всех зарегистрированных пользователей
		if (users.size() > 0)
		{
			std::cout << "Зарегистрированные пользователи: " << std::endl; // users
			for (const auto &u : users)
				u.printUser(); // print();
			std::cout << endl;
		}
		else
		{
			std::cout << "Нет зарегистрированных пользователей! " << std::endl;
		}
	}
	// register user (check user)
	void function2()
	{
		if (currentUserPtr)
		{
			std::cout << "Активный пользователь: ";
			currentUserPtr->printUser();
		}
		if (currentChatPtr)
		{
			std::cout << "Активный чат: ";
			currentChatPtr->printChatName();
		}
		{
			// int foundElement{ 0 }; // временная переменная для индекса найденного пользователя
			std::cout << "Введите ваше имя" << std::endl; // login
			std::cin >> userName;
			std::cout << userName << endl;
			userTmp.setLogin(userName);

			int found_indx{-1};

			for (size_t i = 0; i < users.size(); i++)
			{
				if (users[i].getLogin() == userTmp.getLogin())
				{
					found_indx = i;
					break;
				}
			}

			// ищем пользователя с заданным логином
			if (found_indx >= 0) // если пользователь найден
			{
				// ввод пароля
				std::cout << "Введите пароль: " << std::endl;
				std::string tmp_pass;
				std::cin >> tmp_pass;
				if (users[found_indx].getPass() == tmp_pass)
				{
					// получаем указатель на найденного пользователя
					currentUserPtr = make_shared<User>(users[found_indx]); //&users[found_indx];
					// выводим - залогинен как...
					std::cout << "Вы вошли как: ";
					currentUserPtr->printUser();
				}
				else
				{
					std::cout << "Вы ввели неверный пароль: " << std::endl;
				}
			}
			else // если пользователь не найден
			{
				std::cout << "Нет такого пользователя" << std::endl;
				std::cout << "Зарегистрировать пользователя " << userTmp.getLogin() << "? (y/n)" << std::endl;
				std::cin >> choice;
				if (choice == 'y' || choice == 'Y' /* || choice == 'н' || choice == 'Н' */) // зарегистрировать пользователя с указанным логином
				{
					std::cout << "Введите ваш пароль: " << std::endl;
					std::cin >> pass;

					userTmp.setPass(pass);											  // устанавливаем пароль
					userTmp.setID(static_cast<unsigned long long>(users.size()) + 1); // формируем ID

					users.push_back(userTmp); // добавляем пользователя в массив пользователей

					// ищем добавленного пользователя в массиве
					found_indx = -1;
					for (size_t i = 0; i < users.size(); i++)
					{
						if (users[i].getLogin() == userTmp.getLogin())
						{
							found_indx = i;
							break;
						}
					}

					if (found_indx >= 0) // если пользователь найден
					{
						currentUserPtr = make_shared<User>(users[found_indx]); // получаем указатель на найденного добавленного пользователя и делаем его текущим пользователем
					}
				}
				else if (choice == 'n')
				{
				}
			}
		}
	}
	// Написать пользователю
	void function3()
	{
		// Выводим имя активного пользователя
		if (currentUserPtr)
		{
			std::cout << "Активный пользователь: ";
			currentUserPtr->printUser();

			// выводим имена всех пользователей
			std::cout << "Зарегистрированные пользователи: " << std::endl;
			for (const auto &u : users)
				u.printUser();

			{
				int found_indx{0}; // временная переменная найденный элемент
				int id;			   // временная переменная идентификатор
				std::string id_input;
				std::cout << "Выберите пользователя для чата (введите номер) " << std::endl;
				std::cin >> id_input;
				// проверяем корректность ввода
				try
				{
					id = std::stoi(id_input);
				}
				catch (exception &except)
				{
					cout << endl
						 << except.what() << endl;
					id = -1;
				}

				// поиск пользователя с заданным id
				for (size_t i = 0; i < users.size(); i++)
				{
					if (users[i].getID() == id)
					{
						found_indx = i;
						break;
					}
				}

				if (found_indx != -1)
				{
					// вывод найденного пользователя для отслеживания поведения программы
					std::cout << "Пользователь найден: ";
					users[found_indx].printUser();
				}
				else
				{
					std::cout << "Нет такого пользователя! " << std::endl;
				}

				std::string chatName;	// имя чата первая комбинация
				std::string chatName_2; // имя чата вторая комбинация
				if (currentUserPtr)
				{
					if (users[found_indx].getLogin() == "Общий") // имя пользователя - Общий и имя чата задаём тоже Общий.
					{
						chatName = "Общий";
					}
					else
					{
						// формирования имени чата первая часть
						chatName = currentUserPtr->getLogin();
						chatName += users[found_indx].getLogin();
					}
					// формирования имени чата вторая часть
					chatName_2 = users[found_indx].getLogin();
					chatName_2 += currentUserPtr->getLogin();
				}

				{
					int indx{-1};
					for (size_t i = 0; i < chats.size(); i++)
					{
						if (chats[i]->getChatName() == chatName)
						{
							indx = i;
							break;
						}
					}

					int indx_2{-1};

					for (size_t i = 0; i < chats.size(); i++)
					{
						if (chats[i]->getChatName() == chatName_2)
						{
							indx_2 = i;
							break;
						}
					}

					if (!(indx == -1))
					{
						currentChatPtr = chats[indx];
						std::cout << "Такой чат уже существует: " << currentChatPtr->getChatName() << std::endl;
						currentChatPtr->printMessage();
					}
					else if (!(indx_2 == -1))
					{
						currentChatPtr = chats[indx_2];
						std::cout << "Такой чат уже существует: " << currentChatPtr->getChatName() << std::endl;
						currentChatPtr->printMessage();
					}
					else // если такого чата нет - создаём чат и добавляем указатели на него в список чатов каждого пользователя
					{
						currentChatPtr = make_shared<Chat>(chatName);
						chats.push_back(currentChatPtr);
						currentUserPtr->addChat(currentChatPtr);						// добавляем чат в текущего пользователя
						if (currentUserPtr->getLogin() != users[found_indx].getLogin()) // если собеседник не сам текущий пользователь
						{
							users[found_indx].addChat(currentChatPtr); // добавляем чат в пользователя собеседника
						}
					}

					// послать сообщение
					{
						// формируем сообщение
						std::string tmpString;
						std::cout << "Введите ваше сообщение: "; //<< std::endl;
						cin.ignore();
						std::getline(std::cin, tmpString);

						// текущие дата/время основываясь на текущей системе
						time_t now = time(0);
						// преобразуем now в формат string
						char dt[26];
						ctime_r(&now, dt);
						dt[24] = ' ';
						std::string str = dt;
						Message currentMessage;
						currentMessage.setMessage(tmpString);
						currentMessage.setTimeSend(str);
						if (currentUserPtr)
						{
							currentMessage.userName(currentUserPtr->getLogin());
						}
						currentChatPtr->addMessage(currentMessage);
						currentChatPtr->printMessage(); // закоментить или убрать
						std::cout << endl;
					}
				}
			}
		}
		else
		{
			std::cout << "Вы не зарегистрированы! " << std::endl;
		}
	}
	// logoff
	void function8()
	{
		if (currentUserPtr)
		{
			std::cout << "Вы вышли из учётной записи";
			currentUserPtr->printUser();
			std::cout << std::endl;
			currentUserPtr = nullptr;
		}
		else
		{
			std::cout << "Прежде чем выйти из учётной записи необходимо в неё войти. Вы не вошли в учётую запись." << std::endl;
		}
	}
	// Выход из программы
	void function9()
	{
		Q = true;

	}
};
