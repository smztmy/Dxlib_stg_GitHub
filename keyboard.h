//�L�[�{�[�h�̃w�b�_�t�@�C��

#pragma once

//�w�b�_�t�@�C���ǂݍ���
#include "game.h"

//�}�N����`
#define KEY_KIND_MAX	256	//�擾����L�[�̎��

//�L�[�{�[�h�\����
struct KEYBOARD
{
	char TempKeyStyle[ KEY_KIND_MAX ];		//���͏�Ԃ��Ǘ�
	int AllKeyState[KEY_KIND_MAX];			//���ׂẴL�[�̓��͎��Ԃ��Ǘ�
	int OldAllKeyState[KEY_KIND_MAX];		//�ȑO�̂��ׂẴL�[�̓��͎��Ԃ��Ǘ�
};

//�O���̃O���[�o���ϐ�
extern KEYBOARD keyboard;

//�v���g�^�C�v�錾
extern VOID AllKeyUpdate(VOID);								//���ׂẴL�[�̏�Ԃ��擾����
extern BOOL KeyDown(int KEY_INPUT);							//����̃L�[�������Ă��邩
extern BOOL KeyUp(int KEY_INPUT);							//����̃L�[�������Ă��邩
extern BOOL KeyClick(int KEY_INPUT);						//����̃L�[���N���b�N������
extern BOOL KeyDownKeep(int KEY_INPUT_, int millSec);		//����̃L�[���w��̃~���b�����������Ă��邩