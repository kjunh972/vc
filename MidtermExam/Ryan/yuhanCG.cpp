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
        L"곰돌이",
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