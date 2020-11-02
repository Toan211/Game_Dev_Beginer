#include "game.h"
LPDIRECT3DTEXTURE9 caveman_image;
SPRITE caveman;
LPDIRECT3DSURFACE9 back;


//D3DXSprite chỉ là người điều khiển sprite co chứa những hàm 
//	để vẽ sprites từ texture(với những thông tin về transformations)
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
	/*Phân việc của no là gắn sprite handler 
		vào đối tượng Direct3D chinh và device
		để chúng biết làm thế nào để vẽ sprite vào trong back buffer.*/
	result = D3DXCreateSprite(d3ddev, &sprite_handler);

	if (result != D3D_OK)
		return 0;
	// tai sprite animation
	for (n = 0; n < 6; n++)
	{
		TCHAR Tiles[] = _T("caveman.jpg");
		// Tai texture vơi mau hồng la mau trong suôt
		caveman_image = LoadTexture(Tiles,
			D3DCOLOR_XRGB(0, 0, 0));
		if (caveman_image == NULL)
			return 0;
	}
	TCHAR Back[] = _T("background.bmp"); //lmao fking bug (cant not convert from const char* to char*)
	// tai hinh nên
	back = LoadSurface(Back, NULL);
	// Khơi tao thông sô cho sprite
	caveman.x = 180;
	caveman.y = 180;
	caveman.width = 133;
	caveman.height = 132;
	caveman.curframe = 1;
	caveman.lastframe = 11;
	caveman.animdelay = 3;
	caveman.animcount = 0;
	caveman.movex = 5;
	caveman.movey = 0;
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
		caveman.x += caveman.movex;
		caveman.y += caveman.movey;
		// Đong goi sprite vao trong cac khung man hinh
		if (caveman.x > SCREEN_WIDTH - caveman.width)
			caveman.x = 0;
		if (caveman.x < 0)
			caveman.x = SCREEN_WIDTH - caveman.width;
		// Co môt animation delay
		if (++caveman.animcount > caveman.animdelay)
		{
			// Tai lập bô đêm
			caveman.animcount = 0;
			// Animation sprite
			if (++caveman.curframe > caveman.lastframe)
				caveman.curframe = 0;
		}
	}
	// Băt đâu render
	if (d3ddev->BeginScene())
	{
		// Xoa toan bô man hinh
		d3ddev->StretchRect(back, NULL, backbuffer, NULL,
			D3DTEXF_NONE);

		// Băt đâu Sprite Handler
		/*Tham số flags được yêu câu và thường là D3DXSPRITE_ALPHABLEND,
			nghĩa là sprite được vẽ hỗ trợ trong suốt*/
		sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

		// Tao vector đê cập nhật vị tri cua sprite
		/*D3DXVECTOR3 là một dữ liệu mới 
			được phát sinh với DirecX 9.0b, bao gồm ba biến là : x, y và z. 
		x và y, ta chỉ cân dùg khi di chuyển sprite trên màn hình 2D.*/
		D3DXVECTOR3 position((float)caveman.x,(float)caveman.y, 0);
		// vẽ sprite

		//pTexture đăc ta texture được sử dụng cho hình anh nguồn của sprite
	/*pSrcRect vì ta co thể sử dụng no để cắt “tile” khỏi anh nguồn và
		lưu trữ tất ca những frame animation của sprite trong một tâp tin anh */

		//*pCenter đăc ta điểm tâm được dùng để xoay.
		//pPosition đăc ta cho vị tri của sprite, và no thường là nơi để đăt giá trị x, y.
		/*Color Tham số cuối cùng mô ta cho màu thay thế
			sẽ được áp dụng khi vẽ sprite(không anh hương khi vẽ trong suốt).*/

			// Thiêt đăt kich thươc cho từng tile nguồn
		RECT srcRect;
		int columns = 4;
		srcRect.left = (caveman.curframe % columns) *caveman.width;
		srcRect.top = (caveman.curframe / columns) *caveman.height;
		srcRect.right = srcRect.left + caveman.width;
		srcRect.bottom = srcRect.top + caveman.height;

		// vẽ sprite
		sprite_handler->Draw(
			caveman_image,
			&srcRect,
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
	if (caveman_image != NULL)
		caveman_image->Release();
	if (back != NULL)
		back->Release();
	if (sprite_handler != NULL)
		sprite_handler->Release();
}