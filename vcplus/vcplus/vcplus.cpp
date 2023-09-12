#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <stdio.h>
#include <conio.h>

#define ESC 27



int main()
{


	printf("김준형의 행복한 VC++");
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
			printf("q:qwer화이팅\n");

		}
		else if (s == 'w') {
			printf("w:과제 너무 좋다\n");
		}
		else if (s == 'e') {
			printf("e:담주부턴 과제량 3배다\n");
		}
		else if (s == 'r') {
			printf("r:행복합니다\n");
		}

		else {
			printf("잘못 입력하셨습니다. q,w,e,r중 입력해주세요.\n");
		}
	}
}
