#pragma once
// Anim_Sprite program header file
#ifndef _GAME_H
#define _GAME_H
#include <d3d9.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h> // in case it isn't included 

#include "dxgraphics.h"
//application title
#define APPTITLE "Anim_Sprite"
//screen setup
#define FULLSCREEN 0 //1 = fullscreen, 0 = windowed
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
//macros to read the keyboard asynchronously
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) &0x8000) ? 1 : 0)
#define KEY_UP(vk_code)((GetAsyncKeyState(vk_code) & 0x8000)? 1 : 0)
//function prototypes
int Game_Init(HWND);
void Game_Run(HWND);
void Game_End(HWND);

//sprite structure 
//đủ để đưa vào một hàm riêng, 
//truyền vào một biến SPRITE để update nhiều sprite trong một game.
typedef struct {
	//update tọa độ x, y của sprite suốt quá trình update frame
	int x, y;
	int width, height;
	int movex, movey;

	/*để giữ giá trị frame hiện tai của chuyển động,
		curframe được update liên tục trong vòng lăp game, 
			và khi no đat đến giá trị lastframe, 
				no sẽ được gán lăp lai giá trị 0, cứ thế lăp liên tục*/
	int curframe, lastframe;

	/*dùng để làm việc với hai biến vừa rồi 
		nhằm giới han thời gian hiển thị cho mỗi frame.
		Nếu frame được update liên tục ơ mỗi vòng lăp game,
			chuyển động sẽ diễn ra quá nhanh.
		Ta sẽ không muốn làm cho vòng lăp game châm lai 
			chỉ để giữ cho chuyển động đo như y muốn,
				do đo giai pháp là dừng việc update curframe một khoang thời gian.*/
	int animdelay, animcount;
} SPRITE;
#endif