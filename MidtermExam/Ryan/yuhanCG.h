#pragma once
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
int blink = 0; // ���� ������ �׸��� ����

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


POINT centerPoint; // ���� �߽�
int radius;        // ���� ������

POINT startPoint; // �巡�� ���� ����
POINT endPoint;   // �巡�� �� ����

// ���� ���ڸ� �׸��� �Լ�
void DrawBox(HWND hWnd, HDC hdc) {
    RECT rect;
    GetClientRect(hWnd, &rect);

    int outerMargin = 8;
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
    int innerBoxBottom = innerBoxTop + innerBoxHeight + 2 * outerPadding;
    Rectangle(hdc, outerMargin + outerPadding, innerBoxTop, rect.right - outerMargin - outerPadding, innerBoxBottom);

    // ���� ���� ���� �������� �׸��� �׷������� ��
    IntersectClipRect(hdc, outerMargin + outerPadding, innerBoxTop, rect.right - outerMargin - outerPadding, innerBoxBottom);

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
    newShape.shapeType = 1; // �� Ÿ���� ��Ÿ���� ���� ���� (�ٸ� ���� ��ġ�� �ʵ���)
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
        int right = left + buttonWidth - 16; // // ���� ��ư�� ���̸� ��������
        int bottom = top + buttonHeight;

        SetWindowPos(hButtons[i], NULL, left, top, right - left, bottom - top, SWP_NOZORDER);
    }
}

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

    HBRUSH hBrush = CreateSolidBrush(RGB(127, 200, 255));
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
    HBRUSH mouthBrush = CreateSolidBrush(RGB(255, 150, 255));
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

