#include <windows.h>
#include <vector>
#include <tchar.h>

struct CustomShape {
    RECT rect;
    bool isDrawing;
    int shapeType; // 0: �簢��, 1: �ﰢ��, 2: ������ü, 3: ��, 4: ��
};

std::vector<CustomShape> shapes;

int currentShapeType = 0; // ���� �׸� ���� Ÿ��

POINT startPoint; // �巡�� ���� ����
POINT endPoint;   // �巡�� �� ����

POINT centerPoint; // ���� �߽�
int radius;        // ���� ������

void Draw3DBox(HDC hdc, int left, int top, int right, int bottom) {
    // �׸��� �׸���
    HBRUSH shadowBrush = CreateSolidBrush(RGB(192, 192, 192));
    SelectObject(hdc, shadowBrush);
    Rectangle(hdc, left + 10, top + 10, right + 10, bottom + 10);
    DeleteObject(shadowBrush);

    // ���� �׸���
    HBRUSH frontBrush = CreateSolidBrush(RGB(255, 0, 0)); // ������
    SelectObject(hdc, frontBrush);
    Rectangle(hdc, left, top, right, bottom);
    DeleteObject(frontBrush);

    // ������ �� �׸���
    HBRUSH rightBrush = CreateSolidBrush(RGB(128, 0, 0)); // ��ο� ������
    SelectObject(hdc, rightBrush);
    POINT rightPoints[4] = {
        {right, top},
        {right + 10, top + 10},
        {right + 10, bottom + 10},
        {right, bottom}
    };
    Polygon(hdc, rightPoints, 4);
    DeleteObject(rightBrush);

    // ���� �׸���
    HBRUSH topBrush = CreateSolidBrush(RGB(255, 255, 0)); // �����
    SelectObject(hdc, topBrush);
    POINT topPoints[4] = {
        {left, top},
        {right, top},
        {right + 10, top + 10},
        {left + 10, top + 10}
    };
    Polygon(hdc, topPoints, 4);
    DeleteObject(topBrush);

    // ���� �� ���� �׸���
    MoveToEx(hdc, left, top, NULL);
    LineTo(hdc, left + 10, top + 10);
    MoveToEx(hdc, right, top, NULL);
    LineTo(hdc, right + 10, top + 10);

    // ������ �� ���� �׸���
    MoveToEx(hdc, right, top, NULL);
    LineTo(hdc, right, bottom);
    MoveToEx(hdc, right + 10, top + 10, NULL);
    LineTo(hdc, right + 10, bottom + 10);

    // ���� �� ���� �׸���
    MoveToEx(hdc, left, top, NULL);
    LineTo(hdc, right, top);
    MoveToEx(hdc, left + 10, top + 10, NULL);
    LineTo(hdc, right + 10, top + 10);
}

void DrawCircle(HDC hdc, int centerX, int centerY, int radius) {
    HBRUSH circleBrush = CreateSolidBrush(RGB(0, 255, 0)); // ������� ���� ä���
    SelectObject(hdc, circleBrush);
    Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
    DeleteObject(circleBrush);

    // �� �׸��� (������)
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // ������ ������
    SelectObject(hdc, hPen);
    Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
    DeleteObject(hPen);
}

void DrawStar(HDC hdc, int centerX, int centerY, int size) {
    POINT points[10];
    double angle = 0;
    int length;

    // ���� ������ ��ǥ ���
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            length = size / 2;
        }
        else {
            length = size;
        }

        points[i].x = centerX + length * sin(angle);
        points[i].y = centerY - length * cos(angle);

        angle += 36 * 3.14159265 / 180.0;
    }

    HBRUSH starBrush = CreateSolidBrush(RGB(0, 0, 255)); // �Ķ������� ���� ä���
    SelectObject(hdc, starBrush);
    Polygon(hdc, points, 10);
    DeleteObject(starBrush);

    // �� ������ �׸���
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // ������ ������
    SelectObject(hdc, hPen);
    Polygon(hdc, points, 10);
    DeleteObject(hPen);
}

