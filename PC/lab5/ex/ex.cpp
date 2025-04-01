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
	cout << "������ ���������� ����� ��������� ������� ��� ������ SSE" << endl;
	for (short i = 0; i < arr_size; i++)
		cout << "array[" << i << "] = " << array[i] << endl;
	for (short i = 0; i < arr_size; i++)
		sum_c = sum_c + array[i];
	_asm {
		mov ebx, array // ����� ������� � ebx
		mov ecx, arr_size
		shr ecx, 2 // ����������� ������������ �� 4 float'� - ������� ����� ��������� �� 4
		xorpd xmm0, xmm0 // ����� ���������
		p0 :
		movdqu xmm1, [ebx] // �������� 4� ��������� � XMM1
			addps xmm0, xmm1 // ������������ �������� 4� float'��
				add ebx, 16 // ������� � ��������� ������� �����
				loop p0
				// ��������� ����� � 4� ������� ������ � XMM0, ���������� �� ����������� ���������������
				movdqu xmm1, xmm0 // ����� XMM0 � XMM1
				/*������������ ���: ����� ���������:
				������� 127...96 ��������� � ������� 63...32 ��������
				������� 95...64 ��������� � ������� 31...00 ��������
				��������� �� �������! ����� ��������� 1 1 1 0 1 1 1 0 = 0XEE*/
				pshufd xmm1, xmm1, 0xEE // ����������� ��� � XMM1
				addps xmm0, xmm1 // �����
				movdqu xmm1, xmm0 // ����� XMM0 � XMM1
				/*������������ ���: ����� ���������:
				������� 63...32 ��������� � ������� 31...00 ��������
				��������� �� �������! ����� ��������� 1 1 1 0 0 1 0 1 = 0XE5*/
				pshufd xmm1, xmm1, 0xE5 // ����������� ��� � XMM1
				addps xmm0, xmm1 // �����
				movss sum_sse, xmm0 // �������� ����� � ������� 32 �������� XMM0
	}
	cout << "����� ��������� ������� �� �� = " << sum_c << endl;
	cout << "����� �������� �� ���������� � �������������� SSE = " << sum_sse << endl;
	delete[] array;
	system("pause");
}