#include <windows.h>

// �ڽ��� ��Ÿ���� ���� (�������ڸ��� �ڽ��� �����ǵ��� �ʱⰪ�� true�� ����)
bool isBoxVisible = true;

HWND hButtons[5]; // ��ư �ڵ� �迭

// �ڽ� �׸��� �Լ�
void DrawBox(HWND hWnd, HDC hdc) {
    RECT rect;
    GetClientRect(hWnd, &rect);

    // �ٱ��� ������ Margin �� Padding ũ��
    int outerMargin = 8;
    int outerPadding = 8;

    // ���� ������ Margin �� Padding ũ��
    int innerMargin = 0;
    int innerPadding = 0;

    // �ٱ��� ������ ���� r:255 g:240 b:200�� ����
    HBRUSH hOuterBoxBrush = CreateSolidBrush(RGB(255, 240, 200));
    SelectObject(hdc, hOuterBoxBrush);
    Rectangle(hdc, outerMargin, outerMargin, rect.right - outerMargin, rect.bottom - outerMargin);
    DeleteObject(hOuterBoxBrush);

    // ���� ������ ���� RGB(255, 255, 255)�� ����
    HBRUSH hInnerBoxBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hInnerBoxBrush);
    Rectangle(hdc, outerMargin + outerPadding, outerMargin + outerPadding, rect.right - outerMargin - outerPadding, rect.bottom - outerMargin - outerPadding);
    DeleteObject(hInnerBoxBrush);
}

// ���콺 Ŀ�� ���� �Լ�
void SetCustomCursor(HWND hWnd) {
    HCURSOR hCrossCursor = LoadCursor(NULL, IDC_CROSS); // +(CROSS) ����� Ŀ�� �ε�
    if (hCrossCursor) {
        SetCursor(hCrossCursor);
    }
}

// ������ ũ�Ⱑ ����� �� ��ư ũ�� ����
void ResizeButtons(HWND hWnd) {
    RECT rect;
    GetClientRect(hWnd, &rect);

    int buttonMargin = 16;

    // ��ư ũ�� ���
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

// ������ ���ν���
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            // ù ��° ��ư Ŭ��
            isBoxVisible = true;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (LOWORD(wParam) == 2) {
            // �� ��° ��ư Ŭ��
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
        SetCustomCursor(hWnd); // Ŀ�� ���� �Լ� ȣ��
        break;
    case WM_SIZE:
        ResizeButtons(hWnd); // ������ ũ�Ⱑ ����� �� ��ư ũ�� ����
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���α׷� ������
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

    // �� ����� 800px * 480px�� ����
    hWnd = CreateWindow(
        L"ButtonWindowClass", L"������", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
        CW_USEDEFAULT, 0, 800, 480, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return FALSE;
    }

    // �� ������ r:255 g:240 b:200�� ����
    SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(255, 240, 200)));

    // ��ư 1: Box
    hButtons[0] = CreateWindow(
        L"BUTTON", L"Box", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0, 0, 160, 64, hWnd, (HMENU)1, hInstance, NULL);

    // ��ư 2: Circle
    hButtons[1] = CreateWindow(
        L"BUTTON", L"Circle", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0, 0, 160, 64, hWnd, (HMENU)2, hInstance, NULL);

    // ��ư 3: Bonobono
    hButtons[2] = CreateWindow(
        L"BUTTON", L"Bonobono", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0, 0, 160, 64, hWnd, (HMENU)3, hInstance, NULL);

    // ��ư 4: Ryan
    hButtons[3] = CreateWindow(
        L"BUTTON", L"Ryan", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0, 0, 160, 64, hWnd, (HMENU)4, hInstance, NULL);

    // ��ư 5: Cube
    hButtons[4] = CreateWindow(
        L"BUTTON", L"Cube", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        0, 0, 160, 64, hWnd, (HMENU)5, hInstance, NULL);

    // �ִ�ȭ ��ư ����
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
