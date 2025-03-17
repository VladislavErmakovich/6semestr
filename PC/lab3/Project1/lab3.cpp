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
        cout << "\n\tМеню\n"
            << "[1] Задание 1\n"
            << "[2] Задание 2\n"
            << "[3] Задание 3\n"
            << "[4] Дополнительное задание\n"
            << "[5] Выход\n";

        getline(cin, input);

        // Проверка на корректность ввода
        try {
            choice = stoi(input);
        }
        catch (...) {
            cout << "Ошибка! Введите число от 1 до 5.\n";
            system("pause");
            continue;
        }

        // Обработка выбора
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
            cout << "Выход из программы...\n";
            break;
        default:
            cout << "Неверный пункт! Введите число от 1 до 5.\n";
            system("pause");
        }
    } while (choice != 5);
 
}

void task1() {
    clearScreen();
    cout << "Посчитайте значение функции\nВведите значения для x1 и x2\n";

    float x1 , x2 , y;
    float pi = 3.14159265358979323846;
    int one = 1, two = 2, four = 4;
    std::cout << "Введите первое число (x1): ";
    std::cin >> x1;

    std::cout << "Введите второе число (x2): ";
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
    cout << " Результат FPU y = " << y << endl;
    float y_Cpp = (1 - 2 * pow(sin(x1 - pi / 4),2)) / (1 + cos(4 * x2));
    cout << " Результат C++ y = " << y_Cpp << endl;
    system("pause");
}

void task2() {
clearScreen();
int array_size;
cout << "Разделить каждый элемент массива на значение косинуса первого элемента.\nВведите размер массива";
std::cout << "Размер массива: ";
std::cin >> array_size;
float* array = new float[array_size];
for (short i = 0; i < array_size; i++)
    cin >> array[i];
for (short i = 0; i < array_size; i++)
    cout << "array[" << i << "] = " << array[i] << endl;

_asm {
    mov ECX, array_size    // Загружаем размер массива
    test ECX, ECX          // Проверяем на пустой массив
    jz done                // Выход если размер 0

    mov EDI, array         // Загружаем адрес массива
    finit                  // Инициализация FPU

    fld dword ptr[EDI]    // ST(0) = array[0]
    fcos                   // ST(0) = cos(array[0])
    fstp ST(1)             // Переносим значение в ST(1), очищаем ST(0)

    process_loop:
    fld dword ptr[EDI]    // ST(0) = текущий элемент
    fdiv ST(1),ST(0)             // Делим на cos(array[0]) из ST(1)
    fstp dword ptr[EDI]   // Сохраняем результат обратно
    add EDI, 4             // Переход к следующему элементу
    loop process_loop      // Повторяем для всех элементов

    ffree ST(0)            // Очищаем регистры FPU
    ffree ST(1)
    done:
}

// Вывод результатов
cout << "\nРезультат:" << endl;
for (short i = 0; i < array_size; i++)
    cout << "array[" << i << "] = " << array[i] << endl;

delete[] array;
system("pause");
}