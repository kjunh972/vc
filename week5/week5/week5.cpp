#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include <windows.h>

// 그림 그리기 시작점.
POINT startPoint;
BOOL isDrawing = FALSE; // 그림 그리기 모드인지 여부.
BOOL isMoving = FALSE;  // 그림 이동 모드인지 여부.
HWND hwnd;              // 윈도우 핸들.
HBRUSH blueBrush, whiteBrush; // 파란색과 흰색 브러시.

int moveOffsetX, moveOffsetY;  // 그림 이동 시 시작점과 마우스 포인터의 차이
int lastDrawStartX, lastDrawStartY, lastDrawEndX, lastDrawEndY; // 마지막으로 그린 사각형의 좌표

// 윈도우 프로시저 함수
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    if (uMsg == WM_CREATE) {
        // 초기화
        whiteBrush = CreateSolidBrush(RGB(255, 255, 255)); // 흰색 브러시 생성
        blueBrush = CreateSolidBrush(RGB(0, 0, 255));      // 파란색 브러시 생성
        isMoving = FALSE;
        isDrawing = FALSE;
        moveOffsetX = 0;
        moveOffsetY = 0;
        lastDrawStartX = 0;
        lastDrawStartY = 0;
        lastDrawEndX = 0;
        lastDrawEndY = 0;
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        // 왼쪽 마우스 버튼 클릭 시 그림 그리기 시작
        isDrawing = TRUE;
        startPoint.x = LOWORD(lParam);
        startPoint.y = HIWORD(lParam);
    }
    else if (uMsg == WM_LBUTTONUP) {
        if (isDrawing) {
            // 그림 그리기 모드에서 왼쪽 마우스 버튼 떼면 그림 그리기 종료
            isDrawing = FALSE;
            int endX = LOWORD(lParam);
            int endY = HIWORD(lParam);
            HDC hdc = GetDC(hwnd);
            SelectObject(hdc, whiteBrush);
            SetROP2(hdc, R2_WHITE);
            Rectangle(hdc, lastDrawStartX, lastDrawStartY, lastDrawEndX, lastDrawEndY);
            SetROP2(hdc, R2_COPYPEN);
            SelectObject(hdc, blueBrush);
            Rectangle(hdc, startPoint.x, startPoint.y, endX, endY);
            ReleaseDC(hwnd, hdc);
            lastDrawStartX = startPoint.x;
            lastDrawStartY = startPoint.y;
            lastDrawEndX = endX;
            lastDrawEndY = endY;
        }
    }
    else if (uMsg == WM_RBUTTONDOWN) {
        if (LOWORD(lParam) >= lastDrawStartX && LOWORD(lParam) <= lastDrawEndX &&
            HIWORD(lParam) >= lastDrawStartY && HIWORD(lParam) <= lastDrawEndY) {
            // 마우스 오른쪽 버튼 클릭 시 그림 이동 모드로 전환
            isMoving = TRUE;
            moveOffsetX = LOWORD(lParam) - lastDrawStartX;
            moveOffsetY = HIWORD(lParam) - lastDrawStartY;
        }
    }
    else if (uMsg == WM_RBUTTONUP) {
        // 마우스 오른쪽 버튼 떼면 그림 이동 모드 종료
        isMoving = FALSE;
    }
    else if (uMsg == WM_MOUSEMOVE) {
        if (isDrawing) {
            // 그림 그리기 모드에서 마우스 이동 시 사각형 그리기
            int nowX = LOWORD(lParam);
            int nowY = HIWORD(lParam);
            HDC hdc = GetDC(hwnd);
            SetROP2(hdc, R2_COPYPEN);
            Rectangle(hdc, startPoint.x, startPoint.y, nowX, nowY);
            SetROP2(hdc, R2_NOTXORPEN);
            SelectObject(hdc, blueBrush);
            Rectangle(hdc, startPoint.x, startPoint.y, nowX, nowY);
            ReleaseDC(hwnd, hdc);
        }
        else if (isMoving && (wParam & MK_RBUTTON)) {
            // 그림 이동 모드에서 마우스 이동 시 그림 이동
            int newX = LOWORD(lParam) - moveOffsetX;
            int newY = HIWORD(lParam) - moveOffsetY;
            int width = lastDrawEndX - lastDrawStartX;
            int height = lastDrawEndY - lastDrawStartY;
            HDC hdc = GetDC(hwnd);
            SelectObject(hdc, whiteBrush);
            SetROP2(hdc, R2_WHITE);
            Rectangle(hdc, lastDrawStartX, lastDrawStartY, lastDrawEndX, lastDrawEndY);
            SetROP2(hdc, R2_COPYPEN);
            SelectObject(hdc, blueBrush);
            Rectangle(hdc, newX, newY, newX + width, newY + height);
            ReleaseDC(hwnd, hdc);
            lastDrawStartX = newX;
            lastDrawStartY = newY;
            lastDrawEndX = newX + width;
            lastDrawEndY = newY + height;
        }
    }
    else if (uMsg == WM_DESTROY) {
        // 윈도우 파괴 시 종료
        PostQuitMessage(0);
    }
    else {
        // 기본 처리
        result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return result;
}

// 진입점
#ifdef UNICODE
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
#endif
{
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));

    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("MyDrawingApp"); // 윈도우 클래스 이름
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 아이콘
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);   // 커서
    wc.style = CS_HREDRAW | CS_VREDRAW; // 윈도우 스타일
    wc.lpfnWndProc = WindowProc; // 윈도우 프로시저 함수 설정

    if (RegisterClass(&wc) == 0) {
        // 윈도우 클래스 등록 실패 시 종료
        exit(-1);
    }

    HWND hwnd = CreateWindow(
        wc.lpszClassName,
        TEXT("2022***** Kim Jinju"), // 윈도우 제목
        WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME), // 윈도우 스타일
        0, 0, 800, 600, // 윈도우 위치 및 크기
        NULL, NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        // 윈도우 생성 실패 시 종료
        exit(-1);
    }

    ShowWindow(hwnd, nCmdShow); // 윈도우 표시
    UpdateWindow(hwnd);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (msg.message != WM_QUIT) {
        if (GetMessage(&msg, hwnd, 0, 0)) {
            TranslateMessage(&msg); // 키보드 입력 번역
            DispatchMessage(&msg);  // 메시지 디스패치
        }
    }

    return 0;
}