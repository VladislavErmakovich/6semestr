#include <iostream>
#pragma inline
using namespace std;

void main()
{
	setlocale(LC_ALL, "Russian");
	const int arr_size = 16;
	float* array = new float[arr_size];
	float sum_c = 0;
	float sum_sse = 0;
	for (short i = 0; i < arr_size; i++)
		array[i] = rand() / float(RAND_MAX) * 24.f + 1.f;
	cout << "Пример нахождения суммы элементов массива при помощи SSE" << endl;
	for (short i = 0; i < arr_size; i++)
		cout << "array[" << i << "] = " << array[i] << endl;
	for (short i = 0; i < arr_size; i++)
		sum_c = sum_c + array[i];
	_asm {
		mov ebx, array // Адрес массива в ebx
		mov ecx, arr_size
		shr ecx, 2 // Параллельно обрабатываем по 4 float'а - счётчик нужно разделить на 4
		xorpd xmm0, xmm0 // Сумма элементов
		p0 :
		movdqu xmm1, [ebx] // Загрузка 4х элементов в XMM1
			addps xmm0, xmm1 // Параллельное сложение 4х float'ов
				add ebx, 16 // Переход к следующей четвёрке чисел
				loop p0
				// Результат суммы в 4х двойных словах в XMM0, необходимо их поэлементно проссуммировать
				movdqu xmm1, xmm0 // Копия XMM0 в XMM1
				/*Перестановка бит: нужно перенести:
				разряды 127...96 источника в разряды 63...32 приёмника
				разряды 95...64 источника в разряды 31...00 приёмника
				остальное не трогаем! Маска следующая 1 1 1 0 1 1 1 0 = 0XEE*/
				pshufd xmm1, xmm1, 0xEE // Перетасовка бит в XMM1
				addps xmm0, xmm1 // Сумма
				movdqu xmm1, xmm0 // Копия XMM0 в XMM1
				/*Перестановка бит: нужно перенести:
				разряды 63...32 источника в разряды 31...00 приёмника
				остальное не трогаем! Маска следующая 1 1 1 0 0 1 0 1 = 0XE5*/
				pshufd xmm1, xmm1, 0xE5 // Перетасовка бит в XMM1
				addps xmm0, xmm1 // Сумма
				movss sum_sse, xmm0 // Итоговое число в младших 32 разрядах XMM0
	}
	cout << "Сумма элементов массива на СИ = " << sum_c << endl;
	cout << "Сумма массивов на ассемблере с использованием SSE = " << sum_sse << endl;
	delete[] array;
	system("pause");
}