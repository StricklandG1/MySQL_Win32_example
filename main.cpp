
#include <mysql.h>
#include <iostream>
#include <string>
#include <windows.h>

int qstate;

HWND txt, btn;

enum {ID_BTN};

std::string temp(LPSTR data);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// Register the window class.
	const TCHAR CLASS_NAME[] = "Sample Window Class";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		"Learn to Program Windows",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Run the message loop.

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
		{
			txt = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				"Edit",
				"",
				WS_BORDER | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
				50,
				50,
				400,
				25,
				hwnd,
				NULL,
				NULL,
				NULL
			);

			btn = CreateWindowEx(
				WS_EX_LEFT,
				"BUTTON",
				"test",
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
				50,
				100,
				65,
				25,
				hwnd,
				(HMENU) ID_BTN,
				(HINSTANCE) GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
				NULL
			);
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);



			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

			EndPaint(hwnd, &ps);
			break;
		}
		case WM_COMMAND:
		{
			switch (wParam)
			{
				case ID_BTN:
				{
					int buffersz = 10;
					LPSTR test_txt = new TCHAR[buffersz];
					GetWindowText(txt, test_txt, buffersz);
					std::string result = temp(test_txt);
					MessageBox(hwnd, result.c_str(), result.c_str(), MB_OK);
				}
			}
			break;
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

std::string temp(LPSTR data)
{
	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
	conn = mysql_init(0);

	conn = mysql_real_connect(conn, "localhost", "root", "Ilysem@4", "occ_veteran_club", 3306, nullptr, 0);

	if (!conn) return 0;


	std::string result = "not found";
	std::string query = "SELECT * FROM members WHERE name='" + static_cast<std::string>(data) + "';";
	const char* q = query.c_str();

	qstate = mysql_query(conn, q);

	if (!qstate)
	{
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
			 result = row[1];
	}
	else
	{
		std::cerr << "Query failed: " << mysql_error(conn) << std::endl;
	}
	return result;
}

