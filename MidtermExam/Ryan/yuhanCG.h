#pragma once
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
int blink = 0; // 눈을 선으로 그릴지 여부

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


POINT centerPoint; // 원의 중심
int radius;        // 원의 반지름

POINT startPoint; // 드래그 시작 지점
POINT endPoint;   // 드래그 끝 지점

// 내부 상자를 그리는 함수
void DrawBox(HWND hWnd, HDC hdc) {
    RECT rect;
    GetClientRect(hWnd, &rect);

    int outerMargin = 8;
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
    int innerBoxBottom = innerBoxTop + innerBoxHeight + 2 * outerPadding;
    Rectangle(hdc, outerMargin + outerPadding, innerBoxTop, rect.right - outerMargin - outerPadding, innerBoxBottom);

    // 안쪽 상자 영역 내에서만 그림이 그려지도록 함
    IntersectClipRect(hdc, outerMargin + outerPadding, innerBoxTop, rect.right - outerMargin - outerPadding, innerBoxBottom);

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
        int right = left + buttonWidth - 16; // // 우측 버튼을 길이를 수정가능
        int bottom = top + buttonHeight;

        SetWindowPos(hButtons[i], NULL, left, top, right - left, bottom - top, SWP_NOZORDER);
    }
}

void DrawBonobono(HWND hWnd, HDC hdc, int blink) {
    int radius = 120;  // 전체 얼굴 크기

    // 화면 가로 중심과 세로 중심을 구합니다.
    RECT rect;
    GetClientRect(hWnd, &rect);
    int centerX = (rect.right - rect.left) / 2;
    int centerY = (rect.bottom - rect.top) / 2;

    // 중심 좌표를 변경합니다.
    int x = centerX;
    int y = centerY;

    HBRUSH hBrush = CreateSolidBrush(RGB(127, 200, 255));
    SelectObject(hdc, hBrush);
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

    // 눈 그리기
    HBRUSH eyeBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, eyeBrush);

    // 스페이스바 상태에 따라 눈을 선 또는 원으로 그리기
    if (blink == 1) {  // blink가 1일 경우 (눈 감음)
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
    HBRUSH mouthBrush = CreateSolidBrush(RGB(255, 150, 255));
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

void DrawRyan(HWND hWnd, HDC hdc, int left, int top, int right, int bottom) {
    // 필요한 크기 계산
    int lenX = right - left;
    int lenY = bottom - top;

    // 귀 그리기
    int earsRadius = lenX / 10; // 귀 크기 조절
    HBRUSH earsBrush = CreateSolidBrush(RGB(255, 200, 15));
    SelectObject(hdc, earsBrush);

    // 귀 위치 계산
    int ears1X = left + lenX / 4;
    int ears1Y = top + lenY / 15;
    int ears2X = right - lenX / 4;
    int ears2Y = top + lenY / 15;

    Ellipse(hdc, ears1X - earsRadius, ears1Y - earsRadius, ears1X + earsRadius, ears1Y + earsRadius);
    Ellipse(hdc, ears2X - earsRadius, ears2Y - earsRadius, ears2X + earsRadius, ears2Y + earsRadius);

    DeleteObject(earsBrush);

    // 머리 그리기 (타원)
    HBRUSH headBrush = CreateSolidBrush(RGB(255, 200, 15));
    Ellipse(hdc, left, top, right, bottom);
    DeleteObject(headBrush);

    // 눈 그리기 (타원)
    int eyeRadius = lenX / 26; // 눈 크기 조절
    HBRUSH eyeBrush = CreateSolidBrush(RGB(0, 0, 0)); // 검은색
    SelectObject(hdc, eyeBrush);

    // 눈 위치 계산
    int eye1X = left + lenX / 4 * 1.3;
    int eye1Y = top + lenY / 4 * 1.6;
    int eye2X = right - lenX / 4 * 1.3;
    int eye2Y = top + lenY / 4 * 1.6;

    Ellipse(hdc, eye1X - eyeRadius, eye1Y - eyeRadius, eye1X + eyeRadius, eye1Y + eyeRadius);
    Ellipse(hdc, eye2X - eyeRadius, eye2Y - eyeRadius, eye2X + eyeRadius, eye2Y + eyeRadius);

    DeleteObject(eyeBrush);

    // 흰코 그리기 
    int wNoseRadius = lenX / 19; // 흰코 크기 조절
    HBRUSH wNoseBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, wNoseBrush);

    // 흰코 위치 계산 
    int wNose1X = left + lenX / 4 * 1.83;
    int wNose1Y = top + lenY / 4 * 2.3;
    int wNose2X = right - lenX / 4 * 1.83;
    int wNose2Y = top + lenY / 4 * 2.3;

    Ellipse(hdc, wNose1X - wNoseRadius, wNose1Y - wNoseRadius, wNose1X + wNoseRadius, wNose1Y + wNoseRadius);
    Ellipse(hdc, wNose2X - wNoseRadius, wNose2Y - wNoseRadius, wNose2X + wNoseRadius, wNose2Y + wNoseRadius);

    DeleteObject(wNoseBrush);


    // 검은 코 그리기 (타원)
    int bNoseRadius = lenX / 33; // 코 크기 조절
    HBRUSH bNoseBrush = CreateSolidBrush(RGB(0, 0, 0)); // 검은색
    SelectObject(hdc, bNoseBrush);

    // 검은 코 위치 계산
    int bNose1X = left + lenX / 4 * 2;
    int bNose2Y = top + lenY / 4 * 2.08;

    Ellipse(hdc, bNose1X - bNoseRadius, bNose2Y - bNoseRadius, bNose1X + bNoseRadius, bNose2Y + bNoseRadius);

    DeleteObject(bNoseBrush);

    // 눈썹 그리기 (선)
    HPEN eyebrowPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // 검은색
    SelectObject(hdc, eyebrowPen);

    // 눈썹 위치 계산
    int eyebrowX1 = left + lenX / 4 * 1.1;
    int eyebrowY1 = bottom - lenY / 4 * 2.9;
    int eyebrowX2 = right - lenX / 4 * 2.6;
    int eyebrowY2 = bottom - lenY / 4 * 2.9;
    int eyebrowX3 = left + lenX / 4 * 2.59;
    int eyebrowX4 = right - lenX / 4 * 1.13;
    int eyebrowY3 = bottom - lenY / 4 * 2.87;
    int eyebrowY4 = bottom - lenY / 4 * 2.87;
    int eyebrowY5 = bottom - lenY / 4 * 2.84;
    int eyebrowY6 = bottom - lenY / 4 * 2.84;

    // 눈썹 길이 계산 
    int eyebrowLength = (lenX / 10) / 2;

    MoveToEx(hdc, eyebrowX1 - eyebrowLength, eyebrowY1, NULL);
    LineTo(hdc, eyebrowX2 + eyebrowLength, eyebrowY2);
    MoveToEx(hdc, eyebrowX1 - eyebrowLength, eyebrowY3, NULL);
    LineTo(hdc, eyebrowX2 + eyebrowLength, eyebrowY4);

    MoveToEx(hdc, eyebrowX3 - eyebrowLength, eyebrowY1, NULL);
    LineTo(hdc, eyebrowX4 + eyebrowLength, eyebrowY2);
    MoveToEx(hdc, eyebrowX3 - eyebrowLength, eyebrowY3, NULL);
    LineTo(hdc, eyebrowX4 + eyebrowLength, eyebrowY4);

    DeleteObject(eyebrowPen);

}

