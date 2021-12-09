//�w�b�_�t�@�C���ǂݍ���
#include "game.h"		//�Q�[���S�̂̃w�b�_�t�@�C��
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���

#include "mouse.h"		//�}�E�X�̏���
#include "shape.h"		//�}�`�̏���

#include <math.h>		//���w

//�}�N����`
#define TAMA_DIV_MAX	4	//�e�̉摜�̍ő吔
#define TAMA_MAX		20	//�e�̑���
#define TEKI_KIND		5	//�G�̎��
#define TEKI_MAX		50	//�G�̐�
//#define SHFTNUM			15
#define TEKI_TAMA_DIV_MAX	4	// �G�̃V���b�g�̍ő吔
#define TEKI_TAMA_MAX	500		//�G�̒e�̑���



//�\���̂̒�`

//�摜�̍\����
struct IMAGE
{
	int handle = -1;	//�摜�̃n���h��(�Ǘ��ԍ�)
	char path[255];		//�摜�̏ꏊ(�p�X)

	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����

	BOOL IsDraw = FALSE;//�摜���`��ł���H
};

//�L�����N�^�̍\����
struct CHARACTOR
{
	IMAGE img;			//�摜�\����
	int speed = 8;		//�ړ����x	
	RECT coll;			//�����蔻��̗̈�(�l�p)
};

//����̍\����
struct MOVIE
{
	int handle = -1;	//����̃n���h��
	char path[255];		//����̃p�X

	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����

	int Volume = 255;	//�{�����[��(�ŏ�)0�`255(�ő�)�@
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;	//���y�̃n���h��
	char path[255];		//���y�̃p�X

	int Volume = -1;	//�{�����[���iMIN 0 �` 255 MAX�j
	int playType = -1;	//BGM or SE
};

//�e�̍\����
struct TAMA
{
	int handle[TAMA_DIV_MAX];	//�摜�̃n���h��
	char path[255];	//�摜�̃p�X

	int DivTate;	//�������i�c�j
	int DivYoko;	//�������i���j
	int DivMax;		//��������

	int AnimeCnt = 0;		//�A�j���[�V�����J�E���^
	int AnimeCntMax = 0;	//�A�j���[�V�����J�E���^MAX

	int NowIndex = 0;		//���݂̉摜�̗v�f��

	int StartX;				//X�ŏ��ʒu
	int StartY;				//Y�ŏ��ʒu

	float radius;			//���a
	float degree;			//�p�x

	int x;					//X�ʒu
	int y;					//Y�ʒu
	int width;				//��
	int height;				//����

	int Speed;				//�e�̑���

	RECT coll;				//�����蔻��i��`�j

	BOOL IsDraw = FALSE;	//�`��ł���
};

//// �G�̃V���b�g�̃f�[�^�\���̌^
struct TEKI_TAMA
{
	int handle[TEKI_TAMA_DIV_MAX];	//�摜�̃n���h��
	char path[255];	//�摜�̃p�X

	int DivTate;	//�������i�c�j
	int DivYoko;	//�������i���j
	int DivMax;		//��������

	int AnimeCnt = 0;		//�A�j���[�V�����J�E���^
	int AnimeCntMax = 0;	//�A�j���[�V�����J�E���^MAX

	int NowIndex = 0;		//���݂̉摜�̗v�f��

	int StartX;				//X�ŏ��ʒu
	int StartY;				//Y�ŏ��ʒu

	float radius;			//���a
	float degree;			//�p�x

	int x;					//X�ʒu
	int y;					//Y�ʒu
	int width;				//��
	int height;				//����

	int Speed;				//�e�̑���

	RECT coll;				//�����蔻��i��`�j

	BOOL IsDraw = FALSE;	//�`��ł���
};


//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;	//�O��̃Q�[���̃V�[��
GAME_SCENE NextGameScene;	//���̃Q�[���̃V�[��

//�摜��ǂݍ���
IMAGE TitleLogo;	//�^�C�g�����S
IMAGE TitleEnter;	//�G���^�[�L�[�������Ă�
IMAGE EndClear;		//���S
IMAGE EndEnter;		//�G���^�[�L�[
IMAGE GameOverLogo;		//���S
IMAGE GameOverEnter;		//�G���^�[�L�[
IMAGE TitleBackground;
IMAGE EndBackground;
IMAGE GameOverBackground;

//���y
AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO Play2BGM;
AUDIO GameOverBGM;
AUDIO EndBGM;

AUDIO PlayerSE;
AUDIO EnemySE;
AUDIO CollisionSE;

//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;		//�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;		//�t�F�[�h�C��

int fadeTimeMill = 2000;					//�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//�~���b���t���[���b�ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;				//�����l
int fadeOutCnt = fadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCntMax = fadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;	//�����l
int fadeInCnt = fadeInCntInit;		//�t�F�[�h�A�E�g�̃J�E���^
int fadeInCntMax = fadeTimeMax;		//�t�F�[�h�A�E�g�̃J�E���^MAX

//�e�̍\���̕ϐ�
struct TAMA tama_moto;	//��
struct TAMA tama[TAMA_MAX];	//���ۂɎg��
struct TEKI_TAMA teki_tama_moto;
struct TEKI_TAMA teki_tama[TEKI_TAMA_MAX];

//�e�̔��˃J�E���^
int tamaShotCnt = 0;
int tamaShotCntMax = 5;
int tekitamaShotCnt = 0;
int tekitamaShotCntMax = 5;

//�v���C���[
CHARACTOR player;
CHARACTOR enemy;

//�w�i�摜
IMAGE back[2];			//�w�i�͂Q�̉摜

//�G�f�[�^(��)
CHARACTOR  teki_moto[TEKI_KIND];

//���ۂ̓G�f�[�^
CHARACTOR teki[TEKI_MAX];

char tekiPath[TEKI_KIND][255] =
{
	(".\\Image\\���C���[_14.png"),
	(".\\Image\\teki_1.png"),
	(".\\Image\\teki_2.png"),
	(".\\Image\\teki_3.png"),
	(".\\Image\\teki_4.png"),
};

//�G���o�Ă���J�E���g
int TekiAddCnt = 0;
int TekiAddCntMAX = 60;	//60FPS��1��E�E�E1�b��1��

//�Q�[���̃X�R�A
int Score = 0;

//PushEnter�̓_��
int PushEnterCnt = 0;			//�J�E���^
int EndEnterCnt = 0;			//�J�E���^
int GameOverEnterCnt = 0;			//�J�E���^
int PushEnterCntMax = 60;		//�J�E���^MAX�l
int EndEnterCntMax = 60;		//�J�E���^MAX�l
int GameOverEnterCntMax = 60;		//�J�E���^MAX�l
BOOL PushEnterBrink = FALSE;	//�_�ł��Ă��邩
BOOL EndEnterBrink = FALSE;		//�_�ł��Ă��邩
BOOL GameOverEnterBrink = FALSE;		//�_�ł��Ă��邩

//�v���g�^�C�v�錾
VOID Title(VOID);		//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g�����(����)
VOID TitleDraw(VOID);	//�^�C�g�����(�`��)

VOID Play(VOID);		//�v���C���
VOID PlayProc(VOID);	//�v���C���(����)
VOID PlayDraw(VOID);	//�v���C���(�`��)

