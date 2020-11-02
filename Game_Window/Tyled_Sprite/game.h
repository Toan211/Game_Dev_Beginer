#pragma once
#ifdef _GAME_H
#endif _GAME_H
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <tchar.h> // in case it isn't included 

#include "dxgraphics.h"
// tên cua ưng dung
#define APPTITLE "Trans_Sprite"
// Thiêt lập man hinh
#define FULLSCREEN 0 // 0 = windowed, 1 = fullscreen
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
// Lênh đê đọc keyboard
#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) &0x8000) ? 1 : 0)
#define KEY_UP(vk_code)((GetAsyncKeyState(vk_code) *0x8000) ? 1 : 0)
// prototypes hàm
int Game_Init(HWND);
void Game_Run(HWND);
void Game_End(HWND);
// structure sprite
typedef struct
{
	int x, y;
	int width, height;
	int movex, movey;
	int curframe, lastframe;
	int animdelay, animcount;
} SPRITE;

