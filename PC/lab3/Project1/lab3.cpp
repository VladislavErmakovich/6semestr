#include "func.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>

using namespace std;

void clearScreen() {
    system("cls || clear");
}


int main() {
    
    setlocale(LC_ALL, "Russian");
    int choice;
    string input;

    do {
        clearScreen();
        cout << "\n\t      МЕНЮ ПРОГРАММЫ  \n"
            << "\n\t 1. Задание 1         "
            << "\n\t 2. Задание 2         "
            << "\n\t 3. Задание 3         "
            << "\n\t 4. Дополнительное    "
            << "\n\t 5. Выход             "
            << "\n\nВыбор > ";

        getline(cin, input);

        try {
            choice = stoi(input);

            if (choice < 1 || choice > 5) {
                throw out_of_range("");
            }

            switch (choice) {
            case 1:
                task1();
                break;
            case 2:
                task2();
                break;
            case 3:
                task3();
                break;
            case 4:
                dop();
                break;
            case 5:
                if (confirmExit()) return 0;
                continue;
            }

            // Пауза после выполнения задания
            cout << "\nОперация завершена. Нажмите Enter...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

        }
        catch (const invalid_argument&) {
            cout << "\nОшибка! Введите число от 1 до 5";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        catch (const out_of_range&) {
            cout << "\nНеверный пункт меню! Допустимые значения: 1-5";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (true);
}

bool confirmExit() {
    clearScreen();
    cout << "\nВы уверены? (y/n): ";
    string answer;
    getline(cin, answer);
    return !answer.empty() && (tolower(answer[0]) == 'y');
}

void task1() {
    clearScreen();
    cout << "Посчитайте значение функции\nВведите значения для x1 и x2\n";

    float x1 , x2 , y;
    float pi = 3.141592653589793;
    int one = 1, two = 2, four = 4;
    std::cout << "Введите первое число (x1): ";
    std::cin >> x1;

    std::cout << "Введите второе число (x2): ";
    std::cin >> x2;
    
    //y = (1-2sin(x1-pi/4)^2)/(1+cos(4*x2))
    _asm {
        finit
        fld x1 //st(0) = x1
        FLDPI // st(0) = pi, st(1) = x1
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
    float* array_cpp = new float[array_size];
    std::copy(array_cpp, array_cpp + array_size, array);

    for (short i = 0; i < array_size; i++)
        cin >> array[i];
    for (short i = 0; i < array_size; i++)
        cout << "array[" << i << "] = " << array[i] << endl;


    float cos_val;
    _asm {
        mov ECX, array_size    // Загружаем размер массива
        test ECX, ECX          // Проверяем на пустой массив
        jz done                // Выход если размер 0
        mov EDI, array // заноси в EDI адрес массива
        finit                  // Инициализация FPU

        fld dword ptr[EDI]    // st(0)=array[n]
        fcos    //st(0)=cos(array[0])
        fstp dword ptr[cos_val] //сохраняем значение косинуса 

    process_loop:
        fld dword ptr[EDI]    // st(0)=array[n]
        fdiv dword ptr[cos_val] // st(0) = array[n]/cos(array[0])
        fstp dword ptr[EDI]   // Сохраняем результат обратно
        add EDI, 4             // Переход к следующему элементу
        loop process_loop      // Повторяем для всех элементов

        ffree ST(0)            // Очищаем регистры FPU
        ffree ST(1)
    done:
}

    cout << "\nРезультат FPU:" << endl;
    for (short i = 0; i < array_size; i++)
        cout << "array[" << i << "] = " << array[i] << endl;

    for (short i = 0; i < array_size; i++)
        array_cpp[i] = array_cpp[i] / cos_val;

    cout << "\nРезультат C++:" << endl;
    for (short i = 0; i < array_size; i++)
        cout << "array[" << i << "] = " << array[i] << endl;

    delete[] array;
    delete[] array_cpp;
    system("pause");
}

void task3() {
    clearScreen();

    system("pause");
}

void dop() {
    clearScreen();
    cout << "Посчитайте значение функции\nВведите значения для y и x\n";

    float x, y, R;
    std::cout << "Введите первое число (x): ";
    std::cin >> x;

    std::cout << "Введите второе число (y): ";
    std::cin >> y;
    // cos(x)*sqrt(sin(y)*x^y)) / (y *log2(x+1))+exp(y)-tan(x/y)-2^(x-1)
    _asm{
       finit
       fld x // st(0) = x
       fcos // st(0) = cos(x)
       fld y // st(0) = y, st(1)=cos(x)
       fld x // st(0) =x, st(1)= y, st(2) = cos(x)
       FYL2X // ST(0) = y*log2(x), ST(1) = cos(x)
       F2XM1 // ST(0) = 2^(y*log2(x)) -1, st(1) = cos(x)
       fld1 // st(0) = 1, st(1) = 2^(y*log2(x)) -1, st(2) = cos(x)
       faddp ST(1), ST(0) // st(0) = 2^(y*log2(x))-> x^y, st(1) = cos(x)
       fld y // st(0) = y , st(1) = x^y, st(2) = cos(x)
       fsin // st(0) = sin(y) , st(1) = x^y, st(2) = cos(x)
       fmulp ST(1), ST(0) // st(0) = sin(y)*x^y, st(1) = cos(x)
       fsqrt // st(0) = sqrt(sin(y)*x^y), st(1) = cos(x)
       fmul ST(1), ST(0) // st(0) = cos(x)*sqrt(sin(y)*x^y)
       fld y // st(0) = y, st(1) = cos(x)*sqrt(sin(y)*x^y)
       fld x // st(0) = x, st(1) =y, st(2) = cos(x)*sqrt(sin(y)*x^y)
       fld1 // st(0) =1, st(1) = x, st(2) =y, st(3) = cos(x)*sqrt(sin(y)*x^y)
       faddp ST(1), ST(0) // st(0) =x+1, st(1) =y, st(2) = cos(x)*sqrt(sin(y)*x^y)
       FYL2X // st(0) = y*log2(x+1), st(1) = cos(x)*sqrt(sin(y)*x^y)
       fdivp ST(1), ST(0) // st(0) =  (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)

       FLDL2E // st(0) = log2(e), st(1) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
       fld y //st(0) =y, st(1) = log2(e) , st(2) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
       fmulp ST(1), ST(0) // st(0) = y*log2(e), st(1) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
       F2XM1 // st(0) = 2^(y*log2(e)) -1, st(1) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
       fld1 // st(0) =1, st(1) = 2^(y*log2(e)) -1, st(2) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
       faddp ST(1), ST(0) // st(0) = 2^(y*log2(e)) -> e^y, st(1) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
       
       faddp ST(1), ST(0) // st(0) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y

       fld x // st(0) =x , st(1) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y
       fld y // st(0) =y, st(1) = x, st(2) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y
       fdivp ST(1), ST(0) // st(0) = x/y, st(1) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y
       fptan //st(0) =1, st(1) = tan(x/y), st(2) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y
       fstp ST(0) // st(0) = tan(x/y) ,st(1) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y
       fsubp ST(1), ST(0) // st(0) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y - tan(x/y)

       fld x // st(0) = x, st(1) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y - tan(x/y)
       fld1 // st(0) =1, st(1) = x, st(2) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y - tan(x/y)
       fsubp ST(1), ST(0) // st(0) = x-1, st(1) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y - tan(x/y)
       F2XM1 // st(0) = 2^(x-1) - 1 ,st(1) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y - tan(x/y)
       fld1 // st(0) =1, st(1) = 2^(x-1) -1 , st(2) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y - tan(x/y)
       faddp ST(1), ST(0) // st(0) = 2^(x-1), st(1) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y - tan(x/y)
       fsubp ST(1), ST(0) // st(0)= (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y - tan(x/y) -2^(x-1)
       fst R
    }   

    cout << "Результат вычесления FPU: " << R << endl;
    float R_cpp = (cos(x) * sqrt(sin(y)*pow(x,y))) / (y * log2(x + 1)) + exp(y) - tan(x / y) - exp2(x-1);
    cout << "Результат вычесления C++: " << R_cpp << endl;


    system("pause");
}