VOID Play2(VOID);		//�v���C���
VOID Play2Proc(VOID);	//�v���C���(����)
VOID Play2Draw(VOID);	//�v���C���(�`��)

VOID End(VOID);			//�G���h���
VOID EndProc(VOID);		//�G���h���(����)
VOID EndDraw(VOID);		//�G���h���(�`��)

VOID GameOver(VOID);			//�G���h���
VOID GameOverProc(VOID);		//�G���h���(����)
VOID GameOverDraw(VOID);		//�G���h���(�`��)


VOID Change(VOID);		//�؂�ւ����
VOID ChangeProc(VOID);	//�؂�ւ����(����)
VOID ChangeDraw(VOID);	//�؂�ւ����(�`��)

VOID ChangeScene(GAME_SCENE scene);	//�V�[���؂�ւ�

VOID CollUpdatePlayer(CHARACTOR* chara);	//�����蔻��̗̈���X�V
VOID CollUpdate(CHARACTOR* chara);			//�����蔻��
VOID CollUpdateTama(TAMA* tama);			//�e�̓����蔻��̍X�V
VOID CollUpdateTekiTama(TEKI_TAMA* teki_tama);			//�e�̓����蔻��̍X�V
VOID CollUpdateTeki(CHARACTOR* chara);		//�G�̓����蔻��

BOOL OnCollRect(RECT a, RECT b);			//��`�Ƌ�`�̓����蔻��

BOOL GameLoad(VOID);	//�Q�[���̃f�[�^��ǂݍ���

BOOL LoadImageMem(IMAGE* image, const char* path);							//�Q�[���̉摜��ǂݍ���
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	//�Q�[���̉��y��ǂݍ���

VOID GameInit(VOID);	//�Q�[���̃f�[�^�̏�����

VOID DrawTama(TAMA* tama);					//�e�̕`��
VOID ShotTama(TAMA* tama, float deg);		//�e�̔���
VOID DrawTekiTama(TEKI_TAMA* teki_tama);					//�e�̕`��
VOID ShotTekiTama(TEKI_TAMA* teki_tama, float deg);			//�e�̔���

BOOL LoadImageDivMem(int* handle, const char* path, int bunkatuYoko, int bunkatuTate);	//�Q�[���̉摜�̕����ǂݍ���

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);								//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);						//�E�B���h�E�̃^�C�g���̕���
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(0, 0, 0);					//�f�t�H���g�̔w�i�̐F
	SetWindowIconID(GAME_ICON_ID);						//�A�C�R���t�@�C����Ǎ�
	SetWindowStyleMode(GAME_WINDOW_BAR);				//�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);								//�f�B�X�v���C�̐���������L���ɂ���
	SetAlwaysRunFlag(TRUE);								//�E�B���h�E�������ƃA�N�e�B�u�ɂ���

	//�c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		// �G���[���N�����璼���ɏI��
		return -1;
	}

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�ŏ��̃V�[���́A�^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����

	//�Q�[���ǂݍ���
	if (!GameLoad())
	{
		//�f�[�^�̓ǂݍ��݂Ɏ��s�����Ƃ�
		DxLib_End();	//DxLib�I��
		return -1;		//�ُ�I��
	}

	//�Q�[���̏�����
	GameInit();

	//�������[�v
	while (1)
	{
		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; }	//��ʂ���������

		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//�}�E�X���͂̍X�V
		MouseUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();				//�v���C���
			break;
		case GAME_SCENE_PLAY_2:
			Play2();				//�v���C���
			break;
		case GAME_SCENE_END:
			End();				//�G���h���
			break;
		case GAME_SCENE_GAMEOVER:
			GameOver();				//�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();			//�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		//FPS�l��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWait();

		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��
	}

	//�ǂݍ��񂾉摜�����
	for (int i = 0; i < TAMA_DIV_MAX; i++) { DeleteGraph(tama_moto.handle[i]); }
	for (int i = 0; i < TEKI_TAMA_DIV_MAX; i++) { DeleteGraph(teki_tama_moto.handle[i]); }
	
	//�v���C���[���
	DeleteGraph(player.img.handle);

	//�w�i�摜���
	DeleteGraph(back[0].handle);
	DeleteGraph(back[1].handle);

	for (int i = 0; i < TEKI_KIND; i++)
	{
		DeleteGraph(teki_moto[i].img.handle);
	}

	DeleteSoundMem(PlayerSE.handle);
	DeleteSoundMem(EnemySE.handle);
	DeleteSoundMem(CollisionSE.handle);

	//�c�w���C�u�����g�p�̏I������
	DxLib_End();

	return 0;	// �\�t�g�̏I�� 
}




