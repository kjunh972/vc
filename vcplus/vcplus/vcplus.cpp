#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <stdio.h>
#include <conio.h>

#define ESC 27



int main()
{


	printf("�������� �ູ�� VC++");
	printf("\n");

	while (1)
	{
		char s;
		printf("command>");



		s = _getche();
		if (s == 27) {
			break;
		}
		while (1) {
			char enter = _getch();
			if (enter == '\r')
				break;
		}
		printf("\n");

		if (s == 'q') {
			printf("q:qwerȭ����\n");

		}
		else if (s == 'w') {
			printf("w:���� �ʹ� ����\n");
		}
		else if (s == 'e') {
			printf("e:���ֺ��� ������ 3���\n");
		}
		else if (s == 'r') {
			printf("r:�ູ�մϴ�\n");
		}

		else {
			printf("�߸� �Է��ϼ̽��ϴ�. q,w,e,r�� �Է����ּ���.\n");
		}
	}
}
