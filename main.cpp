//=====================================================================
// 
// ランニングマン [main.cpp]
// Author:岩崎桧翔
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

// マクロ定義
#define CLASS_NAME			"WindowsClass"					// ウィンドウクラスの名前
#define WINDOW_NAME			"シューティングゲーム(アルファ)"		// ウィンドウの名前（キャプションに表示される文字列）

// プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindowed);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawDebug(void);

// グローバル変数宣言
LPDIRECT3D9 g_pD3D = NULL;						// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;			// Direct3Dデバイスへのポインタ
LPD3DXFONT g_pFont = NULL;						// フォントへのポインタ

MODE g_mode = MODE_TITLE;						// 起動時の画面（モード）
int g_nCountFPS = 0;							// FPSカウンタ
HWND g_hWndMain;

//==============================================
// メイン関数
//==============================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// メモリリーク検知用のフラグ
#endif // _DEBUG

	// ウィンドウクラスの設定
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),					// WNDCLASSのメモリサイズ
		CS_CLASSDC,							// ウィンドウのスタイル
		WindowProc,							// ウィンドウプロシージャ
		0,									// 0（使わない）
		0,									// 0（使わない）
		hInstance,							// インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),	// タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),		// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			// クライアント領域の背景色
		NULL,								// メニューバー
		CLASS_NAME,							// ウィンドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION)		// ファイルのアイコン
	};

	MSG msg;		// メッセージを格納する変数
	RECT rect = { 0,0,SCREEN_WIDTH, SCREEN_HEIGHT };
	DWORD dwCurrentTime;
	DWORD dwExecLastTime;
	DWORD dwFrameCount;
	DWORD dwFPSLastTime;

	// ウィンドウの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドウを生成
	g_hWndMain = CreateWindowEx(
		0,							// 拡張ウィンドウスタイル
		CLASS_NAME,					// ウィンドウクラスの名前
		WINDOW_NAME,				// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,		// ウィンドウスタイル
		CW_USEDEFAULT,				// ウィンドウの左上X座標
		CW_USEDEFAULT,				// ウィンドウの左上Y座標
		(rect.right - rect.left),	// ウィンドウの幅
		(rect.bottom - rect.top),	// ウィンドウの高さ
		NULL,						// 親ウィンドウのハンドル
		NULL,						// メニューハンドルまたは子ウィンドウID
		hInstance,					// インスタンスハンドル
		NULL						// ウィンドウ作成データ
	);

	// 初期化処理
	if (FAILED(Init(hInstance, g_hWndMain, WINDOWED)))
	{// 初期化処理が失敗した場合
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = timeGetTime();
	dwExecLastTime = timeGetTime();
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// ウィンドウの表示
	ShowWindow(g_hWndMain, nCmdShow);
	UpdateWindow(g_hWndMain);

	// メッセージループ
	while (1)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windowsの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectXの処理
			dwCurrentTime = timeGetTime();

			if (dwCurrentTime - dwFPSLastTime >= 500)
			{// 0.5秒経過
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60分の1秒経過
				dwExecLastTime = dwCurrentTime;

				dwFrameCount++;

				// 更新処理
				Update();

				// 描画処理
				Draw();

			}
		}
	}

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//==============================================
// ウィンドウプロシージャ
//==============================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:		// ウィンドウ破棄のメッセージ
		// WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:		// キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				// ウィンドウを破棄する（WM_DESTROYメッセージを送る）
				DestroyWindow(hWnd);
			}
			else
			{
				return 0;
			}

			break;
		}
		break;

	case WM_CLOSE:			// 閉じるボタン押下のメッセージ
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{
			// ウィンドウを破棄する（WM_DESTROYメッセージを送る）
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;	// 0を返さないと終了してしまう
		}

		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// 既定の処理を返す
}

