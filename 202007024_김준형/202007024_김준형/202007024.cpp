#include <windows.h>
#include <cmath>

bool isBoxVisible = true;
HWND hButtons[5];

int innerBoxHeight = 338;
int innerBoxTopMargin = 16;
int innerBoxBottomMargin = 16;

bool isMouseInsideInnerBox = false;

int outerMargin = 8;
int outerPaddingX = 8;

HINSTANCE hInstance;

bool isDrawing = false;
bool isDragging = false;
bool isRightDragging = false;
bool isCircleDrawing = false;
bool isBoxDrawing = false;
bool isErasing = false;  // 지우기 모드 변수 추가
bool isDrawingFinished = false;

POINT centerPoint;
int radius;

POINT startPoint;
POINT endPoint;
POINT offset;

POINT circleCenter; // 원의 중심 좌표
int circleRadius;   // 원의 반지름

void DrawBox(HWND hWnd, HDC hdc) {
    RECT rect;
    GetClientRect(hWnd, &rect);

    int outerPadding = 8;
    int outerBoxHeight = rect.bottom - 2 * outerMargin;

    if (innerBoxHeight > (outerBoxHeight - innerBoxTopMargin - innerBoxBottomMargin - 2 * outerPadding))
        innerBoxHeight = outerBoxHeight - innerBoxTopMargin - innerBoxBottomMargin - 2 * outerPadding;

    int innerMargin = 0;
    int innerPadding = 0;

    HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(255, 240, 200));
    SelectObject(hdc, hBackgroundBrush);
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    FillRect(hdc, &clientRect, hBackgroundBrush);
    DeleteObject(hBackgroundBrush);

    HBRUSH hOuterBoxBrush = CreateSolidBrush(RGB(255, 240, 200));
    SelectObject(hdc, hOuterBoxBrush);
    Rectangle(hdc, outerMargin, outerMargin, rect.right - outerMargin, rect.bottom - outerMargin);
    DeleteObject(hOuterBoxBrush);

    HBRUSH hInnerBoxBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hInnerBoxBrush);

    int innerBoxTop = rect.bottom - innerBoxHeight - innerBoxBottomMargin - outerPadding;
    int innerBoxBottom = innerBoxTop + innerBoxHeight + 8;
    Rectangle(hdc, outerMargin + outerPaddingX, innerBoxTop, rect.right - outerMargin - outerPaddingX, innerBoxBottom);

    DeleteObject(hInnerBoxBrush);
}

void DrawCircle(HDC hdc, int centerX, int centerY, int radius) {
    HBRUSH circleBrush = CreateSolidBrush(RGB(0, 255, 0));
    SelectObject(hdc, circleBrush);
    Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
    DeleteObject(circleBrush);

    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hdc, hPen);
    Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
    DeleteObject(hPen);
}

void DrawRectangle(HDC hdc, int left, int top, int right, int bottom) {
    HBRUSH rectBrush = CreateSolidBrush(RGB(0, 0, 255));
    SelectObject(hdc, rectBrush);
    Rectangle(hdc, left, top, right, bottom);
    DeleteObject(rectBrush);
}

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

