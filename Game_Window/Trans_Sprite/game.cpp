#include "game.h"
LPDIRECT3DTEXTURE9 kitty_image[7];
SPRITE kitty;
LPDIRECT3DSURFACE9 back;
LPD3DXSPRITE sprite_handler;
HRESULT result;
// biên thơi gian
long start = GetTickCount();
// Khơi tao game
int Game_Init(HWND hwnd)
{
	char s[20];
	int n;
	// thiêt lập sô ngâu nhiên
	srand(time(NULL));
	// tao đôi tượng sprite handler
	result = D3DXCreateSprite(d3ddev, &sprite_handler);
	if (result != D3D_OK)
		return 0;
	// tai sprite animation
	for (n = 0; n < 6; n++)
	{
		// thiêt lập tên têp
		sprintf(s, "cat%d.bmp", n + 1);
		// tai texture vơi mau "hồng" la mau trong suôt
		kitty_image[n] = LoadTexture(s,D3DCOLOR_XRGB( 0, 0, 0));
		if (kitty_image[n] == NULL)
			return 0;
	}
	TCHAR Back[] = _T("background.bmp"); //lmao fking bug (cant not convert from const char* to char*)
	// tai hinh nên
	back = LoadSurface(Back, NULL);
	// Khơi tao thông tin sprite
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
	// tra vê okay
	return 1;
}
void Game_Run(HWND hwnd)
{
	// Đam bao Direct3D device đa co
	if (d3ddev == NULL)
		return;
	// Sau vai giây delay, sẵn sang frame tiêp theo ?
	// điêu nay giữ cho game chay vơi môt tôc đô ôn định
	if (GetTickCount() - start >= 30)
	{
		// Tai lập lai thơi gian
		start = GetTickCount();
		// di chuyên sprite
		kitty.x += kitty.movex;
		kitty.y += kitty.movey;
		// Đong goi sprite vao trong cac khung man hinh
		if (kitty.x > SCREEN_WIDTH - kitty.width)
			kitty.x = 0;
		if (kitty.x < 0)
			kitty.x = SCREEN_WIDTH - kitty.width;
		// Co môt animation delay
		if (++kitty.animcount > kitty.animdelay)
		{
			// Tai lập bô đêm
			kitty.animcount = 0;
			// Animation sprite
			if (++kitty.curframe > kitty.lastframe)
				kitty.curframe = 0;
		}
	}
	// Băt đâu render
	if (d3ddev->BeginScene())
	{
		// Xoa toan bô man hinh
		d3ddev->StretchRect(back, NULL, backbuffer, NULL,
			D3DTEXF_NONE);
		// Băt đâu Sprite Handler
		sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);
		// Tao vector đê cập nhật vị tri cua sprite
		D3DXVECTOR3 position((float)kitty.x,(float)kitty.y, 0);
		// vẽ sprite
		sprite_handler->Draw(
			kitty_image[kitty.curframe],
			NULL,
			NULL,
			&position,
			D3DCOLOR_XRGB(255, 255, 255));
		// Dừng vẽ
		sprite_handler->End();
		// Dừng render
		d3ddev->EndScene();
	}
	// Thê hiên back buffer lên man hinh
	d3ddev->Present(NULL, NULL, NULL, NULL);
	// Kiêm tra phim escape (đê kêt thuc chương trinh)
	if (KEY_DOWN(VK_ESCAPE))
		PostMessage(hwnd, WM_DESTROY, 0, 0);
}
// Giai phong bô nhơ va dọn dẹp trươc khi kêt thuc game
void Game_End(HWND hwnd)
{
	int n;
	for (n = 0; n < 6; n++)
		if (kitty_image[n] != NULL)
			kitty_image[n]->Release();
	if (back != NULL)
		back->Release();
	if (sprite_handler != NULL)
		sprite_handler->Release();
}