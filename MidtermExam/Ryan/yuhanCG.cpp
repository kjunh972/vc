#include "yuhanCG.h"

void DrawShapes(HWND hWnd, HDC hdc) {
    for (size_t i = 0; i < shapes.size(); i++) {
        HBRUSH hBrush = NULL; // hBrush�� NULL�� �ʱ�ȭ

        if (shapes[i].shapeType == 0) {
            hBrush = CreateSolidBrush(RGB(255, 0, 0)); // ������ �簢��
            SelectObject(hdc, hBrush);
            Rectangle(hdc, shapes[i].rect.left, shapes[i].rect.top, shapes[i].rect.right, shapes[i].rect.bottom);
        }
        else if (shapes[i].shapeType == 1) {
            // �� �׸���
            DrawCircle(hdc, (shapes[i].rect.left + shapes[i].rect.right) / 2, (shapes[i].rect.top + shapes[i].rect.bottom) / 2, (shapes[i].rect.right - shapes[i].rect.left) / 2);
        }
        else if (shapes[i].shapeType == 2) { // "Bonobono" ��� �׸��� ó��
            // "Bonobono" ��� �׸���
            DrawBonobono(hWnd, hdc, blink);
        }
        else if (shapes[i].shapeType == 3) { // "Ryan" ��� �׸��� ó��
            // "Ryan" ��� �׸���
            DrawRyan(hWnd, hdc, shapes[i].rect.left, shapes[i].rect.top, shapes[i].rect.right, shapes[i].rect.bottom);
        }
        else if (shapes[i].shapeType == 4) { // "ť��" ��� �׸��� ó��
            DrawCube(hdc, shapes[i].rect.left, shapes[i].rect.top, shapes[i].rect.right, shapes[i].rect.bottom);
        }

        if (hBrush != NULL) { // hBrush�� NULL�� �ƴ� ��쿡�� ���
            DeleteObject(hBrush);
        }
    }
}

void StartDrawing(int x, int y) {
    startPoint.x = x;
    startPoint.y = y;
    endPoint.x = x;
    endPoint.y = y;
}

void StartDrawingCircle(int x, int y) {
    centerPoint.x = x;
    centerPoint.y = y;
    radius = 0;
}

void UpdateDrawing(int x, int y) {
    endPoint.x = x;
    endPoint.y = y;
    InvalidateRect(NULL, NULL, FALSE);
}

void UpdateDrawingCircle(int x, int y) {
    radius = max(abs(x - centerPoint.x), abs(y - centerPoint.y));
    InvalidateRect(NULL, NULL, FALSE);
}

void StopDrawing() {
    CustomShape newShape;
    newShape.isDrawing = false;
    newShape.shapeType = currentShapeType;
    newShape.rect.left = min(startPoint.x, endPoint.x);
    newShape.rect.top = min(startPoint.y, endPoint.y);
    newShape.rect.right = max(startPoint.x, endPoint.x);
    newShape.rect.bottom = max(startPoint.y, endPoint.y);

    shapes.push_back(newShape);
    InvalidateRect(NULL, NULL, FALSE);
}

void StopDrawingCircle() {
    CustomShape newShape;
    newShape.isDrawing = false;
    newShape.shapeType = 1; // �� Ÿ���� ��Ÿ���� ���� ���� (�ٸ� ���� ��ġ�� �ʵ���)
    newShape.rect.left = centerPoint.x - radius;
    newShape.rect.top = centerPoint.y - radius;
    newShape.rect.right = centerPoint.x + radius;
    newShape.rect.bottom = centerPoint.y + radius;

    shapes.push_back(newShape);
    InvalidateRect(NULL, NULL, FALSE);
}

// Ŀ���� ���콺 Ŀ�� ����
void SetCustomCursor(HWND hWnd, int cursorType) {
    HCURSOR hCursor;
    switch (cursorType) {
    case 0:
        hCursor = LoadCursor(NULL, IDC_ARROW);
        break;
    case 1:
        hCursor = LoadCursor(NULL, IDC_CROSS);
        break;
    default:
        hCursor = LoadCursor(NULL, IDC_ARROW);
    }

    if (hCursor) {
        SetCursor(hCursor);
    }
}

// ��ư ũ��� ��ġ ����
void ResizeButtons(HWND hWnd) {
    RECT rect;
    GetClientRect(hWnd, &rect);

    int buttonMargin = 8;
    int buttonHeight = 55; // ��ư ����
    int buttonWidth = (rect.right - 2 * buttonMargin - (4 * 8)) / 5; // ��ư ����

    int innerBoxTop = rect.bottom - innerBoxHeight - innerBoxBottomMargin - 2 * 8;
    for (int i = 0; i < 5; i++) {
        LPCWSTR buttonName = NULL;

        switch (i) {
        case 0:
            buttonName = L"Box";
            break;
        case 1:
            buttonName = L"Circle";
            break;
        case 2:
            buttonName = L"Bonobono";
            break;
        case 3:
            buttonName = L"Ryan";
            break;
        case 4:
            buttonName = L"Cube";
            break;
        }

        // ��ư ���� �� ��ġ ����
        hButtons[i] = CreateWindow(
            L"BUTTON", buttonName, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            0, 0, 100, 50, hWnd, (HMENU)(i + 1), hInstance, NULL
        );

        int left = buttonMargin + i * (buttonWidth + buttonMargin) + 8; // +8�κ��� �����ϸ� ���������� �̵�����
        int top = innerBoxTop - buttonHeight;
        int right = left + buttonWidth - 15; // // ���� ��ư�� ���̸� ��������
        int bottom = top + buttonHeight;

        SetWindowPos(hButtons[i], NULL, left, top, right - left, bottom - top, SWP_NOZORDER);
    }
}

