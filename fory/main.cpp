#include <iostream>
#include <fstream>
#include <iomanip>
#include <clocale>
#include <windows.h>
#include <cstring>

using namespace std;

struct taxpayer {
    char name[180];
    char surname[180];
    double income;
    bool dec;
    int type;
};

void printbuf(taxpayer buf) {

    cout << setw(15) << buf.name << " " <<
         setw(15) << buf.surname << " " <<
         setw(6) << buf.income << " ";
    if(buf.dec == 1) cout << setw(10) << "Да ";
    else cout << setw(10) << " Нет";
    cout << setw(15);
    switch(buf.type)
    {
        case 0:
            cout << "Individual";
            break;
        case 1:
            cout << "Entity";
            break;
        case 2:
            cout << "IE";
            break;
        default:
            cout << "N/A";
            break;
    }
    cout << endl;
}

int new_list(string source, string data) {
    ifstream in;
    in.open(source.c_str(), ios::in);
    if(!in)
    {
        cout << "Ошибка открытия файла "<< source.c_str() << " для чтения в newlist()" << endl;
        return -1;
    }

    ofstream out;
    out.open(data.c_str() , ios::out | ios::binary);
    if(!out)
    {
        cout << "Ошибка открытия файла " << data.c_str() << " для записи в newlist()" << endl;
        return -1;
    }

    taxpayer buf;

    while(in >> buf.name >> buf.surname >> buf.income >> buf.dec >> buf.type)
    {
        out.write((char *) &buf, sizeof(buf));
    }
    in.close();
    out.close();
    return 1;
}

int add_to_list(string list) {
    taxpayer item;
    ofstream out;
    string input;
    out.open(list.c_str(), ios::out | ios::binary | ios::app);
    if(!out)
    {
        cout << "Ошибка открытия файла для записи" << endl;
        return -1;
    }

    cout << "Введите имя: ";
    cin >> input;
    strcpy(item.name,input.c_str());
    cout << "Введите фамилию: ";
    cin >> input;
    strcpy(item.surname,input.c_str());
    cout << "Введите доход: ";
    cin >> input;
    item.income = atof(input.c_str());
    cout << "Подана ли декларация о доходах?(1/0)";
    cin >> input;
    item.dec = (bool)atoi(input.c_str());

    type:
    cout << "Введите тип\n(0 - Физ.лицо, 1 - Юр.Лицо, 2 - ИП): ";
    cin >> input;
    item.type = atoi(input.c_str());
    if(item.type<0||item.type>2) {
        cout << "Введите значение 0,1 или 2!\n";
        goto type;
    }

    out.seekp(0, ios::end);
    out.write((char *) &item, sizeof(item));
    out.close();
    return 1;
}

void print_list(string data) {
    taxpayer buf;
    ifstream rd;
    rd.open(data.c_str(), ios::in | ios::binary);
    if(!rd)
    {
        cout << "Ошибка чтения файла " << endl;
        return;
    }

    cout << setw(19) << "Имя|" << setw(23) << "Фамилия|" <<
         setw(9) << " Доход|" << setw(5) << "Декларация|"
         << "Тип" << endl;
    cout << "-----------------------------------------------------------------------" << endl;
    rd.seekg(0, rd.beg);

    while(rd.read((char *) &buf, sizeof(buf))) {
        printbuf(buf);
    }
    cout << endl;
    rd.close();
}

int sort_by_income(string list, int num) {
    fstream file;
    file.open(list.c_str(), ios:: in | ios::binary | ios::out);
    if(!file)
    {
        cout << "Ошибка открытия файла" << endl;
        return -1;
    }

    int max_index;
    taxpayer max;
    taxpayer buffer;
    double m_inc;

    file.seekg(0, ios::end);
    int length = file.tellg()/sizeof(struct taxpayer);
    file.seekg(0, ios::beg);


    for(int i = 0; i < length - 1; i++)
    {
        max_index = i;
        file.seekg(i*sizeof(struct taxpayer), ios::beg);
        file.read((char *) &max, sizeof(max));
        m_inc = max.income;
        for(int j = i+1; j < length; j++)
        {
            file.seekg(j*sizeof(struct taxpayer), ios::beg);
            file.read((char *) &buffer, sizeof(buffer));
            if(buffer.income > m_inc) {
                max_index = j;
                m_inc = buffer.income;
            }
        }
        file.seekg(max_index*sizeof(struct taxpayer), ios::beg);
        file.read((char *) &buffer, sizeof(buffer));
        file.seekg(max_index*sizeof(struct taxpayer), ios::beg);
        file.write((char *) &max, sizeof(max));
        file.seekg(i*sizeof(struct taxpayer), ios::beg);
        file.write((char *) &buffer, sizeof(buffer));

    }
    cout << setw(19) << "Имя|" << setw(23) << "Фамилия|" <<
         setw(9) << " Доход|" << setw(5) << "Декларация|"
         << "Тип" << endl;
    cout << "-----------------------------------------------------------------------" << endl;
    file.seekg(0, ios::beg);
    for(int i = 0; i < num; i++)
    {
        file.read((char *) &buffer, sizeof(buffer));
        printbuf(buffer);
    }
    cout << endl;
    file.close();
    return 1;
}

