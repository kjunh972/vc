#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <stdio.h>
#include <conio.h>

#define ESC 27
using namespace std;

int main()
{
    printf("�������� �ູ�� VC++");
    printf("\n");

    while (1)
    {
        char c;
        printf("command>");

        // ���ڿ��� ������ �迭
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
            printf("q: qwer ȭ����\n");
        }
        else if (strcmp(input, "w") == 0) {
            printf("w: ���� �ʹ� ����\n");
        }
        else if (strcmp(input, "e") == 0) {
            printf("e: ���ֺ��� ������ 3���\n");
        }
        else if (strcmp(input, "r") == 0) {
            printf("r: �ູ�մϴ�\n");
        }
        else if (strcmp(input, "exit") == 0) {
            break;
        }
        else {
            printf("�߸� �Է��ϼ̽��ϴ�. q, w, e, r �� �Է����ּ���.\n");
        }
    }
    return 0;
}