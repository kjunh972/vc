#ifdef UNICODE //유니코드 문자열을 사용하기 위한 조건부 컴파일을 설정하는 부분
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") //컴파일러에게 실행 파일을 어떻게 생성할 지 알려주는 부분
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include <windows.h>

HWND hwnd; // 윈도우 핸들
POINT startPos; // 그리기 시작 위치
BOOL isDrawing = FALSE; // 그리기 모드 여부
BOOL isMoving = FALSE; // 이동 모드 여부
HBRUSH pBrush, wBrush; // 브러시 핸들

// 이동을 위한 변수
int setX, setY;
// 이전 사각형의 좌표를 저장하는 변수
int startX, startY, endX, endY;

void Initialize() { //초기화
	isDrawing = FALSE;
	isMoving = FALSE;
	setX = 0;
	setY = 0;
	startX = 0;
	startY = 0;
	endX = 0;
	endY = 0;
}

// 윈도우의 이벤트를 처리하는 콜백(Callback) 함수.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	switch (uMsg) {
	case WM_CREATE: // 초기화 작업
		pBrush = CreateSolidBrush(RGB(255, 0, 255)); // RGB 색상 값으로 분홍색 설정
		wBrush = CreateSolidBrush(RGB(255, 255, 255)); // RGB 색상 값으로 흰색설정
		Initialize();
		break;
	case WM_LBUTTONDOWN: // 왼쪽 마우스 버튼을 눌렀을 때
		isDrawing = TRUE;
		startPos.x = LOWORD(lParam);
		startPos.y = HIWORD(lParam);
		break;
	case WM_RBUTTONDOWN: // 오른쪽 마우스 버튼을 눌렀을 때
		if (LOWORD(lParam) >= startX && LOWORD(lParam) <= endX &&
			HIWORD(lParam) >= startY && HIWORD(lParam) <= endY) {
			isMoving = TRUE;
			setX = LOWORD(lParam) - startX;
			setY = HIWORD(lParam) - startY;
		}
		break;
	case WM_RBUTTONUP:
		// 마우스 오른쪽 버튼 뗄 때, 이동 모드 해제

		isMoving = FALSE;
		break;
	case WM_LBUTTONUP:

		if (isDrawing) {
			isDrawing = FALSE;
			int endX = LOWORD(lParam);
			int endY = HIWORD(lParam);

			// 이전 사각형 삭제
			HDC hdc = GetDC(hwnd);
			SelectObject(hdc, wBrush); // 화이트 브러시 선택
			SetROP2(hdc, R2_WHITE);
			Rectangle(hdc, startX, startY, endX, endY);

			// 새로운 사각형 그리기
			SetROP2(hdc, R2_COPYPEN);
			SelectObject(hdc, pBrush); // 분홍색 브러시 선택
			Rectangle(hdc, startPos.x, startPos.y, endX, endY);
			ReleaseDC(hwnd, hdc);

			// 현재 사각형의 좌표 저장
			startX = startPos.x;
			startY = startPos.y;
			endX = endX;
			endY = endY;
		}
		break;
	case WM_MOUSEMOVE:
		if (isDrawing) {
			int currentX = LOWORD(lParam);
			int currentY = HIWORD(lParam);

			HDC hdc = GetDC(hwnd);
			SetROP2(hdc, R2_COPYPEN);
			Rectangle(hdc, startPos.x, startPos.y, currentX, currentY);


			SetROP2(hdc, R2_NOTXORPEN);
			SelectObject(hdc, pBrush);
			Rectangle(hdc, startPos.x, startPos.y, currentX, currentY);

			ReleaseDC(hwnd, hdc);
		}
		else if (isMoving && (wParam & MK_RBUTTON)) {
			int newX = LOWORD(lParam) - setX;
			int newY = HIWORD(lParam) - setY;
			int area = endX - startX;
			int high = endY - startY;


			// 이전 사각형 삭제
			HDC hdc = GetDC(hwnd);
			SelectObject(hdc, wBrush); // 하얀색 브러시 선택
			SetROP2(hdc, R2_WHITE);
			Rectangle(hdc, startX, startY, endX, endY);

			// 이동한 사각형 그리기
			SetROP2(hdc, R2_COPYPEN);
			SelectObject(hdc, pBrush); // 분홍색 브러시 선택
			Rectangle(hdc, newX, newY, newX + area, newY + high);
			ReleaseDC(hwnd, hdc);

			// 현재 사각형의 좌표 저장
			startX = newX;
			startY = newY;
			endX = newX + area;
			endY = newY + high;

		}
		break;
	case WM_GETMINMAXINFO:
		// 창 크기 변경을 제한하기 위한 메시지 핸들링
		if (1) {
			MINMAXINFO* pInfo = (MINMAXINFO*)lParam;
			pInfo->ptMinTrackSize.x = 800; // 최소 폭
			pInfo->ptMinTrackSize.y = 600; // 최소 높이
			pInfo->ptMaxTrackSize.x = 800; // 최대 폭
			pInfo->ptMaxTrackSize.y = 600; // 최대 높이
			break;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return S_OK;
}
#ifdef UNICODE
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
#else
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR pCmdLine, _In_ int nCmdShow)
#endif
{
	/* 윈도우 클래스 선언.*/
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));// 모두 0으로 초기화.

	// 윈도우 클래스 값 설정
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT("Computer Software");
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;

	// 윈도우 클래스 등록.
	if (RegisterClass(&wc) == 0)
	{
		MessageBox(NULL, L"RegisterClass failed!", L"Error", MB_ICONERROR);
		exit(-1);	//예외
	}

	// Window viewport 영역 조정
	RECT rect = { 150, 100, 800, 600 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;


	// Window viewport 영역 조정

	// 윈도우 생성
	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		TEXT("202007024 김준형"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		0, 0, 800, 600,
		NULL, NULL,
		hInstance,
		NULL
	);

	// 오류 검사.
	if (hwnd == NULL)
	{
		MessageBox(NULL, L"CreateWindow failed!", L"Error", MB_ICONERROR);
		exit(-1);
	}

	// 창 보이기.
	ShowWindow(hwnd, SW_SHOW); // 창 띄움
	UpdateWindow(hwnd); // 업데이트해야 보임. 한 쌍으로 쓰임



	// 메시지 처리 루프.
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		// 메시지 처리.
		if (GetMessage(&msg, hwnd, 0, 0))
			//if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) // PM_REMOVE의 자리는 이 메세지를 쓰고 어떡할거냐.의 의미
		{

			// 메시지 해석
			TranslateMessage(&msg);
			// 메시지를 처리해야할 곳에 전달
			DispatchMessage(&msg);

		}
		/*else
		{

		}*/
	}

	//종료 메시지 보내기
	return (int)msg.wParam;

}