#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <WinUser.h>
#include <tchar.h>


static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("First laboratory work");
HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void AddComponents();
HWND picturebox;
int xcoord = 10;
int ycoord = 300;
HWND hWnd;
int xl = 0, yl = 0;
int priznak = 0;


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

	AddComponents();
	SetTimer(hWnd, 10, 10, (TIMERPROC)NULL); 
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
	PAINTSTRUCT ps;
	HDC hdc;

	WPARAM keys, delta;
	switch (message)
	{
	case WM_TIMER:
		if (wParam == 10 && priznak == 0)
		{
			RECT windowRect;
			GetWindowRect(hWnd, &windowRect);
			if (xcoord < 0)
				xl = 1;
			if (xcoord > windowRect.right - windowRect.left - 250)
				xl = 0;
			if (ycoord < 0)
				yl = 1;
			if (ycoord > windowRect.bottom - windowRect.top - 250)
				yl = 0;

			if (yl == 1)
				ycoord++;
			if (yl == 0)
				ycoord--;
			if (xl == 1)
				xcoord++;
			if (xl == 0)
				xcoord--;
			
			SetWindowPos(picturebox, NULL, xcoord, ycoord, 225, 225, NULL);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOUSEWHEEL:
		keys = GET_KEYSTATE_WPARAM(wParam);
		delta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (keys == MK_SHIFT && (int)delta < 0)
		{
			if (xcoord > 0)
				xcoord--;
			SetWindowPos(picturebox, NULL, xcoord, ycoord, 225, 225, NULL);
		}
		else if (keys == MK_SHIFT && (int)delta > 0)
		{
			if (xcoord < 600 - 225)
				xcoord++;
			SetWindowPos(picturebox, NULL, xcoord, ycoord, 225, 225, NULL);
		}
		else if (keys != MK_SHIFT && (int)delta < 0)
		{
			if (ycoord > 0)
				ycoord--;
			SetWindowPos(picturebox, NULL, xcoord, ycoord, 225, 225, NULL);
		}
		else if(keys != MK_SHIFT && (int)delta > 0)
		{
			if (ycoord < 600 - 225)
			ycoord++;
			SetWindowPos(picturebox, NULL, xcoord, ycoord, 225, 225, NULL);
		}
		break;
	case WM_KEYDOWN:
	{
		if (wParam == VK_DOWN)
		{
			ycoord++;
			SetWindowPos(picturebox, NULL, xcoord, ycoord, 225, 225, NULL);
		}
		else if (wParam == VK_UP)
		{
			ycoord--;
			SetWindowPos(picturebox, NULL, xcoord, ycoord, 225, 225, NULL);
		}
		else if (wParam == VK_LEFT)
		{
			xcoord--;
			SetWindowPos(picturebox, NULL, xcoord, ycoord, 225, 225, NULL);
		}
		else if (wParam == VK_RIGHT)
		{
			xcoord++;
			SetWindowPos(picturebox, NULL, xcoord, ycoord, 225, 225, NULL);
		}
		else if (wParam == VK_SPACE)
		{
			if (priznak == 0)
				priznak = 1;
			else
				priznak = 0;
		}
	}
	case WM_MOUSEMOVE:
		if (wParam == MK_LBUTTON)
		{
			xcoord = LOWORD(lParam) - 100;
			ycoord = HIWORD(lParam) - 100;
			SetWindowPos(picturebox, NULL, xcoord, ycoord, 225, 225, NULL);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}


void AddComponents()
{
	HICON bmp = (HICON)LoadImageW(NULL, L"logo.ico", IMAGE_ICON, 225, 225, LR_LOADFROMFILE);
	picturebox = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD | SS_ICON, 200, 200, 225, 225, hWnd, NULL, NULL, NULL);
	SendMessage(picturebox, STM_SETIMAGE, IMAGE_ICON, (LPARAM)bmp);
}