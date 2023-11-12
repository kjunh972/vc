#pragma once
#include <windows.h> // Windows API ��� ����
#include <vector>
#include <tchar.h> // TCHAR�� ���õ� ��� ����
#include <cmath> // ���� �Լ� ��� ����

POINT startPoint; // �巡�� ���� ����
POINT endPoint;   // �巡�� �� ����

void DrawBonobono(HWND hWnd, HDC hdc, int blink) {
    int radius = 120;  // ��ü �� ũ��

    // ȭ�� ���� �߽ɰ� ���� �߽��� ���մϴ�.
    RECT rect;
    GetClientRect(hWnd, &rect);
    int centerX = (rect.right - rect.left) / 2;
    int centerY = (rect.bottom - rect.top) / 2;

    // �߽� ��ǥ�� �����մϴ�.
    int x = centerX;
    int y = centerY;

    HBRUSH hBrush = CreateSolidBrush(RGB(80, 188, 223));
    SelectObject(hdc, hBrush);
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

    // �� �׸���
    HBRUSH eyeBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, eyeBrush);

    // �����̽��� ���¿� ���� ���� �� �Ǵ� ������ �׸���
    if (blink == 1) {  // blink�� 1�� ��� (�� ����)
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


#include <windows.h>
#include <cmath>

// ���� ������ ����
int initialWidth = 0;
int initialHeight = 0;
int initialCenterX = 0;
int initialCenterY = 0;

void DrawRyan(HWND hWnd, HDC hdc, int left, int top, int right, int bottom) {
    // �ʱ� �� ũ�� �� ��ġ ���
    if (initialWidth == 0 && initialHeight == 0) {
        initialWidth = right - left;
        initialHeight = bottom - top;
        initialCenterX = (left + right) / 2;
        initialCenterY = (top + bottom) / 2;
    }

    // ��, ��, ���� ������� ũ�� �� ��ġ ����
    const double eyeWidthRatio = 0.2;
    const double eyeHeightRatio = 0.2;
    const double noseWidthRatio = 0.15;
    const double noseHeightRatio = 0.1;
    const double mouthHeightRatio = 0.2;

    // ���� �� ũ�� �� ��ġ ���
    int currentWidth = right - left;
    int currentHeight = bottom - top;
    int currentCenterX = (left + right) / 2;
    int currentCenterY = (top + bottom) / 2;

    // ũ�� ��ȭ�� ���
    double sizeChangeRatioX = static_cast<double>(currentWidth) / initialWidth;
    double sizeChangeRatioY = static_cast<double>(currentHeight) / initialHeight;

    // �� �κ��� ũ�� �� ��ġ ����
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

    // �� �׸���
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 196, 63));
    SelectObject(hdc, hBrush);

    // �巡�� ���⿡ ���� �� ������
    if (currentCenterX > initialCenterX) {
        Ellipse(hdc, right, top, left, bottom); // ���������� �巡���ϸ� ������ ��
    }
    else {
        Ellipse(hdc, left, top, right, bottom); // �������� �巡���ϸ� �Ϲ� ��
    }

    // ������ �ڵ�� ����


    // �� �׸���
    HBRUSH earBrush = CreateSolidBrush(RGB(255, 196, 63));
    SelectObject(hdc, earBrush);
    Ellipse(hdc, leftEarX, leftEarY - 90, leftEarX + earWidth, leftEarY + earHeight - 90);
    Ellipse(hdc, rightEarX + 23, rightEarY - 90, rightEarX + earWidth + 23, rightEarY + earHeight - 90);

    // �� �׸���
    HBRUSH eyeBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, eyeBrush);
    Ellipse(hdc, leftEyeX + 26, leftEyeY - 15, leftEyeX + eyeWidth + 6, leftEyeY + eyeHeight - 35);
    Ellipse(hdc, rightEyeX + 26, rightEyeY - 15, rightEyeX + eyeWidth + 6, rightEyeY + eyeHeight - 35);

    // �� �׸���
    HBRUSH noseBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, noseBrush);
    Ellipse(hdc, leftNoseX - 12, leftNoseY - 35, leftNoseX + noseWidth - 12, leftNoseY + noseHeight - 35);
    Ellipse(hdc, rightNoseX - 14, rightNoseY - 35, rightNoseX + noseWidth - 14, rightNoseY + noseHeight - 35);

    // ���� ��
    HBRUSH bNoseBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, bNoseBrush);
    Ellipse(hdc, leftbNoseX + 12, leftbNoseY - 47, leftbNoseX + bNoseWidth - 3, leftbNoseY + bNoseHeight - 52);

    // ���� �׸���
    HBRUSH mBrush = CreateSolidBrush(RGB(255, 0, 0));
    SelectObject(hdc, mBrush);

    // ����
    MoveToEx(hdc, mStartX - 9, mStartY - 112, nullptr);
    LineTo(hdc, mEndX - 79, mEndY - 112);
    MoveToEx(hdc, mStartX - 9, mStartY - 114, nullptr);
    LineTo(hdc, mEndX - 79, mEndY - 114);
    MoveToEx(hdc, mStartX - 9, mStartY - 116, nullptr);
    LineTo(hdc, mEndX - 79, mEndY - 116);

    // ������
    MoveToEx(hdc, mStartX + 80, mStartY - 112, nullptr);
    LineTo(hdc, mEndX + 10, mEndY - 112);
    MoveToEx(hdc, mStartX + 80, mStartY - 114, nullptr);
    LineTo(hdc, mEndX + 10, mEndY - 114);
    MoveToEx(hdc, mStartX + 80, mStartY - 116, nullptr);
    LineTo(hdc, mEndX + 10, mEndY - 116);

    // ��ü ����
    DeleteObject(earBrush);
    DeleteObject(hBrush);

    DeleteObject(eyeBrush);
    DeleteObject(bNoseBrush);
    DeleteObject(noseBrush);
    DeleteObject(mBrush);
}
