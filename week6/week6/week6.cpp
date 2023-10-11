#include <windows.h>

// ������ ���ø����̼ǿ� �ʿ��� ������ ����ü ����

// ���콺 �������� �������� ������ �����ϱ� ���� ����ü
POINT startPoint = { 0 };
POINT endPoint = { 0 };

// Ű���� �Է� ���¸� �����ϴ� ����
int isKeyPressed = 0;

// �Ķ��� ���ڿ� ��ũ�� ������ ��ġ �� ũ�⸦ �����ϴ� RECT ����ü
RECT rect_user = { 5, 5, 10, 10 };
RECT rect_target = { 50, 50, 150, 150 };

// ���� �κ��� �����ϱ� ���� RECT ����ü
RECT outputRect;

// ������ ���ν��� (�޽��� ó�� �Լ�) ����
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ���� �Լ�
#ifdef UNICODE
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
#else
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR pCmdLine, _In_ int nCmdShow)
#endif
{
    // ������ Ŭ���� ���
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));

    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("Computer Software"); // ������ Ŭ���� �̸�
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // ������
    wc.hCursor = LoadCursor(NULL, IDC_CROSS); // Ŀ�� ���
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // ��� ����
    wc.lpfnWndProc = WindowProc; // ������ ���ν��� �Լ�

    if (RegisterClass(&wc) == 0)
    {
        MessageBox(NULL, L"RegisterClass failed!", L"Error", MB_ICONERROR);
        exit(-1);
    }

    RECT rect = { 150, 100, 800, 600 };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    HWND hwnd = CreateWindow(
        wc.lpszClassName,
        TEXT("202007024 ������"), // ������ Ÿ��Ʋ
        WS_OVERLAPPEDWINDOW, // ������ ��Ÿ��
        0, 0,
        width, height,
        NULL, NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"CreateWindow failed!", L"Error", MB_ICONERROR);
        exit(-1);
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (msg.message != WM_QUIT)
    {
        if (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return (int)msg.wParam;
}

// ������ ���ν��� (�޽��� ó�� �Լ�) ����
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd); // �������� �׷��� ���ؽ�Ʈ �ڵ��� ����

    // �귯�� ���� (���� ����)
    HBRUSH hBrush_user = CreateSolidBrush(RGB(0, 0, 255)); // �Ķ��� �귯��
    HBRUSH hBrush_target = CreateSolidBrush(RGB(255, 0, 255)); // ��ũ�� �귯��
    HBRUSH hBrush_eraser = CreateSolidBrush(RGB(255, 255, 255)); // ��� �귯��

    switch (uMsg)
    {
    case WM_CREATE:
    {
        // �ʱ� ������ ���� �� �Ķ��� ���ڸ� �׸��ϴ�.
        InvalidateRect(hwnd, &rect_user, TRUE); // �Ķ��� ���ڸ� �׸����� �����쿡 ��׸��� �޽����� ����
    }
    break;

    case WM_KEYDOWN: //����Ű�� �������� �Ķ��� �ڽ� �̵�
        isKeyPressed = 1;
        if (wParam == VK_RIGHT)
        {
            rect_user.left += 2;
            rect_user.right += 2;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        else if (wParam == VK_LEFT)
        {
            rect_user.left -= 2;
            rect_user.right -= 2;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        else if (wParam == VK_UP)
        {
            rect_user.top -= 2;
            rect_user.bottom -= 2;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        else if (wParam == VK_DOWN)
        {
            rect_user.top += 2;
            rect_user.bottom += 2;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;

    case WM_KEYUP:
        isKeyPressed = 1;
        InvalidateRect(hwnd, NULL, TRUE);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        FillRect(hdc, &rect_target, hBrush_target);

        if (isKeyPressed)
        {
            FillRect(hdc, &rect_user, hBrush_user);
        }

        // �Ķ��� ���� (rect_user)�� ��ũ�� ���� (rect_target)�� �����ϴ��� Ȯ���մϴ�.
        if (IntersectRect(&outputRect, &rect_user, &rect_target))
        {
            SetTextColor(hdc, RGB(80, 80, 80));
            SetBkColor(hdc, RGB(255, 255, 255));
            TextOut(hdc, 0, 0, L"Crash!!!", 8);
        }

        EndPaint(hwnd, &ps);
    }
    break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam); // ó������ �ʴ� �޽����� �⺻ ó��
    }

    DeleteObject(hBrush_user); // ������ �귯�� ����
    DeleteObject(hBrush_target);
    DeleteObject(hBrush_eraser);
    ReleaseDC(hwnd, hdc); // �׷��� ���ؽ�Ʈ ����

    return 0;
}
