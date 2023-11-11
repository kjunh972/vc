#include <windows.h> // Windows API 헤더 파일
#include <vector>
#include <tchar.h> // TCHAR와 관련된 헤더 파일
#include <cmath> // 수학 함수 헤더 파일

// 사용자 지정 도형 구조체
struct CustomShape {
    RECT rect;      // 도형의 위치와 크기
    bool isDrawing; // 현재 그리는 중인지 여부
    int shapeType;  // 도형의 타입 (0: 사각형, 1: 원, 2: 보노보노, 3: 라이언, 4: 큐브)
};

int currentShapeType = 0; // 현재 그릴 도형 타입
bool drawEyesAsLines = false; // 눈을 선으로 그릴지 여부

std::vector<CustomShape> shapes; // 그린 도형을 저장할 벡터

bool isBoxVisible = true; // 사각형이 보이는지 여부
HWND hButtons[5]; // 5개의 버튼 윈도우 핸들을 저장하는 배열

int innerBoxHeight = 338; // 내부 상자의 높이
int innerBoxTopMargin = 16; // 내부 상자 상단 여백
int innerBoxBottomMargin = 16; // 내부 상자 하단 여백

bool isMouseInsideInnerBox = false; // 마우스가 내부 상자 안에 있는지 여부를 나타내는 플래그

int outerMargin = 8; // 외부 여백
int outerPaddingX = 8; // 외부 상자의 가로 패딩

HINSTANCE hInstance; // 현재 인스턴스

POINT startPoint; // 드래그 시작 지점
POINT endPoint;   // 드래그 끝 지점

POINT centerPoint; // 원의 중심
int radius;        // 원의 반지름

// 내부 상자를 그리는 함수
void DrawBox(HWND hWnd, HDC hdc) {
    RECT rect;
    GetClientRect(hWnd, &rect);

    int outerPadding = 8;
    int outerBoxHeight = rect.bottom - 2 * outerMargin;

    // 내부 상자의 높이가 제한을 초과하지 않도록
    if (innerBoxHeight > (outerBoxHeight - innerBoxTopMargin - innerBoxBottomMargin - 2 * outerPadding))
        innerBoxHeight = outerBoxHeight - innerBoxTopMargin - innerBoxBottomMargin - 2 * outerPadding;

    int innerMargin = 0;
    int innerPadding = 0;

    // 배경 브러시 생성 및 설정
    HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(255, 240, 200));
    SelectObject(hdc, hBackgroundBrush);
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    FillRect(hdc, &clientRect, hBackgroundBrush);
    DeleteObject(hBackgroundBrush);

    // 외부 상자의 배경 브러시 생성 및 그리기
    HBRUSH hOuterBoxBrush = CreateSolidBrush(RGB(255, 240, 200));
    SelectObject(hdc, hOuterBoxBrush);
    Rectangle(hdc, outerMargin, outerMargin, rect.right - outerMargin, rect.bottom - outerMargin);
    DeleteObject(hOuterBoxBrush);

    // 내부 상자의 배경 브러시 생성 및 그리기
    HBRUSH hInnerBoxBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hInnerBoxBrush);

    int innerBoxTop = rect.bottom - innerBoxHeight - innerBoxBottomMargin - outerPadding;
    int innerBoxBottom = innerBoxTop + innerBoxHeight + 8;
    Rectangle(hdc, outerMargin + outerPaddingX, innerBoxTop, rect.right - outerMargin - outerPaddingX, innerBoxBottom);

    DeleteObject(hInnerBoxBrush);
}

// 원을 그리는 함수
void DrawCircle(HDC hdc, int centerX, int centerY, int radius) {
    HBRUSH circleBrush = CreateSolidBrush(RGB(0, 255, 0)); // 녹색으로 내부 채우기
    SelectObject(hdc, circleBrush);
    Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
    DeleteObject(circleBrush);

    // 원 그리기 (윤곽선)
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // 빨간색 윤곽선
    SelectObject(hdc, hPen);
    Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
    DeleteObject(hPen);
}

