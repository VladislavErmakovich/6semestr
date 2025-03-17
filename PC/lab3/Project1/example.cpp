#include <iostream>
#pragma inline
using namespace std;
void main1()
{
	setlocale(LC_ALL, "Russian");
	float a = 2.2, b = 8.4, c = 5.87, d = 2.5, y = 0.0;
	cout << "Пример вычисления выражения y = ((a + b)*c)/d припомощи FPU" << endl;
	cout << "\t где " << "a = " << a << ", b = " << b << ", c = " << c << ", d = " << d << endl;
	_asm {
		finit // Инициализация FPU
		fld a // Загрузить a в стек. ST(0) = a
		fld b // Загрузить b в стек. ST(0) = b, ST(1) = a
		fadd // Сложить a и b. ST(0) = ST(0) + ST(1)
		fld c // Загрузить c в стек. ST(0) = c, ST(1) = a+b
		fmul ST(0), ST(1) // Умножить (a+b) на c. ST(0) = ST(0)*ST(1)
		fdiv d // Разделить (a+b)*c на d. ST(0) = ST(0) / d
		fst y // Сохранить результат в y
		fwait // Синхронизация
	}
	cout << "Результат y = " << y << endl;
	system("pause");
}