int search_dec(string list) {
    taxpayer buf;
    ifstream in;
    in.open(list.c_str(), ios::in | ios::binary);
    if(!in)
    {
        cout << "Ошибка при открытии файла " << list << " для чтения в searchdec()";
        return -1;
    }

    cout << setw(19) << "Имя|" << setw(23) << "Фамилия|" <<
         setw(9) << " Доход|" << setw(5) << "Декларация|"
         << "Тип" << endl;
    cout << "-----------------------------------------------------------------------" << endl;
    while(in.read((char *) &buf, sizeof(buf)))
    {
        if(buf.dec == 0) printbuf(buf);
    }
    cout << endl;
    in.close();
    return 1;
}

int search_IE(string list) {
    ifstream in;
    in.open(list.c_str(), ios::out | ios::binary);
    if(!in)
    {
        cout << "Ошибка открытия файла " << list << " для чтения в searchIE()";
        return -1;
    }

    cout << setw(19) << "Имя|" << setw(23) << "Фамилия|" <<
         setw(9) << " Доход|" << setw(5) << "Декларация|"
         << "Тип" << endl;
    cout << "-----------------------------------------------------------------------" << endl;
    taxpayer buf;
    while(in.read((char *) &buf, sizeof(buf)))
    {
        if(buf.type == 2) printbuf(buf);
    }
    cout << endl;
    in.close();
    return 1;
}

int main(int argc, char **argv) {
    system("chcp 65001");
    string use, textfile_name;
    list_choice:
    int choice = 0;
    while(1) // выбор списка
    {
        cout << "1. Использовать существующий список.\n";
        cout << "2. Создать новый список.\n";
        cout << "3. Выход.\n";
        cout << ": ";
        cin >> choice;
        if(choice == 1)
        {
            cout << "Имя файла со списком: ";
            cin >> use;
            ifstream test;
            test.open(use.c_str());
            if(!test)
            {
                cout << "Такого списка нет.\n";
                test.close();
                continue;
            }
            else
            {
                cout << "Список открыт.\n";
                test.close();
                break;
            }
        }
        else if(choice == 2)
        {
            cout << "Введите имя файла списка: ";
            cin >> use;
            cout << "Имя текстового файла с Вашим списком: ";
            cin >> textfile_name;
            if(new_list(textfile_name, use))
            {
                cout << "Файл создан.\n";
                break;
            }
            else continue;
        }
        if(choice == 3)
        {
            cout << "До свидания :)\n";
            return 0;
        }
        else
        {
            cout << "Выберите пункты(1,2,3)\n";
            continue;
        }
    }
    int top;
    while(1) {
        cout << "Возможные действия:\n";
        cout << "1. Вывести список.\n";
        cout << "2. Добавить запись.\n";
        cout << "3. Вывести записи, не подавшие декларацию о доходах.\n";
        cout << "4. Вывести ИП.\n";
        cout << "5. Вывести n самых богатых налогоплательщиков.\n";
        cout << "6. Вернуться к выбору списка.\n";
        cout << "7. Выйти из программы.\n";
        cout << ": ";
        cin >> choice;
        switch(choice)
        {
            case 1:
                print_list(use);
                break;
            case 2:
                add_to_list(use);
                break;
            case 3:
                search_dec(use);
                break;
            case 4:
                search_IE(use);
                break;
            case 5:
                cout << "Введите кол-во позиций: ";
                cin >> top;
                sort_by_income(use,top);
                break;
            case 6:
                goto list_choice;
            case 7:
                cout << "До свидания :)\n";
                return 0;
            default:
                cout << "Выберите пункт(1-7)\n";
                break;
        }
        cout << "Продолжить?(1/0)\n";
        cin >> choice;
        if(choice == 0) return 0;
        else continue;
    }
}