/// @file
//main.cpp ���c�V��
#include "include.h"

GAME_STATE game_state;

/**
 *	@brief	WinMain�֐� �ȉ��ȗ�
 *	@author	���c�V��
 */
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR ,INT )
{
	GAME_MANARGER game_manager;
	game_manager.Init();
	game_manager.MainLoop();
	game_manager.End();
	return 0;
	//return msg.wParam;//�{���̓��b�Z�[�W�̏I���R�[�h���V�X�e���ɕԂ�
}