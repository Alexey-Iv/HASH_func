
//
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
using namespace std;

//-----------------------------------------------------------------
struct UserData
{
    int email;
    int login;
    int password;
};
//-----------------------------------------------------------------


//-----------------------------------------------------------------
namespace hashing
{
    unsigned int _hash(string s);
}

namespace sign_in
{
    void registration(struct UserData& ud);
    void login(struct UserData& ud);
}

namespace information
{
    void displayInf(struct UserData& ud);
}
//-----------------------------------------------------------------


//-----------------------------------------------------------------
//-----------------------------------------------------------------
int main(int argc, char** argv)
{
    string answer = "aa";
    setlocale(LC_ALL, "Russian");

    UserData ud;

    cout << "Do you want to registrate?\n";
    cin >> answer;


    if (answer == "yes" || answer == "1")
    {
        sign_in::registration(ud);
        sign_in::login(ud);
    }
    if (answer == "no" || answer == "0")
    {
        sign_in::login(ud);
    }
    return 0;
}
//-----------------------------------------------------------------
//-----------------------------------------------------------------


//-----------------------------------------------------------------
namespace hashing
{
    unsigned int _hash(string s)
    {
        unsigned result = 0;
        for (int i = 0; i < int(s.size()); i++)
        {
            result = result * 101 + (unsigned)s[i];
        }
        return result;
    }
}
//-----------------------------------------------------------------


//-----------------------------------------------------------------
namespace sign_in
{
    void registration(struct UserData& ud)
    {
        string email;
        string password;
        string login;
        ofstream f("Passwords.txt", ios::app | ios::binary);

        cout << "Write your e-mail: ";
        cin >> email;

        cout << "Write your new login: ";
        cin >> login;

        cout << "Write your new password: ";
        cin >> password;

        unsigned int hLogin = hashing::_hash(login);
        unsigned int hPassword = hashing::_hash(password);

        ud.login = hashing::_hash(login);
        ud.password = hashing::_hash(password);
        
        //Запись данных в файл по (по 4 байта)
        f.write((const char*)&hLogin, sizeof(unsigned int));
        f.write((const char*)&hPassword, sizeof(unsigned int));
        f.close();
    }


    void login(struct UserData& ud)
    {
        ifstream f("Passwords.txt", ios::binary);

        string login; string password;
        unsigned int prime_1;
        unsigned int prime_2;
        bool check = false;

        //Automatic input information, when man had just regstrated
        while (!f.eof()) {
            f.read((char*)&prime_1, sizeof(unsigned int));
            f.read((char*)&prime_2, sizeof(unsigned int));
            if (prime_1 == ud.login && prime_2 == ud.password) {
                cout << "You can continue!" << endl;
                check = true;
                break;
            }
        }

        f.close();

        if (check == false) {
            ifstream f("Passwords.txt", ios::binary);
            cout << "Write your login: ";
            cin >> login;
            ud.login = hashing::_hash(login);

            cout << "Write your password: ";
            cin >> password;
            ud.password = hashing::_hash(password);
            cout << 1;
            while (!f.eof()) {
                f.read((char*)&prime_1, sizeof(unsigned int));
                f.read((char*)&prime_2, sizeof(unsigned int));
                cout << prime_1 << ud.login << endl;
                if (prime_1 == ud.login && prime_2 == ud.password) {
                    cout << "You can continue!" << endl;
                    check = true;
                    break;
                }
            }

            f.close();
        }

        if (check == true) {
            information::displayInf(ud);
        }
        else {
            cout << "We haven't got these password or login!" << endl;
        }



    }
}
//-----------------------------------------------------------------


//-----------------------------------------------------------------
namespace information
{
    void displayInf(struct UserData& ud) {
        string name;
        string surname;
        string date;
        string space;
        string clas;

        cout << "Write your name, please: ";
        cin >> name;

        cout << "Write yor surname, please: ";
        cin >> surname;

        cout << "Write yor class, please: ";
        cin >> clas;

        cout << "Write your date, please: ";
        cin >> date;

        name = name + "-" + surname + "-" + clas + ".txt";

        //If client wants to see his/her marks

        if (date == "0" || date == " ") {
            name = ".\\..\\..\\" + name;
            cout << name << endl;
            ifstream f(name.c_str(), ios::in);
            if (f.is_open()) {
                while (!f.eof()) {
                    getline(f, space);
                    cout << space << endl;
                }
                f.close();
            }
        }
        //if client wants to see his/her schedule

        else {
            name = ".\\Classes\\Schedule\\" + name;
            cout << name << endl;
            ifstream f(name.c_str(), ios::in);
            if (f.is_open()) {
                while (!f.eof()) {
                    if (date == "Пн" || date == "1") {
                        getline(f, space);
                        if (space == "Понедельник") {
                            while (space != "\n") {
                                getline(f, space);
                                cout << space << endl;
                            }
                        }
                    }
                    else if (date == "Вт" || date == "2") {
                        getline(f, space);
                        if (space == "Вторник") {
                            while (space != "\n") {
                                getline(f, space);
                                cout << space << endl;
                            }
                        }
                    }
                    else if (date == "Ср" || date == "3") {
                        getline(f, space);
                        if (space == "Среда") {
                            while (space != "\n") {
                                getline(f, space);
                                cout << space << endl;
                            }
                        }
                    }
                    else if (date == "Чт" || date == "4") {
                        getline(f, space);
                        if (space == "Четверг") {
                            while (space != "\n") {
                                getline(f, space);
                                cout << space << endl;
                            }
                        }
                    }
                    else if (date == "Пт" || date == "5") {
                        getline(f, space);
                        if (space == "Пятница") {
                            while (space != "\n") {
                                getline(f, space);
                                cout << space << endl;
                            }
                        }
                    }
                    f.close();
                }
            }
            else {
                cout << "We have not got any information for this student. Do you want to registrate your shedule?";
                //TODO ввыбор кнопок
            }
        }

    }

    //finction, that makes one day of schedule
    void mk_day(string personalinf) {
        /*
        * personalinf - [0] - name
        *               [1] - surname
        *               [2] - class
        *               [3] - date
        */
        //TODO make name file (from massiv)
        string file = personalinf[0] + "-" + personalinf[1] + personalinf[2];
        fstream f(file.c_str());
        if (f.is_open()) {
            //TODO: make func that can schedule for person(all schedule!)
        }
    }
}
//-----------------------------------------------------------------