/// <summary>
/// �Q�[���̃f�[�^��ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE / �ǂݍ��߂Ȃ�������FALSE</returns>
BOOL GameLoad(VOID)
{
	//�e�̕�������ݒ�
	tama_moto.DivYoko = 4;
	tama_moto.DivTate = 1;
	teki_tama_moto.DivYoko = 4;
	teki_tama_moto.DivTate = 1;

	//�e�̃p�X
	strcpyDx(tama_moto.path, ".\\Image\\dia_blue.png");
	strcpyDx(teki_tama_moto.path, ".\\Image\\dia_red.png");

	//�摜�𕪊����ēǂݍ���
	if (LoadImageDivMem(&tama_moto.handle[0], tama_moto.path, tama_moto.DivYoko, tama_moto.DivTate) == FALSE) { return FALSE; }
	if (LoadImageDivMem(&teki_tama_moto.handle[0], teki_tama_moto.path, teki_tama_moto.DivYoko, teki_tama_moto.DivTate) == FALSE) { return FALSE; }

	//���ƍ������擾
	GetGraphSize(tama_moto.handle[0], &tama_moto.width, &tama_moto.height);
	GetGraphSize(teki_tama_moto.handle[0], &teki_tama_moto.width, &teki_tama_moto.height);

	//�ʒu��ݒ�
	tama_moto.x = GAME_WIDTH / 2 - tama_moto.width / 2;	//��������
	tama_moto.y = GAME_HEIGHT- tama_moto.height;		//��ʉ�
	teki_tama_moto.x = GAME_WIDTH / 2 - teki_tama_moto.width / 2;	//��������
	teki_tama_moto.y = GAME_HEIGHT - teki_tama_moto.height;		//��ʉ�

	tama_moto.Speed = 15;	//���x
	teki_tama_moto.Speed = 15;	//���x

	tama_moto.AnimeCntMax = 50;
	teki_tama_moto.AnimeCntMax = 50;

	//�����蔻���ݒ�
	CollUpdateTama(&tama_moto);
	CollUpdateTekiTama(&teki_tama_moto);

	//�摜��\�����Ȃ�
	tama_moto.IsDraw = FALSE;
	teki_tama_moto.IsDraw = FALSE;

	//�S�Ă̒e�ɏ����R�s�[
	for (int i = 0; i < TAMA_MAX; i++)
	{
		tama[i] = tama_moto;
	}

	for (int i = 0; i < TEKI_TAMA_MAX; i++)
	{
		teki_tama[i] = teki_tama_moto;
	}

	//�v���C���[�̉摜��ǂݍ���
	if (LoadImageMem(&player.img, ".\\Image\\player_1.png") == FALSE) { return FALSE; }
	player.img.x = GAME_WIDTH / 2 - player.img.width;
	player.img.y = GAME_HEIGHT / 2 - player.img.height;
	CollUpdatePlayer(&player);		//�����蔻��X�V
	player.img.IsDraw = TRUE;		//�`�悷��

	//�w�i�̉摜��ǂݍ��݇@
	if (LoadImageMem(&back[0], ".\\Image\\�v���C���_1.png") == FALSE) { return FALSE; }
	back[0].x = 0;
	back[0].y = -back[0].height;		//�摜�̍������A�ʒu���グ��
	back[0].IsDraw = TRUE;				//�`�悷��

	//�w�i�̉摜��ǂݍ��݇A
	if (LoadImageMem(&back[1], ".\\Image\\�v���C���_2.png") == FALSE) { return FALSE; }
	back[1].x = 0;
	back[1].y = 0;		//�摜�̍������A�ʒu���グ��
	back[1].IsDraw = TRUE;				//�`�悷��

	//�G�̉摜��ǂݍ���
	for (int i = 0; i < TEKI_KIND; i++)
	{
		if (LoadImageMem(&teki_moto[i].img, tekiPath[i]) == FALSE) { return FALSE; }
		teki_moto[i].img.x = GAME_WIDTH / 2 - teki_moto[i].img.width;
		teki_moto[i].img.y = -teki_moto[i].img.height;
		CollUpdatePlayer(&teki_moto[i]);		//�����蔻��X�V
		teki_moto[i].img.IsDraw = FALSE;		//�`�悷��
	}

	//���S��ǂݍ���
	if (!LoadImageMem(&TitleLogo, ".\\Image\\�^�C�g�����S.png")) { return FALSE; }
	if (!LoadImageMem(&TitleEnter, ".\\Image\\��Game Start��.png")) { return FALSE; }
	if (!LoadImageMem(&EndClear, ".\\Image\\End Game.png")) { return FALSE; }
	if (!LoadImageMem(&EndEnter, ".\\Image\\End_Push Enter.png")) { return FALSE; }
	if (!LoadImageMem(&GameOverLogo, ".\\Image\\�Q�[���I�[�o�[�I.png")) { return FALSE; }
	if (!LoadImageMem(&GameOverEnter, ".\\Image\\GameOver_Push Enter.png")) { return FALSE; }

	//�w�i��ǂݍ���
	if (!LoadImageMem(&TitleBackground, ".\\Image\\�^�C�g��.png")) { return FALSE; }
	if (!LoadImageMem(&EndBackground, ".\\Image\\�G���h.png")) { return FALSE; }
	if (!LoadImageMem(&GameOverBackground, ".\\Image\\�Q�[���I�[�o�[.png")) { return FALSE; }

	//���y��ǂݍ���
	if (!LoadAudio(&TitleBGM, ".\\Audio\\�^�C�g��.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&PlayBGM, ".\\Audio\\�v���C.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&Play2BGM, ".\\Audio\\�v���C.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&EndBGM, ".\\Audio\\�G���h.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&GameOverBGM, ".\\Audio\\�Q�[���I�[�o�[.mp3", 255, DX_PLAYTYPE_LOOP)) { return FALSE; }

	//���ʉ�
	if (!LoadAudio(&PlayerSE, ".\\Audio\\�V���b�g.mp3", 255, DX_PLAYTYPE_BACK)) { return FALSE; }
	if (!LoadAudio(&EnemySE, ".\\Audio\\�V���b�g����.mp3", 255, DX_PLAYTYPE_BACK)) { return FALSE; }
	if (!LoadAudio(&CollisionSE, ".\\Audio\\�ՓˁE�Ռ��i�S�j01.mp3", 255, DX_PLAYTYPE_BACK)) { return FALSE; }

	return TRUE;	//�S�ēǂݍ��݂��I
}

/// <summary>
/// �摜�𕪊����ă������ɓǂݍ���
/// </summary>
/// <param name="handle">�n���h���z��̐擪�A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <param name="bunkatuYoko">�������鎞�̉��̐�</param>
/// <param name="bunkatuTate">�������鎞�̏c�̐�</param>
/// <returns></returns>
BOOL LoadImageDivMem(int* handle, const char* path, int bunkatuYoko, int bunkatuTate)
{
	//�e�̓ǂݍ���
	int IsTamaLoad = -1;	//�摜���ǂݍ��݂����H

	//�ꎞ�I�ɉ摜�̃n���h����p�ӂ���
	int TamaHandle = LoadGraph(path);

	//�ǂݍ��݃G���[
	if (TamaHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//�E�B���h�E�n���h��
			path,					//�{��
			"�摜�ǂݍ��݃G���[",	//�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	int TamaWidth = -1;		//��
	int TamaHeight = -1;	//����
	GetGraphSize(TamaHandle, &TamaWidth, &TamaHeight);

	//�������ēǂݍ���
	IsTamaLoad = LoadDivGraph(
		path,												//�摜�̃p�X
		TAMA_DIV_MAX,										//��������
		bunkatuYoko, bunkatuTate,							//���̕���,�c�̕���
		TamaWidth / bunkatuYoko, TamaHeight / bunkatuTate,	//�摜1���̕�,����
		handle												//�A���ŊǗ�����z��̐擪�A�h���X
	);

	//�����G���[
	if (IsTamaLoad == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//�E�B���h�E�n���h��
			path,					//�{��
			"�摜�����G���[",		//�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�ꎞ�I�ɓǂݍ��񂾃n���h�������
	DeleteGraph(TamaHandle);

	return TRUE;
}

/// <summary>
/// �摜���������ɓǂݍ���
/// </summary>
/// <param name="image">�摜�\���̂̃A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	//�S�[���̉摜��ǂݍ���
	strcpyDx(image->path, path);	//�p�X�̃R�s�[
	image->handle = LoadGraph(image->path);	//�摜�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			image->path,			//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(image->handle, &image->width, &image->height);

	//�ǂݍ��߂�
	return TRUE;
}

/// <summary>
/// ���y���������ɓǂݍ���
/// </summary>
/// <param name="audio">Audio�\���̕ϐ��̃A�h���X</param>
/// <param name="path">Audio�̉��y�p�X</param>
/// <param name="volume">�{�����[��</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
{
	//���y�̓ǂݍ���
	strcpyDx(audio->path, path);					//�p�X�̃R�s�[
	audio->handle = LoadSoundMem(audio->path);		//���y�̓ǂݍ���

	//���y���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			audio->path,			//���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//���̑��̐ݒ�
	audio->Volume = volume;
	audio->playType = playType;

	return TRUE;
}

/// <summary>
/// �Q�[���f�[�^��������
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//�Q�[���X�R�A�̏�����
	Score = 0;

	//�v���C���[�̏�����
	player.img.x = GAME_WIDTH / 2 - player.img.width;
	player.img.y = GAME_HEIGHT / 2 - player.img.height;
	CollUpdatePlayer(&player);							//�����蔻��X�V
	player.img.IsDraw = TRUE;							//�`�悷��

	//�w�i�̉摜��ǂݍ��݇@
	back[0].x = -back[0].width;
	back[0].y = 0;		//�摜�̍������A�ʒu���グ��
	back[0].IsDraw = TRUE;				//�`�悷��

	//�w�i�̉摜��ǂݍ��݇A
	back[1].x = 0;
	back[1].y = 0;		//�摜�̍������A�ʒu���グ��
	back[1].IsDraw = TRUE;				//�`�悷��

	//�G�̉摜��ǂݍ���
	for (int i = 0; i < TEKI_KIND; i++)
	{
		teki_moto[i].img.x = GAME_WIDTH / 2 - teki_moto[i].img.width;
		teki_moto[i].img.y = -teki_moto[i].img.height;
		CollUpdatePlayer(&teki_moto[i]);		//�����蔻��X�V
		teki_moto[i].img.IsDraw = FALSE;		//�`�悷��
	}

	//�^�C�g�����S�̈ʒu�����߂�
	TitleLogo.x = GAME_WIDTH / 2 - TitleLogo.width / 2;	//�������낦
	TitleLogo.y = 100;

	//PushEnter�̈ʒu�����߂�
	TitleEnter.x = GAME_WIDTH / 2.3 - TitleLogo.width / 2.3;	//��������
	TitleEnter.y = GAME_HEIGHT - TitleEnter.height - 100;
	EndEnter.x = GAME_WIDTH / 2.2 - TitleLogo.width / 2.2;	//��������
	EndEnter.y = GAME_HEIGHT - EndEnter.height - 100;

	//pushEnter�̓_��
	PushEnterCnt = 0;
	EndEnterCnt = 0;
	PushEnterCntMax = 60;
	EndEnterCntMax = 60;
	PushEnterBrink = FALSE;
	EndEnterBrink = FALSE;

	//�N���A���S�̈ʒu�����߂�
	EndClear.x = GAME_WIDTH / 2 - EndClear.width / 2;	//��������
	EndClear.y = 100;	//��������


}

