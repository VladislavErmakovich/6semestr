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
        cout << "\n\t      ���� ���������  \n"
            << "\n\t 1. ������� 1         "
            << "\n\t 2. ������� 2         "
            << "\n\t 3. ������� 3         "
            << "\n\t 4. ��������������    "
            << "\n\t 5. �����             "
            << "\n\n����� > ";

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
            /*case 3:
                task3();
                break;
            case 4:
                dop();
                break;*/
            case 5:
                if (confirmExit()) return 0;
                continue;
            }

            // ����� ����� ���������� �������
            cout << "\n�������� ���������. ������� Enter...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

        }
        catch (const invalid_argument&) {
            cout << "\n������! ������� ����� �� 1 �� 5";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        catch (const out_of_range&) {
            cout << "\n�������� ����� ����! ���������� ��������: 1-5";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (true);
}

bool confirmExit() {
    clearScreen();
    cout << "\n�� �������? (y/n): ";
    string answer;
    getline(cin, answer);
    return !answer.empty() && (tolower(answer[0]) == 'y');
}

void task1() {
    clearScreen();

    int array_size;
    cout << "����� ����� �������� ��������� �������\n";
    std::cout << "������� ������ ������� ������� 4: ";
    std::cin >> array_size;

    if (array_size % 4 != 0) {
        cout << "������! ������ ������ ���� ������ 4";
        return;
    }

    float* array = new float[array_size];
    float* array_c = new float[array_size];

    for (short i = 0; i < array_size; i++) {
        cin >> array[i];
        array_c[i] = array[i];
    }

    for (short i = 0; i < array_size; i++)
        cout << "array[" << i << "] = " << array[i] << endl;

    float sum = 0, sum_c = 0;

    _asm {
        mov esi, array
        mov ecx, array_size
        shr ecx, 2
        xorps xmm0, xmm0

        loop_start :
        movaps xmm1, [esi]
            rcpps xmm2, xmm1
                addps xmm0, xmm2
                add esi, 16
                dec ecx
                jnz loop_start

                
                movhlps xmm1, xmm0
                addps xmm0, xmm1
                pshufd xmm1, xmm0, 0x1
                addss xmm0, xmm1
                movss sum, xmm0
    }
  
    for (int i = 0; i < array_size; i++)
        sum_c =sum_c + 1.0f / array_c[i];

    
    cout << " ��������� SSE = " << sum << endl;
    
    cout << " ��������� C = " << sum_c << endl;

    delete[] array;
    delete[] array_c;
    system("pause");
}

void task2() {
    clearScreen();
    int array_size;
    cout << "��������� ������������ ���������� ��� ��������.�������� ������� � ����� ���� double.\n";
    std::cout << "������  ��������: ";
    std::cin >> array_size;

    double* result = static_cast<double*>(_aligned_malloc(array_size * sizeof(double), 16));

    double* array1 = new double[array_size];
    double* array1_cpp = new double[array_size];
    std::copy(array1_cpp, array1_cpp + array_size, array1);

    double* array2 = new double[array_size];
    double* array2_cpp = new double[array_size];
    std::copy(array2_cpp, array2_cpp + array_size, array2);

    for (short i = 0; i < array_size; i++)
        cin >> array1[i];
    for (short i = 0; i < array_size; i++)
        cout << "array[" << i << "] = " << array1[i] << endl;

    for (short i = 0; i < array_size; i++)
        cin >> array2[i];
    for (short i = 0; i < array_size; i++)
        cout << "array[" << i << "] = " << array2[i] << endl;


    _asm {
        mov esi, array1    // ����� ������� �������
        mov edi, array2    // ����� ������� �������
        mov edx, result    // ����� ����������
        mov ecx, array_size// ������� ���������
        shr ecx, 1         // ������������ �� 2 �������� �� ��� (128 ���)
        jz remaining       // ���� ������ ������ 2 ���������

        loop_start :
        movapd xmm0, [esi] // �������� 128 ��� �� ������� �������
            movapd xmm1, [edi] // �������� 128 ��� �� ������� �������
                orpd xmm0, xmm1    // ��������� ���
                    movapd[edx], xmm0 // ���������� ����������
                    add esi, 16        // ��������� 16 ���� (2 ��������)
                    add edi, 16
                    add edx, 16
                    dec ecx
                    jnz loop_start

                    remaining :
                test array_size, 1 // �������� ��������� �������
                    jz done
                    movsd xmm0, [esi]  // ��������� ���������� ��������
                    movsd xmm1, [edi]
                        orpd xmm0, xmm1
                            movsd[edx], xmm0

                            done :
    }

    delete[] array1;
    delete[] array1_cpp;
    delete[] array2;
    delete[] array2_cpp;

    system("pause");
}

/*void task3() {
    clearScreen();

    system("pause");
}

void dop() {
    clearScreen();
    cout << "���������� �������� �������\n������� �������� ��� y � x\n";

    float x, y, R;
    std::cout << "������� ������ ����� (x): ";
    std::cin >> x;

    std::cout << "������� ������ ����� (y): ";
    std::cin >> y;
    // cos(x)*sqrt(sin(y)*x^y)) / (y *log2(x+1))+exp(y)-tan(x/y)-2^(x-1)
    _asm {
        finit
        fld x // st(0) = x
        fcos // st(0) = cos(x)

        fld y // st(0) = y, st(1)=cos(x)
        fld x // st(0) =x, st(1)= y, st(2) = cos(x)
        FYL2X // ST(0) = y*log2(x), ST(1) = cos(x)
        fld st(0) // st(0) = y*log2(x), st(1) = y*log2(x), st(2)= cos(x) 
        frndint //st(0) = ����� ����� y*log2(x), st(1) = y*log2(x), st(2)= cos(x) 
        fsub ST(1), ST(0) //st(0) = ����� ����� y*log2(x), st(1) = ������� y*log2(x), st(2)= cos(x) 
        fxch // st(0) = ���� ��� y*log2(x), st(1) = ����� ����� y*log2(x)
        F2XM1 // ST(0) =������� 2^(y*log2(x)) -1, st(1) = ����� ����� y*log2(x), st(2) = cos(x)
        fld1 // st(0) = 1, ST(1) =������� 2^(y*log2(x)) -1, st(2) = ����� ����� y*log2(x), st(3) = cos(x)
        faddp ST(1), ST(0) // st(0) = ������� 2^(y*log2(x)), st(1) = ����� ����� y*log2(x), st(2) = cos(x)
        fscale // st(0) = ������� 2^(y*log2(x)) * 2^(����� ����� y*log2(x)) -> x^y, st(1) = ����� ����� y*log2(x), st(2) = cos(x)
        fxch // st(0) = ����� ����� y*log2(x), st(1) = x^y, st(2) = cos(x)
        FSTP ST(0) // st(0) = x^y, st(1) = cos(x)

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
        fld st(0) // st(0) = y*log2(e), st(1) = y*log2(e), st(2) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
        frndint // st(0) = ����� ����� y*log2(e), st(1) = y*log2(e), st(2) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
        fsub ST(1), ST(0) // st(0) = ����� ����� y*log2(e), st(1) = ������� y*log2(e), st(2) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
        fxch // st(0) = ������� ����� y*log2(e), st(1) = ����� y*log2(e), st(2) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
        F2XM1 // st(0) = ������� ����� 2^(y*log2(e)) -1, st(1) = ����� y*log2(e), st(2) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
        fld1 // st(0) =1, st(1) = ������� ����� 2^(y*log2(e)) -1, st(2) = ����� y*log2(e), st(3) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
        faddp ST(1), ST(0) // st(0) = ������� ����� 2^(y*log2(e)), st(1) = ����� y*log2(e), st(2) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
        fscale // st(0) = ������� ����� 2^(y*log2(e)) * 2^(����� y*log2(e)) -> e^y, st(1) = ����� y*log2(e), st(2) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
        fxch // st(0) = ����� y*log2(e), st(1) = e^y, st(2) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
        FSTP ST(0) // st(0) = e^y, st(1) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)

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
        fld1 // st(0) = 1, st(1) = x-1, st(2) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y - tan(x/y)
        fscale // st(0) = 2^(x-1), st(1) = x-1, st(2) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y - tan(x/y)
        fxch // st(0) = x-1, st(1) = 2^(x-1), st(2) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y - tan(x/y)
        FSTP ST(0) // st(0) = 2^(x-1), st(1) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y - tan(x/y)

        fsubp ST(1), ST(0) // st(0)= (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1) + e^y - tan(x/y) -2^(x-1)
        fst R
    }

    cout << "��������� ���������� FPU: " << R << endl;
    float R_cpp = (cos(x) * sqrt(sin(y) * pow(x, y))) / (y * log2(x + 1)) + exp(y) - tan(x / y) - exp2(x - 1);
    cout << "��������� ���������� C++: " << R_cpp << endl;


    system("pause");
}*/