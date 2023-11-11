#include <windows.h> // Windows API ��� ����
#include <vector>
#include <tchar.h> // TCHAR�� ���õ� ��� ����
#include <cmath> // ���� �Լ� ��� ����

// ����� ���� ���� ����ü
struct CustomShape {
    RECT rect;      // ������ ��ġ�� ũ��
    bool isDrawing; // ���� �׸��� ������ ����
    int shapeType;  // ������ Ÿ�� (0: �簢��, 1: ��, 2: ���뺸��, 3: ���̾�, 4: ť��)
};

int currentShapeType = 0; // ���� �׸� ���� Ÿ��
bool drawEyesAsLines = false; // ���� ������ �׸��� ����

std::vector<CustomShape> shapes; // �׸� ������ ������ ����

bool isBoxVisible = true; // �簢���� ���̴��� ����
HWND hButtons[5]; // 5���� ��ư ������ �ڵ��� �����ϴ� �迭

int innerBoxHeight = 338; // ���� ������ ����
int innerBoxTopMargin = 16; // ���� ���� ��� ����
int innerBoxBottomMargin = 16; // ���� ���� �ϴ� ����

bool isMouseInsideInnerBox = false; // ���콺�� ���� ���� �ȿ� �ִ��� ���θ� ��Ÿ���� �÷���

int outerMargin = 8; // �ܺ� ����
int outerPaddingX = 8; // �ܺ� ������ ���� �е�

HINSTANCE hInstance; // ���� �ν��Ͻ�

POINT startPoint; // �巡�� ���� ����
POINT endPoint;   // �巡�� �� ����

POINT centerPoint; // ���� �߽�
int radius;        // ���� ������

// ���� ���ڸ� �׸��� �Լ�
void DrawBox(HWND hWnd, HDC hdc) {
    RECT rect;
    GetClientRect(hWnd, &rect);

    int outerPadding = 8;
    int outerBoxHeight = rect.bottom - 2 * outerMargin;

    // ���� ������ ���̰� ������ �ʰ����� �ʵ���
    if (innerBoxHeight > (outerBoxHeight - innerBoxTopMargin - innerBoxBottomMargin - 2 * outerPadding))
        innerBoxHeight = outerBoxHeight - innerBoxTopMargin - innerBoxBottomMargin - 2 * outerPadding;

    int innerMargin = 0;
    int innerPadding = 0;

    // ��� �귯�� ���� �� ����
    HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(255, 240, 200));
    SelectObject(hdc, hBackgroundBrush);
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    FillRect(hdc, &clientRect, hBackgroundBrush);
    DeleteObject(hBackgroundBrush);

    // �ܺ� ������ ��� �귯�� ���� �� �׸���
    HBRUSH hOuterBoxBrush = CreateSolidBrush(RGB(255, 240, 200));
    SelectObject(hdc, hOuterBoxBrush);
    Rectangle(hdc, outerMargin, outerMargin, rect.right - outerMargin, rect.bottom - outerMargin);
    DeleteObject(hOuterBoxBrush);

    // ���� ������ ��� �귯�� ���� �� �׸���
    HBRUSH hInnerBoxBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hInnerBoxBrush);

    int innerBoxTop = rect.bottom - innerBoxHeight - innerBoxBottomMargin - outerPadding;
    int innerBoxBottom = innerBoxTop + innerBoxHeight + 8;
    Rectangle(hdc, outerMargin + outerPaddingX, innerBoxTop, rect.right - outerMargin - outerPaddingX, innerBoxBottom);

    DeleteObject(hInnerBoxBrush);
}

// ���� �׸��� �Լ�
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