/// <summary>
/// �V�[����؂�ւ���֐�
/// </summary>
/// <param name="scene">�V�[��</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//�V�[����؂�ւ�
	IsFadeIn = FALSE;	//�t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;	//�t�F�[�h�A�E�g����

	return;
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//����
	TitleDraw();	//�`��

	return;
}

/// <summary>
/// �^�C�g����ʂ̏���
/// </summary>
VOID TitleProc(VOID)
{

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy
		//BGM���~�߂�
		StopSoundMem(TitleBGM.handle);

		//�Q�[���̏�����
		GameInit();

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);

		//�}�E�X��`�悵�Ȃ�
		SetMouseDispFlag(FALSE);

		return;
	}

	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(TitleBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(TitleBGM.handle, TitleBGM.playType);
	}

	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
VOID TitleDraw(VOID)
{
	DrawGraph(TitleBackground.x, TitleBackground.y, TitleBackground.handle, TRUE);
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, TitleBackground.handle, TRUE);

	//�^�C�g�����S�̕`��
	DrawGraph(TitleLogo.x, TitleLogo.y, TitleLogo.handle, TRUE);

	//MAX�l�܂ő҂�
	if (PushEnterCnt < PushEnterCntMax) { PushEnterCnt++; }
	else
	{
		if (PushEnterBrink == TRUE)PushEnterBrink = FALSE;
		else if (PushEnterBrink == FALSE)PushEnterBrink = TRUE;

		PushEnterCnt = 0;	//�J�E���^��������
	}


	//PushEnter�̓_��
	if (PushEnterBrink == TRUE)
	{
		//PushEnter�̕`��
		DrawGraph(TitleEnter.x, TitleEnter.y, TitleEnter.handle, TRUE);
	}

	//DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �e�̕`��
/// </summary>
/// <param name="tama">�e�̍\����</param>
VOID DrawTama(TAMA* tama)
{
	//�e�̕`�悪�ł���Ƃ�
	if (tama->IsDraw == TRUE)
	{
		//�e�̕`��
		DrawGraph(tama->x, tama->y, tama->handle[tama->NowIndex], TRUE);

		if (tama->AnimeCnt < tama->AnimeCntMax)
		{
			tama->AnimeCnt++;
		}
		else
		{
			//�e�̓Y�����e�̕������̍ő������������
			if (tama->NowIndex < TAMA_DIV_MAX - 1)
			{
				tama->NowIndex++;	//���̉摜
			}
			else
			{
				tama->NowIndex = 0;	//�ŏ��ɂ��߂�
			}

			tama->AnimeCnt = 0;
		}
	}
}

VOID DrawTekiTama(TEKI_TAMA* teki_tama)
{
	//�e�̕`�悪�ł���Ƃ�
	if (teki_tama->IsDraw == TRUE)
	{
		//�e�̕`��
		DrawGraph(teki_tama->x, teki_tama->y, teki_tama->handle[teki_tama->NowIndex], TRUE);

		if (teki_tama->AnimeCnt < teki_tama->AnimeCntMax)
		{
			teki_tama->AnimeCnt++;
		}
		else
		{
			//�e�̓Y�����e�̕������̍ő������������
			if (teki_tama->NowIndex < TEKI_TAMA_DIV_MAX - 1)
			{
				teki_tama->NowIndex++;	//���̉摜
			}
			else
			{
				teki_tama->NowIndex = 0;	//�ŏ��ɂ��߂�
			}

			teki_tama->AnimeCnt = 0;
		}
	}
}

/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();	//����
	PlayDraw();	//�`��

	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
