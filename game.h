#pragma once

//ゲーム全体のヘッダファイル

//ヘッダーファイルの読み込み
#include "DxLib.h"		//DxLibを使う時は必要

//マクロ定義
#define GAME_TITLE  "ゲームタイトル"	//ゲームタイトル
#define GAME_WIDTH  1280				//ゲーム画面の幅
#define GAME_HEIGHT 720					//ゲーム画面の高さ
#define GAME_COLOR  32					//ゲーム画面の色域

#define GAME_ICON_ID  333				//ゲームのICOのID

#define GAME_WINDOW_BAR  0				//ウィンドウバーの種類

#define GAME_DEBUG  TRUE				//デバッグモード

//列挙型
enum GAME_SCENE {
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_PLAY_2,
	GAME_SCENE_PLAY_3,
	GAME_SCENE_END,
	GAME_SCENE_GAMEOVER,
	GAME_SCENE_CHANGE
};	//ゲームのシーン