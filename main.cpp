//=====================================================================
// 
// �����j���O�}�� [main.cpp]
// Author:���O��
//
//=====================================================================
#include <crtdbg.h>

#include "main.h"
#include "input.h"
#include "sound.h"

#include "title.h"
#include "game.h"
#include "stage1.h"
#include "result.h"
#include "fade.h"

#include "player.h"

// �}�N����`
#define CLASS_NAME			"WindowsClass"					// �E�B���h�E�N���X�̖��O
#define WINDOW_NAME			"�V���[�e�B���O�Q�[��(�A���t�@)"		// �E�B���h�E�̖��O�i�L���v�V�����ɕ\������镶����j

// �v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindowed);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawDebug(void);

// �O���[�o���ϐ��錾
LPDIRECT3D9 g_pD3D = NULL;						// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;			// Direct3D�f�o�C�X�ւ̃|�C���^
LPD3DXFONT g_pFont = NULL;						// �t�H���g�ւ̃|�C���^

MODE g_mode = MODE_TITLE;						// �N�����̉�ʁi���[�h�j
int g_nCountFPS = 0;							// FPS�J�E���^
HWND g_hWndMain;

//==============================================
// ���C���֐�
//==============================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// ���������[�N���m�p�̃t���O
#endif // _DEBUG

	// �E�B���h�E�N���X�̐ݒ�
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),					// WNDCLASS�̃������T�C�Y
		CS_CLASSDC,							// �E�B���h�E�̃X�^�C��
		WindowProc,							// �E�B���h�E�v���V�[�W��
		0,									// 0�i�g��Ȃ��j
		0,									// 0�i�g��Ȃ��j
		hInstance,							// �C���X�^���X�n���h��
		LoadIcon(NULL, IDI_APPLICATION),	// �^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL, IDC_ARROW),		// �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),			// �N���C�A���g�̈�̔w�i�F
		NULL,								// ���j���[�o�[
		CLASS_NAME,							// �E�B���h�E�N���X�̖��O
		LoadIcon(NULL, IDI_APPLICATION)		// �t�@�C���̃A�C�R��
	};

	MSG msg;		// ���b�Z�[�W���i�[����ϐ�
	RECT rect = { 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
	DWORD dwCurrentTime;
	DWORD dwExecLastTime;
	DWORD dwFrameCount;
	DWORD dwFPSLastTime;

	// �E�B���h�E�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�B���h�E�𐶐�
	g_hWndMain = CreateWindowEx(
		0,							// �g���E�B���h�E�X�^�C��
		CLASS_NAME,					// �E�B���h�E�N���X�̖��O
		WINDOW_NAME,				// �E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,		// �E�B���h�E�X�^�C��
		CW_USEDEFAULT,				// �E�B���h�E�̍���X���W
		CW_USEDEFAULT,				// �E�B���h�E�̍���Y���W
		(rect.right - rect.left),	// �E�B���h�E�̕�
		(rect.bottom - rect.top),	// �E�B���h�E�̍���
		NULL,						// �e�E�B���h�E�̃n���h��
		NULL,						// ���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,					// �C���X�^���X�n���h��
		NULL						// �E�B���h�E�쐬�f�[�^
	);

	// ����������
	if (FAILED(Init(hInstance, g_hWndMain, WINDOWED)))
	{// ���������������s�����ꍇ
		return -1;
	}

	// ����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = timeGetTime();
	dwExecLastTime = timeGetTime();
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// �E�B���h�E�̕\��
	ShowWindow(g_hWndMain, nCmdShow);
	UpdateWindow(g_hWndMain);

	// ���b�Z�[�W���[�v
	while (1)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windows�̏���
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// ���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectX�̏���
			dwCurrentTime = timeGetTime();

			if (dwCurrentTime - dwFPSLastTime >= 500)
			{// 0.5�b�o��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60����1�b�o��
				dwExecLastTime = dwCurrentTime;

				dwFrameCount++;

				// �X�V����
				Update();

				// �`�揈��
				Draw();

			}
		}
	}

	// �I������
	Uninit();

	// ����\��߂�
	timeEndPeriod(1);

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//==============================================
// �E�B���h�E�v���V�[�W��
//==============================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:		// �E�B���h�E�j���̃��b�Z�[�W
		// WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:		// �L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

			if (nID == IDYES)
			{
				// �E�B���h�E��j������iWM_DESTROY���b�Z�[�W�𑗂�j
				DestroyWindow(hWnd);
			}
			else
			{
				return 0;
			}

			break;
		}
		break;

	case WM_CLOSE:			// ����{�^�������̃��b�Z�[�W
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

		if (nID == IDYES)
		{
			// �E�B���h�E��j������iWM_DESTROY���b�Z�[�W�𑗂�j
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;	// 0��Ԃ��Ȃ��ƏI�����Ă��܂�
		}

		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// ����̏�����Ԃ�
}

