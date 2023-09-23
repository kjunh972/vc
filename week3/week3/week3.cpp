#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#define up 72 
#define down 80
#define left 75 
#define right 77 

// 문자 배열을 초기화
void set(int array[], int x)
{
    for (int i = 0; i < x; i++) {
        array[i] = 0;
    }
}

int main()
{
    char num = ' '; // 사용자에게 입력 받을 변수
    int value[100] = {}; // 사용자에게 입력받은 문자를 저장할 배열
    int cn = 0; // 배열의 입력된 수

    while (num != 27) {
        // 문자 배열이 꽉 찬 경우 프로그램 종료
        if (cn > 92) {
            break;
        }
        else if (cn >= 5) {

            for (int i = 0; i < cn; i++) {
                // UP DOWN UP DOWN
                if ((value[i] == -32 && value[i + 1] == 1) &&
                    (value[i + 2] == -32 && value[i + 3] == 2) &&
                    (value[i + 4] == -32 && value[i + 5] == 1) &&
                    (value[i + 6] == -32 && value[i + 7] == 2)) {
                    printf("\n퇴근퇴근퇴근 버프 ~!!!\n");
                    set(value, cn);
                    cn = 0;
                }
                // LEFT UP RIGHT t
                if ((value[i] == -32 && value[i + 1] == 3) && 
                    (value[i + 2] == -32 && value[i + 3] == 1) && 
                    (value[i + 4] == -32 && value[i + 5] == 4) && 
                    ( value[i + 6] == 't')) {
                    printf("\n너 t야..?? 아브라케다브라!!!\n");
                    set(value, cn);
                    cn = 0;
                }
                // j LEFT RIGHT
                if ((char)(value[i] == 'j') && 
                    (value[i + 1] == -32 && value[i + 2] == 3) && 
                    (value[i + 3] == -32 && value[i + 4] == 4)) {
                    printf("\n흐아아아아압 기합!!\n");
                    set(value, cn);
                    cn = 0;
                }
            }
        }

        num = _getch();
        if (num == left) {
            if (value[cn - 1] == -32) {
                printf("← ");
                value[cn] = 3;
            }
            else {
                printf("%c ", num);
                value[cn] = num;
            }
            cn++;
        }
        else if (num == up) {
            if (value[cn - 1] == -32) {
                printf("↑ ");
                value[cn] = 1;
            }
            else {
                printf("%c ", num);
                value[cn] = num;
            }
            cn++;
        }
        else if (num == right) {
            if (value[cn - 1] == -32) {
                printf("→ ");
                value[cn] = 4;
            }
            else {
                printf("%c ", num);
                value[cn] = num;
            }
            cn++;
        }
        else if (num == down) {
            if (value[cn - 1] == -32) {
                printf("↓ ");
                value[cn] = 2;
            }
            else {
                printf("%c ", num);
                value[cn] = num;
            }
            cn++;
        }
        else {
            if (num != -32) {
                printf("%c ", num);
                value[cn] = num;
            }
            else {
                value[cn] = num;
            }
            cn++;
        }

        Sleep(300);
    }
}