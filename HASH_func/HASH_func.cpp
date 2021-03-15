
//
#include <iostream>
#include <string>
#include <fstream>
using namespace std;


struct UserData
{
    int email;
    int login;
    int password;
};


// Define my functions
unsigned int _hash(string s);
void registration(struct UserData& ud);
void login(struct UserData& ud);
void displayInf(struct UserData& ud);




int main(int argc, char** argv)
{
    string answer = "aa";
    setlocale(LC_ALL, "Russian");


    UserData ud;


    cout << "Do you want to registrate?\n";
    cin >> answer;


    if (answer == "yes" || answer == "1")
    {
        registration(ud);
        login(ud);
    }
    if (answer == "no" || answer == "0")
    {


        login(ud);
    }


    return 0;
}


unsigned int _hash(string s)
{
    unsigned result = 0;
    for (int i = 0; i < int(s.size()); i++)
    {
        result = result * 101 + (unsigned)s[i];
    }
    return result;
}


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

    unsigned int hLogin = _hash(login);
    unsigned int hPassword = _hash(password);

    ud.login = _hash(login);
    cout << ud.login << endl;
    ud.password = _hash(password);

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
        ud.login = _hash(login);

        cout << "Write your password: ";
        cin >> password;
        ud.password = _hash(password);
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
        displayInf(ud);
    }
    else {
        cout << "We haven't got these password or login!" << endl;
    }



}


void displayInf(struct UserData& ud) {
    string name;
    string date;
    string space;

    cout << "Write your name, please: ";
    cin >> name;

    cout << "Write your date, please: ";
    cin >> date;
    //ShellExecute(NULL, "open", "Classes", NULL, NULL, SW_RESTORE);
    name = name + ".txt";
    ifstream f(name.c_str());
    while (!f.eof()) {
        getline(f, space);
        cout << space << endl;
    }

}