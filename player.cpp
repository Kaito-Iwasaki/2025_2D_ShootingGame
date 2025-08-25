//=====================================================================
// 
// プレイヤー処理 [player.cpp]
// Author:岩崎桧翔
//
//=====================================================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "util.h"

#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"

// マクロ定義
#define TEXTURE_FILENAME		"data\\TEXTURE\\player000.png"				// テクスチャファイル名
#define TEXTURE_POS_X			(SCREEN_WIDTH / 2)						// ポリゴン位置X
#define TEXTURE_POS_Y			(SCREEN_HEIGHT * 0.9)					// ポリゴン位置Y
#define TEXTURE_SCALE_X			(64.0f)									// ポリゴンサイズX
#define TEXTURE_SCALE_Y			(64.0f)									// ポリゴンサイズY
#define TEXTURE_SCALE_MIN		(0.5f)									// ポリゴン最大縮小率
#define TEXTURE_SCALE_MAX		(1.5f)									// ポリゴン最大拡大率
#define TEXTURE_MAX_X			(1)										// テクスチャパターン数X
#define TEXTURE_MAX_Y			(1)										// テクスチャパターン数Y
#define TEXTURE_MAX_PATTERN		(TEXTURE_MAX_X * TEXTURE_MAX_Y)			// テクスチャ総パターン数
#define TEXTURE_INTERVAL		(1)										// パターン切り替えフレーム数
#define TEXTURE_COLOR			D3DCOLOR_RGBA(255, 255, 255, 255);

#define PLAYER_SPEED			(8.0f)									// プレイヤーのスピード
#define SHOOT_SPEED				(20.0f)									// 弾速	
#define SHOOT_INTERVAL			(5)										// 弾発射の間隔

// プロトタイプ宣言
void ShootBullet(void);

// グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer;		// 頂点バッファへのポインタ
Player g_aPlayer;

float g_fLengthPlayer;
float g_fAnglePlayer;