//==============================================
// ����������
//==============================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindowed)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	// �����̎��ݒ�
	srand((unsigned int)timeGetTime());

	// Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindowed;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// Direct3D�f�o�C�X�̍쐬
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice
	)))
	{// �`�揈�����n�[�h�E�F�A�A���_������CPU��
		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice
		)))
		{// �`�揈���A���_�����Ƃ���CPU��
			if (FAILED(g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice
			)))
			{// ���ׂĕs�\�Ȃ�I��
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���̊g�k�⊮�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���̊g�k�⊮�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���̌J��Ԃ��̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���̌J��Ԃ��̐ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	AddFontResourceEx("data\\FONT\\x10y12pxDonguriDuel.ttf", FR_PRIVATE, 0);

	// �f�o�b�O�\���p�̃t�H���g��ݒ�
	D3DXFONT_DESC fontDesc = {
	24,                        // Height
	0,                         // Width (0 = ����)
	FW_NORMAL,                 // Weight
	1,                         // MipLevels
	FALSE,                     // Italic
	DEFAULT_CHARSET,          // CharSet
	OUT_DEFAULT_PRECIS,       // OutputPrecision
	CLIP_DEFAULT_PRECIS,      // Quality
	DEFAULT_PITCH | FF_DONTCARE, // PitchAndFamily
	"x10y12pxDonguriDuel"           // FaceName�i.ttf���j
	};

	// �f�o�b�O�\���p�̃t�H���g���쐬
	D3DXCreateFontIndirect(g_pD3DDevice, &fontDesc, &g_pFont);

	// �L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// �W���C�p�b�h�̏���������
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	// �T�E���h�̏���������
	InitSound(hWnd);

	InitTitle();

	// ���[�h�̐ݒ�
	InitFade(MODE_TITLE);

	return S_OK;
}

//==============================================
// �I������
//==============================================
void Uninit(void)
{
	// �T�E���h�̒�~
	StopSound();

	// <<< �e�탂�[�h�̏I������ >>>
	UninitTitle();		// �^�C�g�����
	UninitGame();		// �Q�[�����
	UninitStage1();
	UninitResult();		// ���U���g���

	// �t�F�[�h�̏I������
	UninitFade();

	// �T�E���h�̏I������
	UninitSound();

	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	// �W���C�p�b�h�̂̏I������
	UninitJoypad();

	// �f�o�b�O�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	RemoveFontResourceEx("data\\FONT\\x10y12pxDonguriDuel.ttf", FR_PRIVATE, 0);

	// Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//==============================================
// �X�V����
//==============================================
void Update(void)
{
	// �L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	// �W���C�p�b�h�̍X�V����
	UpdateJoypad();

	// ���[�h�̍X�V����
	switch (g_mode)
	{
	case MODE_TITLE:	// �Q�[�����
		UpdateTitle();
		break;

	case MODE_GAME:		// �Q�[�����
		UpdateGame();
		break;

	case MODE_STAGE1:
		UpdateStage1();
		break;

	case MODE_RESULT:	// ���U���g���
		UpdateResult();
		break;
	}

	// �t�F�[�h�̍X�V����
	UpdateFade();
}

//==============================================
// �`�揈��
//==============================================
void Draw(void)
{
	// ��ʃN���A�i�o�b�N�o�b�t�@�Ƃy�o�b�t�@�̃N���A�j
	g_pD3DDevice->Clear(
		0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		BG_COLOR, 1.0f, 0
	);

	// �`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// �`��J�n�����������ꍇ

		// ��ʂ̕`�揈��
		switch (g_mode)
		{
		case MODE_TITLE:	// �^�C�g�����
			DrawTitle();
			break;

		case MODE_GAME:		// �Q�[�����
			DrawGame();
			break;

		case MODE_STAGE1:
			DrawStage1();
			break;
			
		case MODE_RESULT:	// ���U���g���
			DrawResult();
			break;
		}

		// �t�F�[�h�̕`�揈��
		DrawFade();

#ifdef _DEBUG
		// �f�o�b�O�\��
		DrawDebug();
#endif

		// �`��I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==============================================
// Direct3D�f�o�C�X�̎擾
//==============================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//==============================================
// �E�B���h�E�擾����
//==============================================
HWND GetMainWindow(void)
{
	return g_hWndMain;
}

//==============================================
// ���[�h�ݒ菈��
//==============================================
void SetMode(MODE mode)
{
	// �Đ����̃T�E���h���~
	StopSound();

	// ���݂̃��[�h�̏I������
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		break;

	case MODE_GAME:
		UninitGame();
		break;

	case MODE_STAGE1:
		UninitStage1();
		break;

	case MODE_RESULT:
		UninitResult();
		break;
	}

	// �V�������[�h�̏���������
	switch (mode)
	{
	case MODE_TITLE:
		InitTitle();
		break;

	case MODE_GAME:
		InitGame();
		break;

	case MODE_STAGE1:
		InitStage1();
		break;

	case MODE_RESULT:
		InitResult();
		break;
	}

	g_mode = mode;
}

//==============================================
// ���[�h�擾����
//==============================================
MODE GetMode(void)
{
	return g_mode;
}

//==============================================
// �f�o�b�O�\��
//==============================================
void DrawDebug()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_WIDTH };
	char aStr[512] = {};

	Player* pPlayer = GetPlayer();

	// ���������
	// FPS
	sprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	// �X�e�[�g
	sprintf(&aStr[0], "%sState:%d\n", aStr, pPlayer->state);
	sprintf(&aStr[0], "%sStateCount:%d\n", aStr, pPlayer->nCountState);

	sprintf(&aStr[0], "%sHealth:%.3f\n", aStr, pPlayer->fLife);

	// �e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DXCOLOR(255, 255, 255, 255));
}