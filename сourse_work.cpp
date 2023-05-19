#include <iostream>
#include <string>
#include <windows.h>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

enum class markType : char { Fail, Pass, Excellent, Good, Satisfactory, Bad };
enum class sex : char { Woman, Man, Any };  

struct ExamsRecords {
	string name; 
	markType mark; 
	bool isEmpty; 
};

string markToString(markType mark) {
	switch (mark) {
	case markType::Fail:
		return "Незачет";
	case markType::Pass:
		return "Зачет";
	case markType::Excellent:
		return "Отлично";
	case markType::Good:
		return "Хорошо";
	case markType::Satisfactory:
		return "Удовл.";
	case markType::Bad:
		return "Плохо";
	}
	return ""; 
}

struct studentNode  
{
	char surName[30]; 
	char name[30]; 
	char middleName[30]; 
	char birthDate[30];
	int startYear;
	char institute[30];
	char department[30];
	char group[30];
	char studentID[30];
	sex gender;
	ExamsRecords examsRecordsData[9][10];
};

class studentClass
{
private:

    vector<studentNode> students;
    studentNode st;
    void loadDataFromFile() {
        ifstream data;
        ofstream res;
        string path = "studentsData.txt";
        string way = "reserve.txt";
        data.open(path);
        res.open(way);

        if (!data.is_open() && !res.is_open())
        {
            cout << "Ошибка открытия файла" << endl;
        }
        else
        {
            string line;
            while (getline(data, line)) {
                res << line << endl;
            }
        }
        data.close();

        ifstream res2(way);
        string binary = "studentsDataUpd.bin";

        if (!res2.is_open())
        {
            cout << " Ошибка открытия файла" << endl;
        }
        else
        {
            ofstream binary_file(binary, ios::binary | ios::out);

            if (!binary_file.is_open())
            {
                cout << " Ошибка открытия файла" << endl;
            }
            else
            {
                char ch;
                while (res2.get(ch))
                {
                    binary_file.put(ch);
                }

                res2.close();
                binary_file.close();
            }
        }

        ifstream binary_file(binary, ios::binary | ios::in);

        while (binary_file.peek() != ifstream::traits_type::eof()) {
            studentNode tempStudent;
            binary_file.getline(tempStudent.surName, sizeof(tempStudent.surName));
            binary_file.getline(tempStudent.name, sizeof(tempStudent.name));
            binary_file.getline(tempStudent.middleName, sizeof(tempStudent.middleName));
            binary_file.getline(tempStudent.birthDate, sizeof(tempStudent.birthDate));
            binary_file >> tempStudent.startYear; binary_file.ignore();
            binary_file.getline(tempStudent.institute, sizeof(tempStudent.institute));
            binary_file.getline(tempStudent.department, sizeof(tempStudent.department));
            binary_file.getline(tempStudent.group, sizeof(tempStudent.group));
            binary_file.getline(tempStudent.studentID, sizeof(tempStudent.studentID));
            char gender;
            binary_file >> gender;
            switch (gender)
            {
            case 'М':
                tempStudent.gender = sex::Man;
                break;
            case 'Ж':
                tempStudent.gender = sex::Woman;
                break;
            default:
                tempStudent.gender = sex::Any;
                break;
            }
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 10; j++) {
                    char mark;
                    binary_file >> mark;
                    switch (mark)
                    {
                    case 'Н':
                        tempStudent.examsRecordsData[i][j].mark = markType::Fail;
                        break;
                    case 'З':
                        tempStudent.examsRecordsData[i][j].mark = markType::Pass;
                        break;
                    case 'О':
                        tempStudent.examsRecordsData[i][j].mark = markType::Excellent;
                        break;
                    case 'Х':
                        tempStudent.examsRecordsData[i][j].mark = markType::Good;
                        break;
                    case 'У':
                        tempStudent.examsRecordsData[i][j].mark = markType::Satisfactory;
                        break;
                    case 'П':
                        tempStudent.examsRecordsData[i][j].mark = markType::Bad;
                        break;
                    }
                    tempStudent.examsRecordsData[i][j].isEmpty = false;
                    binary_file.ignore();
                }
            }
            students.push_back(tempStudent);
        }
    }


