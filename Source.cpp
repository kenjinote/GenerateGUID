#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "rpcrt4")

#include <windows.h>

BOOL GenerateGUID(HWND hWnd, LPTSTR lpszGUID)
{
	GUID guid = GUID_NULL;
	HRESULT hr = UuidCreate(&guid);
	if (HRESULT_CODE(hr) != RPC_S_OK)
	{
		MessageBox(
			hWnd,
			TEXT("UuidCreate関数が失敗しました。"),
			0,
			0);
		return FALSE;
	}
	if (guid == GUID_NULL)
	{
		MessageBox(
			hWnd,
			TEXT("新しいGUIDは生成されませんでした。"),
			0,
			0);
		return FALSE;
	}
	if (HRESULT_CODE(hr) == RPC_S_UUID_NO_ADDRESS)
		MessageBox(
			hWnd,
			TEXT("UUIDを作成するために使用できる")
			TEXT("ネットワークアドレスがありません。"),
			0,
			0);
	if (HRESULT_CODE(hr) == RPC_S_UUID_LOCAL_ONLY)
		MessageBox(
			hWnd,
			TEXT("このコンピュータでのみ有効な")
			TEXT("UUIDが割り当てられました。"),
			0,
			0);
	wsprintf(
		lpszGUID,
		TEXT("%08lX-%04X-%04X-%02X%02X-")
		TEXT("%02X%02X%02X%02X%02X%02X"),
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	return TRUE;
}

LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	static HWND hEdit;
	switch (msg)
	{
	case WM_CREATE:
		CreateWindow(
			TEXT("STATIC"),
			TEXT("GUID:"),
			WS_CHILD | WS_VISIBLE,
			10, 10, 60, 28,
			hWnd,
			0,
			((LPCREATESTRUCT)lParam)->hInstance,
			0);
		hEdit = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			TEXT("EDIT"),
			0,
			WS_CHILD | WS_VISIBLE | ES_READONLY | ES_AUTOHSCROLL,
			80, 10, 480, 28,
			hWnd,
			0,
			((LPCREATESTRUCT)lParam)->hInstance,
			0);
		CreateWindow(
			TEXT("BUTTON"),
			TEXT("生成 ( Enter )"),
			WS_CHILD | WS_VISIBLE,
			80, 50, 128, 28,
			hWnd,
			(HMENU)IDOK,
			((LPCREATESTRUCT)lParam)->hInstance,
			0);
		PostMessage(hWnd, WM_COMMAND, IDOK, 0);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			LPTSTR lpszGUID = (LPTSTR)GlobalAlloc(
				GMEM_FIXED,
				sizeof(TCHAR)*(37));
			if (GenerateGUID(hWnd, lpszGUID))
			{
				SetWindowText(hEdit, lpszGUID);
			}
			GlobalFree(lpszGUID);

			SendMessage(hEdit, EM_SETSEL, 0, -1);
			SetFocus(hEdit);
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefDlgProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	TCHAR szClassName[] = TEXT("Window");
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		DLGWINDOWEXTRA,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("GUIDを生成"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		if (!IsDialogMessage(hWnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
