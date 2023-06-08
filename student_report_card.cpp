#include <iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <thread>

class Student
{
private:
    int rollNo;
    char name[50];
    int p_marks, c_marks, m_marks, e_marks, cs_marks;
    double per;
    char grade;

public:
    void getData();
    void showData() const;
    void showTabular() const;
    int getRollNo() const;
};

void Student::getData()
{
    std::cout << "\nEnter The roll number of the student: ";
    std::cin >> rollNo;
    std::cin.ignore();

    std::cout << "Enter The Name of the student: ";
    std::cin.getline(name, 50);

    std::cout << "Enter The marks in Physics out of 100: ";
    std::cin >> p_marks;

    std::cout << "Enter The marks in Chemistry out of 100: ";
    std::cin >> c_marks;

    std::cout << "Enter The marks in Mathematics out of 100: ";
    std::cin >> m_marks;

    std::cout << "Enter The marks in English out of 100: ";
    std::cin >> e_marks;

    std::cout << "Enter The marks in Computer Science out of 100: ";
    std::cin >> cs_marks;

    per = (p_marks + c_marks + m_marks + e_marks + cs_marks) / 5.0;

    if (per >= 90)
        grade = 'A';
    else if (per >= 80)
        grade = 'B';
    else if (per >= 70)
        grade = 'C';
    else if (per >= 60)
        grade = 'D';
    else if (per >= 40)
        grade = 'E';
    else
        grade = 'F';
}

void Student::showData() const
{
    std::cout << "\nRoll number: " << rollNo;
    std::cout << "\nName: " << name;
    std::cout << "\nMarks in Physics: " << p_marks;
    std::cout << "\nMarks in Chemistry: " << c_marks;
    std::cout << "\nMarks in Mathematics: " << m_marks;
    std::cout << "\nMarks in English: " << e_marks;
    std::cout << "\nMarks in Computer Science: " << cs_marks;
    std::cout << "\nPercentage: " << per;
    std::cout << "\nGrade: " << grade;
}

void Student::showTabular() const
{
    std::cout << rollNo << "   " << name << "   " << p_marks << "   " << c_marks << "   "
              << m_marks << "   " << e_marks << "   " << cs_marks << "   " << per << "   " << grade << std::endl;
}

int Student::getRollNo() const
{
    return rollNo;
}

std::fstream file;
Student student;

void writeStudent()
{
    try
    {
        std::ofstream file;
        file.open("student.dat", std::ios::out | std::ios::app);

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open the file.");
        }

        Student student;
        student.getData();

        // Write student data to the file
        file.write(reinterpret_cast<char*>(&student), sizeof(Student));

        file.close();
        std::cout << "\n\nStudent record has been created.\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }

    std::cin.ignore();
    std::cin.get();
}

void displayAll()
{
    file.open("student.dat", std::ios::in);
    while (file.read(reinterpret_cast<char*>(&student), sizeof(Student)))
    {
        student.showData();
        std::cout << "\n====================================\n";
    }
    file.close();
    std::cin.ignore();
    std::cin.get();
}

void displaySpecific(int n)
{
    bool found = false;
    file.open("student.dat", std::ios::in);
    while (file.read(reinterpret_cast<char*>(&student), sizeof(Student)))
    {
        if (student.getRollNo() == n)
        {
            student.showData();
            found = true;
        }
    }
    file.close();
    if (!found)
        std::cout << "\n\nRecord not found.";
    std::cin.ignore();
    std::cin.get();
}

void modifyStudent()
{
    int rollNumber;
    bool found = false;

    file.open("student.dat", std::ios::in | std::ios::out);
    if (!file)
    {
        std::cout << "Error opening the file." << std::endl;
        return;
    }

    std::cout << "Enter the roll number of the student to modify: ";
    std::cin >> rollNumber;

    while (file.read(reinterpret_cast<char*>(&student), sizeof(Student)) && !found)
    {
        if (student.getRollNo() == rollNumber)
        {
            student.showData();
            std::cout << "\nEnter the new details of the student" << std::endl;
            student.getData();

            // Get the position of the current record
            std::streampos position = file.tellg();
            position -= static_cast<std::streampos>(sizeof(Student));

            // Move the file pointer to the position of the current record
            file.seekp(position);

            // Write the modified student record to the file
            file.write(reinterpret_cast<char*>(&student), sizeof(Student));

            std::cout << "\n\nRecord has been modified.";
            found = true;
        }
    }

    file.close();

    if (!found)
        std::cout << "\n\nRecord not found.";

    std::cin.ignore();
    std::cin.get();
}
void deleteStudent()
{
    int n;
    std::cout << "\nEnter the roll number of the student to delete: ";
    std::cin >> n;
    file.open("student.dat", std::ios::in | std::ios::out);
    if(!file)
    {
        std::cout<<"Error to opening the file"<<std::endl;
        return;
    }
    std::fstream file2;
    file2.open("temp.dat", std::ios::out);
    if(!file)
    {
        std::cout<<"Error to opening the tempory file";
        file.close();
        return;
    }
    file.seekg(0, std::ios::beg);
    bool recordfound=false;
    while (file.read(reinterpret_cast<char*>(&student), sizeof(Student)))
    {
        if (student.getRollNo() != n)
        {
            file2.write(reinterpret_cast<char*>(&student), sizeof(Student));
        }
        else
        {
            recordfound=true;
        }
    }
    file2.close();
    file.close();
    if (!recordfound)
    {
        std::cout << "\n\nRecord not found.";
        std::remove("temp.dat");
    }
    else
    {
        std::remove("student.dat");
        std::rename("temp.dat", "student.dat");
        std::cout << "\n\nRecord has been deleted.";
    }

    std::cin.ignore();
    std::cin.get();
}

