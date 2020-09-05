#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <WinUser.h>
#include <tchar.h>

static TCHAR szWindowClass[] = L"DesktopApp";
static TCHAR szTitle[] = L"Second laboratory work";
HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND hWnd;
HDC dc;
PAINTSTRUCT ps;
VOID OnPaint(HWND);

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	WNDCLASSEX wcex;

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
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = wcex.hIcon;
	

	RegisterClassEx(&wcex);

	hInst = hInstance;

	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		300, 100,
		600, 600,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPMINMAXINFO minsize;
	POINT pt = { 400, 400 };
	switch (message)
	{
	//	case WM_LBUTTON
	case WM_PAINT:
		dc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_GETMINMAXINFO:
		minsize = (LPMINMAXINFO)lParam;
		minsize->ptMinTrackSize.x = pt.x;
		minsize->ptMinTrackSize.y = pt.y;
		break;
	case WM_SIZE:
		OnPaint(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}



VOID OnPaint(HWND hWnd)
{
	RECT  rect;
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	RECT wsize;
	GetWindowRect(hWnd, &wsize);

	GetClientRect(hWnd, &rect);
	SetTextColor(hdc, RGB(0xFF, 0x00, 0x00));
	SetBkMode(hdc, TRANSPARENT);
	int height = wsize.bottom - wsize.top;
	int width = wsize.right - wsize.left;

	SetDCBrushColor(hdc, RGB(255,0,0));

	// header
	MoveToEx(hdc, 0, 50, NULL);
	LineTo(hdc, width, 50);
	
	//1
	MoveToEx(hdc, width / 4, 0, NULL);
	LineTo(hdc, width / 4, height);
	//2
	MoveToEx(hdc, width / 4 * 2, 0, NULL);
	LineTo(hdc, width / 4 * 2, height);
	//3
	MoveToEx(hdc, width / 4 * 3, 0, NULL);
	LineTo(hdc, width / 4 * 3, height);
	//4
	MoveToEx(hdc, width, 0, NULL);
	LineTo(hdc, width, height);
	
	SetTextColor(hdc, RGB(255, 40, 40));

	static _TCHAR printStr[1000] = L"Name, Surname, FatherName";
	RECT textBlocks;
	textBlocks.left = 10;
	textBlocks.top = 10;
	textBlocks.right = width / 4;
	DrawText(hdc, printStr, wcslen(printStr), &textBlocks, DT_WORDBREAK);
	textBlocks.top = 55;
	textBlocks.bottom = height - 5;
	wcscpy_s(printStr, 1000, L"skldj sdfsfglhxlkj lkjhgsriunjflk fsdlkjvh vfsdflkjhs dfsdfv sdf vsdf");
	DrawText(hdc, printStr, wcslen(printStr), &textBlocks, DT_WORDBREAK);

	EndPaint(hWnd, &ps);
}