VOID PlayProc(VOID)
{
	//if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	if (teki[0].img.y > GAME_HEIGHT == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy
		//BGM���~�߂�
		StopSoundMem(PlayBGM.handle);

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_GAMEOVER);

		//�}�E�X��`�悷��
		SetMouseDispFlag(TRUE);

		return;
	}
	else if (OnCollRect(player.coll,teki->coll) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy
		//BGM���~�߂�
		StopSoundMem(PlayBGM.handle);

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_GAMEOVER);

		//�}�E�X��`�悷��
		SetMouseDispFlag(TRUE);

		if (CheckSoundMem(CollisionSE.handle) == 0)
		{
			PlaySoundMem(CollisionSE.handle, CollisionSE.playType);
		}

		return;
	}
	else if(Score>=10000)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy
		//BGM���~�߂�
		StopSoundMem(PlayBGM.handle);

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY_2);

		//�}�E�X��`�悷��
		SetMouseDispFlag(TRUE);

		return;
	}
	

	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(PlayBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(PlayBGM.handle, PlayBGM.playType);
	}

	
	//�v���C���[�𑀍�
	if (KeyDown(KEY_INPUT_A) == TRUE)
	{
		if (player.img.x - player.speed >= 0)
		{
			player.img.x -= player.speed;
		}
	}

	if (KeyDown(KEY_INPUT_D) == TRUE)
	{
		if (player.img.x + player.img.width + player.speed <= GAME_WIDTH)
		{
			player.img.x += player.speed;
		}
	}
	
	if (KeyDown(KEY_INPUT_W) == TRUE)
	{
		if (player.img.y - player.speed >= 0)
		{
			player.img.y -= player.speed;
		}
	}

	if (KeyDown(KEY_INPUT_S) == TRUE)
	{
		if (player.img.y + player.img.height + player.speed <= GAME_HEIGHT)
		{
			player.img.y += player.speed;
		}
	}
	
	//�v���C���[�̓����蔻��̍X�V
	CollUpdatePlayer(&player);

	//�}�E�X�����{�^���������Ă���Ƃ�
	if (MouseDown(MOUSE_INPUT_LEFT) == TRUE)
	{
		if (tamaShotCnt == 0)
		{
			//for (int i = 0; i < 360; i += 30)
			//{
				//�e�𔭎˂���(�e��`�悷��)
				for (int i = 0; i < TAMA_MAX; i++)
				{
					if (CheckSoundMem(PlayerSE.handle) == 0)
					{
						PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
					}
					if (tama[i].IsDraw == FALSE)
					{
						ShotTama(&tama[i], 360);

						//�e��1���o������A���[�v�𔲂���
						break;
					}
				}

				/*
				//�e�𔭎˂���(�e��`�悷��)
				for (int i = 0; i < TAMA_MAX; i++)
				{
					
					if (CheckSoundMem(PlayerSE.handle) == 0)
					{
						PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
					}
					
					if (tama[i].IsDraw == FALSE)
					{
						ShotTama(&tama[i], 280);

						//�e��1���o������A���[�v�𔲂���
						break;
					}
				}


				//�e�𔭎˂���(�e��`�悷��)
				for (int i = 0; i < TAMA_MAX; i++)
				{
					
					if (CheckSoundMem(PlayerSE.handle) == 0)
					{
						PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
					}
					
					if (tama[i].IsDraw == FALSE)
					{
						ShotTama(&tama[i], 260);

						//�e��1���o������A���[�v�𔲂���
						break;
					}
				}
				*/

			//}
		}

		//�e�̔��ˑ҂�
		if (tamaShotCnt < tamaShotCntMax)
		{
			tamaShotCnt++;
		}
		else
		{
			tamaShotCnt = 0;
		}
	}

	//�e���΂�
	for (int i = 0; i < TAMA_MAX; i++)
	{
		//�`�悳��Ă���Ƃ�
		if (tama[i].IsDraw == TRUE)
		{
			//tama[i].x;
			//�e�̈ʒu���C��
			//�@���S�ʒu�@�{�@��΂��p�x����΂��������v�Z�@���@����
			tama[i].x = tama[i].StartX + cos(tama[i].degree * DX_PI / 180.0f) * tama[i].radius;
			tama[i].y = tama[i].StartY + sin(tama[i].degree * DX_PI / 180.0f) * tama[i].radius;

			//���a�𑫂�
			tama[i].radius +=tama[i].Speed;

			//�e�𓖂��蔻����X�V
			CollUpdateTama(&tama[i]);

			//��ʊO�ɏo����A�`�悵�Ȃ�
			if (tama[i].y + tama[i].height < 0 ||
				tama[i].y > GAME_HEIGHT ||
				tama[i].x + tama[i].width < 0 ||
				tama[i].x > GAME_WIDTH)
			{
				tama[i].IsDraw = FALSE;
			}
		}
	}

	if (TekiAddCnt < TekiAddCntMAX)
	{
		TekiAddCnt++;
	}
	else
	{
		TekiAddCnt = 0;

		//�G�𐶐�
		for (int i = 0; i < TEKI_MAX; i++)
		{
			//�`�悳��Ă��Ȃ��Ă���T��
			if (teki[i].img.IsDraw == FALSE)
			{
				int Bunkatu = 30;

				if (Score < 1000)
				{
					teki[i] = teki_moto[0];
				}

				else if (Score < 2000)
				{
					teki[i] = teki_moto[1];
				}

				else
				{
					teki[i] = teki_moto[GetRand(TEKI_KIND - 1)];
				}

				teki[i].img.x = GetRand(Bunkatu - 1) * GAME_WIDTH / Bunkatu;
				teki[i].img.y = -teki[i].img.height;

				teki[i].img.IsDraw = TRUE;		//�`�悷��
				break;
			}
		}
	}

	//�G�̏���
	for (int i = 0; i < TEKI_MAX; i++)
	{
		//�`�悵�Ă���G
		if (teki[i].img.IsDraw == TRUE)
		{
			teki[i].img.y += 1;

			//�G�̓����蔻��X�V
			CollUpdateTeki(&teki[i]);

			//���܂ōs������\�����Ȃ�
			if (teki[i].img.y > GAME_HEIGHT)
			{
				teki[i].img.IsDraw = FALSE;
			}

			//�G�Ǝ����̒e������������
			for (int cnt = 0; cnt < TAMA_MAX; cnt++)
			{
				//�`�悳��Ă���Ƃ�
				if (tama[cnt].IsDraw == TRUE)
				{
					//�����蔻��
					if (OnCollRect(teki[i].coll,tama[cnt].coll) == TRUE)
					{
						tama[cnt].IsDraw = FALSE;			//�e�̕\�������Ȃ�
						teki[i].img.IsDraw = FALSE;		//�G�̕\�������Ȃ�

						Score += 500;					//�X�R�A���Z

						if (CheckSoundMem(EnemySE.handle) == 0)
						{
							PlaySoundMem(EnemySE.handle, EnemySE.playType);
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < TEKI_TAMA_MAX; i++)
	{
		if (teki[i].img.IsDraw == TRUE)
		{
			if (teki_tama[i].IsDraw == FALSE)
			{
				ShotTekiTama(&teki_tama[i], 0);

				//�e��1���o������A���[�v�𔲂���
				break;
			}
		}
		//�e�̔��ˑ҂�
		if (tekitamaShotCnt < tekitamaShotCntMax)
		{
			tekitamaShotCnt++;
		}
		else
		{
			tekitamaShotCnt = 0;
		}
	}
	for (int i = 0; i < TEKI_TAMA_MAX; i++)
	{
		//�`�悳��Ă���Ƃ�
		if (teki_tama[i].IsDraw == TRUE)
		{
			for (int i = 0; i < TAMA_MAX; i++)
			{
				/*
				if (CheckSoundMem(PlayerSE.handle) == 0)
				{
					PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
				}
				*/
				if (teki_tama[i].IsDraw == FALSE)
				{
					ShotTekiTama(&teki_tama[i], 0);

					//�e��1���o������A���[�v�𔲂���
					break;
				}
			}

			//tama[i].x;
			//�e�̈ʒu���C��
			//�@���S�ʒu�@�{�@��΂��p�x����΂��������v�Z�@���@����
			teki_tama[i].x = teki_tama[i].StartX + cos(teki_tama[i].degree * DX_PI / 180.0f) * teki_tama[i].radius;
			teki_tama[i].y = teki_tama[i].StartY + sin(teki_tama[i].degree * DX_PI / 180.0f) * teki_tama[i].radius;

			//���a�𑫂�
			teki_tama[i].radius += teki_tama[i].Speed;

			//�e�𓖂��蔻����X�V
			CollUpdateTekiTama(&teki_tama[i]);

			//��ʊO�ɏo����A�`�悵�Ȃ�
			if (teki_tama[i].y + teki_tama[i].height < 0 ||
				teki_tama[i].y > GAME_HEIGHT ||
				teki_tama[i].x + teki_tama[i].width < 0 ||
				teki_tama[i].x > GAME_WIDTH)
			{
				teki_tama[i].IsDraw = FALSE;
			}
		}
	}
	return;
}

/// <summary>
/// �e���΂�
/// </summary>
VOID ShotTama(TAMA* tama,float deg)
{
	
		//�e�𔭎˂���
		tama->IsDraw = TRUE;

		//�e�̈ʒu�����߂�
		tama->StartX = player.img.x + player.img.width / 2 - tama->width / 2;
		tama->StartY = player.img.y ;

		//�e�̑��x��ς���
		tama->Speed = 8;

		//�e�̊p�x
		tama->degree = deg;

		//�e�̔��a
		tama->radius = 0.0f;

		//�e�̓����蔻��̍X�V
		CollUpdateTama(tama);
	
}

/// <summary>
/// �e���΂�
/// </summary>
VOID ShotTekiTama(TEKI_TAMA* teki_tama, float deg)
{

	//�e�𔭎˂���
	teki_tama->IsDraw = TRUE;

	//�e�̈ʒu�����߂�
	teki_tama->StartX = enemy.img.x + enemy.img.width / 2 - teki_tama->width / 2;
	teki_tama->StartY = enemy.img.y;

	//�e�̑��x��ς���
	teki_tama->Speed = 8;

	//�e�̊p�x
	teki_tama->degree = deg;

	//�e�̔��a
	teki_tama->radius = 0.0f;

	//�e�̓����蔻��̍X�V
	CollUpdateTekiTama(teki_tama);

}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{
	//�w�i�̕`��
	for (int i = 0; i < 2; i++)
	{
		//�`��
		DrawGraph(back[i].x, back[i].y, back[i].handle, TRUE);

		//�摜�����܂ł�������
		if (back[i].y > GAME_HEIGHT)
		{
			back[i].y = -back[i].height + 1;
		}

		//�摜�����ɓ�����
		back[i].x++;
	}

	//�G�̕`��
	for (int i = 0; i < TEKI_MAX; i++)
	{
		//�G���`��ł���Ƃ���
		if (teki[i].img.IsDraw == TRUE)
		{
			DrawGraph(teki[i].img.x, teki[i].img.y, teki[i].img.handle, TRUE);

			//�����蔻��̕`��
			if (GAME_DEBUG == FALSE)
			{
				DrawBox(
					teki[i].coll.left, teki[i].coll.top, teki[i].coll.right, teki[i].coll.bottom,
					GetColor(0, 0, 0), FALSE);
			}
		}
	}

	//�v���C���[�̕`��
	if (player.img.IsDraw == TRUE)
	{
		//�v���C���[�̕`��
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		//�����蔻��
		if (GAME_DEBUG == FALSE)
		{
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(0, 0, 0), FALSE);
		}
	}

	//�e�̕`��
	for (int i = 0; i < TAMA_MAX; i++)
	{
		//�`�悳��Ă���Ƃ�
		if (tama[i].IsDraw == TRUE)
		{
			DrawTama(&tama[i]);

			//�����蔻��
			if (GAME_DEBUG == FALSE)
			{
				DrawBox(tama[i].coll.left, tama[i].coll.top, tama[i].coll.right, tama[i].coll.bottom,
					GetColor(0, 0, 0), FALSE);
			}
		}
	}

	for (int i = 0; i < TEKI_TAMA_MAX; i++)
	{
		if (teki_tama[i].IsDraw == TRUE)
		{
			DrawTekiTama(&teki_tama[i]);
			if (GAME_DEBUG == FALSE)
			{
				DrawBox(teki_tama[i].coll.left, teki_tama[i].coll.top, teki_tama[i].coll.right, teki_tama[i].coll.bottom,
					GetColor(0, 0, 0), FALSE);
			}
		}
	}

	//�X�R�A�̕`��
	int old = GetFontSize();		//�ȑO�̃t�H���g�T�C�Y���擾
	SetFontSize(40);				//�t�H���g��傫������
	DrawFormatString(0, 100, GetColor(255, 255, 255), "SCORE:%05d", Score);
	SetFontSize(10);				//�t�H���g������������

	//���Ԃ�`��
	DrawFormatString(0, 50, GetColor(255, 255, 255), "�c��:%3.2f", 30.0f - GetGameTime());

	//�}�E�X�̈ʒu��`��
	MouseDraw();

	//DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �v���C���
/// </summary>
VOID Play2(VOID)
{
	Play2Proc();	//����
	Play2Draw();	//�`��

	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
VOID Play2Proc(VOID)
{
	//if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	if (teki[0].img.y > GAME_HEIGHT == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy
		//BGM���~�߂�
		StopSoundMem(Play2BGM.handle);

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_GAMEOVER);

		//�}�E�X��`�悷��
		SetMouseDispFlag(TRUE);

		return;
	}
	else if (OnCollRect(player.coll, teki->coll) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy
		//BGM���~�߂�
		StopSoundMem(Play2BGM.handle);

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_GAMEOVER);

		//�}�E�X��`�悷��
		SetMouseDispFlag(TRUE);

		if (CheckSoundMem(CollisionSE.handle) == 0)
		{
			PlaySoundMem(CollisionSE.handle, CollisionSE.playType);
		}

		return;
	}
	else if (Score >= 10000)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy
		//BGM���~�߂�
		StopSoundMem(Play2BGM.handle);

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);

		//�}�E�X��`�悷��
		SetMouseDispFlag(TRUE);

		return;
	}


	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(Play2BGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(Play2BGM.handle, Play2BGM.playType);
	}


	//�v���C���[�𑀍�
	if (KeyDown(KEY_INPUT_A) == TRUE)
	{
		if (player.img.x - player.speed >= 0)
		{
			player.img.x -= player.speed;
		}
	}

	if (KeyDown(KEY_INPUT_D) == TRUE)
	{
		if (player.img.x + player.img.width + player.speed <= GAME_WIDTH)
		{
			player.img.x += player.speed;
		}
	}

	if (KeyDown(KEY_INPUT_W) == TRUE)
	{
		if (player.img.y - player.speed >= 0)
		{
			player.img.y -= player.speed;
		}
	}

	if (KeyDown(KEY_INPUT_S) == TRUE)
	{
		if (player.img.y + player.img.height + player.speed <= GAME_HEIGHT)
		{
			player.img.y += player.speed;
		}
	}

	//�v���C���[�̓����蔻��̍X�V
	CollUpdatePlayer(&player);

	//�}�E�X�����{�^���������Ă���Ƃ�
	if (MouseDown(MOUSE_INPUT_LEFT) == TRUE)
	{
		if (tamaShotCnt == 0)
		{
			//for (int i = 0; i < 360; i += 30)
			//{
				//�e�𔭎˂���(�e��`�悷��)
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (CheckSoundMem(PlayerSE.handle) == 0)
				{
					PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
				}
				if (tama[i].IsDraw == FALSE)
				{
					ShotTama(&tama[i], 360);

					//�e��1���o������A���[�v�𔲂���
					break;
				}
			}
		}

		//�e�̔��ˑ҂�
		if (tamaShotCnt < tamaShotCntMax)
		{
			tamaShotCnt++;
		}
		else
		{
			tamaShotCnt = 0;
		}
	}

	//�e���΂�
	for (int i = 0; i < TAMA_MAX; i++)
	{
		//�`�悳��Ă���Ƃ�
		if (tama[i].IsDraw == TRUE)
		{
			//tama[i].x;
			//�e�̈ʒu���C��
			//�@���S�ʒu�@�{�@��΂��p�x����΂��������v�Z�@���@����
			tama[i].x = tama[i].StartX + cos(tama[i].degree * DX_PI / 180.0f) * tama[i].radius;
			tama[i].y = tama[i].StartY + sin(tama[i].degree * DX_PI / 180.0f) * tama[i].radius;

			//���a�𑫂�
			tama[i].radius += tama[i].Speed;

			//�e�𓖂��蔻����X�V
			CollUpdateTama(&tama[i]);

			//��ʊO�ɏo����A�`�悵�Ȃ�
			if (tama[i].y + tama[i].height < 0 ||
				tama[i].y > GAME_HEIGHT ||
				tama[i].x + tama[i].width < 0 ||
				tama[i].x > GAME_WIDTH)
			{
				tama[i].IsDraw = FALSE;
			}
		}
	}

	if (TekiAddCnt < TekiAddCntMAX)
	{
		TekiAddCnt++;
	}
	else
	{
		TekiAddCnt = 0;

		//�G�𐶐�
		for (int i = 0; i < TEKI_MAX; i++)
		{
			//�`�悳��Ă��Ȃ��Ă���T��
			if (teki[i].img.IsDraw == FALSE)
			{
				int Bunkatu = 30;

				if (Score < 1000)
				{
					teki[i] = teki_moto[0];
				}

				else if (Score < 2000)
				{
					teki[i] = teki_moto[1];
				}

				else
				{
					teki[i] = teki_moto[GetRand(TEKI_KIND - 1)];
				}

				teki[i].img.x = GetRand(Bunkatu - 1) * GAME_WIDTH / Bunkatu;
				teki[i].img.y = -teki[i].img.height;

				teki[i].img.IsDraw = TRUE;		//�`�悷��
				break;
			}
		}
	}

	//�G�̏���
	for (int i = 0; i < TEKI_MAX; i++)
	{
		//�`�悵�Ă���G
		if (teki[i].img.IsDraw == TRUE)
		{
			teki[i].img.y += 1;

			//�G�̓����蔻��X�V
			CollUpdateTeki(&teki[i]);

			//���܂ōs������\�����Ȃ�
			if (teki[i].img.y > GAME_HEIGHT)
			{
				teki[i].img.IsDraw = FALSE;
			}

			//�G�Ǝ����̒e������������
			for (int cnt = 0; cnt < TAMA_MAX; cnt++)
			{
				//�`�悳��Ă���Ƃ�
				if (tama[cnt].IsDraw == TRUE)
				{
					//�����蔻��
					if (OnCollRect(teki[i].coll, tama[cnt].coll) == TRUE)
					{
						tama[cnt].IsDraw = FALSE;			//�e�̕\�������Ȃ�
						teki[i].img.IsDraw = FALSE;		//�G�̕\�������Ȃ�

						Score += 500;					//�X�R�A���Z

						if (CheckSoundMem(EnemySE.handle) == 0)
						{
							PlaySoundMem(EnemySE.handle, EnemySE.playType);
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < TEKI_TAMA_MAX; i++)
	{
		if (teki[i].img.IsDraw == TRUE)
		{
			if (teki_tama[i].IsDraw == FALSE)
			{
				ShotTekiTama(&teki_tama[i], 0);

				//�e��1���o������A���[�v�𔲂���
				break;
			}
		}
		//�e�̔��ˑ҂�
		if (tekitamaShotCnt < tekitamaShotCntMax)
		{
			tekitamaShotCnt++;
		}
		else
		{
			tekitamaShotCnt = 0;
		}
	}
	for (int i = 0; i < TEKI_TAMA_MAX; i++)
	{
		//�`�悳��Ă���Ƃ�
		if (teki_tama[i].IsDraw == TRUE)
		{
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (teki_tama[i].IsDraw == FALSE)
				{
					ShotTekiTama(&teki_tama[i], 0);

					//�e��1���o������A���[�v�𔲂���
					break;
				}
			}

			//tama[i].x;
			//�e�̈ʒu���C��
			//�@���S�ʒu�@�{�@��΂��p�x����΂��������v�Z�@���@����
			teki_tama[i].x = teki_tama[i].StartX + cos(teki_tama[i].degree * DX_PI / 180.0f) * teki_tama[i].radius;
			teki_tama[i].y = teki_tama[i].StartY + sin(teki_tama[i].degree * DX_PI / 180.0f) * teki_tama[i].radius;

			//���a�𑫂�
			teki_tama[i].radius += teki_tama[i].Speed;

			//�e�𓖂��蔻����X�V
			CollUpdateTekiTama(&teki_tama[i]);

			//��ʊO�ɏo����A�`�悵�Ȃ�
			if (teki_tama[i].y + teki_tama[i].height < 0 ||
				teki_tama[i].y > GAME_HEIGHT ||
				teki_tama[i].x + teki_tama[i].width < 0 ||
				teki_tama[i].x > GAME_WIDTH)
			{
				teki_tama[i].IsDraw = FALSE;
			}
		}
	}
	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID Play2Draw(VOID)
{
	//�w�i�̕`��
	for (int i = 0; i < 2; i++)
	{
		//�`��
		DrawGraph(back[i].x, back[i].y, back[i].handle, TRUE);

		//�摜�����܂ł�������
		if (back[i].y > GAME_HEIGHT)
		{
			back[i].y = -back[i].height + 1;
		}

		//�摜�����ɓ�����
		back[i].x++;
	}

	//�G�̕`��
	for (int i = 0; i < TEKI_MAX; i++)
	{
		//�G���`��ł���Ƃ���
		if (teki[i].img.IsDraw == TRUE)
		{
			DrawGraph(teki[i].img.x, teki[i].img.y, teki[i].img.handle, TRUE);

			//�����蔻��̕`��
			if (GAME_DEBUG == FALSE)
			{
				DrawBox(
					teki[i].coll.left, teki[i].coll.top, teki[i].coll.right, teki[i].coll.bottom,
					GetColor(0, 0, 0), FALSE);
			}
		}
	}

	//�v���C���[�̕`��
	if (player.img.IsDraw == TRUE)
	{
		//�v���C���[�̕`��
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		//�����蔻��
		if (GAME_DEBUG == FALSE)
		{
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(0, 0, 0), FALSE);
		}
	}

	//�e�̕`��
	for (int i = 0; i < TAMA_MAX; i++)
	{
		//�`�悳��Ă���Ƃ�
		if (tama[i].IsDraw == TRUE)
		{
			DrawTama(&tama[i]);

			//�����蔻��
			if (GAME_DEBUG == FALSE)
			{
				DrawBox(tama[i].coll.left, tama[i].coll.top, tama[i].coll.right, tama[i].coll.bottom,
					GetColor(0, 0, 0), FALSE);
			}
		}
	}

	for (int i = 0; i < TEKI_TAMA_MAX; i++)
	{
		if (teki_tama[i].IsDraw == TRUE)
		{
			DrawTekiTama(&teki_tama[i]);
			if (GAME_DEBUG == FALSE)
			{
				DrawBox(teki_tama[i].coll.left, teki_tama[i].coll.top, teki_tama[i].coll.right, teki_tama[i].coll.bottom,
					GetColor(0, 0, 0), FALSE);
			}
		}
	}

	//�X�R�A�̕`��
	int old = GetFontSize();		//�ȑO�̃t�H���g�T�C�Y���擾
	SetFontSize(40);				//�t�H���g��傫������
	DrawFormatString(0, 100, GetColor(255, 255, 255), "SCORE:%05d", Score);
	SetFontSize(10);				//�t�H���g������������

	//���Ԃ�`��
	DrawFormatString(0, 50, GetColor(255, 255, 255), "�c��:%3.2f", 30.0f - GetGameTime());

	//�}�E�X�̈ʒu��`��
	MouseDraw();
	return;
}

/// <summary>
/// �Q�[���I�[�o�[���
/// </summary>
/// <param name=""></param>
VOID GameOver(VOID)
{
	GameOverProc();
	GameOverDraw();
}

/// <summary>
/// �Q�[���I�[�o�[��ʂ̏���
/// </summary>
/// <param name=""></param>
VOID GameOverProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGM���~�߂�
		StopSoundMem(GameOverBGM.handle);
		//�^�C�g����ʐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	//BGM������Ă��Ȃ���
	if (CheckSoundMem(GameOverBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(GameOverBGM.handle, GameOverBGM.playType);
	}
}

/// <summary>
/// �Q�[���I�[�o�[��ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID GameOverDraw(VOID)
{
	DrawGraph(GameOverBackground.x, GameOverBackground.y, GameOverBackground.handle, TRUE);
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, GameOverBackground.handle, TRUE);

	DrawGraph(GameOverEnter.x, GameOverEnter.y, GameOverEnter.handle, TRUE);

	if (PushEnterCnt < PushEnterCntMax) { PushEnterCnt++; }
	else
	{
		if (GameOverEnterBrink == TRUE)GameOverEnterBrink = FALSE;
		else if (GameOverEnterBrink == FALSE)GameOverEnterBrink = TRUE;

		PushEnterCnt = 0;	//�J�E���^��������
	}
	//PushEnter�̓_��
	if (GameOverEnterBrink == TRUE)
	{
		//PushEnter�̕`��
		DrawGraph(GameOverEnter.x, GameOverEnter.y, GameOverEnter.handle, TRUE);
	}

	//DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();	//����
	EndDraw();	//�`��

	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
VOID EndProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy
		//BGM���~�߂�
		StopSoundMem(EndBGM.handle);
		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(EndBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(EndBGM.handle, EndBGM.playType);
	}

	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
VOID EndDraw(VOID)
{
	DrawGraph(EndBackground.x, EndBackground.y, EndBackground.handle, TRUE);
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, EndBackground.handle, TRUE);

	//EndClear�̕`��
	DrawGraph(EndClear.x, EndClear.y, EndClear.handle, TRUE);

	//MAX�l�܂ő҂�
	if (PushEnterCnt < PushEnterCntMax) { PushEnterCnt++; }
	else
	{
		if (EndEnterBrink == TRUE)EndEnterBrink = FALSE;
		else if (EndEnterBrink == FALSE)EndEnterBrink = TRUE;

		PushEnterCnt = 0;	//�J�E���^��������
	}

	//PushEnter�̓_��
	if (EndEnterBrink == TRUE)
	{
		//PushEnter�̕`��
		DrawGraph(EndEnter.x, EndEnter.y, EndEnter.handle, TRUE);
	}

	//DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �؂�ւ����
/// </summary>
VOID Change(VOID)
{
	ChangeProc();	//����
	ChangeDraw();	//�`��

	return;
}

/// <summary>
/// �؂�ւ���ʂ̏���
/// </summary>
VOID ChangeProc(VOID)
{

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//�J�E���^�����炷
		}
		else
		{
			//�t�F�[�h�C���������I�����

			fadeInCnt = fadeInCntInit;	//�J�E���^������
			IsFadeIn = FALSE;			//�t�F�[�h�C�������I��
		}
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//�J�E���^�𑝂₷
		}
		else
		{
			//�t�F�[�h�A�E�g�������I�����

			fadeOutCnt = fadeOutCntInit;	//�J�E���^������
			IsFadeOut = FALSE;				//�t�F�[�h�A�E�g�����I��
		}
	}

	//�؂�ւ������I��
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�C�����Ă��Ȃ��A�t�F�[�h�A�E�g�����Ă��Ȃ��Ƃ�
		GameScene = NextGameScene;	//���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;	//�ȑO�̃Q�[���V�[���X�V
	}


	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();		//�G���h��ʂ̕`��
		break;
	case GAME_SCENE_GAMEOVER:
		GameOverDraw();		//�G���h��ʂ̕`��
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �����蔻��̗̈�X�V�i�v���C���[�j
/// </summary>
/// <param name="chara">�����蔻��̗̈�</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;					//�����蔻��������
	chara->coll.top = chara->img.y;						//�����蔻��������

	chara->coll.right = chara->img.x + chara->img.width ;		//�����蔻��������
	chara->coll.bottom = chara->img.y + chara->img.height ;		//�����蔻��������

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V�i�G�j
/// </summary>
/// <param name="chara">�����蔻��̗̈�</param>
VOID CollUpdateTeki(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;					//�����蔻��������
	chara->coll.top = chara->img.y;						//�����蔻��������

	chara->coll.right = chara->img.x + chara->img.width;		//�����蔻��������
	chara->coll.bottom = chara->img.y + chara->img.height;		//�����蔻��������

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V(�e)
/// </summary>
/// <param name="tama">�e�̍\����</param>
VOID CollUpdateTama(TAMA* tama)
{
	tama->coll.left = tama->x;
	tama->coll.top = tama->y;

	tama->coll.right = tama->x + tama->width;
	tama->coll.bottom = tama->y + tama->height;

	return;
}

VOID CollUpdateTekiTama(TEKI_TAMA* teki_tama)
{
	teki_tama->coll.left = teki_tama->x;
	teki_tama->coll.top = teki_tama->y;

	teki_tama->coll.right = teki_tama->x + teki_tama->width;
	teki_tama->coll.bottom = teki_tama->y + teki_tama->height;

	return;
}

/// <summary>
/// ��`�Ƌ�`�̓����蔻��
/// </summary>
/// <param name="a">��`A</param>
/// <param name="b">��`B</param>
/// <returns>����������TRUE/������Ȃ��Ȃ�FALSE</returns>
BOOL OnCollRect(RECT a, RECT b)
{
	if (
		a.left < b.right &&		//�@��`A�̍���X���W < ��`B�̉E��X���W�@����
		a.right > b.left &&		//�@��`A�̉E��X���W > ��`B�̍���X���W�@����
		a.top  < b.bottom &&	//�@��`A�̏��Y���W < ��`B�̉���Y���W�@����
		a.bottom > b.top		//�@��`A�̉���Y���W > ��`B�̏��Y���W
		)
	{
		//�������Ă���Ƃ�
		return TRUE;
	}
	else
	{
		//�������Ă��Ȃ��Ƃ�
		return FALSE;
	}
}