#include <windows.h>

// 박스를 나타내는 변수 (시작하자마자 박스가 생성되도록 초기값을 true로 설정)
bool isBoxVisible = true;

HWND hButtons[5]; // 버튼 핸들 배열

// 박스 그리기 함수
void DrawBox(HWND hWnd, HDC hdc) {
    RECT rect;
    GetClientRect(hWnd, &rect);

    // 바깥쪽 상자의 Margin 및 Padding 크기
    int outerMargin = 8;
    int outerPadding = 8;

    // 내부 상자의 Margin 및 Padding 크기
    int innerMargin = 0;
    int innerPadding = 0;

    // 바깥쪽 상자의 색을 r:255 g:240 b:200로 설정
    HBRUSH hOuterBoxBrush = CreateSolidBrush(RGB(255, 240, 200));
    SelectObject(hdc, hOuterBoxBrush);
    Rectangle(hdc, outerMargin, outerMargin, rect.right - outerMargin, rect.bottom - outerMargin);
    DeleteObject(hOuterBoxBrush);

    // 내부 상자의 색을 RGB(255, 255, 255)로 설정
    HBRUSH hInnerBoxBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hInnerBoxBrush);
    Rectangle(hdc, outerMargin + outerPadding, outerMargin + outerPadding, rect.right - outerMargin - outerPadding, rect.bottom - outerMargin - outerPadding);
    DeleteObject(hInnerBoxBrush);
}

// 마우스 커서 변경 함수
void SetCustomCursor(HWND hWnd) {
    HCURSOR hCrossCursor = LoadCursor(NULL, IDC_CROSS); // +(CROSS) 모양의 커서 로드
    if (hCrossCursor) {
        SetCursor(hCrossCursor);
    }
}

// 윈도우 크기가 변경될 때 버튼 크기 조정
void ResizeButtons(HWND hWnd) {
    RECT rect;
    GetClientRect(hWnd, &rect);

    int buttonMargin = 16;

    // 버튼 크기 계산
    int buttonWidth = (rect.right - 6 * buttonMargin) / 5;
    int buttonHeight = 64;

    for (int i = 0; i < 5; i++) {
        int left = buttonMargin + i * (buttonWidth + buttonMargin);
        int top = buttonMargin;
        int right = left + buttonWidth;
        int bottom = top + buttonHeight;
        SetWindowPos(hButtons[i], NULL, left, top, right - left, bottom - top, SWP_NOZORDER);
    }
}

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            // 첫 번째 버튼 클릭
            isBoxVisible = true;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (LOWORD(wParam) == 2) {
            // 두 번째 버튼 클릭
            isBoxVisible = false;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        DrawBox(hWnd, hdc);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_SETCURSOR:
        SetCustomCursor(hWnd); // 커서 설정 함수 호출
        break;
    case WM_SIZE:
        ResizeButtons(hWnd); // 윈도우 크기가 변경될 때 버튼 크기 조정
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 프로그램 진입점
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

    HWND hWnd;

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"ButtonWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        return 1;
    }

    // 뷰 사이즈를 800px * 480px로 설정
    hWnd = CreateWindow(
        L"ButtonWindowClass", L"곰돌이", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
        CW_USEDEFAULT, 0, 800, 480, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return FALSE;
    }

    // 뷰 색상을 r:255 g:240 b:200로 설정
    SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(255, 240, 200)));

    // 버튼 1: Box
    hButtons[0] = CreateWindow(
        L"BUTTON", L"Box", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0, 0, 160, 64, hWnd, (HMENU)1, hInstance, NULL);

    // 버튼 2: Circle
    hButtons[1] = CreateWindow(
        L"BUTTON", L"Circle", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0, 0, 160, 64, hWnd, (HMENU)2, hInstance, NULL);

    // 버튼 3: Bonobono
    hButtons[2] = CreateWindow(
        L"BUTTON", L"Bonobono", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0, 0, 160, 64, hWnd, (HMENU)3, hInstance, NULL);

    // 버튼 4: Ryan
    hButtons[3] = CreateWindow(
        L"BUTTON", L"Ryan", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0, 0, 160, 64, hWnd, (HMENU)4, hInstance, NULL);

    // 버튼 5: Cube
    hButtons[4] = CreateWindow(
        L"BUTTON", L"Cube", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0, 0, 160, 64, hWnd, (HMENU)5, hInstance, NULL);

    // 최대화 버튼 제거
    HMENU hSysMenu = GetSystemMenu(hWnd, FALSE);
    RemoveMenu(hSysMenu, SC_MAXIMIZE, MF_BYCOMMAND);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
