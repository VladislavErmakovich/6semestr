#include "func.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>

#define T 3.0           // Длительность сигнала (сек)
#define F0 500.0        // Начальная частота (Гц)
#define F1 6000.0       // Конечная частота (Гц)
#define FS 16000.0      // Частота дискретизации (Гц)
#define A 10.0          // Амплитуда (Гц)
#define PI 3.14159265358979323846

typedef struct {
    char chunkID[4];
    uint32_t chunkSize;
    char format[4];
    char subchunk1ID[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char subchunk2ID[4];
    uint32_t subchunk2Size;
} WavHeader;

using namespace std;

void clearScreen() {
    system("cls || clear");
}

void writeWavFile(const char* filename, const double* samples, size_t numSamples) {
    WavHeader header;
    memcpy(header.chunkID, "RIFF", 4);
    memcpy(header.format, "WAVE", 4);
    memcpy(header.subchunk1ID, "fmt ", 4);
    header.subchunk1Size = 16;
    header.audioFormat = 1; // PCM
    header.numChannels = 1;
    header.sampleRate = (uint32_t)FS;
    header.bitsPerSample = 16;
    header.byteRate = header.sampleRate * header.numChannels * header.bitsPerSample / 8;
    header.blockAlign = header.numChannels * header.bitsPerSample / 8;
    memcpy(header.subchunk2ID, "data", 4);
    header.subchunk2Size = (uint32_t)(numSamples * header.bitsPerSample / 8);
    header.chunkSize = 36 + header.subchunk2Size;

    FILE* file;
    fopen_s(&file, filename, "wb");
    if (!file) {
        perror("Ошибка создания файла");
        return;
    }

    // Запись заголовка
    fwrite(&header, sizeof(WavHeader), 1, file);

    // Запись аудиоданных
    for (size_t i = 0; i < numSamples; i++) {
        float sample = samples[i];
        // Ограничение амплитуды
        if (sample > 1.0f) sample = 1.0f;
        if (sample < -1.0f) sample = -1.0f;
        int16_t intSample = (int16_t)(sample * 32767.0f);
        fwrite(&intSample, sizeof(int16_t), 1, file);
    }

    fclose(file);
}

int main() {
    size_t size = sizeof(int);
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
    double fs = 16000;
    int chirpSamples = (int)(T * fs);

    float* chirp = (float*)malloc(chirpSamples * sizeof(float));
   
    double k = pow(F1 / F0, 1 / T);
    double lnk = log(k);

    const double fs_inv = 1.0 / FS;      // 1/fs для оптимизации
    const double two_pi = 2.0 * PI;      // 2*PI
    const double f0 = F0;
    const double a = A;

    double result = 0;
    __asm {
        push ebx
        push edi
        push esi

        mov edi, chirp
        xor esi, esi

        finit

        p0 :
        cmp esi, [chirpSamples]   // Проверка завершения цикла
            jge p1

            mov ebx, esi
                push ebx
                fild dword ptr[esp]      // st(0) = i
                fld[fs_inv]              // st(0) = 1/fs, st(1) = i
                    fmulp st(1), st(0)        // st(0) = t = i/fs

                        fld st(0)
                        fld[k]
                        fyl2x
                            fld1
                            fld st(1)
                            fprem
                            f2xm1
                            fadd
                            fscale
                            fxch st(1)
                            fstp st  // st(0) = k^t


                            fld1
                            fsub
                            fdiv[lnk] // st(0) = (k ^ t - 1)/ ln(k)

                            fmul[f0] // st(0) = st(0) * f0
                            fmul[two_pi] // st(0) = st(0) * 2 * PI

                            fsin
                            fmul[a]
                            fstp dword ptr[edi + esi * 4]
                            ffree st(0)
                            add esp, 4
                            inc esi
                            jmp p0
                        p1 :
                            fwait

                            pop esi
                            pop edi
                            pop ebx
    }

    for (int i = 0; i < chirpSamples; i++) {
        cout << chirp[i] << ",";
    }
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
       fld st(0) // st(0) = y*log2(x), st(1) = y*log2(x), st(2)= cos(x) 
       frndint //st(0) = целая часть y*log2(x), st(1) = y*log2(x), st(2)= cos(x) 
       fsub ST(1), ST(0) //st(0) = целая часть y*log2(x), st(1) = дробная y*log2(x), st(2)= cos(x) 
       fxch // st(0) = дроб ная y*log2(x), st(1) = целая часть y*log2(x)
       F2XM1 // ST(0) =дробная 2^(y*log2(x)) -1, st(1) = целая часть y*log2(x), st(2) = cos(x)
       fld1 // st(0) = 1, ST(1) =дробная 2^(y*log2(x)) -1, st(2) = целая часть y*log2(x), st(3) = cos(x)
       faddp ST(1), ST(0) // st(0) = дробная 2^(y*log2(x)), st(1) = целая часть y*log2(x), st(2) = cos(x)
       fscale // st(0) = дробная 2^(y*log2(x)) * 2^(целая часть y*log2(x)) -> x^y, st(1) = целая часть y*log2(x), st(2) = cos(x)
       fxch // st(0) = целая часть y*log2(x), st(1) = x^y, st(2) = cos(x)
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
       frndint // st(0) = целая часть y*log2(e), st(1) = y*log2(e), st(2) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
       fsub ST(1), ST(0) // st(0) = целая часть y*log2(e), st(1) = дробная y*log2(e), st(2) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
       fxch // st(0) = дробная часть y*log2(e), st(1) = целая y*log2(e), st(2) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
       F2XM1 // st(0) = дробная часть 2^(y*log2(e)) -1, st(1) = целая y*log2(e), st(2) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
       fld1 // st(0) =1, st(1) = дробная часть 2^(y*log2(e)) -1, st(2) = целая y*log2(e), st(3) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
       faddp ST(1), ST(0) // st(0) = дробная часть 2^(y*log2(e)), st(1) = целая y*log2(e), st(2) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
       fscale // st(0) = дробная часть 2^(y*log2(e)) * 2^(целая y*log2(e)) -> e^y, st(1) = целая y*log2(e), st(2) =(cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
       fxch // st(0) = целая y*log2(e), st(1) = e^y, st(2) = (cos(x)*sqrt(sin(y)*x^y))/y*log2(x+1)
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

    cout << "Результат вычесления FPU: " << R << endl;
    //float R_cpp = (cos(x) * sqrt(sin(y)*pow(x,y))) / (y * log2(x + 1)) + exp(y) - tan(x / y) - exp2(x-1);
    //cout << "Результат вычесления C++: " << R_cpp << endl;


    system("pause");
}