#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include <windows.h>

// �׸� �׸��� ������.
POINT startPoint;
BOOL isDrawing = FALSE; // �׸� �׸��� ������� ����.
BOOL isMoving = FALSE;  // �׸� �̵� ������� ����.
HWND hwnd;              // ������ �ڵ�.
HBRUSH blueBrush, whiteBrush; // �Ķ����� ��� �귯��.

int moveOffsetX, moveOffsetY;  // �׸� �̵� �� �������� ���콺 �������� ����
int lastDrawStartX, lastDrawStartY, lastDrawEndX, lastDrawEndY; // ���������� �׸� �簢���� ��ǥ

// ������ ���ν��� �Լ�
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    if (uMsg == WM_CREATE) {
        // �ʱ�ȭ
        whiteBrush = CreateSolidBrush(RGB(255, 255, 255)); // ��� �귯�� ����
        blueBrush = CreateSolidBrush(RGB(0, 0, 255));      // �Ķ��� �귯�� ����
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
        // ���� ���콺 ��ư Ŭ�� �� �׸� �׸��� ����
        isDrawing = TRUE;
        startPoint.x = LOWORD(lParam);
        startPoint.y = HIWORD(lParam);
    }
    else if (uMsg == WM_LBUTTONUP) {
        if (isDrawing) {
            // �׸� �׸��� ��忡�� ���� ���콺 ��ư ���� �׸� �׸��� ����
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
            // ���콺 ������ ��ư Ŭ�� �� �׸� �̵� ���� ��ȯ
            isMoving = TRUE;
            moveOffsetX = LOWORD(lParam) - lastDrawStartX;
            moveOffsetY = HIWORD(lParam) - lastDrawStartY;
        }
    }
    else if (uMsg == WM_RBUTTONUP) {
        // ���콺 ������ ��ư ���� �׸� �̵� ��� ����
        isMoving = FALSE;
    }
    else if (uMsg == WM_MOUSEMOVE) {
        if (isDrawing) {
            // �׸� �׸��� ��忡�� ���콺 �̵� �� �簢�� �׸���
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
            // �׸� �̵� ��忡�� ���콺 �̵� �� �׸� �̵�
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
        // ������ �ı� �� ����
        PostQuitMessage(0);
    }
    else {
        // �⺻ ó��
        result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return result;
}

// ������
#ifdef UNICODE
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
#endif
{
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));

    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("MyDrawingApp"); // ������ Ŭ���� �̸�
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // ������
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);   // Ŀ��
    wc.style = CS_HREDRAW | CS_VREDRAW; // ������ ��Ÿ��
    wc.lpfnWndProc = WindowProc; // ������ ���ν��� �Լ� ����

    if (RegisterClass(&wc) == 0) {
        // ������ Ŭ���� ��� ���� �� ����
        exit(-1);
    }

    HWND hwnd = CreateWindow(
        wc.lpszClassName,
        TEXT("2022***** Kim Jinju"), // ������ ����
        WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME), // ������ ��Ÿ��
        0, 0, 800, 600, // ������ ��ġ �� ũ��
        NULL, NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        // ������ ���� ���� �� ����
        exit(-1);
    }

    ShowWindow(hwnd, nCmdShow); // ������ ǥ��
    UpdateWindow(hwnd);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (msg.message != WM_QUIT) {
        if (GetMessage(&msg, hwnd, 0, 0)) {
            TranslateMessage(&msg); // Ű���� �Է� ����
            DispatchMessage(&msg);  // �޽��� ����ġ
        }
    }

    return 0;
}