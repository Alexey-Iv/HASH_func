
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
using namespace std;

//-----------------------------------------------------------------
struct UserData_H
 // datas for hashing information(when we want to check passwords)
{
    int email;
    int login;
    int password;
};


struct UserData
 //datas for making new shedule
{
    string name;
    string surname;
    string classes;
    string date;
};
//-----------------------------------------------------------------

//-----------------------------------------------------------------
namespace hashing
{
    unsigned int _hash(string s);
}

namespace sign_in
{
    void registration(struct UserData_H& ud_h, struct UserData& ud);
    void login(struct UserData_H& ud_h, struct UserData& ud);
    bool proverka_login(unsigned int login);
}

namespace information
{
    void displayInf(struct UserData_H& ud_h, struct UserData& ud);
    void display_day(string name, string day);    //func, that can be output information every day of list
    void mk_day(struct UserData& ud);
}
//-----------------------------------------------------------------


//-----------------------------------------------------------------
//-----------------------------------------------------------------
int main(int argc, char** argv)
{
    string answer = "aa";
    setlocale(LC_CTYPE, "rus");

    /*Получение дескриптора*/
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
    //SetConsoleCP(1251);         // установка кодовой страницы win-cp 1251 в поток ввода
    //SetConsoleOutputCP(1251);       // установка кодовой страницы win-cp 1251 в поток вывода

    UserData_H ud_h;
    UserData ud;

    cout << "Do you want to registrate? ";
    cin >> answer;
    cout << answer;
    if (answer == "yes" || answer == "1")
    {
        sign_in::registration(ud_h, ud);
        sign_in::login(ud_h, ud);
    }
    if (answer == "no" || answer == "0")
    {
        sign_in::login(ud_h, ud);
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


    //-----------------------------------------------------------------
    void registration(struct UserData_H& ud_h, struct UserData& ud)
    {
        string email;
        string password;
        string login;
        ofstream f("Passwords.txt", ios::app | ios::binary);
        ofstream p("Info.txt", ios::app);

        cout << "Write your e-mail: ";
        cin >> email;

        cout << "Write your new login: ";
        cin >> login;
        bool flag = proverka_login(hashing::_hash(login));

        while (!flag) {
            cout << "This login has written there. Please writer another one." << endl;
            cin >> login;
            flag = proverka_login(hashing::_hash(login));
        }
        if (flag) {

            cout << "Write your new password: ";
            cin >> password;

            if (password.length() < 7)
            {
                cout << "So small count of simvolvs in your password... Try change it" << endl;
                while (password.length() < 7) {
                    cin >> password;
                }
            }

            cout << "Write your name ";
            cin >> ud.name;

            cout << "Write your surname ";
            cin >> ud.surname;

            cout << "Write your class ";
            cin >> ud.classes;

            unsigned int hLogin = hashing::_hash(login);
            unsigned int hPassword = hashing::_hash(password);

            ud_h.login = hashing::_hash(login);
            ud_h.password = hashing::_hash(password);

            //Запись данных в файл по (по 4 байта)
            f.write((const char*)&hLogin, sizeof(unsigned int));
            f.write((const char*)&hPassword, sizeof(unsigned int));
            f.close();
            p << hLogin << " " << ud.name << " " << ud.surname << " " << ud.classes;
            p.close();
        }
    }


    void login(struct UserData_H& ud_h, struct UserData& ud)
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
            if (prime_1 == ud_h.login && prime_2 == ud_h.password) {
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
            ud_h.login = hashing::_hash(login);

            cout << "Write your password: ";
            cin >> password;
            ud_h.password = hashing::_hash(password);
            while (!f.eof()) {
                f.read((char*)&prime_1, sizeof(unsigned int));
                f.read((char*)&prime_2, sizeof(unsigned int));

                if (prime_1 == ud_h.login && prime_2 == ud_h.password) {
                    cout << "You can continue!" << endl;
                    check = true;
                    break;
                }
            }

            f.close();
        }

        if (check == true) {
            information::displayInf(ud_h, ud);
        }
        else {
            cout << "We haven't got these password or login!" << endl;
        }



    }
    //-----------------------------------------------------------------



    //-----------------------------------------------------------------
    bool proverka_login(unsigned int login)
    {
        unsigned int prime_1;
        unsigned int prime_2;

        ifstream f("Passwords.txt", ios::binary);
        while (!f.eof()) {
            f.read((char*)&prime_1, sizeof(unsigned int));
            f.read((char*)&prime_2, sizeof(unsigned int));
            cout << prime_1 << endl;
            cout << login << endl;
            if (prime_1 == login)
            {
                break;
                f.close();
                return false;
                
            }
            
        }
        f.close();
        return true;

    }
}
//-----------------------------------------------------------------