void DrawCube(HDC hdc, int left, int top, int right, int bottom) {
    // 펜 색상 설정
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);

    // 전면 그리기
    MoveToEx(hdc, left, top, NULL);
    LineTo(hdc, right, top);
    LineTo(hdc, right, bottom);
    LineTo(hdc, left, bottom);
    LineTo(hdc, left, top);

    // 위쪽 면 그리기
    MoveToEx(hdc, right, top, NULL);
    LineTo(hdc, right - (right - left) / 5, top - (bottom - top) / 5);
    LineTo(hdc, left - (right - left) / 5, top - (bottom - top) / 5);
    LineTo(hdc, left, top);

    // 왼쪽 변 라인 그리기
    MoveToEx(hdc, left, top, NULL);
    LineTo(hdc, left - (right - left) / 5, top - (bottom - top) / 5);
    LineTo(hdc, left - (right - left) / 5, bottom - (bottom - top) / 5);

    // 윗면 변 라인 그리기
    MoveToEx(hdc, left - (right - left) / 5, top - (bottom - top) / 5, NULL);
    LineTo(hdc, right - (right - left) / 5, top - (bottom - top) / 5);

    // 전면 변 라인 그리기
    MoveToEx(hdc, left, bottom, NULL);
    LineTo(hdc, left - (right - left) / 5, bottom - (bottom - top) / 5);
    MoveToEx(hdc, left, top, NULL);
    LineTo(hdc, left - (right - left) / 5, top - (bottom - top) / 5);
    MoveToEx(hdc, right, top, NULL);
    LineTo(hdc, right - (right - left) / 5, top - (bottom - top) / 5);

    // 전면 색 그리기
    HBRUSH frontBrush = CreateSolidBrush(RGB(192, 192, 192));
    SelectObject(hdc, frontBrush);
    Rectangle(hdc, left, top, right, bottom);
    DeleteObject(frontBrush);

    POINT pointsFront[5];

    // 모든 면을 회색으로 채우기
    HBRUSH hBrush = CreateSolidBrush(RGB(192, 192, 192));
    HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);

    POINT points[4];

    // 위쪽 면
    points[0] = { right, top };
    points[1] = { right - (right - left) / 5, top - (bottom - top) / 5 };
    points[2] = { left - (right - left) / 5, top - (bottom - top) / 5 };
    points[3] = { left, top };
    Polygon(hdc, points, 4);

    // 왼쪽 변
    points[0] = { left, top };
    points[1] = { left - (right - left) / 5, top - (bottom - top) / 5 };
    points[2] = { left - (right - left) / 5, bottom - (bottom - top) / 5 };
    points[3] = { left, bottom };
    Polygon(hdc, points, 4);
 
    // 펜과 브러시 해제
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}