// 보노보노 그리는 함수
void DrawBonobono(HWND hWnd, HDC hdc, int x, int y) {
    int radius = 120;  // 전체 얼굴 크기

    // 화면 가로 중심과 세로 중심을 구합니다.
    RECT rect;
    GetClientRect(hWnd, &rect);
    int centerX = (rect.right - rect.left) / 2;
    int centerY = (rect.bottom - rect.top) / 2;

    // 중심 좌표를 변경합니다.
    x = centerX;
    y = centerY;

    HBRUSH hBrush = CreateSolidBrush(RGB(80, 188, 223));
    SelectObject(hdc, hBrush);
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

    // 눈 그리기
    HBRUSH eyeBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, eyeBrush);

    // 스페이스바 상태에 따라 눈을 선 또는 원으로 그리기
    if (drawEyesAsLines) {
        HPEN eyePen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        SelectObject(hdc, eyePen);
        int wklstartX = x - 50;
        int wklendX = x + 50;
        int wklstartY = y + 70;
        int wklendY = y + 100;
        // 첫번째 눈 선 
        wklstartX -= 105; // 시작점 x 좌표 이동
        wklstartY -= 127; // 시작점 y 좌표 이동
        wklendX -= 135;   // 끝점 x 좌표 이동
        wklendY -= 127;   // 끝점 y 좌표 이동
        MoveToEx(hdc, wklstartX + 50, wklstartY + 20, NULL);
        LineTo(hdc, wklendX, wklendY);
        // 두번째  
        wklstartX += 0;
        wklstartY += 0;
        wklendX += 0;
        wklendY += 0;
        MoveToEx(hdc, wklstartX + 50, wklstartY + 40, NULL);
        LineTo(hdc, wklendX, wklendY);
        // 세번째
        wklstartX += 235;
        wklstartY += 10;
        wklendX += 188;
        wklendY -= 31;
        MoveToEx(hdc, wklstartX, wklstartY + 20, NULL);
        LineTo(hdc, wklendX, wklendY + 20);
        // 네번째
        wklstartX;
        wklstartY -= 10;
        wklendX;
        wklendY += 9;
        MoveToEx(hdc, wklstartX, wklstartY + 30, NULL);
        LineTo(hdc, wklendX, wklendY + 30);

        DeleteObject(eyePen);
    }
    else {
        // 눈을 원으로 그리기
        Ellipse(hdc, x - 84, y - 36, x - 100, y - 20);  // 왼쪽 눈 (왼쪽  x, 위쪽 y, 오른쪽 x, 아래쪽 y)
        Ellipse(hdc, x + 84, y - 36, x + 100, y - 20);  // 오른쪽 눈
        // 하얀색 눈
        HBRUSH whiteEyes = CreateSolidBrush(RGB(255, 255, 255));
        SelectObject(hdc, whiteEyes);
        Ellipse(hdc, x - 95, y - 32, x - 88, y - 25);  // 왼쪽 눈 (왼쪽  x, 위쪽 y, 오른쪽 x, 아래쪽 y)
        Ellipse(hdc, x + 95, y - 32, x + 88, y - 25);  // 오른쪽 눈
        DeleteObject(whiteEyes);
    }

    // 입
    HBRUSH mouthBrush = CreateSolidBrush(RGB(255, 192, 203));
    SelectObject(hdc, mouthBrush);
    Ellipse(hdc, x + 8 - 35, y + 5, x + 28, y + 88);

    // 하얀색 코
    HBRUSH noseBrush2 = CreateSolidBrush(RGB(255, 255, 255)); // 하얀색 원
    SelectObject(hdc, noseBrush2);
    Ellipse(hdc, x - 47, y, x + 3, y + 50);
    Ellipse(hdc, x, y, x + 50, y + 50);

    // 코
    HBRUSH noseBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, noseBrush);
    Ellipse(hdc, x - 15, y - 13, x + 15, y + 18);

    // 수염
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // 검은색 선
    SelectObject(hdc, hPen);

    int wklstartX = x - 50;
    int wklendX = x + 50;
    int wklstartY = y + 70;
    int wklendY = y + 100;
    // 첫번째 수염
    wklstartX -= 80; // 시작점 x 좌표 이동
    wklstartY -= 80; // 시작점 y 좌표 이동
    wklendX -= 80;   // 끝점 x 좌표 이동
    wklendY -= 80;   // 끝점 y 좌표 이동
    MoveToEx(hdc, wklstartX + 50, wklstartY + 20, NULL);
    LineTo(hdc, wklendX, wklendY);
    // 두번째 수염
    wklstartX += 0;
    wklstartY += 10;
    wklendX += 0;
    wklendY += 10;
    MoveToEx(hdc, wklstartX + 50, wklstartY + 40, NULL);
    LineTo(hdc, wklendX, wklendY);
    // 세번째 수염
    wklstartX += 165;
    wklstartY += 0;
    wklendX += 117;
    wklendY -= 40;
    MoveToEx(hdc, wklstartX, wklstartY + 20, NULL);
    LineTo(hdc, wklendX, wklendY + 20);
    // 네번째 수염
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
            DrawBonobono(hWnd, hdc, (shapes[i].rect.left + shapes[i].rect.right) / 2, (shapes[i].rect.top + shapes[i].rect.bottom) / 2);
        }
        else if (shapes[i].shapeType == 3) { // "원" 모양 그리기 처리
            // 원 그리기
            DrawCircle(hdc, (shapes[i].rect.left + shapes[i].rect.right) / 2, (shapes[i].rect.top + shapes[i].rect.bottom) / 2, (shapes[i].rect.right - shapes[i].rect.left) / 2);
        }
        else if (shapes[i].shapeType == 4) { // "큐브" 모양 그리기 처리
            // 여기에서 큐브를 그립니다.
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
    newShape.shapeType = 3; // 원 타입을 나타내는 값을 정의 (다른 값과 겹치지 않도록)
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
        if (currentShapeType == 3) { // 원 그리기 모드일 때
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
        break;

    case WM_LBUTTONUP:
        if (currentShapeType == 3) { // 원 그리기 모드일 때
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
            // "라이언" 버튼을 누를 때 다른 도형 지우기
            shapes.clear();
            StopDrawing(); // 즉시 라이언 추가
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
        DrawShapes(hWnd, hdc); // 도형 그리

        EndPaint(hWnd, &ps);
        break;
    }

    case WM_SIZE:
        ResizeButtons(hWnd);
        break;
        // 마우스 이동 시 처리
    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON) { // 마우스 왼쪽 버튼이 눌린 상태에서만 업데이트
            if (currentShapeType == 3) { // 원 그리기 모드일 때
                UpdateDrawingCircle(LOWORD(lParam), HIWORD(lParam));
            }
            else {
                UpdateDrawing(LOWORD(lParam), HIWORD(lParam));
            }
        }
        break;
        // 스페이스바 누를 때 감지
    case WM_KEYDOWN:
        if (wParam == VK_SPACE) {
            drawEyesAsLines = true; // 눈을 선으로 그리도록 설정
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;

        // 스페이스바 놓을 때 감지
    case WM_KEYUP:
        if (wParam == VK_SPACE) {
            drawEyesAsLines = false; // 눈을 선으로 그리지 않도록 설정
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