#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <stdio.h>
#include <conio.h>

#define ESC 27
using namespace std;

int main()
{
    printf("김준형의 행복한 VC++");
    printf("\n");

    while (1)
    {
        char c;
        printf("command>");

        // 문자열을 저장할 배열
        char input[100];
        int index = 0;

        while (1)
        {
            c = _getch();
            if (c == 27) {
                return 0;;
            }
            else if (c == '\r') {
                input[index] = '\0'; 
                break;
            }
            else {
                printf("%c", c);
                input[index++] = c;
            }
        }
        printf("\n");

        if (strcmp(input, "q") == 0) {
            printf("q: qwer 화이팅\n");
        }
        else if (strcmp(input, "w") == 0) {
            printf("w: 과제 너무 좋다\n");
        }
        else if (strcmp(input, "e") == 0) {
            printf("e: 담주부턴 과제량 3배다\n");
        }
        else if (strcmp(input, "r") == 0) {
            printf("r: 행복합니다\n");
        }
        else if (strcmp(input, "exit") == 0) {
            break;
        }
        else {
            printf("잘못 입력하셨습니다. q, w, e, r 중 입력해주세요.\n");
        }
    }
    return 0;
}