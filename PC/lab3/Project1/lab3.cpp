#include "func.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <clocale>
#include <limits>
#include <cmath>

using namespace std;

void clearScreen() {
	system("cls || clear");
}

void main() {
    setlocale(LC_ALL, "Russian");
    int choice;
    string input;

    do {
        clearScreen();
        cout << "\n\t����\n"
            << "[1] ������� 1\n"
            << "[2] ������� 2\n"
            << "[3] ������� 3\n"
            << "[4] �������������� �������\n"
            << "[5] �����\n";

        getline(cin, input);

        // �������� �� ������������ �����
        try {
            choice = stoi(input);
        }
        catch (...) {
            cout << "������! ������� ����� �� 1 �� 5.\n";
            system("pause");
            continue;
        }

        // ��������� ������
        switch (choice) {
        case 1:
            task1();
            break;
        case 2:
            task2();
            break;
        /*case 3:
            task3();
            break;*/
        //case 4:
           // additionalTask();
           // break;
        case 5:
            cout << "����� �� ���������...\n";
            break;
        default:
            cout << "�������� �����! ������� ����� �� 1 �� 5.\n";
            system("pause");
        }
    } while (choice != 5);
 
}

void task1() {
    clearScreen();
    cout << "���������� �������� �������\n������� �������� ��� x1 � x2\n";

    float x1 , x2 , y;
    float pi = 3.14159265358979323846;
    int one = 1, two = 2, four = 4;
    std::cout << "������� ������ ����� (x1): ";
    std::cin >> x1;

    std::cout << "������� ������ ����� (x2): ";
    std::cin >> x2;
    
    //y = (1-2sin(x1-pi/4)^2)/(1+cos(4*x2))
    _asm {
        finit
        fld x1 //st(0) = x1
        fld pi // st(0) = pi, st(1) = x1
        fild four // st(0) = 4, st(1) = pi, st(2) = x1
        fdivp ST(1), ST(0) //st(0) = pi/4, st(1) = x1
        fsubp ST(1), ST(0) // ST(0) = x1 - pi/4
        fsin //ST(0) =sin(x1-pi/4)
        fmul ST(0), ST(0) //ST(0) =sin(x1-pi/4)^2
        fild two //st(0) = 2, st(1) = sin(x1-pi/4)^2
        fmulp ST(1), ST(0)//st(0) = 2*sin(x1-pi/4)^2
        fild one // st(0)=1, st(1) = 2*sin(x1-pi/4)^2
        fxch // st(0) = 2*sin(x1-pi/4)^2, st(1)=1
        fsubp ST(1), ST(0) //st(0)=1 - 2sin(x1 - pi / 4) ^ 2

        fld x2 // st(0)=x2,st(1)=1 - 2sin(x1 - pi / 4) ^ 2
        fild four // st(0)=4,st(1)=x2,st(2)=1 - 2sin(x1 - pi / 4) ^ 2
        fmulp ST(1), ST(0) // st(0) = x2*4, st(1)=1 - 2sin(x1 - pi / 4) ^ 2
        fcos    //st(0) = cos(x2*4), st(1)=1 - 2sin(x1 - pi / 4) ^ 2
        fild one //st(0)= 1,st(1) = cos(x2*4), st(2)=1 - 2sin(x1 - pi / 4) ^ 2 
        faddp ST(1), ST(0) // st(0) = 1+cos(4*x2), ST(1) = 1 - 2sin(x1 - pi / 4) ^ 2 
        fdivp ST(1), ST(0)
        fstp y
    }
    cout << " ��������� FPU y = " << y << endl;
    float y_Cpp = (1 - 2 * pow(sin(x1 - pi / 4),2)) / (1 + cos(4 * x2));
    cout << " ��������� C++ y = " << y_Cpp << endl;
    system("pause");
}

void task2() {
clearScreen();
int array_size;
cout << "��������� ������ ������� ������� �� �������� �������� ������� ��������.\n������� ������ �������";
std::cout << "������ �������: ";
std::cin >> array_size;
float* array = new float[array_size];
for (short i = 0; i < array_size; i++)
    cin >> array[i];
for (short i = 0; i < array_size; i++)
    cout << "array[" << i << "] = " << array[i] << endl;

_asm {
    mov ECX, array_size    // ��������� ������ �������
    test ECX, ECX          // ��������� �� ������ ������
    jz done                // ����� ���� ������ 0

    mov EDI, array         // ��������� ����� �������
    finit                  // ������������� FPU

    fld dword ptr[EDI]    // ST(0) = array[0]
    fcos                   // ST(0) = cos(array[0])
    fstp ST(1)             // ��������� �������� � ST(1), ������� ST(0)

    process_loop:
    fld dword ptr[EDI]    // ST(0) = ������� �������
    fdiv ST(1),ST(0)             // ����� �� cos(array[0]) �� ST(1)
    fstp dword ptr[EDI]   // ��������� ��������� �������
    add EDI, 4             // ������� � ���������� ��������
    loop process_loop      // ��������� ��� ���� ���������

    ffree ST(0)            // ������� �������� FPU
    ffree ST(1)
    done:
}

// ����� �����������
cout << "\n���������:" << endl;
for (short i = 0; i < array_size; i++)
    cout << "array[" << i << "] = " << array[i] << endl;

delete[] array;
system("pause");
}