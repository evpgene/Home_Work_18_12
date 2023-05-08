#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

class User {
	string _name;
	string _login;
	string _pass;

public:
	User(string name, string login, string pass):_name(name), _login(login), _pass(pass) {}
	
	friend fstream& operator >>(fstream& is, User& obj);
	friend ostream& operator <<(ostream& os, const User& obj);
};

fstream& operator >>(fstream& is, User& obj)
{
	is >> obj._name;
	is >> obj._login;
	is >> obj._pass;
	return is;
}
ostream& operator <<(ostream& os, const User& obj)
{
	os << obj._name;
	os << ' ';
	os << obj._login;
	os << ' ';
	os << obj._pass;
	return os;
}

class Message {
    string _text;
    string _sender;
    string _receiver;
};




////////////////////////////////////////////////////////////////
void demo_perms(fs::perms p)
{
    std::cout << ((p & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
        << ((p & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
        << ((p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
        << ((p & fs::perms::group_read) != fs::perms::none ? "r" : "-")
        << ((p & fs::perms::group_write) != fs::perms::none ? "w" : "-")
        << ((p & fs::perms::group_exec) != fs::perms::none ? "x" : "-")
        << ((p & fs::perms::others_read) != fs::perms::none ? "r" : "-")
        << ((p & fs::perms::others_write) != fs::perms::none ? "w" : "-")
        << ((p & fs::perms::others_exec) != fs::perms::none ? "x" : "-")
        << '\n';
}
////////////////////////////////////////////////////////////////

int main()
{


   // Создаем объект типа ifstream с именем file_reader
    ifstream file_reader( "input_file.txt" );
    
    // Проверяем, открылся ли файл, если нет, то выходим из программы
    if( ! file_reader.is_open() ) {
        cout << "Could not open input file!" << '\n';
        return 1;
    }

    int number;
    // Проверяем успешность считывания целого значения
    if ( file_reader >> number ) {
        cout << "The value is: " << number;
    }

///////////////////////////////////////////////////////////////////////////////////

	fstream user_file = fstream("users.txt", ios::in | ios::out);
	if (!user_file) 
		// Для создания файла используем параметр ios::trunc
           user_file = fstream("users.txt", ios::in | ios::out | ios::trunc);
	
	if (user_file) {
		User obj("Alex", "qwerty", "12345");
           // Запишем данные по в файл
		user_file << obj << endl;
           // Чтобы считать данные из файла, надо установить позицию для чтения в начало файла
		user_file.seekg(0, ios_base::beg);
           // Считываем данные из файла
		user_file >> obj;
		cout << obj << endl;
	}
	else
	{
		cout << "Could not open file users.txt !" << '\n';
		return 0;

	}


////////////////////////////////////////////
    std::ofstream("test.txt"); // create file

    std::cout << "Created file with permissions: ";
    demo_perms(fs::status("test.txt").permissions());

    fs::permissions("test.txt",
        fs::perms::group_write | fs::perms::others_all,
        fs::perm_options::remove);

    std::cout << "After removing g-w and o-r:  ";
    demo_perms(fs::status("test.txt").permissions());

    fs::remove("test.txt");
///////////////////////////////////////////
   // Точка - это текущий каталог, из которого запускается программа
    // Программа выведет информацию о диске, на котором этот каталог
    fs::space_info currentVolumeSpace = fs::space(".");

    std::cout << std::setw(15) << "Capacity"
        << std::setw(15) << "Free"
        << std::setw(15) << "Available"
        << "\n"
        // capacity - общий объем носителя
        << std::setw(15) << currentVolumeSpace.capacity
        // free - объём свободного места
        << std::setw(15) << currentVolumeSpace.free
        // available - объём места, доступного программе
        << std::setw(15) << currentVolumeSpace.available
        << "\n";
////////////////////////////////////////////
// определит имя текущего каталога, 
//из которого запущена программа, 
//и выведет на экран содержимое этого каталога

    fs::path path = fs::current_path();
    std::cout << path << std::endl;
    for (const auto& entry : fs::directory_iterator(path))
        std::cout << entry.path() << std::endl;
///////////////////////////////////////////


    return 0;
}