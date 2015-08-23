/// @file
//main.cpp 村田昂平
#include "include.h"

GAME_STATE game_state;

/**
 *	@brief	WinMain関数 以下省略
 *	@author	村田昂平
 */
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR ,INT )
{
	GAME_MANARGER game_manager;
	game_manager.Init();
	game_manager.MainLoop();
	game_manager.End();
	return 0;
	//return msg.wParam;//本来はメッセージの終了コードをシステムに返す
}