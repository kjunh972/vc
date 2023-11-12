#pragma once
#include <windows.h> // Windows API 헤더 파일
#include <vector>
#include <tchar.h> // TCHAR와 관련된 헤더 파일
#include <cmath> // 수학 함수 헤더 파일

POINT startPoint; // 드래그 시작 지점
POINT endPoint;   // 드래그 끝 지점

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

    HBRUSH hBrush = CreateSolidBrush(RGB(80, 188, 223));
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


#include <windows.h>
#include <cmath>

// 전역 변수로 선언
int initialWidth = 0;
int initialHeight = 0;
int initialCenterX = 0;
int initialCenterY = 0;

void DrawRyan(HWND hWnd, HDC hdc, int left, int top, int right, int bottom) {
    // 초기 얼굴 크기 및 위치 계산
    if (initialWidth == 0 && initialHeight == 0) {
        initialWidth = right - left;
        initialHeight = bottom - top;
        initialCenterX = (left + right) / 2;
        initialCenterY = (top + bottom) / 2;
    }

    // 눈, 코, 입의 상대적인 크기 및 위치 비율
    const double eyeWidthRatio = 0.2;
    const double eyeHeightRatio = 0.2;
    const double noseWidthRatio = 0.15;
    const double noseHeightRatio = 0.1;
    const double mouthHeightRatio = 0.2;

    // 현재 얼굴 크기 및 위치 계산
    int currentWidth = right - left;
    int currentHeight = bottom - top;
    int currentCenterX = (left + right) / 2;
    int currentCenterY = (top + bottom) / 2;

    // 크기 변화량 계산
    double sizeChangeRatioX = static_cast<double>(currentWidth) / initialWidth;
    double sizeChangeRatioY = static_cast<double>(currentHeight) / initialHeight;

    // 각 부분의 크기 및 위치 재계산
    int earWidth = static_cast<int>(initialWidth * eyeWidthRatio * sizeChangeRatioX);
    int earHeight = static_cast<int>(initialHeight * eyeHeightRatio * sizeChangeRatioY);
    int eyeWidth = static_cast<int>(initialWidth * eyeWidthRatio * sizeChangeRatioX);
    int eyeHeight = static_cast<int>(initialHeight * eyeHeightRatio * sizeChangeRatioY);
    int noseWidth = static_cast<int>(initialWidth * noseWidthRatio * sizeChangeRatioX);
    int noseHeight = static_cast<int>(initialHeight * noseHeightRatio * sizeChangeRatioY);
    int bNoseWidth = static_cast<int>(initialWidth * noseWidthRatio * sizeChangeRatioX);
    int bNoseHeight = static_cast<int>(initialHeight * noseHeightRatio * sizeChangeRatioY);
    int mouthHeight = static_cast<int>(initialHeight * mouthHeightRatio * sizeChangeRatioY);

    int leftEarX = currentCenterX - currentWidth / 4 - eyeWidth / 2;
    int leftEarY = currentCenterY - eyeHeight / 2;
    int rightEarX = currentCenterX + currentWidth / 8 - eyeWidth / 2;
    int rightEarY = currentCenterY - eyeHeight / 2;

    int leftEyeX = currentCenterX - currentWidth / 4 - eyeWidth / 2;
    int leftEyeY = currentCenterY - eyeHeight / 2;
    int rightEyeX = currentCenterX + currentWidth / 8 - eyeWidth / 2;
    int rightEyeY = currentCenterY - eyeHeight / 2;

    int leftNoseX = currentCenterX - noseWidth / 2;
    int leftNoseY = currentCenterY + currentHeight / 4 - noseHeight / 2;
    int rightNoseX = currentCenterX + noseWidth / 2;
    int rightNoseY = currentCenterY + currentHeight / 4 - noseHeight / 2;

    int leftbNoseX = currentCenterX - noseWidth / 2;
    int leftbNoseY = currentCenterY + currentHeight / 4 - noseHeight / 2;
    int rightbNoseX = currentCenterX + noseWidth / 2;
    int rightbNoseY = currentCenterY + currentHeight / 4 - noseHeight / 2;

    int mStartX = currentCenterX - currentWidth / 4;
    int mStartY = currentCenterY + currentHeight / 4;
    int mEndX = currentCenterX + currentWidth / 4;
    int mEndY = currentCenterY + currentHeight / 4;

    // 얼굴 그리기
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 196, 63));
    SelectObject(hdc, hBrush);

    // 드래그 방향에 따라 얼굴 뒤집기
    if (currentCenterX > initialCenterX) {
        Ellipse(hdc, right, top, left, bottom); // 오른쪽으로 드래그하면 뒤집힌 얼굴
    }
    else {
        Ellipse(hdc, left, top, right, bottom); // 왼쪽으로 드래그하면 일반 얼굴
    }

    // 나머지 코드는 동일


    // 귀 그리기
    HBRUSH earBrush = CreateSolidBrush(RGB(255, 196, 63));
    SelectObject(hdc, earBrush);
    Ellipse(hdc, leftEarX, leftEarY - 90, leftEarX + earWidth, leftEarY + earHeight - 90);
    Ellipse(hdc, rightEarX + 23, rightEarY - 90, rightEarX + earWidth + 23, rightEarY + earHeight - 90);

    // 눈 그리기
    HBRUSH eyeBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, eyeBrush);
    Ellipse(hdc, leftEyeX + 26, leftEyeY - 15, leftEyeX + eyeWidth + 6, leftEyeY + eyeHeight - 35);
    Ellipse(hdc, rightEyeX + 26, rightEyeY - 15, rightEyeX + eyeWidth + 6, rightEyeY + eyeHeight - 35);

    // 코 그리기
    HBRUSH noseBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, noseBrush);
    Ellipse(hdc, leftNoseX - 12, leftNoseY - 35, leftNoseX + noseWidth - 12, leftNoseY + noseHeight - 35);
    Ellipse(hdc, rightNoseX - 14, rightNoseY - 35, rightNoseX + noseWidth - 14, rightNoseY + noseHeight - 35);

    // 검은 코
    HBRUSH bNoseBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, bNoseBrush);
    Ellipse(hdc, leftbNoseX + 12, leftbNoseY - 47, leftbNoseX + bNoseWidth - 3, leftbNoseY + bNoseHeight - 52);

    // 눈썹 그리기
    HBRUSH mBrush = CreateSolidBrush(RGB(255, 0, 0));
    SelectObject(hdc, mBrush);

    // 왼쪽
    MoveToEx(hdc, mStartX - 9, mStartY - 112, nullptr);
    LineTo(hdc, mEndX - 79, mEndY - 112);
    MoveToEx(hdc, mStartX - 9, mStartY - 114, nullptr);
    LineTo(hdc, mEndX - 79, mEndY - 114);
    MoveToEx(hdc, mStartX - 9, mStartY - 116, nullptr);
    LineTo(hdc, mEndX - 79, mEndY - 116);

    // 오른쪽
    MoveToEx(hdc, mStartX + 80, mStartY - 112, nullptr);
    LineTo(hdc, mEndX + 10, mEndY - 112);
    MoveToEx(hdc, mStartX + 80, mStartY - 114, nullptr);
    LineTo(hdc, mEndX + 10, mEndY - 114);
    MoveToEx(hdc, mStartX + 80, mStartY - 116, nullptr);
    LineTo(hdc, mEndX + 10, mEndY - 116);

    // 개체 해제
    DeleteObject(earBrush);
    DeleteObject(hBrush);

    DeleteObject(eyeBrush);
    DeleteObject(bNoseBrush);
    DeleteObject(noseBrush);
    DeleteObject(mBrush);
}
