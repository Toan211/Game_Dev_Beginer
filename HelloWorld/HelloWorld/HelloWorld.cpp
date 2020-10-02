#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
///*require <windows.h>. 
///		<tchar.h> defines the TCHAR macro, 
///			which resolves ultimately to wchar_t if the UNICODE symbol is defined in your project, 
///				otherwise it resolves to char. 
///	If you always build with UNICODE enabled, you don't need TCHAR and can just use wchar_t directly*/


//https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain
//if anything, go to google for more detail, no need waste your time reading this
#if 0 Explanation_VN
HINSTANCE hInstance.Tham số này định danh thể hiên (instance) của chương
	trình được nap và thi hành trên bộ nhớ
		Tham số hInstance cho phép ta biết mình đang chay ơ instance nào.
HINSTANCE hPrevInstance : Tham số này cho biết định danh 
							của thể hiện vừa được gọi trước của chương trình.
	Một ứng dụng thường của hPrevInstance là để đam bao
		chỉ co một thể hiện duy nhất của chương trình.
	Nếu “chúng ta” là thể hiện đâu tiên thì
		hPrevInstance sẽ là null, ngược lai hPrevInstance sẽ khác null.
	Điều này khá hợp với game vì chúng ta thường sẽ không muốn người dùng mơ
		cùng lúc nhiều cửa sổ của cùng một game.
LPSTR lpCmdLine : Tham số thứ 3 là chuỗi 
						chứa các tham số dòng lệnh truyền cho chương trình.
					Tham số dòng lệnh thường dùng để thiết lâp 
							hoăc truyền thêm dữ liệu gì đo cho chương trình.
					Chẳng han : notepad	
						C : \tmp\test.txt sẽ truyền tham số là tên tâp tin cân mơ.
int nCmdShow : tham số này gợi y cho chương trình
					nên tao ra cửa sổ thế nào khi thực thi.
			Giá trị tra về của WinMain là một số nguyên thuộc kiểu int WINAPI .
			Giá trị tra về bằng 0 nghĩa là co lỗi và khác 0 nghĩa là mọi thứ bình thường

#endif 

#if 1 HelloWorld_easy
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,_In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	MessageBox(NULL,"Welcome!","Hello World",MB_OK | MB_ICONEXCLAMATION);
	return 0;

}
#endif

#if 0 HelloWorld_but_harder

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Windows Desktop Guided Tour Application");

HINSTANCE hInst;

/*Along with the WinMain function,
	every Windows desktop application must also have a window-procedure function
In this function, you write code
	to handle messages that the application receives from Windows when events occur.
For example, if a user chooses an OK button in your application,
	Windows will send a message to you and
		you can write code inside your WndProc function that does whatever work is appropriate.
	It's called handling an event.
	You only handle the events that are relevant for your application.
https://docs.microsoft.com/en-us/windows/win32/winmsg/window-procedures
*/

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(_In_ HWND   hWnd, _In_ UINT   message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{

	PAINTSTRUCT ps;
	/*
	HDC in the code is a handle to a device context, which is used to draw in the window's client area.
	*/
	HDC hdc;
	TCHAR greeting[] = _T("Hello, World!");

	switch (message)
	{
		/*One important message to handle is the WM_PAINT message.
	The application receives the WM_PAINT message when part of its displayed window must be updated.
	The event can occur when a user moves a window in front of your window, then moves it away again.
	Your application doesn't know when these events occur.
	Only Windows knows, so it notifies your app with a WM_PAINT message.
	When the window is first displayed, all of it must be updated.

	To handle a WM_PAINT message,
	first call BeginPaint,
		then handle all the logic to lay out the text, buttons, and other controls in the window,
			and then call EndPaint.
	For the application, the logic between the beginning call and the ending call
		displays the string "Hello, Windows desktop!" in the window.
	In the following code, the TextOut function is used to display the string.*/

	case WM_PAINT:
		/*Use the BeginPaint and EndPaint functions to prepare for and complete the drawing in the client area.
			BeginPaint returns a handle to the display device context used for drawing in the client area;*/
		hdc = BeginPaint(hWnd, &ps);

		// Here your application is laid out.
		// For this introduction, we just print out "Hello, Windows desktop!"
		// in the top left corner.
		TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
		// End application-specific layout section.

		//	EndPaint ends the paint request and releases the device context.*/
		EndPaint(hWnd, &ps);
		break;

		/*An application typically handles many other messages.
		For example, WM_CREATE when a window is first created,
		and WM_DESTROY when the window is closed*/
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}


int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	/*
	In the WinMain function, you populate a structure of type WNDCLASSEX. 
	The structure contains information about the window: 
		the application icon, the background color of the window,
			the name to display in the title bar, among other things. 
	Importantly, it contains a function pointer to your window procedure
//https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
	*/
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
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	/*Register the WNDCLASSEX with Windows
		so that it knows about your window and how to send messages to it. 
	Use the RegisterClassEx function and pass the window class structure as an argument. 
	The _T macro is used because we use the TCHAR type.*/
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,_T("Call to RegisterClassEx failed!"),_T("Windows Desktop Guided Tour"),NULL);

		return 1;
	}
	// Store instance handle in our global variable
	hInst = hInstance;

	// Now you can create a window.Use the CreateWindow function.
	//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindoww

	/*
	This function returns an HWND, which is a handle to a window. 
		A handle is somewhat like a pointer that Windows uses to keep track of open windows.
	The parameters to CreateWindow explained:
		szWindowClass: the name of the application
		szTitle: the text that appears in the title bar
		WS_OVERLAPPEDWINDOW: the type of window to create
		CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
		500, 100: initial size (width, length)
		NULL: the parent of this window
		NULL: this application does not have a menu bar
		hInstance: the first parameter from WinMain
		NULL: not used in this application
	https://docs.microsoft.com/en-us/windows/win32/WinProg/windows-data-types
	*/
	HWND hWnd = CreateWindow(szWindowClass,szTitle,WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, CW_USEDEFAULT,
									500, 100,NULL,NULL,hInstance,NULL);
	
	if (!hWnd)
	{
		MessageBox(NULL,_T("Call to CreateWindow failed!"),_T("Windows Desktop Guided Tour"),NULL);

		return 1;
	}

	//The window has been created, but we still need to tell Windows to make it visible
	//The parameters to ShowWindow explained:
//		 hWnd: the value returned from CreateWindow
//		 nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	/*Main message loop:
	The displayed window doesn't have much content 
		because you haven't yet implemented the WndProc function. 
		In other words, the application isn't yet handling the messages
			that Windows is now sending to it.

To handle the messages, we first add a message loop to listen for the messages that Windows sends.
	When the application receives a message,
		this loop dispatches it to your WndProc function to be handled
*/
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}


#endif