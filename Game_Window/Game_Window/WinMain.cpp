#include <windows.h>
//cac file header cân đưa vao
#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>

//tiêu đê ưng dung
#define APPTITLE "Create_Surface"

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

//Để viết chương trình co thể dùng Direct3D,
//	ta cân phai tao một biến để giữ một interface của Direct3D và một interface đến lớp thiết bị đồ họa.
//Kiểu biến của Direct3D interface là LPDIRECT3D9
//	và của thiết bị đồ họa là LPDIRECT3DDEVICE9.
//cac đôi tượng cua Direct3D trang 103
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;

LPDIRECT3DSURFACE9 backbuffer = NULL;
LPDIRECT3DSURFACE9 surface = NULL;

//--------------------------------------------------------------


//window event callback function
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam,
	LPARAM lParam)
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


int Game_Init(HWND hwnd)
{
	HRESULT result;
	//Đối tượng Direct3D được tao bằng một lệnh đơn gian sau
	//initialize Direct3D
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL)
	{
		MessageBox(hwnd, "Error initializing Direct3D", "Error",MB_OK);
		
		return 0;
	}

	/*ta phai thiết lâp các tham số cho thiết bị 
		thông qua một biến D3DPRESENT_PARAMETERS d3dpp 
			và truyền con trỏ đến cho lời gọi hàm.*/
	//set Direct3D presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp)); // xoa mọi thư vê 0 trươc khi sư dung

	d3dpp.Windowed = FALSE;		// thê hiên ơ chê đô (TRUE: cửa sổ), (FALSE: toàn màn hình)
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8; //>???
		//-----------------đủ để khơi tao một cửa sổ để co thể vẽ bằng Direct3D.
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.hDeviceWindow = hwnd;

	//create Direct3D device
	/*tao ra đối tượng device đai diện cho card màn hình từ đối
			tượng Direct3D vừa tao.*/
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT, // dùng card man hinh măc định
		D3DDEVTYPE_HAL, // vẽ bằng phân cưng
						//i.e. bằng card man hinh thay vi gia lập
		hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, // cac tham sô thê hiên cua thiêt bị
		&d3ddev); // đôi tượng dev được tao ra


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
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO,
		&backbuffer);
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
	
		/*Hàm Clear tô toàn bộ backbuffer bằng một màu cho trước – 
			trong vi dụ trên là màu xanh lá.
			Chúng ta cân phai gọi hàm Clear ứng với mỗi frame
				để xoa toàn bộ nội dung đã vẽ ơ những frame trước.
			Nếu không xoa, hình anh của những frame trước sẽ còn lai, 
				kết hợp với những hình anh vẽ ơ frame này
					sẽ tao ra một đống hỗn độn trên màn hình*/
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0), 1.0f, 0);

	//đanh dâu băt đâu vẽ môt frame
	if (d3ddev->BeginScene())
	{
		//fill the surface with random color
		r = rand() % 255;
		g = rand() % 255;
		b = rand() % 255;
		d3ddev->ColorFill(surface, NULL, D3DCOLOR_XRGB(r, g, b));
		//copy the surface to the backbuffer
		rect.left = rand() % SCREEN_WIDTH / 2;
		rect.right = rect.left + rand() % SCREEN_WIDTH / 2;
		rect.top = rand() % SCREEN_HEIGHT;
		rect.bottom = rect.top + rand() % SCREEN_HEIGHT / 2;
		d3ddev->StretchRect(surface, NULL, backbuffer, &rect,
			D3DTEXF_NONE);
		
		//đanh dâu kêt thuc vẽ môt frame//stop rendering
		d3ddev->EndScene();
	}

	//đao nôi dung backbuffer lên front buffer để thể hiện frame lên màn hình.
	//display the back buffer on the screen
	d3ddev->Present(NULL, NULL, NULL, NULL);
	//check for escape key (to exit program)
	if (KEY_DOWN(VK_ESCAPE))
		PostMessage(hwnd, WM_DESTROY, 0, 0);
}

//------------------------------------------------------------------------------------

	/*Trước khi đong game, ta cân giai phong các đối tượng d3d và d3ddev đã tao.
		Khá đơn gian, chỉ cân gọi phương thức Release của chúng*/
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


//------------------------------------------------


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
	//Biến này sẽ được hàm GetMessage ơ sau đổ dữ liệu vào
	MSG msg; // khai bao biên chưa thông điêp
	
	//Ca hai hàm đều nhân tham số là hInstance – định danh của thể hiện truyền từ WinMain.
	////// đăng ky lơp cưa sô
	
	MyRegisterClass(hInstance);
		
	//hàm initinstance sẽ tao ra cửa sổ chương trình nếu instance của chúng là instance đâu tiên.
	//Ngược lai no sẽ thoát.
	
	
	//// khơi tao ưng dung
	HWND hWnd;

	/*if (!InitInstance(hInstance, nCmdShow))
		return FALSE;*/

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
		WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP, //window style
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
	{
		MessageBox(hWnd, "Error initializing the game", "Error", MB_OK);
		return 0;
	}
		

	// vong lăp thông điêp chinh
	int done = 0;
	while (!done)
	{
		/*kiểm tra xem co thông điệp trong hàng đợi hay không,
			nếu co thì lấy thông điệp ra y như GetMessage, nếu không co – thay vì
			đứng đợi như GetMessage – PeekMessage sẽ đơn gian là thoát ra để
			mọi thứ tiếp tục chay.*/
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{

			// kiêm tra xem co phai thông điêp thoat?
				/*Thông điệp sẽ được gửi đến
					trình khi người dùng thoát chương trình một cách cưỡng bức
					(như bấm Alt-F4) hoăc chúng ta tự gửi thông điệp khi phát hiện người
					dùng bấm một phim nào đo như ESC*/
			if (msg.message == WM_QUIT)
				done = 1;

			//giai ma va truyên thông điêp cho WndProc
			// vong lăp thông điêp chinh
				/*vòng lăp sẽ chay mãi mãi cho đến khi nhân được một thông điệp
					khiến cho hàm GetMessage tra về kết qua false
						(thường là thông điệp yêu câu đong cửa sổ ứng dụng).
				Hàm GetMessage co nhiệm vụ lấy ra một thông điệp
					trong số những thông điệp Windows gửi đến chương trình trong hàng đợi thông điệp của ứng dụng.*/
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