void DrawShapes(HWND hWnd, HDC hdc) {
    for (size_t i = 0; i < shapes.size(); i++) {
        HBRUSH hBrush = NULL; // hBrush�� NULL�� �ʱ�ȭ

        if (shapes[i].shapeType == 0) {
            hBrush = CreateSolidBrush(RGB(255, 0, 0)); // ������ �簢��
        }
        else if (shapes[i].shapeType == 1) {
            hBrush = CreateSolidBrush(RGB(0, 0, 255)); // �Ķ��� �ﰢ��
        }
        else if (shapes[i].shapeType == 2) {
            Draw3DBox(hdc, shapes[i].rect.left, shapes[i].rect.top, shapes[i].rect.right, shapes[i].rect.bottom);
        }
        else if (shapes[i].shapeType == 3) {
            DrawCircle(hdc, (shapes[i].rect.left + shapes[i].rect.right) / 2, (shapes[i].rect.top + shapes[i].rect.bottom) / 2, (shapes[i].rect.right - shapes[i].rect.left) / 2);
        }
        else if (shapes[i].shapeType == 4) {
            DrawStar(hdc, (shapes[i].rect.left + shapes[i].rect.right) / 2, (shapes[i].rect.top + shapes[i].rect.bottom) / 2, (shapes[i].rect.right - shapes[i].rect.left));
        }

        if (hBrush != NULL) { // hBrush�� NULL�� �ƴ� ���� ���
            SelectObject(hdc, hBrush);

            if (shapes[i].shapeType == 0) {
                Rectangle(hdc, shapes[i].rect.left, shapes[i].rect.top, shapes[i].rect.right, shapes[i].rect.bottom);
            }
            else if (shapes[i].shapeType == 1) {
                POINT points[3];
                points[0].x = (shapes[i].rect.left + shapes[i].rect.right) / 2;
                points[0].y = shapes[i].rect.top;
                points[1].x = shapes[i].rect.left;
                points[1].y = shapes[i].rect.bottom;
                points[2].x = shapes[i].rect.right;
                points[2].y = shapes[i].rect.bottom;
                Polygon(hdc, points, 3);
            }

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
    newShape.shapeType = 3; // �� Ÿ���� ��Ÿ���� ���� ���� (�ٸ� ���� ��ġ�� �ʵ���)
    newShape.rect.left = centerPoint.x - radius;
    newShape.rect.top = centerPoint.y - radius;
    newShape.rect.right = centerPoint.x + radius;
    newShape.rect.bottom = centerPoint.y + radius;

    shapes.push_back(newShape);
    InvalidateRect(NULL, NULL, FALSE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_LBUTTONDOWN:
        if (currentShapeType == 3) { // �� �׸��� ����� ��
            StartDrawingCircle(LOWORD(lParam), HIWORD(lParam));
        }
        else {
            StartDrawing(LOWORD(lParam), HIWORD(lParam));
        }
        break;
    case WM_LBUTTONUP:
        if (currentShapeType == 3) { // �� �׸��� ����� ��
            StopDrawingCircle();
        }
        else {
            StopDrawing();
        }
        break;
    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON) { // ���콺 ���� ��ư�� ���� ���¿����� ������Ʈ
            if (currentShapeType == 3) { // �� �׸��� ����� ��
                UpdateDrawingCircle(LOWORD(lParam), HIWORD(lParam));
            }
            else {
                UpdateDrawing(LOWORD(lParam), HIWORD(lParam));
            }
        }
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        DrawShapes(hWnd, hdc);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1: // "�簢��" ��ư�� ������ ��
            currentShapeType = 0;
            break;
        case 2: // "�ﰢ��" ��ư�� ������ ��
            currentShapeType = 1;
            break;
        case 3: // "������ü" ��ư�� ������ ��
            currentShapeType = 2;
            break;
        case 4: // "��" ��ư�� ������ ��
            currentShapeType = 3;
            break;
        case 5: // "��" ��ư�� ������ ��
            currentShapeType = 4;
            break;
        }
        break;
    case WM_CREATE: {
        // "�簢��" ��ư ����
        CreateWindow(_T("BUTTON"), _T("�簢��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 20, 100, 40, hWnd, (HMENU)1, NULL, NULL);

        // "�ﰢ��" ��ư ����
        CreateWindow(_T("BUTTON"), _T("�ﰢ��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 130, 20, 100, 40, hWnd, (HMENU)2, NULL, NULL);

        // "������ü" ��ư ����
        CreateWindow(_T("BUTTON"), _T("������ü"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 240, 20, 120, 40, hWnd, (HMENU)3, NULL, NULL);

        // "��" ��ư ����
        CreateWindow(_T("BUTTON"), _T("��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 370, 20, 100, 40, hWnd, (HMENU)4, NULL, NULL);

        // "��" ��ư ����
        CreateWindow(_T("BUTTON"), _T("��"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 480, 20, 100, 40, hWnd, (HMENU)5, NULL, NULL);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

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
    wcex.lpszClassName = L"DrawingWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        return 1;
    }

    hWnd = CreateWindow(
        L"DrawingWindowClass", L"Drawing Example", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
