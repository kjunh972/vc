#include "yuhanCG.h"

void DrawShapes(HWND hWnd, HDC hdc) {
    for (size_t i = 0; i < shapes.size(); i++) {
        HBRUSH hBrush = NULL; // hBrush를 NULL로 초기화

        if (shapes[i].shapeType == 0) {
            hBrush = CreateSolidBrush(RGB(255, 0, 0)); // 빨간색 사각형
            SelectObject(hdc, hBrush);
            Rectangle(hdc, shapes[i].rect.left, shapes[i].rect.top, shapes[i].rect.right, shapes[i].rect.bottom);
        }
        else if (shapes[i].shapeType == 1) {
            // 원 그리기
            DrawCircle(hdc, (shapes[i].rect.left + shapes[i].rect.right) / 2, (shapes[i].rect.top + shapes[i].rect.bottom) / 2, (shapes[i].rect.right - shapes[i].rect.left) / 2);
        }
        else if (shapes[i].shapeType == 2) { // "Bonobono" 모양 그리기 처리
            // "Bonobono" 모양 그리기
            DrawBonobono(hWnd, hdc, blink);
        }
        else if (shapes[i].shapeType == 3) { // "Ryan" 모양 그리기 처리
            // "Ryan" 모양 그리기
            DrawRyan(hWnd, hdc, shapes[i].rect.left, shapes[i].rect.top, shapes[i].rect.right, shapes[i].rect.bottom);
        }
        else if (shapes[i].shapeType == 4) { // "큐브" 모양 그리기 처리
            DrawCube(hdc, shapes[i].rect.left, shapes[i].rect.top, shapes[i].rect.right, shapes[i].rect.bottom);
        }

        if (hBrush != NULL) { // hBrush가 NULL이 아닌 경우에만 사용
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
    newShape.shapeType = 1; // 원 타입을 나타내는 값을 정의 (다른 값과 겹치지 않도록)
    newShape.rect.left = centerPoint.x - radius;
    newShape.rect.top = centerPoint.y - radius;
    newShape.rect.right = centerPoint.x + radius;
    newShape.rect.bottom = centerPoint.y + radius;

    shapes.push_back(newShape);
    InvalidateRect(NULL, NULL, FALSE);
}

// 커스텀 마우스 커서 설정
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

// 버튼 크기와 위치 조정
void ResizeButtons(HWND hWnd) {
    RECT rect;
    GetClientRect(hWnd, &rect);

    int buttonMargin = 8;
    int buttonHeight = 55; // 버튼 높이
    int buttonWidth = (rect.right - 2 * buttonMargin - (4 * 8)) / 5; // 버튼 넓이

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

        // 버튼 생성 및 위치 설정
        hButtons[i] = CreateWindow(
            L"BUTTON", buttonName, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            0, 0, 100, 50, hWnd, (HMENU)(i + 1), hInstance, NULL
        );

        int left = buttonMargin + i * (buttonWidth + buttonMargin) + 8; // +8부분을 수정하면 오른쪽으로 이동가능
        int top = innerBoxTop - buttonHeight;
        int right = left + buttonWidth - 15; // // 우측 버튼을 길이를 수정가능
        int bottom = top + buttonHeight;

        SetWindowPos(hButtons[i], NULL, left, top, right - left, bottom - top, SWP_NOZORDER);
    }
}

// 윈도우 프로시저 (메시지 처리)
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_LBUTTONDOWN:
        if (currentShapeType == 1) { // 원 그리기 모드일 때
            shapes.clear();
            HDC hdc = GetDC(hWnd); // 'hdc' 초기화
            InvalidateRect(hWnd, NULL, FALSE);
            StartDrawingCircle(LOWORD(lParam), HIWORD(lParam));
        }
        else {
            shapes.clear();
            HDC hdc = GetDC(hWnd); // 'hdc' 초기화
            InvalidateRect(hWnd, NULL, FALSE);
            StartDrawing(LOWORD(lParam), HIWORD(lParam));
        }
        if (currentShapeType == 3) { // 라이언 얼굴 그리기 모드일 때
            shapes.clear();
            HDC hdc = GetDC(hWnd);
            InvalidateRect(hWnd, NULL, FALSE);
            StartDrawing(LOWORD(lParam), HIWORD(lParam));
        }
        break;

    case WM_LBUTTONUP:
        if (currentShapeType == 1) { // 원 그리기 모드일 때
            StopDrawingCircle();
        }
        else {
            StopDrawing();
        }
        if (currentShapeType == 3) { // 라이언 얼굴 그리기 모드일 때
            StopDrawing();
        }
        break;
    case WM_CREATE:
        ResizeButtons(hWnd);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1: // "사각형" 버튼을 눌렀을 때
            currentShapeType = 0;
            SetFocus(hWnd); // 포커스를 윈도우로 설정
            // "사각형" 버튼을 누를 때 다른 도형 지우기
            shapes.clear();
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        case 2: // "원" 버튼을 눌렀을 때
            currentShapeType = 1;
            SetFocus(hWnd); // 포커스를 윈도우로 설정
            // "원" 버튼을 누를 때 다른 도형 지우기
            shapes.clear();
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        case 3: // "보노보노" 버튼을 눌렀을 때
            currentShapeType = 2; // "보노보노" 모양을 나타내는 값
            SetFocus(hWnd); // 포커스를 윈도우로 설정
            shapes.clear(); // 다른 도형을 지우기
            StopDrawing(); // 즉시 보노보노 추가
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        case 4: // "라이언" 버튼을 눌렀을 때
            currentShapeType = 3;
            SetFocus(hWnd); // 포커스를 윈도우로 설정
            shapes.clear();
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        case 5: // "큐브" 버튼을 눌렀을 때
            currentShapeType = 4;
            SetFocus(hWnd); // 포커스를 윈도우로 설정
            // "큐브" 버튼을 누를 때 다른 도형 지우기
            shapes.clear();
            StopDrawing(); // 즉시 큐브 추가
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps); // 'hdc' 초기화
        DrawBox(hWnd, hdc); // 바깥쪽 상자와 안쪽 상자 그리기
        DrawShapes(hWnd, hdc); // 도형 그리기

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_SIZE:
        ResizeButtons(hWnd);
        break;
        // 마우스 이동 시 처리
    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON) { // 마우스 왼쪽 버튼이 눌린 상태에서만 업데이트
            if (currentShapeType == 1) { // 원 그리기 모드일 때
                UpdateDrawingCircle(LOWORD(lParam), HIWORD(lParam));
            }
            else {
                UpdateDrawing(LOWORD(lParam), HIWORD(lParam));
            }
            if (currentShapeType == 3) { // 라이언 얼굴 그리기 모드일 때
                UpdateDrawing(LOWORD(lParam), HIWORD(lParam));
            }
        }
        break;
        // 스페이스바 누를 때 감지
    case WM_KEYDOWN:
        if (wParam == VK_SPACE) {
            blink = 1; // 눈을 선으로 그리도록 설정
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;

        // 스페이스바 놓을 때 감지
    case WM_KEYUP:
        if (wParam == VK_SPACE) {
            blink = 0; // 눈을 선으로 그리지 않도록 설정
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

// 메인 함수
int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wcex;
    HWND hWnd;
    MSG msg;

    hInstance = hInst;

    // 윈도우 클래스 등록
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

    // 윈도우 생성
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

    // 메시지 루프
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}