//==============================================
// 初期化処理
//==============================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindowed)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;

	// 乱数の種を設定
	srand((unsigned int)timeGetTime());

	// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
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

	// Direct3Dデバイスの作成
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice
	)))
	{// 描画処理をハードウェア、頂点処理をCPUに
		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice
		)))
		{// 描画処理、頂点処理ともにCPUに
			if (FAILED(g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice
			)))
			{// すべて不可能なら終了
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャの拡縮補完の設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャの拡縮補完の設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャの繰り返しの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャの繰り返しの設定

	// テクスチャステージステート
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	AddFontResourceEx("data\\FONT\\x10y12pxDonguriDuel.ttf", FR_PRIVATE, 0);

	// デバッグ表示用のフォントを設定
	D3DXFONT_DESC fontDesc = {
	24,                        // Height
	0,                         // Width (0 = 自動)
	FW_NORMAL,                 // Weight
	1,                         // MipLevels
	FALSE,                     // Italic
	DEFAULT_CHARSET,          // CharSet
	OUT_DEFAULT_PRECIS,       // OutputPrecision
	CLIP_DEFAULT_PRECIS,      // Quality
	DEFAULT_PITCH | FF_DONTCARE, // PitchAndFamily
	"x10y12pxDonguriDuel"           // FaceName（.ttf名）
	};

	// デバッグ表示用のフォントを作成
	D3DXCreateFontIndirect(g_pD3DDevice, &fontDesc, &g_pFont);

	// キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// ジョイパッドの初期化処理
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	// サウンドの初期化処理
	InitSound(hWnd);

	InitTitle();

	// モードの設定
	InitFade(MODE_TITLE);

	return S_OK;
}

//==============================================
// 終了処理
//==============================================
void Uninit(void)
{
	// サウンドの停止
	StopSound();

	// <<< 各種モードの終了処理 >>>
	UninitTitle();		// タイトル画面
	UninitGame();		// ゲーム画面
	UninitStage1();
	UninitResult();		// リザルト画面

	// フェードの終了処理
	UninitFade();

	// サウンドの終了処理
	UninitSound();

	// キーボードの終了処理
	UninitKeyboard();

	// ジョイパッドのの終了処理
	UninitJoypad();

	// デバッグ表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	RemoveFontResourceEx("data\\FONT\\x10y12pxDonguriDuel.ttf", FR_PRIVATE, 0);

	// Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//==============================================
// 更新処理
//==============================================
void Update(void)
{
	// キーボードの更新処理
	UpdateKeyboard();

	// ジョイパッドの更新処理
	UpdateJoypad();

	// モードの更新処理
	switch (g_mode)
	{
	case MODE_TITLE:	// ゲーム画面
		UpdateTitle();
		break;

	case MODE_GAME:		// ゲーム画面
		UpdateGame();
		break;

	case MODE_STAGE1:
		UpdateStage1();
		break;

	case MODE_RESULT:	// リザルト画面
		UpdateResult();
		break;
	}

	// フェードの更新処理
	UpdateFade();
}

//==============================================
// 描画処理
//==============================================
void Draw(void)
{
	// 画面クリア（バックバッファとＺバッファのクリア）
	g_pD3DDevice->Clear(
		0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		BG_COLOR, 1.0f, 0
	);

	// 描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// 描画開始が成功した場合

		// 画面の描画処理
		switch (g_mode)
		{
		case MODE_TITLE:	// タイトル画面
			DrawTitle();
			break;

		case MODE_GAME:		// ゲーム画面
			DrawGame();
			break;

		case MODE_STAGE1:
			DrawStage1();
			break;
			
		case MODE_RESULT:	// リザルト画面
			DrawResult();
			break;
		}

		// フェードの描画処理
		DrawFade();

#ifdef _DEBUG
		// デバッグ表示
		DrawDebug();
#endif

		// 描画終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==============================================
// Direct3Dデバイスの取得
//==============================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//==============================================
// ウィンドウ取得処理
//==============================================
HWND GetMainWindow(void)
{
	return g_hWndMain;
}

//==============================================
// モード設定処理
//==============================================
void SetMode(MODE mode)
{
	// 再生中のサウンドを停止
	StopSound();

	// 現在のモードの終了処理
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

	// 新しいモードの初期化処理
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
// モード取得処理
//==============================================
MODE GetMode(void)
{
	return g_mode;
}

//==============================================
// デバッグ表示
//==============================================
void DrawDebug()
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_WIDTH };
	char aStr[512] = {};

	Player* pPlayer = GetPlayer();

	// 文字列を代入
	// FPS
	sprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	// ステート
	sprintf(&aStr[0], "%sState:%d\n", aStr, pPlayer->state);
	sprintf(&aStr[0], "%sStateCount:%d\n", aStr, pPlayer->nCountState);

	sprintf(&aStr[0], "%sHealth:%.3f\n", aStr, pPlayer->fLife);

	// テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DXCOLOR(255, 255, 255, 255));
}