#include <iostream>
#include <conio.h>
#include "img_lib.h"


using namespace std;

void task1() {
    int arr_size;
    cout << "Введите размер массива: ";
    cin >> arr_size;

    int* array = new int[arr_size];
    cout << "Введите элементы массива: ";
    for (int i = 0; i < arr_size; i++) {
        cin >> array[i];
    }

    int odd_count_c = 0, odd_count_mmx = 0;

    for (int i = 0; i < arr_size; i++) {
        if (array[i] % 2 != 0) odd_count_c++;
    }

    int one = 1;

    __asm {
        mov esi, array
        mov ecx, arr_size
        pxor mm1, mm1
        check_loop :
        movd mm0, [esi]
            movd eax, mm0
                test eax, 1
                jz skip_inc
                paddd mm1, [one]
                skip_inc :
                    add esi, 4
                    loop check_loop
                    movd odd_count_mmx, mm1
                    emms
    }

    cout << "Кол-во нечётных элементов (C++): " << odd_count_c << endl;
    cout << "Кол-во нечётных элементов (MMX): " << odd_count_mmx << endl;

    delete[] array;
}

void task2() {
    int row_size = 1;
    const int col_size = 4;
        
    cout << "Введите кол-во строк матрицы: ";
    cin >> row_size;
    short* matrix_1 = new short[row_size * col_size];
    short* matrix_2 = new short[row_size * col_size];
    short* matrix_c = new short[row_size * col_size];
    short* matrix_MMX = new short[row_size * col_size];

    cout << "Введите элементы матрицы 1" << endl;

    for (int i = 0; i < row_size; i++)
        for (int j = 0; j < col_size; j++)
            cin >> *(matrix_1 + i * col_size + j);

    cout << "Введите элементы матрицы 2" << endl;

    for (int i = 0; i < row_size; i++)
        for (int j = 0; j < col_size; j++)
            cin >> *(matrix_2 + i * col_size + j);

    cout << "Матрица 1:" << endl;

    for (int i = 0; i < row_size; i++) {
        for (int j = 0; j < col_size; j++) {
            cout << *(matrix_1 + i * col_size + j) << " ";
        }
        cout << endl;
    }

    cout << "Матрица 2:" << endl;

    for (int i = 0; i < row_size; i++) {
        for (int j = 0; j < col_size; j++) {
            cout << *(matrix_2 + i * col_size + j) << " ";
        }
        cout << endl;
    }

    // Вычисление произведения матриц
    for (int i = 0; i < row_size; i++)
        for (int j = 0; j < col_size; j++)
            *(matrix_c + i * col_size + j) = *(matrix_1 + i * col_size + j) * *(matrix_2 + i * col_size + j);

    cout << "Результат произведения матриц на СИ:" << endl;
    for (int i = 0; i < row_size; i++) {
        for (int j = 0; j < col_size; j++) {
            cout << *(matrix_c + i * col_size + j) << " ";
        }
        cout << endl;
    }

    _asm {
        mov esi, matrix_1
        mov edi, matrix_2
        mov ebx, matrix_MMX
        mov ecx, row_size

        p0 : 
            movq mm0, [esi]
            movq mm1, [edi]
            pmullw mm0, mm1
            movq[ebx], mm0

            add esi, 8
            add edi, 8
            add ebx, 8

            loop p0
    }

    cout << "Результат произведения матриц 2x2 при помощи MMX:" << endl;

    for (int i = 0; i < row_size; i++) {
        for (int j = 0; j < col_size; j++) {
            cout << *(matrix_MMX + i * col_size + j) << " ";
        }
        cout << endl;
    }
    delete[] matrix_1;
    delete[] matrix_2;
    delete[] matrix_c;
    delete[] matrix_MMX;
}

void dop1() {
    const int ARR_SIZE = 4 * 5;
    const int MASK = 0x000000FF;
    int arr_sum = 0;
    short* array = new short[ARR_SIZE];
    // get array from user
    std::cout << " Input " << ARR_SIZE << " elements :" << std::endl;
    for (short i = 0; i < ARR_SIZE; i++)
        std::cin >> array[i];
    // print entered array
    std::cout << " Entered array : \n ";
    for (short i = 0; i < ARR_SIZE; i++)
        std::cout << " array [" << i << "] = " << array[i] << std::endl;

    _asm{
        xor esi , esi
        mov esi , array // array -> esi
        mov ecx , ARR_SIZE // ecx = array size
        xor ebx , ebx // tmp = 0
        // calculate sum of 4 elements
     lp : 
        movq mm0 ,[esi] // arr [ i ] -> mm 0
        movq mm1 , mm0 // arr [ i ] -> mm 1
        psrlq mm1 , 16 // mm 1 >> 2
        paddd mm0 , mm1 // mm 0 = mm 0 + mm 1
        movq mm1 , mm0 // mm 1 = mm 0
        psrlq mm1 , 32 // mm 1 >> 4
        pand mm0, MASK // use mask on mm 0
        pand mm1, MASK // use mask on mm 1
        paddd mm0, mm1 // find final sum
        xor eax, eax // accumulator = 0
        movd eax, mm0 // accumulator = sum of 4 elements
        add arr_sum, eax
        // move to next 4 numbers
        add esi, 8
        sub ecx, 4
        jnz lp
        emms
    }
    std::cout << " Number of even elements is : " << arr_sum << std::endl;
}