public:

    void setDefaultData() {
        strcpy_s(st.surName, " Иванов");
        strcpy_s(st.name, " Иван");
        strcpy_s(st.middleName, " Иванович");
        strcpy_s(st.institute, " ИКБ");
        strcpy_s(st.department, " Ифнормационная безопасность");
        strcpy_s(st.group, " БББО-01-22");
        strcpy_s(st.studentID, " 11Б2233");
        strcpy_s(st.birthDate, " 27.12.1984");
        st.gender = sex::Any;
        st.startYear = 2002;
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 10; j++) {
                st.examsRecordsData[i][j].isEmpty = true;
            }
    }

        bool isBirthDateValid(const char* birthDate) {
            int dotCount = 0;
            while (*birthDate) {
                if (*birthDate == '.') {
                    dotCount++;
                    if (dotCount > 2) {
                        return false;
                    }
                }
                else if (!std::isdigit(static_cast<unsigned char>(*birthDate))) {
                    return false;
                }
                birthDate++;
            }
            return dotCount == 2;
        }

        int getYearFromBirthDate(const char* birthDate) {
            const char* yearStart = strrchr(birthDate, '.') + 1; 
            return atoi(yearStart); 
        }

        void addStudent() {
            studentNode newStudent;
            string input;

            cout << " Введите фамилию студента: ";
            cin.getline(newStudent.surName, sizeof(newStudent.surName));

            cout << " Введите имя студента: ";
            cin.getline(newStudent.name, sizeof(newStudent.name));

            cout << " Введите отчество студента: ";
            cin.getline(newStudent.middleName, sizeof(newStudent.middleName));

            while (true) {
                cout << " Введите дату рождения студента (в формате ДД.ММ.ГГГГ): ";
                cin.getline(newStudent.birthDate, sizeof(newStudent.birthDate));

                if (!isBirthDateValid(newStudent.birthDate)) {
                    cout << " Неверный формат даты рождения. Попробуйте еще раз." << endl;
                }
                else {
                    break;
                }
            }

            int birthYear = getYearFromBirthDate(newStudent.birthDate);

            while (true) {
                cout << " Введите год поступления студента: ";
                cin >> newStudent.startYear;
                cin.ignore();

                if (birthYear >= newStudent.startYear || birthYear <= 0) {
                    cout << " Неверные данные. Попробуйте еще раз." << endl;
                }
                else {
                    break;
                }
            }

            cout << " Введите институт студента: ";
            cin.getline(newStudent.institute, sizeof(newStudent.institute));

            cout << " Введите отделение студента: ";
            cin.getline(newStudent.department, sizeof(newStudent.department));

            cout << " Введите группу студента: ";
            cin.getline(newStudent.group, sizeof(newStudent.group));

            cout << " Введите ID студента: ";
            cin.getline(newStudent.studentID, sizeof(newStudent.studentID));

            cout << " Введите пол студента (M для мужского, Ж для женского, Н для неизвестного/неопределенного): ";
            char gender;
            cin >> gender; cin.ignore();
            switch (gender)
            {
            case 'М':
                newStudent.gender = sex::Man;
                break;
            case 'Ж':
                newStudent.gender = sex::Woman;
                break;
            case 'Н':
                newStudent.gender = sex::Any;
                break;
            }

            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 10; j++) {
                    cout << " Введите оценку студента для экзамена номер " << j + 1 << " в семестре номер " << i + 1 << " (Н для Незачет, З для Зачет, О для Отлично, Х для Хорошо, У для Удовл., П для Плохо): ";
                    char mark;
                    cin >> mark; cin.ignore();
                    switch (mark)
                    {
                    case 'F':
                        newStudent.examsRecordsData[i][j].mark = markType::Fail;
                        break;
                    case 'P':
                        newStudent.examsRecordsData[i][j].mark = markType::Pass;
                        break;
                    case 'E':
                        newStudent.examsRecordsData[i][j].mark = markType::Excellent;
                        break;
                    case 'G':
                        newStudent.examsRecordsData[i][j].mark = markType::Good;
                        break;
                    case 'S':
                        newStudent.examsRecordsData[i][j].mark = markType::Satisfactory;
                        break;
                    case 'B':
                        newStudent.examsRecordsData[i][j].mark = markType::Bad;
                        break;
                    }
                    newStudent.examsRecordsData[i][j].isEmpty = false;
                }
            }

            ofstream outFile("studentsData.txt", ios::app);
            if (outFile.is_open()) {
                outFile << newStudent.surName << endl;
                outFile << newStudent.name << endl;
                outFile << newStudent.middleName << endl;
                outFile << newStudent.birthDate << endl;
                outFile << newStudent.startYear << endl;
                outFile << newStudent.institute << endl;
                outFile << newStudent.department << endl;
                outFile << newStudent.group << endl;
                outFile << newStudent.studentID << endl;

                outFile.close();
            }
            else {
                cout << "Не удалось открыть файл для записи" << endl;
            }
        }



    void printInfo() {
        loadDataFromFile();
        for (const auto& st : students) {
            cout << endl << " Информация о студенте:" << endl;
            cout << " Фамилия: " << st.surName << endl;
            cout << " Имя: " << st.name << endl;
            cout << " Отчество: " << st.middleName << endl;
            cout << " Дата рождения: " << st.birthDate << endl;
            cout << " Год начала обучения: " << st.startYear << endl;
            cout << " Институт: " << st.institute << endl;
            cout << " Отделение: " << st.department << endl;
            cout << " Группа: " << st.group << endl;
            cout << " ID студента: " << st.studentID << endl;
            cout << " Пол: ";
            if (st.gender == sex::Man) { cout << "Мужской " << endl; }
            if (st.gender == sex::Woman) { cout << "Женский " << endl; }
            if (st.gender == sex::Any) { cout << "Не определен " << endl; }
            cout << " Оценки: " << endl << endl;
            for (int i = 0; i < 9; i++) {
                cout << " Семестр " << i + 1 << endl;
                cout << " -------------------------------------" << endl;
                cout << " | " << std::setw(22) << std::left << "Предмет" << " | " << std::setw(8) << std::left << "Оценка" << " |" << endl;
                cout << " -------------------------------------" << endl;
                for (int j = 0; j < 10; j++) {
                    if (!st.examsRecordsData[i][j].isEmpty) {
                        string subjects[10] = { "Физика", "Русский язык", "Иностранный язык", "Языки программирования",
                           "Мат. анализ", "Лин. алгебра", "Физкультура", "Дискретная математика", "История", "СИБ" };
                        cout << " | " << std::setw(22) << std::left << subjects[j] << " | " << std::setw(8) 
                            << std::left << markToString(st.examsRecordsData[i][j].mark) << " |" << endl;
                    }
                }
                cout << " -------------------------------------" << endl;
            }
        }
    }
};

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    studentClass student;
    student.setDefaultData();
    student.addStudent();
    student.printInfo();
    return 0;
}