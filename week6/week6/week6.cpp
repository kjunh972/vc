#include <windows.h>

// 윈도우 애플리케이션에 필요한 변수와 구조체 선언

// 마우스 포인터의 시작점과 끝점을 저장하기 위한 구조체
POINT startPoint = { 0 };
POINT endPoint = { 0 };

// 키보드 입력 상태를 저장하는 변수
int isKeyPressed = 0;

// 파란색 상자와 핑크색 상자의 위치 및 크기를 저장하는 RECT 구조체
RECT rect_user = { 5, 5, 10, 10 };
RECT rect_target = { 50, 50, 150, 150 };

// 교차 부분을 저장하기 위한 RECT 구조체
RECT outputRect;

// 윈도우 프로시저 (메시지 처리 함수) 선언
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 메인 함수
#ifdef UNICODE
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
#else
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR pCmdLine, _In_ int nCmdShow)
#endif
{
    // 윈도우 클래스 등록
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));

    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("Computer Software"); // 윈도우 클래스 이름
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 아이콘
    wc.hCursor = LoadCursor(NULL, IDC_CROSS); // 커서 모양
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 배경 색상
    wc.lpfnWndProc = WindowProc; // 윈도우 프로시저 함수

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
        TEXT("202007024 김준형"), // 윈도우 타이틀
        WS_OVERLAPPEDWINDOW, // 윈도우 스타일
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

// 윈도우 프로시저 (메시지 처리 함수) 구현
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd); // 윈도우의 그래픽 컨텍스트 핸들을 얻어옴

    // 브러시 생성 (색상 설정)
    HBRUSH hBrush_user = CreateSolidBrush(RGB(0, 0, 255)); // 파란색 브러시
    HBRUSH hBrush_target = CreateSolidBrush(RGB(255, 0, 255)); // 핑크색 브러시
    HBRUSH hBrush_eraser = CreateSolidBrush(RGB(255, 255, 255)); // 흰색 브러시

    switch (uMsg)
    {
    case WM_CREATE:
    {
        // 초기 윈도우 생성 시 파란색 상자를 그립니다.
        InvalidateRect(hwnd, &rect_user, TRUE); // 파란색 상자를 그리도록 윈도우에 재그리기 메시지를 보냄
    }
    break;

    case WM_KEYDOWN: //방향키가 눌렸을때 파란색 박스 이동
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

        // 파란색 상자 (rect_user)가 핑크색 상자 (rect_target)와 교차하는지 확인합니다.
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
        return DefWindowProc(hwnd, uMsg, wParam, lParam); // 처리하지 않는 메시지는 기본 처리
    }

    DeleteObject(hBrush_user); // 생성한 브러시 해제
    DeleteObject(hBrush_target);
    DeleteObject(hBrush_eraser);
    ReleaseDC(hwnd, hdc); // 그래픽 컨텍스트 해제

    return 0;
}
