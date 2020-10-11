// Anim_Sprite program source code file
#include "game.h"
LPDIRECT3DSURFACE9 kitty_image[7];

LPDIRECT3DSURFACE9 back; //load background

SPRITE kitty;
//timing variable
long start = GetTickCount();
//initializes the game
int Game_Init(HWND hwnd)
{
	char s[20];
	int n;

	TCHAR Back[] = _T("background.bmp"); //lmao fking bug (cant not convert from const char* to char*)

	back = LoadSurface(Back, NULL);

	//set random number seed
	srand(time(NULL));
	//load the sprite animation
	for (n = 0; n < 6; n++)
	{
		sprintf(s, "cat%d.bmp", n + 1); //important: there must be cat1-cat6.bmp in your project (dont know why 1-6 but not 0-5, though)
		kitty_image[n] = LoadSurface(s, D3DCOLOR_XRGB(255, 0, 255)); //màu nến là màu hồng???
		/*tai sao hàm “LoadSurface” lai cân phai quan tâm về màu nền.
		Hãy để y, project này không sử dụng chế độ trong suốt(transparent).
		Ta cân chỉ ra màu trong suốt 
			để hàm “StretchRect” vẽ màu trong suốt thành màu đen
				(hàm StretchRect không hỗ trợ màu trong suốt thât sự).
		Thât tiện dụng, bơi vì ta co thể sử dụng bất kỳ màu gì mình muốn
			trong khi sửa sprite để bù no từ background, 
				và no sẽ được vẽ thành màu đen khi load vào game.*/
		if (kitty_image[n] == NULL)
			return 0;
	}
	//initialize the sprite's properties
	kitty.x = 100;
	kitty.y = 150;
	kitty.width = 96;
	kitty.height = 96;
	kitty.curframe = 0;
	kitty.lastframe = 5;
	kitty.animdelay = 2;
	kitty.animcount = 0;
	kitty.movex = 8;
	kitty.movey = 0;
	//return okay
	return 1;
}
//the main game loop
/*Luôn nhớ rằng no phai xử ly trong lân update màn hình và chỉ co vây.
	Đừng bao giờ đăt vòng lăp while ơ đây hoăc giống như thế
		bơi vì điều khiển sẽ không tra về WinMain).
Co 2 phân cho hàm Game_Run.
	Phân đâu thực hiện di chuyển và chuyển động cho sprite.
	Phân 2 thực hiện vẽ sprite lên màn hình.
Ly do chia ra thành 2 phân(1 cho logic, 1 cho làm mới màn hình) 
	là bơi vì ta không muốn tốn nhiều thời gian xử ly 
		giữa 2 lời gọi BeginScene và EndScene,
	do đo, hãy tối gian code ơ đây, 
		chỉ dùng để update đồ họa và đăt các xử ly khác ơ phân còn lai.*/
void Game_Run(HWND hwnd)
{
	RECT rect;
	//make sure the Direct3D device is valid
	if (d3ddev == NULL)
		return;
	//after short delay, ready for next frame?
	//this keeps the game running at a steady frame rate
	if (GetTickCount() - start >= 30)
	{
		//reset timing
		start = GetTickCount();
		//move the sprite
		kitty.x += kitty.movex;
		kitty.y += kitty.movey;
		//"warp" the sprite at screen edges
		if (kitty.x > SCREEN_WIDTH - kitty.width)
			kitty.x = 0;
		if (kitty.x < 0)
			kitty.x = SCREEN_WIDTH - kitty.width;
		//has animation delay reached threshold?
		if (++kitty.animcount > kitty.animdelay)
		{
			//reset counter
			kitty.animcount = 0;
			//animate the sprite
			if (++kitty.curframe > kitty.lastframe)
				kitty.curframe = 0;
		}
	}
	//start rendering
	if (d3ddev->BeginScene())
	{
		//erase the entire background
		//d3ddev->ColorFill(backbuffer, NULL,D3DCOLOR_XRGB(0, 0, 0));

		d3ddev->StretchRect(back, NULL, backbuffer, NULL,D3DTEXF_NONE);


		//set the sprite's rect for drawing
		rect.left = kitty.x;
		rect.top = kitty.y;
		rect.right = kitty.x + kitty.width;
		rect.bottom = kitty.y + kitty.height;
		//draw the sprite
		d3ddev->StretchRect(kitty_image[kitty.curframe],
			NULL, backbuffer, &rect, D3DTEXF_NONE);
		//stop rendering
		d3ddev->EndScene();
	}
	//display the back buffer on the screen
	d3ddev->Present(NULL, NULL, NULL, NULL);
	//check for escape key (to exit program)
	if (KEY_DOWN(VK_ESCAPE))
		PostMessage(hwnd, WM_DESTROY, 0, 0);
}
//frees memory and cleans up before the game ends
void Game_End(HWND hwnd)
{
	int n;
	//free the surface
	for (n = 0; n < 6; n++)
		kitty_image[n]->Release();
	back->Release();
}