void DrawRyan(HWND hWnd, HDC hdc, int left, int top, int right, int bottom) {
    // �ʿ��� ũ�� ���
    int lenX = right - left;
    int lenY = bottom - top;

    // �� �׸���
    int earsRadius = lenX / 10; // �� ũ�� ����
    HBRUSH earsBrush = CreateSolidBrush(RGB(255, 200, 15));
    SelectObject(hdc, earsBrush);

    // �� ��ġ ���
    int ears1X = left + lenX / 4;
    int ears1Y = top + lenY / 15;
    int ears2X = right - lenX / 4;
    int ears2Y = top + lenY / 15;

    Ellipse(hdc, ears1X - earsRadius, ears1Y - earsRadius, ears1X + earsRadius, ears1Y + earsRadius);
    Ellipse(hdc, ears2X - earsRadius, ears2Y - earsRadius, ears2X + earsRadius, ears2Y + earsRadius);

    DeleteObject(earsBrush);

    // �Ӹ� �׸��� (Ÿ��)
    HBRUSH headBrush = CreateSolidBrush(RGB(255, 200, 15));
    Ellipse(hdc, left, top, right, bottom);
    DeleteObject(headBrush);

    // �� �׸��� (Ÿ��)
    int eyeRadius = lenX / 26; // �� ũ�� ����
    HBRUSH eyeBrush = CreateSolidBrush(RGB(0, 0, 0)); // ������
    SelectObject(hdc, eyeBrush);

    // �� ��ġ ���
    int eye1X = left + lenX / 4 * 1.3;
    int eye1Y = top + lenY / 4 * 1.6;
    int eye2X = right - lenX / 4 * 1.3;
    int eye2Y = top + lenY / 4 * 1.6;

    Ellipse(hdc, eye1X - eyeRadius, eye1Y - eyeRadius, eye1X + eyeRadius, eye1Y + eyeRadius);
    Ellipse(hdc, eye2X - eyeRadius, eye2Y - eyeRadius, eye2X + eyeRadius, eye2Y + eyeRadius);

    DeleteObject(eyeBrush);

    // ���� �׸��� 
    int wNoseRadius = lenX / 19; // ���� ũ�� ����
    HBRUSH wNoseBrush = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, wNoseBrush);

    // ���� ��ġ ��� 
    int wNose1X = left + lenX / 4 * 1.83;
    int wNose1Y = top + lenY / 4 * 2.3;
    int wNose2X = right - lenX / 4 * 1.83;
    int wNose2Y = top + lenY / 4 * 2.3;

    Ellipse(hdc, wNose1X - wNoseRadius, wNose1Y - wNoseRadius, wNose1X + wNoseRadius, wNose1Y + wNoseRadius);
    Ellipse(hdc, wNose2X - wNoseRadius, wNose2Y - wNoseRadius, wNose2X + wNoseRadius, wNose2Y + wNoseRadius);

    DeleteObject(wNoseBrush);


    // ���� �� �׸��� (Ÿ��)
    int bNoseRadius = lenX / 33; // �� ũ�� ����
    HBRUSH bNoseBrush = CreateSolidBrush(RGB(0, 0, 0)); // ������
    SelectObject(hdc, bNoseBrush);

    // ���� �� ��ġ ���
    int bNose1X = left + lenX / 4 * 2;
    int bNose2Y = top + lenY / 4 * 2.08;

    Ellipse(hdc, bNose1X - bNoseRadius, bNose2Y - bNoseRadius, bNose1X + bNoseRadius, bNose2Y + bNoseRadius);

    DeleteObject(bNoseBrush);

    // ���� �׸��� (��)
    HPEN eyebrowPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // ������
    SelectObject(hdc, eyebrowPen);

    // ���� ��ġ ���
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

    // ���� ���� ��� 
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
    // �� ���� ����
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);

    // ���� �׸���
    MoveToEx(hdc, left, top, NULL);
    LineTo(hdc, right, top);
    LineTo(hdc, right, bottom);
    LineTo(hdc, left, bottom);
    LineTo(hdc, left, top);

    // ���� �� �׸���
    MoveToEx(hdc, right, top, NULL);
    LineTo(hdc, right - (right - left) / 5, top - (bottom - top) / 5);
    LineTo(hdc, left - (right - left) / 5, top - (bottom - top) / 5);
    LineTo(hdc, left, top);

    // ���� �� ���� �׸���
    MoveToEx(hdc, left, top, NULL);
    LineTo(hdc, left - (right - left) / 5, top - (bottom - top) / 5);
    LineTo(hdc, left - (right - left) / 5, bottom - (bottom - top) / 5);

    // ���� �� ���� �׸���
    MoveToEx(hdc, left - (right - left) / 5, top - (bottom - top) / 5, NULL);
    LineTo(hdc, right - (right - left) / 5, top - (bottom - top) / 5);

    // ���� �� ���� �׸���
    MoveToEx(hdc, left, bottom, NULL);
    LineTo(hdc, left - (right - left) / 5, bottom - (bottom - top) / 5);
    MoveToEx(hdc, left, top, NULL);
    LineTo(hdc, left - (right - left) / 5, top - (bottom - top) / 5);
    MoveToEx(hdc, right, top, NULL);
    LineTo(hdc, right - (right - left) / 5, top - (bottom - top) / 5);

    // ���� �� �׸���
    HBRUSH frontBrush = CreateSolidBrush(RGB(192, 192, 192));
    SelectObject(hdc, frontBrush);
    Rectangle(hdc, left, top, right, bottom);
    DeleteObject(frontBrush);

    POINT pointsFront[5];

    // ��� ���� ȸ������ ä���
    HBRUSH hBrush = CreateSolidBrush(RGB(192, 192, 192));
    HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);

    POINT points[4];

    // ���� ��
    points[0] = { right, top };
    points[1] = { right - (right - left) / 5, top - (bottom - top) / 5 };
    points[2] = { left - (right - left) / 5, top - (bottom - top) / 5 };
    points[3] = { left, top };
    Polygon(hdc, points, 4);

    // ���� ��
    points[0] = { left, top };
    points[1] = { left - (right - left) / 5, top - (bottom - top) / 5 };
    points[2] = { left - (right - left) / 5, bottom - (bottom - top) / 5 };
    points[3] = { left, bottom };
    Polygon(hdc, points, 4);
 
    // ��� �귯�� ����
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
    DeleteObject(hBrush);
}