void dop2() {
    int row_size = 1;
    int col_size = 4;

    cout << "Введите кол-во строк матрицы: ";
    cin >> row_size;
    cout << "Введите кол-во строк матрицы: ";
    cin >> col_size;

    int matrix_size = row_size * col_size;
    short* matrix_1 = new short[row_size * col_size];
    short* matrix_2 = new short[row_size * col_size];
    short* matrix_c = new short[row_size * col_size];
    short* matrix_MMX = new short[row_size * col_size];

    cout << "Введите элементы матрицы 1" << endl;

    for (int i = 0; i < row_size; i++)
        for (int j = 0; j < col_size; j++)
            cin >> *(matrix_1 + i * col_size + j);

    cout << "Введите элементы матрицы 2" << endl;

    for (int i = 0; i < row_size; i++)
        for (int j = 0; j < col_size; j++)
            cin >> *(matrix_2 + i * col_size + j);

    cout << "Матрица 1:" << endl;

    for (int i = 0; i < row_size; i++) {
        for (int j = 0; j < col_size; j++) {
            cout << *(matrix_1 + i * col_size + j) << " ";
        }
        cout << endl;
    }

    cout << "Матрица 2:" << endl;

    for (int i = 0; i < row_size; i++) {
        for (int j = 0; j < col_size; j++) {
            cout << *(matrix_2 + i * col_size + j) << " ";
        }
        cout << endl;
    }

    // Вычисление произведения матриц
    for (int i = 0; i < row_size; i++)
        for (int j = 0; j < col_size; j++)
            *(matrix_c + i * col_size + j) = *(matrix_1 + i * col_size + j) * *(matrix_2 + i * col_size + j);

    cout << "Результат произведения матриц на СИ:" << endl;
    for (int i = 0; i < row_size; i++) {
        for (int j = 0; j < col_size; j++) {
            cout << *(matrix_c + i * col_size + j) << " ";
        }
        cout << endl;
    }

    _asm{
        mov esi, matrix_1
        mov edi, matrix_2
        mov ebx, matrix_MMX
        mov ecx, matrix_size

        lp : 
            movq mm0 ,[esi]
            movq mm1 ,[edi]
            pmullw mm0 , mm1
            movq[ebx] , mm0
            
            add esi, 8
            add edi, 8
            add ebx, 8
            sub ecx, 4
            jnz lp
    }

    cout << "Результат произведения матриц 2x2 при помощи MMX:" << endl;

    for (int i = 0; i < row_size; i++) {
        for (int j = 0; j < col_size; j++) {
            cout << *(matrix_MMX + i * col_size + j) << " ";
        }
        cout << endl;
    }
    delete[] matrix_1;
    delete[] matrix_2;
    delete[] matrix_c;
    delete[] matrix_MMX;
}

void dop3() {
    char src_file[] = "lena.bmp";
    char proc_file[] = "lena_C.bmp";
    char proc_file_mmx[] = "lena_MMX.bmp";
    const unsigned int MASK = 0xFFFFFFFF;
    unsigned __int8 * src_img;
    unsigned __int8 color_map[1024];
    BmpFileHeader im_header;
    BmpInfoHeader im_info;
    src_img = imread(src_file, im_info, im_header, color_map);
    const unsigned int IMG_SIZE = im_info.width * im_info.height;
    _asm{
        mov esi , src_img
        mov ecx , IMG_SIZE
    lp : 
        movd mm0 , MASK
        movd mm1 ,[esi]
        psubd mm0 , mm1
        movd[esi] , mm0
        add esi , 4
        sub ecx , 4
        jnz lp
    }
    imwrite(proc_file, src_img, im_info, im_header, color_map);
    cout << " C ++ processing is done ." << endl;
    cout << " width : " << im_info.width << "\n";
    cout << " height : " << im_info.height << "\n";
    delete[] src_img;
}

int main() {
    setlocale(LC_ALL, "rus");
    dop3();
}