void ResizeButtons(HWND hWnd) {
    RECT rect;
    GetClientRect(hWnd, &rect);

    int buttonMargin = 8;
    int buttonHeight = 64;
    int buttonWidth = (rect.right - 2 * buttonMargin - (4 * 8)) / 5;

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

        hButtons[i] = CreateWindow(
            L"BUTTON", buttonName, WS_CHILD | WS_VISIBLE,
            0, 0, 100, 50, hWnd, (HMENU)(i + 1), hInstance, NULL
        );

        int left = buttonMargin + i * (buttonWidth + buttonMargin);
        int top = innerBoxTop - buttonHeight;
        int right = left + buttonWidth;
        int bottom = top + buttonHeight;

        SetWindowPos(hButtons[i], NULL, left, top, right - left, bottom - top, SWP_NOZORDER);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        ResizeButtons(hWnd);
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            isBoxVisible = true;
            isCircleDrawing = false;
            isBoxDrawing = false;
            isErasing = false;  // 박스 그리기 모드 활성화 시 지우기 모드 비활성화
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (LOWORD(wParam) == 2) {
            isBoxVisible = false;  // 원 그리기 모드 활성화 시 사각형 비활성화
            isCircleDrawing = true;
            isBoxDrawing = false;
            isErasing = false;  // 원 그리기 모드 활성화 시 지우기 모드 비활성화
            InvalidateRect(hWnd, NULL, TRUE);
        }
        // 나머지 버튼에 대한 처리 추가
        else if (LOWORD(wParam) == 3 || LOWORD(wParam) == 4 || LOWORD(wParam) == 5) {
            isErasing = true;  // 다른 버튼 누를 시 지우기 모드 활성화
            isBoxVisible = false;  // 지우기 모드 활성화 시 사각형 비활성화
            isCircleDrawing = false;
            isBoxDrawing = false;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        DrawBox(hWnd, hdc);

        if (isBoxVisible) {
            DrawRectangle(hdc, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
        }
        else if (isCircleDrawing) {
            if (!isErasing && isMouseInsideInnerBox) {
                DrawCircle(hdc, circleCenter.x, circleCenter.y, circleRadius);
            }
        }

        // 추가: 지우기 모드
        if (isErasing && isMouseInsideInnerBox && isDrawingFinished) {
            // 지우기 모드이고 내부 상자 안에 있으며 그림이 그려진 경우
            HBRUSH eraseBrush = CreateSolidBrush(RGB(255, 240, 200));  // 배경색으로 덮어쓰기
            SelectObject(hdc, eraseBrush);
            Rectangle(hdc, outerMargin + outerPaddingX, innerBoxTopMargin, ps.rcPaint.right, innerBoxTopMargin + innerBoxHeight);
            DeleteObject(eraseBrush);
        }

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_SETCURSOR:
        if (LOWORD(lParam) == HTCLIENT) {
            if (!isErasing && isMouseInsideInnerBox) {
                SetCustomCursor(hWnd, 1);
            }
            else {
                SetCustomCursor(hWnd, 0);
            }
            return TRUE;
        }
        break;
    case WM_SIZE:
        ResizeButtons(hWnd);
        break;
    case WM_MOUSEMOVE:
        if (isBoxDrawing) { // Box 그리기 모드
            GetCursorPos(&endPoint);
            ScreenToClient(hWnd, &endPoint);
            isDrawingFinished = true;  // 그림 그리기 완료
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (isCircleDrawing) {
            GetCursorPos(&endPoint);
            ScreenToClient(hWnd, &endPoint);
            if (isMouseInsideInnerBox && GetAsyncKeyState(VK_LBUTTON)) {
                int dx = endPoint.x - startPoint.x;
                int dy = endPoint.y - startPoint.y;
                circleRadius = static_cast<int>(sqrt(dx * dx + dy * dy));
                circleCenter = startPoint;
                isDrawingFinished = true;  // 그림 그리기 완료
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        else {
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(hWnd, &pt);
            RECT innerBoxRect;
            GetClientRect(hWnd, &innerBoxRect);
            if (PtInRect(&innerBoxRect, pt)) {
                isMouseInsideInnerBox = true;
                SetCustomCursor(hWnd, 1);
            }
            else {
                isMouseInsideInnerBox = false;
                SetCustomCursor(hWnd, 0);
            }
        }
        break;
    case WM_LBUTTONDOWN:
        RECT innerBoxRect;
        GetClientRect(hWnd, &innerBoxRect);
        if (LOWORD(wParam) == 1) {
            isBoxDrawing = true; // Box 그리기 모드 활성화
            isCircleDrawing = false;
            isErasing = false;  // 박스 그리기 모드 활성화 시 지우기 모드 비활성화
            GetCursorPos(&startPoint);
            ScreenToClient(hWnd, &startPoint);
        }
        else if (PtInRect(&innerBoxRect, startPoint)) {
            isDragging = true;
            GetCursorPos(&startPoint);
            ScreenToClient(hWnd, &startPoint);
            offset.x = startPoint.x - endPoint.x;
            offset.y = startPoint.y - endPoint.y;
        }
        break;
    case WM_LBUTTONUP:
        if (isBoxDrawing) { // Box 그리기 모드
            isBoxDrawing = false; // Box 그리기 모드 비활성화
            isDrawingFinished = false;  // 그림 그리기 완료 해제
        }
        else if (isDragging) {
            isDragging = false;
        }
        break;
    case WM_RBUTTONDOWN:
        if (PtInRect(&innerBoxRect, startPoint)) {
            isRightDragging = true;
            GetCursorPos(&startPoint);
            ScreenToClient(hWnd, &startPoint);
        }
        break;
    case WM_RBUTTONUP:
        if (isRightDragging) {
            isRightDragging = false;
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

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wcex;
    HWND hWnd;
    MSG msg;

    hInstance = hInst;

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

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}