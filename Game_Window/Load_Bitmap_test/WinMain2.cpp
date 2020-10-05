#include <windows.h>
//cac file header cân đưa vao
#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>


//tiêu đê ưng dung
#define APPTITLE "Load_Bitmap"

//cac macro đê đọc phim
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000)? 1 : 0)
#define KEY_UP(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1: 0)

	//đô phân giai man hinh
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

//cac khai bao ham
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

ATOM MyRegisterClass(HINSTANCE);
int Game_Init(HWND);
void Game_Run(HWND);
void Game_End(HWND);

HWND hWnd = 0;

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;
int BackBufferWidth = 0;
int BackBufferHeight = 0;
LPDIRECT3DSURFACE9 surface = NULL;



//-----------------------------------------------------------

//window event callback function
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		Game_End(hWnd);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-------------------------------------------------------------------------

//cac ham hô trợ đê khơi đông Window
ATOM MyRegisterClass(HINSTANCE hInstance) //creatGameWindow
{
	//create the window class structure
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	//điên tham sô ham vao struct
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = APPTITLE;
	wc.hIconSm = NULL;
	//đăng ky lơp cưa sô
	return RegisterClassEx(&wc);

	
}

//-----------------------------------------------------------------------

int Game_Init(HWND hwnd)
{
	HRESULT result;
	//Đối tượng Direct3D được tao bằng một lệnh đơn gian sau
	//initialize Direct3D
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp)); // xoa mọi thư vê 0 trươc khi sư dung

	d3dpp.Windowed = TRUE;		// thê hiên ơ chê đô cửa sổ
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8; //>???
		//-----------------đủ để khơi tao một cửa sổ để co thể vẽ bằng Direct3D.
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.hDeviceWindow = hwnd;

	
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT, // dùng card man hinh măc định
		D3DDEVTYPE_HAL, // vẽ bằng phân cưng
		hwnd,			//i.e. bằng card man hinh thay vi gia lập
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, // cac tham sô thê hiên cua thiêt bị
		&d3ddev); // đôi tượng dev được tao ra

#if 1
	if (d3ddev == NULL)
	{
		
		MessageBox(hwnd, "Error creating Direct3D device","Error", MB_OK);
		return 0;
	}
	//set random number seed
	srand(time(NULL));
	//clear the backbuffer to black
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	//create pointer to the back buffer
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO,&backbuffer);

	//create surface
	result = d3ddev->CreateOffscreenPlainSurface(
		100, //width of the surface
		100, //height of the surface
		D3DFMT_X8R8G8B8, //surface format
		D3DPOOL_DEFAULT, //memory pool to use
		&surface, //pointer to the surface
		NULL); //reserved (always NULL)
	if (!result)
		return 1;
	
#endif
	//return okay
	return 1;
}

//--------------------------------------------------------------------

void Game_Run(HWND hwnd)
{
	RECT rect;
	int r, g, b;

	//đam bao con trỏ đên Direct3D la hợp lê
	if (d3ddev == NULL)
		return;
	//Xoa backbuffer vê mau đen
	
		
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0), 1.0f, 0);

	//đanh dâu băt đâu vẽ môt frame
	if (d3ddev->BeginScene())
	{
		//fill the surface with random color
		r = rand() % 255;
		g = rand() % 255;
		b = rand() % 255;

		//Nếu muốn thay màu những thứ đang vẽ, ta co thể dùng hàm ColorFill
		d3ddev->ColorFill(surface, NULL, D3DCOLOR_XRGB(r, g, b)); 
		
		//copy the surface to the backbuffer 
		rect.left = SCREEN_WIDTH / 10;
		rect.right = SCREEN_WIDTH / 2;
		rect.top =  SCREEN_HEIGHT/10;
		rect.bottom = SCREEN_HEIGHT / 2;
		
		//Ta co thể vẽ một surface lên surface khác.Chúng ta co hàm StretchRect.
		d3ddev->StretchRect(surface, NULL, backbuffer, &rect,D3DTEXF_NONE); 
		
		//đanh dâu kêt thuc vẽ môt frame//stop rendering
		d3ddev->EndScene();
	}

	
	//display the back buffer on the screen
	d3ddev->Present(NULL, NULL, NULL, NULL);
	//check for escape key (to exit program)
	if (KEY_DOWN(VK_ESCAPE))
		PostMessage(hwnd, WM_DESTROY, 0, 0); //thoát game//thoát cd toàn màn hình
}

//------------------------------------------------------------------------------------

void Game_End(HWND hwnd)
{
	MessageBox(hwnd, "Program is about to end", "Game_End", MB_OK);
	//free the surface
	surface->Release();
	
	//release the Direct3D device
	if (d3ddev != NULL) d3ddev->Release();
	//release the Direct3D object
	if (d3d != NULL) d3d->Release();
}

//-------------------------------------------------------------------------

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	
	MSG msg; // khai bao biên chưa thông điêp
	
	
	////// đăng ky lơp cưa sô
	
	MyRegisterClass(hInstance);
		
	
	HWND hWnd;

	

	//khơi đông bô tao sô ngâu nhiên
	srand(time(NULL));
#if 0
		while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	} //trang 98
	return msg.wParam;
#endif
	//tao môt cưa sô
	hWnd = CreateWindow(
		APPTITLE, //window class
		APPTITLE, //title bar
		WS_OVERLAPPEDWINDOW, //tạo cửa sổ đóng mở dc (có -,x)
		//WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP, //window style
		CW_USEDEFAULT, //x position of window
		CW_USEDEFAULT, //y position of window
		SCREEN_WIDTH, //width of the window
		SCREEN_HEIGHT, //height of the window
		NULL, //parent window
		NULL, //menu
		hInstance, //application instance
		NULL); //window parameters

	//kiêm tra lôi nêu không tao được cưa sô
	if (!hWnd)
		return FALSE;

	//hiên thị cưa sô
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//khơi tao game
	if (!Game_Init(hWnd))
		return 0;

	// vong lăp thông điêp chinh
	int done = 0;
	while (!done)
	{
		
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{

			// kiêm tra xem co phai thông điêp thoat?
				
			if (msg.message == WM_QUIT)
				done = 1;

			//giai ma va truyên thông điêp cho WndProc
						TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//process game loop
		else
			//xư ly game
			Game_Run(hWnd);
	}//trang 99-100
	return msg.wParam;
}