void classResult()
{
    file.open("student.dat", std::ios::in);
    if (!file)
    {
        std::cout << "File could not be opened! Press any key...";
        std::cin.ignore();
        std::cin.get();
        return;
    }

    std::cout << "\n\n\t\tALL STUDENTS RESULT \n\n";
    std::cout << "====================================================\n";
    std::cout << "R.No.  Name        P   C   M   E   CS  %age   Grade" << std::endl;
    std::cout << "====================================================\n";

    bool recordsFound = false;  // Flag to track if any records were found

    while (file.read(reinterpret_cast<char*>(&student), sizeof(Student)))
    {
        student.showTabular();
        recordsFound = true;  // Set the flag to true if at least one record is found
    }

    file.close();
    std::cin.ignore();
    std::cin.get();

    if (!recordsFound)
    {
        std::cout << "\nNo student records found." << std::endl;
        return;
    }
}


void result()
{
    int n;
    std::cout << "\nEnter roll number of the student to display result: ";
    std::cin >> n;

    bool found = false;
    file.open("student.dat", std::ios::in);
    if(!file)
    {
        std::cout<<"Error to opening the file"<<std::endl;
        return;
    }
    while (file.read(reinterpret_cast<char*>(&student), sizeof(Student)))
    {
        if (student.getRollNo() == n)
        {
            student.showData();
            found = true;
        }
    }
    file.close();
    if (!found)
        std::cout << "\n\nRecord not found.";
    std::cin.ignore();
    std::cin.get();
}

void intro()
{
    std::cout << "\n\n\n\t\t  STUDENT";
    std::cout << "\n\n\t\tREPORT CARD";
    std::cout << "\n\n\t\t  PROJECT";
    std::cout << "\n\n\n\tMADE BY : YOUR NAME";
    std::cout << "\n\n\tSCHOOL : YOUR SCHOOL NAME";
    std::cout << "\n\n\tADDRESS: YOUR SCHOOL ADDRESS";
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Delay for 2 seconds
}

int main()
{
    char ch;
    intro();
    do
    {
        system("cls");
        std::cout << "\n\n\n\tMAIN MENU";
        std::cout << "\n\n\t01. RESULT MENU";
        std::cout << "\n\n\t02. ENTRY/EDIT MENU";
        std::cout << "\n\n\t03. EXIT";
        std::cout << "\n\n\tPlease Select Your Option (1-3): ";
        std::cin >> ch;
        switch (ch)
        {
        case '1':
            system("cls");
            std::cout << "\n\n\n\tRESULT MENU";
            std::cout << "\n\n\t1. Class Result";
            std::cout << "\n\n\t2. Student Report Card";
            std::cout << "\n\n\t3. Back to Main Menu";
            std::cout << "\n\n\n\tEnter Choice (1/2/3)? ";
            std::cin >> ch;
            switch (ch)
            {
            case '1':
                system("cls");
                classResult();
                break;
            case '2':
                result();
                break;
            case '3':
                continue;
            default:
                std::cout << "\a";
            }
            break;
        case '2':
            system("cls");
            std::cout << "\n\n\n\tENTRY/EDIT MENU";
            std::cout << "\n\n\t1. Create Student Record";
            std::cout << "\n\n\t2. Display All Students Records";
            std::cout << "\n\n\t3. Search Student Record";
            std::cout << "\n\n\t4. Modify Student Record";
            std::cout << "\n\n\t5. Delete Student Record";
            std::cout << "\n\n\t6. Back to Main Menu";
            std::cout << "\n\n\n\tEnter Choice (1-6): ";
            std::cin >> ch;
            switch (ch)
            {
            case '1':
                writeStudent();
                break;
            case '2':
                displayAll();
                break;
            case '3':
                int num;
                std::cout << "\n\n\tPlease enter the roll number: ";
                std::cin >> num;
                displaySpecific(num);
                break;
            case '4':
                modifyStudent();
                break;
            case '5':
                deleteStudent();
                break;
            case '6':
                continue;
            default:
                std::cout << "\a";
            }
            break;
        case '3':
            std::cout<<"\n\n\tThanks For using this\n";
            exit(0);
        default:
            std::cout << "\a";
        }
    }
    while (ch != '3');

    return 0;
}