// ���뺸�� �׸��� �Լ�
void DrawBonobono(HWND hWnd, HDC hdc, int x, int y) {
    int radius = 120;  // ��ü �� ũ��

    // ȭ�� ���� �߽ɰ� ���� �߽��� ���մϴ�.
    RECT rect;
    GetClientRect(hWnd, &rect);
    int centerX = (rect.right - rect.left) / 2;
    int centerY = (rect.bottom - rect.top) / 2;

    // �߽� ��ǥ�� �����մϴ�.
    x = centerX;
    y = centerY;

    HBRUSH hBrush = CreateSolidBrush(RGB(80, 188, 223));
    SelectObject(hdc, hBrush);
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

    // �� �׸���
    HBRUSH eyeBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, eyeBrush);

    // �����̽��� ���¿� ���� ���� �� �Ǵ� ������ �׸���
    if (drawEyesAsLines) {
        HPEN eyePen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        SelectObject(hdc, eyePen);
        int wklstartX = x - 50;
        int wklendX = x + 50;
        int wklstartY = y + 70;
        int wklendY = y + 100;
        // ù��° �� �� 
        wklstartX -= 105; // ������ x ��ǥ �̵�
        wklstartY -= 127; // ������ y ��ǥ �̵�
        wklendX -= 135;   // ���� x ��ǥ �̵�
        wklendY -= 127;   // ���� y ��ǥ �̵�
        MoveToEx(hdc, wklstartX + 50, wklstartY + 20, NULL);
        LineTo(hdc, wklendX, wklendY);
        // �ι�°  
        wklstartX += 0;
        wklstartY += 0;
        wklendX += 0;
        wklendY += 0;
        MoveToEx(hdc, wklstartX + 50, wklstartY + 40, NULL);
        LineTo(hdc, wklendX, wklendY);
        // ����°
        wklstartX += 235;
        wklstartY += 10;
        wklendX += 188;
        wklendY -= 31;
        MoveToEx(hdc, wklstartX, wklstartY + 20, NULL);
        LineTo(hdc, wklendX, wklendY + 20);
        // �׹�°
        wklstartX;
        wklstartY -= 10;
        wklendX;
        wklendY += 9;
        MoveToEx(hdc, wklstartX, wklstartY + 30, NULL);
        LineTo(hdc, wklendX, wklendY + 30);

        DeleteObject(eyePen);
    }
    else {
        // ���� ������ �׸���
        Ellipse(hdc, x - 84, y - 36, x - 100, y - 20);  // ���� �� (����  x, ���� y, ������ x, �Ʒ��� y)
        Ellipse(hdc, x + 84, y - 36, x + 100, y - 20);  // ������ ��
        // �Ͼ�� ��
        HBRUSH whiteEyes = CreateSolidBrush(RGB(255, 255, 255));
        SelectObject(hdc, whiteEyes);
        Ellipse(hdc, x - 95, y - 32, x - 88, y - 25);  // ���� �� (����  x, ���� y, ������ x, �Ʒ��� y)
        Ellipse(hdc, x + 95, y - 32, x + 88, y - 25);  // ������ ��
        DeleteObject(whiteEyes);
    }

    // ��
    HBRUSH mouthBrush = CreateSolidBrush(RGB(255, 192, 203));
    SelectObject(hdc, mouthBrush);
    Ellipse(hdc, x + 8 - 35, y + 5, x + 28, y + 88);

    // �Ͼ�� ��
    HBRUSH noseBrush2 = CreateSolidBrush(RGB(255, 255, 255)); // �Ͼ�� ��
    SelectObject(hdc, noseBrush2);
    Ellipse(hdc, x - 47, y, x + 3, y + 50);
    Ellipse(hdc, x, y, x + 50, y + 50);

    // ��
    HBRUSH noseBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, noseBrush);
    Ellipse(hdc, x - 15, y - 13, x + 15, y + 18);

    // ����
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // ������ ��
    SelectObject(hdc, hPen);

    int wklstartX = x - 50;
    int wklendX = x + 50;
    int wklstartY = y + 70;
    int wklendY = y + 100;
    // ù��° ����
    wklstartX -= 80; // ������ x ��ǥ �̵�
    wklstartY -= 80; // ������ y ��ǥ �̵�
    wklendX -= 80;   // ���� x ��ǥ �̵�
    wklendY -= 80;   // ���� y ��ǥ �̵�
    MoveToEx(hdc, wklstartX + 50, wklstartY + 20, NULL);
    LineTo(hdc, wklendX, wklendY);
    // �ι�° ����
    wklstartX += 0;
    wklstartY += 10;
    wklendX += 0;
    wklendY += 10;
    MoveToEx(hdc, wklstartX + 50, wklstartY + 40, NULL);
    LineTo(hdc, wklendX, wklendY);
    // ����° ����
    wklstartX += 165;
    wklstartY += 0;
    wklendX += 117;
    wklendY -= 40;
    MoveToEx(hdc, wklstartX, wklstartY + 20, NULL);
    LineTo(hdc, wklendX, wklendY + 20);
    // �׹�° ����
    wklstartX;
    wklstartY;
    wklendX;
    wklendY += 18;
    MoveToEx(hdc, wklstartX, wklstartY + 30, NULL);
    LineTo(hdc, wklendX, wklendY + 30);

    DeleteObject(hBrush);
    DeleteObject(eyeBrush);
    DeleteObject(mouthBrush);
    DeleteObject(noseBrush2);
    DeleteObject(noseBrush);
    DeleteObject(hPen);
}







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
            DrawBonobono(hWnd, hdc, (shapes[i].rect.left + shapes[i].rect.right) / 2, (shapes[i].rect.top + shapes[i].rect.bottom) / 2);
        }
        else if (shapes[i].shapeType == 3) { // "��" ��� �׸��� ó��
            // �� �׸���
            DrawCircle(hdc, (shapes[i].rect.left + shapes[i].rect.right) / 2, (shapes[i].rect.top + shapes[i].rect.bottom) / 2, (shapes[i].rect.right - shapes[i].rect.left) / 2);
        }
        else if (shapes[i].shapeType == 4) { // "ť��" ��� �׸��� ó��
            // ���⿡�� ť�긦 �׸��ϴ�.
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
    newShape.shapeType = 3; // �� Ÿ���� ��Ÿ���� ���� ���� (�ٸ� ���� ��ġ�� �ʵ���)
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
        if (currentShapeType == 3) { // �� �׸��� ����� ��
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
        break;

    case WM_LBUTTONUP:
        if (currentShapeType == 3) { // �� �׸��� ����� ��
            StopDrawingCircle();
        }
        else {
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
            // "���̾�" ��ư�� ���� �� �ٸ� ���� �����
            shapes.clear();
            StopDrawing(); // ��� ���̾� �߰�
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
        DrawShapes(hWnd, hdc); // ���� �׸�

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_SIZE:
        ResizeButtons(hWnd);
        break;
        // ���콺 �̵� �� ó��
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
        // �����̽��� ���� �� ����
    case WM_KEYDOWN:
        if (wParam == VK_SPACE) {
            drawEyesAsLines = true; // ���� ������ �׸����� ����
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;

        // �����̽��� ���� �� ����
    case WM_KEYUP:
        if (wParam == VK_SPACE) {
            drawEyesAsLines = false; // ���� ������ �׸��� �ʵ��� ����
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