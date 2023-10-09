#ifdef UNICODE //�����ڵ� ���ڿ��� ����ϱ� ���� ���Ǻ� �������� �����ϴ� �κ�
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") //�����Ϸ����� ���� ������ ��� ������ �� �˷��ִ� �κ�
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include <windows.h>

HWND hwnd; // ������ �ڵ�
POINT startPos; // �׸��� ���� ��ġ
BOOL isDrawing = FALSE; // �׸��� ��� ����
BOOL isMoving = FALSE; // �̵� ��� ����
HBRUSH pBrush, wBrush; // �귯�� �ڵ�

// �̵��� ���� ����
int offsetX, offsetY;
// ���� �簢���� ��ǥ�� �����ϴ� ����
int prevStartX, prevStartY, prevEndX, prevEndY;
// �ʱ�ȭ �Լ�
void Initialize() {
	isDrawing = FALSE;
	isMoving = FALSE;
	offsetX = 0;
	offsetY = 0;
	prevStartX = 0;
	prevStartY = 0;
	prevEndX = 0;
	prevEndY = 0;
}

// �������� �̺�Ʈ�� ó���ϴ� �ݹ�(Callback) �Լ�.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	switch (uMsg) {
	case WM_CREATE: // �ʱ�ȭ �۾�
		pBrush = CreateSolidBrush(RGB(255, 0, 255)); // RGB ���� ������ ��ȫ�� ����
		wBrush = CreateSolidBrush(RGB(255, 255, 255)); // RGB ���� ������ �������
		Initialize();
		break;
	case WM_LBUTTONDOWN: // ���� ���콺 ��ư�� ������ ��
		isDrawing = TRUE;
		startPos.x = LOWORD(lParam);
		startPos.y = HIWORD(lParam);
		break;
	case WM_RBUTTONDOWN: // ������ ���콺 ��ư�� ������ ��
		if (LOWORD(lParam) >= prevStartX && LOWORD(lParam) <= prevEndX &&
			HIWORD(lParam) >= prevStartY && HIWORD(lParam) <= prevEndY) {
			isMoving = TRUE;
			offsetX = LOWORD(lParam) - prevStartX;
			offsetY = HIWORD(lParam) - prevStartY;
		}
		break;
	case WM_RBUTTONUP:
		// ���콺 ������ ��ư �� ��, �̵� ��� ����

		isMoving = FALSE;
		break;
	case WM_LBUTTONUP:

		if (isDrawing) {
			isDrawing = FALSE;
			int endX = LOWORD(lParam);
			int endY = HIWORD(lParam);

			// ���� �簢�� ����
			HDC hdc = GetDC(hwnd);
			SelectObject(hdc, wBrush); // ȭ��Ʈ �귯�� ����
			SetROP2(hdc, R2_WHITE);
			Rectangle(hdc, prevStartX, prevStartY, prevEndX, prevEndY);

			// ���ο� �簢�� �׸���
			SetROP2(hdc, R2_COPYPEN);
			SelectObject(hdc, pBrush); // ��ȫ�� �귯�� ����
			Rectangle(hdc, startPos.x, startPos.y, endX, endY);
			ReleaseDC(hwnd, hdc);

			// ���� �簢���� ��ǥ ����
			prevStartX = startPos.x;
			prevStartY = startPos.y;
			prevEndX = endX;
			prevEndY = endY;
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
			int newX = LOWORD(lParam) - offsetX;
			int newY = HIWORD(lParam) - offsetY;
			int width = prevEndX - prevStartX;
			int height = prevEndY - prevStartY;


			// ���� �簢�� ����
			HDC hdc = GetDC(hwnd);
			SelectObject(hdc, wBrush); // �Ͼ�� �귯�� ����
			SetROP2(hdc, R2_WHITE);
			Rectangle(hdc, prevStartX, prevStartY, prevEndX, prevEndY);

			// �̵��� �簢�� �׸���
			SetROP2(hdc, R2_COPYPEN);
			SelectObject(hdc, pBrush); // ��ȫ�� �귯�� ����
			Rectangle(hdc, newX, newY, newX + width, newY + height);
			ReleaseDC(hwnd, hdc);

			// ���� �簢���� ��ǥ ����
			prevStartX = newX;
			prevStartY = newY;
			prevEndX = newX + width;
			prevEndY = newY + height;

		}
		break;
	case WM_GETMINMAXINFO:
		// â ũ�� ������ �����ϱ� ���� �޽��� �ڵ鸵
		if (1) {
			MINMAXINFO* pInfo = (MINMAXINFO*)lParam;
			pInfo->ptMinTrackSize.x = 800; // �ּ� ��
			pInfo->ptMinTrackSize.y = 600; // �ּ� ����
			pInfo->ptMaxTrackSize.x = 800; // �ִ� ��
			pInfo->ptMaxTrackSize.y = 600; // �ִ� ����
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
	/* ������ Ŭ���� ����.*/
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));// ��� 0���� �ʱ�ȭ.

	// ������ Ŭ���� �� ����
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT("Computer Software");
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;

	// ������ Ŭ���� ���.
	if (RegisterClass(&wc) == 0)
	{
		MessageBox(NULL, L"RegisterClass failed!", L"Error", MB_ICONERROR);
		exit(-1);	//����
	}

	// Window viewport ���� ����
	RECT rect = { 150, 100, 800, 600 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;


	// Window viewport ���� ����

	// ������ ����
	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		TEXT("202007024 ������"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		0, 0, 800, 600,
		NULL, NULL,
		hInstance,
		NULL
	);

	// ���� �˻�.
	if (hwnd == NULL)
	{
		MessageBox(NULL, L"CreateWindow failed!", L"Error", MB_ICONERROR);
		exit(-1);
	}

	// â ���̱�.
	ShowWindow(hwnd, SW_SHOW); // â ���
	UpdateWindow(hwnd); // ������Ʈ�ؾ� ����. �� ������ ����



	// �޽��� ó�� ����.
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		// �޽��� ó��.
		if (GetMessage(&msg, hwnd, 0, 0))
			//if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) // PM_REMOVE�� �ڸ��� �� �޼����� ���� ��Ұų�.�� �ǹ�
		{

			// �޽��� �ؼ�
			TranslateMessage(&msg);
			// �޽����� ó���ؾ��� ���� ����
			DispatchMessage(&msg);

		}
		/*else
		{

		}*/
	}

	//���� �޽��� ������
	return (int)msg.wParam;

}