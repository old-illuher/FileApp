#include <iostream>
#include <fstream>


/*
Задание:
Создать массив структур, должна быть возможность ввода данных с клавиатуры /вывода массива на экран.

структура Студент имеет поля:
Имя
Фамилия
Номер группы
Массив из 5 оценок

Реализовать функции сохранения данных в файл/считывания из файла
*/

using namespace std;

char fpath[30];
const int maxCount = 100;//максимальное число строк в файле

struct Student {
    char name[30] = "\0", surname[30] = "\0", groupNum[10] = "\0";
    int grades[5] = { 0,0,0,0,0 };
};

void arrayInput(Student array[], int n);
void arrayOutput(Student array[], int n);
void writeToFile(Student array[], int mode = 1);
void readFromFile(Student array[]);
int getFileLines();
void setPath();

int main()
{
    system("chcp 1251");
    setPath();
    int records = getFileLines();
    do {
        Student* students = new Student[maxCount];  
        cout << "Выберите операцию" << endl << "r - загрузить данные из файла в массив и вывести на экран" << endl;
        cout << "w - заполнить массив и записать его в конец файла" << endl << "o - заполнить массив и перезаписать файл" << endl;
        cout << "f - сменить файл, e - выход" << endl;
        char c;
        cin >> c;
        switch (c) {
            case 'r':
                if (records == 0) {
                    cout << "В файле нет записей" << endl;
                    break;
                }
                else {
                    readFromFile(students);
                    break;
                }
            case 'w':
                writeToFile(students);
                records = getFileLines();
                break;
            case 'o':
                writeToFile(students, 2);
                records = getFileLines();
                break;
            case 'f':
                cin.ignore(100, '\n');
                setPath();
                records = getFileLines();
                break;
            case 'e':
                delete[] students;
                return 0;
                break;
            default:
                cin.ignore(100, '\n');
                break;
        }
        delete[] students;
    } while (1);
}

//установка имени файла
void setPath() {    
    cout << "Введите имя файла" << endl;
    char fname[25]; //переменные для хранения пути файла
    cin.getline(fname, sizeof(fname));//получаем имя файла без расширения
    fname[sizeof(fname) - 1] = '\0';
    strncpy_s(fpath, fname, sizeof(fpath));
    fpath[sizeof(fpath) - 1] = '\0';
    strncat_s(fpath, ".txt\0", (sizeof(fpath) - strlen(fpath)) - 1);
    if (cin.fail()) {
        cin.clear();
        cin.ignore(100, '\n');
    }
}

//функция ввода элементов массива структур
void arrayInput(Student array[], int n) {
    for (int i = 0; i < n; i++) {
        cout << "Введите имя студента" << endl;
        cin >> array[i].name;
        cin.ignore(100, '\n');//игнорируем 100 символов до переноса строки в потоке
        cout << "Введите фамилию студента" << endl;
        cin >> array[i].surname;
        cin.ignore(100, '\n');
        cout << "Введите номер группы студента" << endl;
        cin >> array[i].groupNum;
        cin.ignore(100, '\n');
        cout << "Введите оценки студента" << endl;
        //ввод оценок студента
        for (int j = 0; j < 5; j++) {
            do {
                cout << "Введите оценку (" << j + 1 << " из 5): ";
                cin >> array[i].grades[j];
                //проверка оценки (является ли числом от 2 до 5)
                if (cin.fail() || array[i].grades[j] < 2 || array[i].grades[j] > 5) {
                    cout << "Оценка должна быть числом от 2 до 5" << endl;
                    cin.clear();
                    array[i].grades[j] = 0;
                }
                cin.ignore(100, '\n');
            } while (array[i].grades[j] == 0);
        }
        //установка нулей-терминаторов в конец строк
        array[i].name[sizeof(array[i].name) - 1] = '\0';
        array[i].surname[sizeof(array[i].surname) - 1] = '\0';
        array[i].groupNum[sizeof(array[i].groupNum) - 1] = '\0';
    }
}

//функция вывода значений массива структур
void arrayOutput(Student array[], int n) {
    for (int i = 0; i < n; i++) {
        cout << "Имя студента: " << array[i].name << endl;
        cout << "Фамилия студента: " << array[i].surname << endl;
        cout << "Номер группы студента: " << array[i].groupNum << endl;
        cout << "Оценки студента:" << endl;
        for (int j = 0; j < 5; j++) {
            cout << array[i].grades[j] << " ";
        }
        cout << endl;
    }
}

//записа массива структур в файл
void writeToFile(Student array[], int mode) {
    unsigned int n=0;
    do {
        cout << "Введите количество элементов массива структур" << endl;
        cin >> n;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Пожалуйста, введите число" << endl;
        }
    } while (n == 0);
    arrayInput(array, n);
    ofstream fout;
    switch (mode) {
        case 1:
            fout.open(fpath, ios::app);//открываем на запись в конец
            break;
        case 2:
            fout.open(fpath, ios::trunc);//открыавем на перезапись
            break;
    }
    cout << "Запись в файл..." << endl;
    for (unsigned int i = 0; i < n; i++) {
        fout << array[i].name << ';' << array[i].surname << ';' << array[i].groupNum << ';';
        for (int j = 0; j < 5; j++)fout << array[i].grades[j];
        fout << endl;
    }
    fout.close();//закрываем файл
    cout << "Готово" << endl;
}

//чтение массива структур из файла
void readFromFile(Student array[]) {
    ifstream fin(fpath);//открываем файл на чтение
    cout << "Получение данных из файла..." << endl;
    int i = 0;
    bool fail = false;
    while (!fin.eof() && i<maxCount) {
        Student student_temp; //временная структура
        char test[100];
        fin.getline(test, 2);
        fin.clear();
        if (test[0] == 0)fin.setstate(3); // проверка на пустые строки
        long long pos = fin.tellg();
        fin.seekg(pos - 1); //перенос после проверки на 1 символ обратно
        fin.getline(student_temp.name, 30, ';');
        fin.getline(student_temp.surname, 30, ';');
        fin.getline(student_temp.groupNum, 10, ';');
        char grades[6];
        fin.getline(grades, 6, '\n');
        for (int j = 0; j < 5; j++) {
            student_temp.grades[j] = grades[j] - 48;
            //если оценки не в диапазоне от 2 до 5 - ошибка чтения
            if (student_temp.grades[j] > 5 || student_temp.grades[j] < 2)
                fin.setstate(3);
        }
        //если в какой-то момент произошла ошибка чтения, то текущая строка пропускается
        if (fin.fail()) {
            fail = true;
            fin.clear();
            fin.ignore(1000, '\n');            
            continue;
        }
        else {           
            array[i] = student_temp;
            i++;
        }
    }
    fin.close();//закрываем файл
    if(fail)cout << "Во время чтения некоторых строк произошла ошибка, эти строки будут пропущены" << endl;
    cout << "Вывод данных:" << endl;
    arrayOutput(array, i);
}

//получение кол-ва строк в файле
int getFileLines() {   
    int count = 0;
    ofstream fout(fpath, 1);
    //если файла не существует, то создаем его
    if (!fout.is_open()) {
        fout.open(fpath);
        fout.close();
    }
    else {
        ifstream fin(fpath);//открываем файл на чтение
        while (!fin.eof()) {
            char line[200];
            fin.getline(line, 200, '\n');
            if (line[0]==0)continue; //пустые строки пропускаются
            count++;
        }
        fin.close();
    }
    cout << "В файле " << count << " из " << maxCount << " строк" << endl;
    return count;
}