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
        L"������",
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