//=====================================================================
// 初期化処理
//=====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// 頂点情報へのポインタ
	VERTEX_2D* pVtx;			// デバイス

	// デバイスの取得
	pDevice = GetDevice();

	// 値の初期化
	g_aPlayer.pos = { TEXTURE_POS_X, TEXTURE_POS_Y, 0.0f};
	g_aPlayer.posMove = { 0.0f, 0.0f, 0.0f };
	g_aPlayer.fRot = 0.0f;
	g_aPlayer.fRotMove = 0.0f;
	g_aPlayer.size = { TEXTURE_SCALE_X, TEXTURE_SCALE_Y, 0.0f };

	g_aPlayer.state = PLAYERSTATE_NORMAL;
	g_aPlayer.nCountState = 0;
	g_aPlayer.fLife = 3.0f;
	g_aPlayer.nLastShot = 0;
	g_aPlayer.nUpgrade = 0;
	g_aPlayer.nCharge = 0;

	g_aPlayer.bVisible = true;

	// 対角線の長さを算出
	g_fLengthPlayer = sqrtf(g_aPlayer.size.x * g_aPlayer.size.x + g_aPlayer.size.y * g_aPlayer.size.y) * 0.5f;

	// 対角線の角度を算出
	g_fAnglePlayer = atan2f(g_aPlayer.size.x, g_aPlayer.size.y);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		TEXTURE_FILENAME,
		&g_pTexturePlayer
	);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL
	);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.pos.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_aPlayer.pos.x + cosf(g_aPlayer.pos.z + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.pos.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_aPlayer.pos.x + cosf(g_aPlayer.pos.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.pos.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_aPlayer.pos.x + cosf(g_aPlayer.pos.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.pos.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_aPlayer.pos.x + cosf(g_aPlayer.pos.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = TEXTURE_COLOR;
	pVtx[1].col = TEXTURE_COLOR;
	pVtx[2].col = TEXTURE_COLOR;
	pVtx[3].col = TEXTURE_COLOR;

	// テクスチャ座標の設定
	pVtx[0].tex.x = 1.0f / TEXTURE_MAX_X * g_aPlayer.nPatternAnim;
	pVtx[0].tex.y = 1.0f / TEXTURE_MAX_Y * (g_aPlayer.nPatternAnim / TEXTURE_MAX_X);

	pVtx[1].tex.x = 1.0f / TEXTURE_MAX_X * (g_aPlayer.nPatternAnim + 1);
	pVtx[1].tex.y = 1.0f / TEXTURE_MAX_Y * (g_aPlayer.nPatternAnim / TEXTURE_MAX_X);

	pVtx[2].tex.x = 1.0f / TEXTURE_MAX_X * g_aPlayer.nPatternAnim;
	pVtx[2].tex.y = 1.0f / TEXTURE_MAX_Y * (g_aPlayer.nPatternAnim / TEXTURE_MAX_X + 1);

	pVtx[3].tex.x = 1.0f / TEXTURE_MAX_X * (g_aPlayer.nPatternAnim + 1);
	pVtx[3].tex.y = 1.0f / TEXTURE_MAX_Y * (g_aPlayer.nPatternAnim / TEXTURE_MAX_X + 1);

	// 頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitPlayer(void)
{
	// テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdatePlayer(void)
{
	VERTEX_2D* pVtx;
	D3DXVECTOR3 keyboardInput = D3DXVECTOR3_ZERO;
	D3DXVECTOR3 joypadInput = D3DXVECTOR3_ZERO;
	XINPUT_STATE* joypad = GetJoypad();

	//**************************************************
	// プレイヤーの状態による処理
	//**************************************************
	switch (g_aPlayer.state)
	{
	case PLAYERSTATE_NORMAL:		// 通常
		g_aPlayer.bVisible = true;
		
		if (g_aPlayer.fLife < 3)
		{
			g_aPlayer.fLife += 0.001f;
		}
		break;

	case PLAYERSTATE_DAMAGED:		// ダメージ状態
		g_aPlayer.nCountState++;


		if (g_aPlayer.nCountState % 5 == 0)
		{
			g_aPlayer.bVisible = g_aPlayer.bVisible ^ 1;
		}

		if (g_aPlayer.nCountState > 20)
		{
			SetVibration(0, 0);
		}
		else
		{
			SetVibration(60000, 60000);
		}

		if (g_aPlayer.nCountState > 100)
		{
			g_aPlayer.state = PLAYERSTATE_NORMAL;
			g_aPlayer.nCountState = 0;
			g_aPlayer.bVisible = true;
		}
		break;

	case PLAYERSTATE_DEAD:		// 死亡状態
		g_aPlayer.bVisible = false;

		SetVibration(60000, 60000);

		g_aPlayer.nCountState++;

		if (g_aPlayer.nCountState > 100)
		{
			g_aPlayer.state = PLAYERSTATE_END;
		}
		break;

	case PLAYERSTATE_END:		// 終了
		SetVibration(0, 0);
		break;
	}

	if (IsPlayerDead() == true)
	{// プレイヤーが死んでいたら処理を中断
		return;
	}

	//**************************************************
	// キーボード＆コントローラー入力
	//**************************************************
	float fThumbLX = joypad->Gamepad.sThumbLX;		// 左スティックのX位置
	float fThumbLY = joypad->Gamepad.sThumbLY;		// 左スティックのY位置
	float fNormalizedLX;							// 正規化された左スティックのX位置
	float fNormalizedLY;							// 正規化された左スティックのY位置
	float fJoypadMagnitude;							// ジョイパッドでの移動量
	float fKeyboardMagnitude;						// キーボードでの移動量

	// ジョイパッドでの移動量を取得
	fJoypadMagnitude = sqrt(fThumbLX * fThumbLX + fThumbLY * fThumbLY);

	if (fJoypadMagnitude > INPUT_DEADZONE)
	{// デッドゾーン内で無ければ移動量を正規化
		fNormalizedLX = (fThumbLX) / (32767 - INPUT_DEADZONE);
		fNormalizedLY = (fThumbLY) / (32767 - INPUT_DEADZONE);

	}
	else
	{// デッドゾーン内であれば移動量を0にする（入力を無効にする）
		fJoypadMagnitude = 0.0f;
		fNormalizedLX = 0.0f;
		fNormalizedLY = 0.0f;
	}

	// キーボード&十字キー入力
	if (GetKeyboardPress(DIK_UPARROW) == true || GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
	{// 上
		keyboardInput.y -= 1;
	}
	if (GetKeyboardPress(DIK_DOWNARROW) == true || GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
	{// 下
		keyboardInput.y += 1;
	}
	if (GetKeyboardPress(DIK_LEFTARROW) == true || GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
	{// 左
		keyboardInput.x -= 1;
	}
	if (GetKeyboardPress(DIK_RIGHTARROW) == true || GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
	{// 右
		keyboardInput.x += 1;
	}

	// キーボードでの移動量を取得
	fKeyboardMagnitude = sqrtf(keyboardInput.x * keyboardInput.x + keyboardInput.y * keyboardInput.y);

	if (fKeyboardMagnitude == 0)
	{// キーボードでの移動量が0になっていたら1にする（0除算しないように）
		fKeyboardMagnitude = 1;
	}

	if (fJoypadMagnitude != 0)
	{// もしジョイパッドでの入力が反映する
		g_aPlayer.pos += D3DXVECTOR3(fNormalizedLX, -fNormalizedLY, 0.0f) * PLAYER_SPEED;
	}
	else
	{// ジョイパッドの入力がなければキーボードでの入力を反映する
		g_aPlayer.pos += D3DXVECTOR3(keyboardInput.x / fKeyboardMagnitude, keyboardInput.y / fKeyboardMagnitude, 0.0f) * PLAYER_SPEED;
	}

	// ショット
	g_aPlayer.nLastShot += 1;
	if (GetKeyboardPress(DIK_H) || GetKeyboardPress(DIK_J) || GetJoypadPress(JOYKEY_A) || GetJoypadPress(JOYKEY_X))
	{// スペースキーが押された
		if (g_aPlayer.nLastShot >= SHOOT_INTERVAL)
		{// 前回発射してからSHOOT_INTERVAL分経過していたら発射
			g_aPlayer.nLastShot = 0;

			if (GetKeyboardPress(DIK_H) || GetJoypadPress(JOYKEY_A))
			{
				g_aPlayer.nUpgrade = 0;
			}
			else if (GetKeyboardPress(DIK_J) || GetJoypadPress(JOYKEY_X))
			{
				g_aPlayer.nUpgrade = 1;
			}
			ShootBullet();
		}
	}

	if (g_aPlayer.pos.x < g_aPlayer.size.x / 2)
	{// 左制限
		g_aPlayer.pos.x = g_aPlayer.size.x / 2;
	}
	if (g_aPlayer.pos.x  > SCREEN_WIDTH - g_aPlayer.size.x / 2)
	{// 右制限
		g_aPlayer.pos.x = SCREEN_WIDTH - g_aPlayer.size.x / 2;
	}
	if (g_aPlayer.pos.y < g_aPlayer.size.y / 2)
	{// 上制限
		g_aPlayer.pos.y = g_aPlayer.size.y / 2;
	}
	if (g_aPlayer.pos.y > SCREEN_HEIGHT - g_aPlayer.size.y / 2)
	{// 下制限
		g_aPlayer.pos.y = SCREEN_HEIGHT - g_aPlayer.size.y / 2;
	}

	//**************************************************
	// ポリゴン位置の設定
	//**************************************************
	
	// アニメーションのカウントを増加
	g_aPlayer.nCountAnim++;
	if (g_aPlayer.nCountAnim % TEXTURE_INTERVAL == 0)
	{
		g_aPlayer.nPatternAnim = (g_aPlayer.nPatternAnim + 1) % TEXTURE_MAX_PATTERN;
	}

	// 対角線の長さを算出
	g_fLengthPlayer = sqrtf(TEXTURE_SCALE_X * TEXTURE_SCALE_X + TEXTURE_SCALE_Y * TEXTURE_SCALE_X) * 0.5;

	// 対角線の角度を算出
	g_fAnglePlayer = atan2f(TEXTURE_SCALE_X, TEXTURE_SCALE_Y);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.fRot + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.fRot + D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.fRot + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.fRot + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.fRot - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.fRot - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(g_aPlayer.fRot + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(g_aPlayer.fRot + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	// テクスチャ座標の設定
	pVtx[0].tex.x = 1.0f / TEXTURE_MAX_X * g_aPlayer.nPatternAnim;
	pVtx[0].tex.y = 1.0f / TEXTURE_MAX_Y * (g_aPlayer.nPatternAnim / TEXTURE_MAX_X);

	pVtx[1].tex.x = 1.0f / TEXTURE_MAX_X * (g_aPlayer.nPatternAnim + 1);
	pVtx[1].tex.y = 1.0f / TEXTURE_MAX_Y * (g_aPlayer.nPatternAnim / TEXTURE_MAX_X);

	pVtx[2].tex.x = 1.0f / TEXTURE_MAX_X * g_aPlayer.nPatternAnim;
	pVtx[2].tex.y = 1.0f / TEXTURE_MAX_Y * (g_aPlayer.nPatternAnim / TEXTURE_MAX_X + 1);

	pVtx[3].tex.x = 1.0f / TEXTURE_MAX_X * (g_aPlayer.nPatternAnim + 1);
	pVtx[3].tex.y = 1.0f / TEXTURE_MAX_Y * (g_aPlayer.nPatternAnim / TEXTURE_MAX_X + 1);

	// 頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (g_aPlayer.bVisible == true)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=====================================================================
// プレイヤーのヒット処理
//=====================================================================
void HitPlayer(int nDamage)
{
	if (g_aPlayer.state == PLAYERSTATE_NORMAL)
	{
		g_aPlayer.fLife--;

		if (IsPlayerDead() == true)
		{
			SetExplosion(g_aPlayer.pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			g_aPlayer.state = PLAYERSTATE_DEAD;
			g_aPlayer.nCountState = 0;

			PlaySound(SOUND_LABEL_SE_EXPLOSION);
		}
		else
		{
			g_aPlayer.state = PLAYERSTATE_DAMAGED;
			PlaySound(SOUND_LABEL_SE_DAMAGE);
		}

	}
}

//=====================================================================
// プレイヤーの位置を取得する処理
//=====================================================================
Player* GetPlayer(void)
{
	return &g_aPlayer;
}

//=====================================================================
// プレイヤーが生存しているか確認する処理
//=====================================================================
bool IsPlayerDead(void)
{
	return (g_aPlayer.state == PLAYERSTATE_DEAD || (g_aPlayer.fLife <= 0.0f));
}

//=====================================================================
// 弾発射処理
//=====================================================================
void ShootBullet(void)
{
	PlaySound(SOUND_LABEL_SE_SHOT, 0.25f);

	switch (g_aPlayer.nUpgrade)
	{
	case 1:
		SetBullet(g_aPlayer.pos, SHOOT_SPEED, g_aPlayer.fRot + D3DX_PI, BULLETTYPE_PLAYER, BULLETPATTERN_PLAYER);
		SetBullet(g_aPlayer.pos + D3DXVECTOR3(15, 10, 0), SHOOT_SPEED, g_aPlayer.fRot + D3DX_PI, BULLETTYPE_PLAYER, BULLETPATTERN_PLAYER);
		SetBullet(g_aPlayer.pos + D3DXVECTOR3(-15, 10, 0), SHOOT_SPEED, g_aPlayer.fRot + D3DX_PI, BULLETTYPE_PLAYER, BULLETPATTERN_PLAYER);
		break;

	case 0:		// 5Way
		SetBullet(g_aPlayer.pos, SHOOT_SPEED, g_aPlayer.fRot + D3DX_PI, BULLETTYPE_PLAYER, BULLETPATTERN_PLAYER);
		SetBullet(g_aPlayer.pos + D3DXVECTOR3(sinf(g_aPlayer.fRot), cosf(g_aPlayer.fRot), 0.0f) * -20.0f, SHOOT_SPEED, g_aPlayer.fRot + D3DX_PI + (D3DX_PI * 0.25), BULLETTYPE_PLAYER, BULLETPATTERN_PLAYER);
		SetBullet(g_aPlayer.pos + D3DXVECTOR3(sinf(g_aPlayer.fRot), cosf(g_aPlayer.fRot), 0.0f) * -20.0f, SHOOT_SPEED, g_aPlayer.fRot + D3DX_PI - (D3DX_PI * 0.25), BULLETTYPE_PLAYER, BULLETPATTERN_PLAYER);
		SetBullet(g_aPlayer.pos + D3DXVECTOR3(sinf(g_aPlayer.fRot), cosf(g_aPlayer.fRot), 0.0f) * -20.0f, SHOOT_SPEED, g_aPlayer.fRot + D3DX_PI + (D3DX_PI * 0.75), BULLETTYPE_PLAYER, BULLETPATTERN_PLAYER);
		SetBullet(g_aPlayer.pos + D3DXVECTOR3(sinf(g_aPlayer.fRot), cosf(g_aPlayer.fRot), 0.0f) * -20.0f, SHOOT_SPEED, g_aPlayer.fRot + D3DX_PI - (D3DX_PI * 0.75), BULLETTYPE_PLAYER, BULLETPATTERN_PLAYER);
		break;
	}
}