// ������ ���ν��� (�޽��� ó��)
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_LBUTTONDOWN:
        if (currentShapeType == 1) { // �� �׸��� ����� ��
            shapes.clear();
            HDC hdc = GetDC(hWnd); // 'hdc' �ʱ�ȭ
            InvalidateRect(hWnd, NULL, FALSE);
            StartDrawingCircle(LOWORD(lParam), HIWORD(lParam));
        }
        else {
            shapes.clear();
            HDC hdc = GetDC(hWnd); // 'hdc' �ʱ�ȭ
            InvalidateRect(hWnd, NULL, FALSE);
            StartDrawing(LOWORD(lParam), HIWORD(lParam));
        }
        if (currentShapeType == 3) { // ���̾� �� �׸��� ����� ��
            shapes.clear();
            HDC hdc = GetDC(hWnd);
            InvalidateRect(hWnd, NULL, FALSE);
            StartDrawing(LOWORD(lParam), HIWORD(lParam));
        }
        break;

    case WM_LBUTTONUP:
        if (currentShapeType == 1) { // �� �׸��� ����� ��
            StopDrawingCircle();
        }
        else {
            StopDrawing();
        }
        if (currentShapeType == 3) { // ���̾� �� �׸��� ����� ��
            StopDrawing();
        }
        break;
    case WM_CREATE:
        ResizeButtons(hWnd);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1: // "�簢��" ��ư�� ������ ��
            currentShapeType = 0;
            SetFocus(hWnd); // ��Ŀ���� ������� ����
            // "�簢��" ��ư�� ���� �� �ٸ� ���� �����
            shapes.clear();
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        case 2: // "��" ��ư�� ������ ��
            currentShapeType = 1;
            SetFocus(hWnd); // ��Ŀ���� ������� ����
            // "��" ��ư�� ���� �� �ٸ� ���� �����
            shapes.clear();
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        case 3: // "���뺸��" ��ư�� ������ ��
            currentShapeType = 2; // "���뺸��" ����� ��Ÿ���� ��
            SetFocus(hWnd); // ��Ŀ���� ������� ����
            shapes.clear(); // �ٸ� ������ �����
            StopDrawing(); // ��� ���뺸�� �߰�
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        case 4: // "���̾�" ��ư�� ������ ��
            currentShapeType = 3;
            SetFocus(hWnd); // ��Ŀ���� ������� ����
            shapes.clear();
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        case 5: // "ť��" ��ư�� ������ ��
            currentShapeType = 4;
            SetFocus(hWnd); // ��Ŀ���� ������� ����
            // "ť��" ��ư�� ���� �� �ٸ� ���� �����
            shapes.clear();
            StopDrawing(); // ��� ť�� �߰�
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps); // 'hdc' �ʱ�ȭ
        DrawBox(hWnd, hdc); // �ٱ��� ���ڿ� ���� ���� �׸���
        DrawShapes(hWnd, hdc); // ���� �׸���

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_SIZE:
        ResizeButtons(hWnd);
        break;
        // ���콺 �̵� �� ó��
    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON) { // ���콺 ���� ��ư�� ���� ���¿����� ������Ʈ
            if (currentShapeType == 1) { // �� �׸��� ����� ��
                UpdateDrawingCircle(LOWORD(lParam), HIWORD(lParam));
            }
            else {
                UpdateDrawing(LOWORD(lParam), HIWORD(lParam));
            }
            if (currentShapeType == 3) { // ���̾� �� �׸��� ����� ��
                UpdateDrawing(LOWORD(lParam), HIWORD(lParam));
            }
        }
        break;
        // �����̽��� ���� �� ����
    case WM_KEYDOWN:
        if (wParam == VK_SPACE) {
            blink = 1; // ���� ������ �׸����� ����
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;

        // �����̽��� ���� �� ����
    case WM_KEYUP:
        if (wParam == VK_SPACE) {
            blink = 0; // ���� ������ �׸��� �ʵ��� ����
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// ���� �Լ�
int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wcex;
    HWND hWnd;
    MSG msg;

    hInstance = hInst;

    // ������ Ŭ���� ���
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
    wcex.lpszClassName = L"MainWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, L"Call to RegisterClassEx failed!", L"Win32 Guided Tour", NULL);
        return 1;
    }

    // ������ ����
    hWnd = CreateWindow(
        L"MainWindowClass",
        L"Drawing App",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 480,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd) {
        MessageBox(NULL, L"Call to CreateWindow failed!", L"Win32 Guided Tour", NULL);
        return 1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // �޽��� ����
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}