//-----------------------------------------------------------------
namespace information
{

    void displayInf(struct UserData_H& ud_h, struct UserData& ud) 
    {
        string space;
        string name;

        name = ud.name + "-" + ud.surname + "-" + ud.classes + ".txt";

        //If client wants to see his/her marks

        if (ud.date == "0" || ud.date == " ") {
            name = ".\\Classes\\Marks\\" + name;
            cout << name << endl;
            ifstream f(name.c_str(), ios::in);
            if (f.is_open()) {
                while (!f.eof()) {
                    getline(f, space);
                    cout << space << endl;
                }
                f.close();
            }
            else {
                cout << "We have not got any information for this student. Do you want to registrate your shedule?";
                //TODO ввыбор кнопок
                information::mk_day(ud);
            }
        }
        //if client wants to see his/her schedule

        else {
            name = ".\\Classes\\Schedule\\" + name;
            cout << name << endl;
            ifstream f(name.c_str(), ios::in);
            if (f.is_open()) {
                while (!f.eof()) {
                    if (ud.date == "Пн" || ud.date == "1")
                    {
                        information::display_day(name, "Monday");
                        break;
                    }
                    else if (ud.date == "Вт" || ud.date == "2")
                    {
                        information::display_day(name, "Tuesday");
                        break;
                    }
                    else if (ud.date == "Ср" || ud.date == "3")
                    {
                        information::display_day(name, "Wendsday");
                        break;
                    }
                    else if (ud.date == "Чт" || ud.date == "4")
                    {
                        information::display_day(name, "Thursday");
                        break;
                    }
                    else if (ud.date == "Пт" || ud.date == "5") {
                        information::display_day(name, "Friday");
                        break;
                    }
                    f.close();
                }
            }
            else {
                cout << "We have not got any information for this student. Do you want to registrate your shedule?";
                //TODO ввыбор кнопок
                information::mk_day(ud);
            }
        }

    }
    //-----------------------------------------------------------------



    //-----------------------------------------------------------------
    void display_day(string name, string day)
    {
        string space;
        bool reading = true;

        ifstream f(name.c_str(), ios::in);
        if (f.is_open()) {
            while (!f.eof()) {
                getline(f, space);
                if (space == day) {
                    while (space != "+") {
                        getline(f, space);
                        if (space == "+") {
                            reading = false;
                            break;
                        }
                        cout << space << endl;
                    }
                }
                if (reading == false) {
                    break;
                }
            }
        }
    }
    //-----------------------------------------------------------------




    //-----------------------------------------------------------------
    void mk_day(UserData& ud)
    {
        string s = "a";
        string ss = "b";
        string file = ".\\Classes\\Schedule\\" + ud.name + "-" + ud.surname + "-" + ud.classes + ".txt";
        ofstream f(file.c_str());
        if (f.is_open())
        {
            cout << "\tWrite your lessons. One lesson - one string\n";
            cout << "---------------------------------------------" << endl;
            cout << "For example: Russian language (406 cab, Igor Igorevich).";
            cout << "If you want to stop writing? write 0(Enter)\n";
            cin >> s;
            while (s != "0")
            {
                //writing;
                if (s == "\n"){
                    f << ss + "\n";
                }       
                cin >> s;
                ss += s;
                if (s == "\n") {
                    cout << 1;
                }
            }
        }
        f.close();
    }
}
//-----------------------------------------------------------------