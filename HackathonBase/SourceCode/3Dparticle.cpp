// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3Dパーティクルの処理[3Dparticle.h]
// Author : Nishiyama Koki
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "3Dparticle.h"
#include "2Deffect.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// マクロ定義
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define PARTICLE_FILEPATH ("data/TEXT/ParticleInfo.txt")	// ファイルパス

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DParticle::PARTICLE_INFO *C3DParticle::m_ParticleOffset[C3DParticle::PARTICLE_ID_MAX] = {};

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DParticle::C3DParticle() : CScene(CScene::PRIORITY::PRIORITY_2DEFFECT)
{
	m_offsetID = C3DParticle::PARTICLE_ID_NONE;	// オフセットID
	m_b2D = false;								// 2D描画状態
	m_nFrameCount = 0;							// フレームのカウント
	m_Origin = ML_VEC3_UNSET;					// 原点
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DParticle::~C3DParticle()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Init(void)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Uninit(void)
{

}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Update(void)
{
	// フレームカウントアップ
	m_nFrameCount++;
	// フレームカウントがパーティクル情報のフレームを超えたら
	if (m_nFrameCount > m_ParticleOffset[m_offsetID]->nFrame)
	{
		// フレームカウント初期化
		m_nFrameCount = 0;
		// 開放処理
		Release();
	}
	// それ以外
	else
	{
		// 変数宣言
		D3DXVECTOR3	move		= ML_VEC3_UNSET;	// 移動量
		D3DXVECTOR3	pos			= ML_VEC3_UNSET;	// 位置
		D3DXVECTOR3 rot			= ML_VEC3_UNSET;	// 回転
		D3DXVECTOR2	size		= ML_VEC2_UNSET;	// サイズ
		D3DXVECTOR2 sizeValue	= ML_VEC2_UNSET;	// サイズ変化値
		D3DXCOLOR	col			= ML_D3DXCOR_SET;	// 色
		D3DXVECTOR3	posRand		= ML_VEC3_UNSET;	// 位置ランダム
		D3DXVECTOR3 vector = ML_VEC3_UNSET;		// ベクトル
		int			nLife		= 0;				// ライフ
		int			nColRand	= 0;				// 色のランダム
		int			nRand = 0;						// ランダム値
		float		fSpeed		= 0;				// 速度
		float		fAngle[3];						// 角度
		float		fAlphaDerease = 0.0f;			// α変化値
		// 初期化
		fAngle[0] = 0;	// 角度0
		fAngle[1] = 0;	// 角度1
		fAngle[2] = 0;	// 角度2

		// エフェクトの個数ループ
		for (int nCntEffect = 0; nCntEffect < m_ParticleOffset[m_offsetID]->nNumber; nCntEffect++)
		{
			/* ライフ設定 */
			nLife = m_ParticleOffset[m_offsetID]->nLife;
			// ランダムのライフを設定
			if (m_ParticleOffset[m_offsetID]->nLifeRand.nX > 0)
			{
				nLife +=
					(rand() % m_ParticleOffset[m_offsetID]->nLifeRand.nX +
						m_ParticleOffset[m_offsetID]->nLifeRand.nY);
			}
			/* カラー設定 */
			col = m_ParticleOffset[m_offsetID]->Col;
			if (m_ParticleOffset[m_offsetID]->bRedRand)
			{
				// ランダム値取得
				nRand = (int)(col.r * 10);
				if (nRand > 0)
				{
					nColRand = rand() % nRand;
					col.r = (float)nColRand / 10;
				}
			}
			if (m_ParticleOffset[m_offsetID]->bGreenRand)
			{
				// ランダム値取得
				nRand = (int)(col.g * 10);
				if (nRand > 0)
				{
					nColRand = rand() % nRand;
					col.g = (float)nColRand / 10;
				}
			}
			if (m_ParticleOffset[m_offsetID]->bBlueRand)
			{
				// ランダム値取得
				nRand = (int)(col.b * 10);
				if (nRand > 0)
				{
					nColRand = rand() % nRand;
					col.b = (float)nColRand / 10;
				}
			}
			if (m_ParticleOffset[m_offsetID]->bAlphaRand)
			{
				// ランダム値取得
				nRand = (int)(col.a * 10);
				if (nRand > 0)
				{
					nColRand = rand() % nRand;
					col.a = (float)nColRand / 10;
				}
			}
			if (m_ParticleOffset[m_offsetID]->bAlphaDecrease)
			{
				// α値の減少
				fAlphaDerease = col.a / nLife;
			}

			/* 半径設定 */
			// 読み込んだサイズを代入
			size = m_ParticleOffset[m_offsetID]->Size;
			// ランダムで設定した分加算
			if (m_ParticleOffset[m_offsetID]->SizeXRand.nX > 0)
			{
				size.x += (float)(rand() % m_ParticleOffset[m_offsetID]->SizeXRand.nX + m_ParticleOffset[m_offsetID]->SizeXRand.nY);
			}
			if (m_ParticleOffset[m_offsetID]->SizeYRand.nX > 0)
			{
				size.y += (float)(rand() % m_ParticleOffset[m_offsetID]->SizeYRand.nX + m_ParticleOffset[m_offsetID]->SizeXRand.nY);
			}
			// サイズ減少状態がtrueなら
			if (m_ParticleOffset[m_offsetID]->bSizeDecrease)
			{
				// サイズ値の減少
				sizeValue = -size / (float)nLife;
			}
			// それ以外
			else
			{
				// サイズの変化値代入
				sizeValue.x = sizeValue.y =
					m_ParticleOffset[m_offsetID]->fSizeChange;
			}
			/* 速度の設定 */
			fSpeed = m_ParticleOffset[m_offsetID]->fSpeed;
			// ランダムのスピード設定
			if (m_ParticleOffset[m_offsetID]->nSpeedRand.nX > 0)
			{
				fSpeed +=
					(float)(rand() % m_ParticleOffset[m_offsetID]->nSpeedRand.nX +
						m_ParticleOffset[m_offsetID]->nSpeedRand.nY);
			}

			/* 角度の設定 */
			fAngle[0] = m_ParticleOffset[m_offsetID]->Rot.x;
			fAngle[1] = m_ParticleOffset[m_offsetID]->Rot.y;
			fAngle[2] = m_ParticleOffset[m_offsetID]->Rot.z;
			// ランダムの角度設定
			if (m_ParticleOffset[m_offsetID]->nAngleRand.nX > 0)
			{
				fAngle[0] +=
					(rand() % m_ParticleOffset[m_offsetID]->nAngleRand.nX +
						m_ParticleOffset[m_offsetID]->nAngleRand.nY)*0.01f;
				fAngle[1] +=
					(rand() % m_ParticleOffset[m_offsetID]->nAngleRand.nX +
						m_ParticleOffset[m_offsetID]->nAngleRand.nY)*0.01f;
				fAngle[2] +=
					(rand() % m_ParticleOffset[m_offsetID]->nAngleRand.nX +
						m_ParticleOffset[m_offsetID]->nAngleRand.nY)*0.01f;
			}

			/* 位置設定 */
			// ランダムの角度設定
			if (m_ParticleOffset[m_offsetID]->PosXRand.nX > 0)
			{
				posRand.x = (float)(rand() % m_ParticleOffset[m_offsetID]->PosXRand.nX + m_ParticleOffset[m_offsetID]->PosXRand.nY);
			}
			if (m_ParticleOffset[m_offsetID]->PosYRand.nX > 0)
			{
				posRand.y = (float)(rand() % m_ParticleOffset[m_offsetID]->PosYRand.nX + m_ParticleOffset[m_offsetID]->PosYRand.nY);
			}
			if (m_ParticleOffset[m_offsetID]->PosZRand.nX > 0)
			{
				posRand.z = (float)(rand() % m_ParticleOffset[m_offsetID]->PosZRand.nX + m_ParticleOffset[m_offsetID]->PosZRand.nY);
			}

			// 位置
			pos =
			{
				posRand.x + m_ParticleOffset[m_offsetID]->Pos.x,
				posRand.y + m_ParticleOffset[m_offsetID]->Pos.y,
				posRand.z + m_ParticleOffset[m_offsetID]->Pos.z
			};

			// パーティクルタイプ
			if (m_ParticleOffset[m_offsetID]->nParticleType == TYPE_ANGLE_TO_VECTOR)
			{
				// 位置によって移動量
				/* 移動量設定 */
				move =
				{
					sinf(fAngle[1])*sinf(fAngle[2])*fSpeed,
					cosf(fAngle[0])*cosf(fAngle[2])*fSpeed,
					cosf(fAngle[0])*sinf(fAngle[1])*fSpeed
				};
			}
			else if (m_ParticleOffset[m_offsetID]->nParticleType == TYPE_ANGLE_AND_VECTOR)
			{
				/* 方向設定 */
				vector = m_ParticleOffset[m_offsetID]->Vector;
				// ランダムの方向設定
				if (m_ParticleOffset[m_offsetID]->VectorXRand.nX > 0)
				{
					vector.x += (float)(rand() % m_ParticleOffset[m_offsetID]->VectorXRand.nX + m_ParticleOffset[m_offsetID]->VectorXRand.nY);
				}
				if (m_ParticleOffset[m_offsetID]->VectorYRand.nX > 0)
				{
					vector.y += (float)(rand() % m_ParticleOffset[m_offsetID]->VectorYRand.nX + m_ParticleOffset[m_offsetID]->VectorYRand.nY);
				}
				if (m_ParticleOffset[m_offsetID]->VectorZRand.nX > 0)
				{
					vector.z += (float)(rand() % m_ParticleOffset[m_offsetID]->VectorZRand.nX + m_ParticleOffset[m_offsetID]->VectorZRand.nY);
				}
				D3DXVec3Normalize(&vector, &vector);
				/* 移動量設定 */
				move = vector * fSpeed;
			}
			// 2D描画状態なら
			if (m_b2D)
			{
				// 2Dエフェクト生成
				C2DEffect::Set2DEffect(
					(C2DEffect::EFFECT_TYPE)m_ParticleOffset[m_offsetID]->nEffectType,
					m_ParticleOffset[m_offsetID]->nEffectTexType,
					pos + m_Origin,
					D3DXVECTOR3(
						fAngle[0],
						fAngle[1],
						fAngle[2]
					),
					move,
					col,
					size,
					nLife,
					m_ParticleOffset[m_offsetID]->Anim,
					m_ParticleOffset[m_offsetID]->Blend,
					sizeValue,
					fAlphaDerease
				);

			}
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Draw(void)
{

}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 生成
//	ParticleId	: パーティクル番号
//	origin		: 初期位置
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DParticle * C3DParticle::Create(
	PARTICLE_ID const	&ParticleId,	// パーティクル番号
	D3DXVECTOR3 const	&origin,		// 初期位置
	bool		const	&b2D			// 2D描画状態
)
{
	// 変数宣言
	C3DParticle *p3DParticle = new C3DParticle;
	// 設定
	p3DParticle->SetParticle(ParticleId);
	p3DParticle->SetOrigin(origin);
	p3DParticle->m_b2D = b2D;
	return p3DParticle;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込み
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C3DParticle::Load(void)
{
	// 変数宣言
	FILE *pFile = NULL;				// ファイルのポインタ
	char cRead[128] = {};				// 読み込み用
	char cComp[128] = {};				// 比較用
	char cEmpty[128] = {};				// 要らないもの用
	char cName[64] = {};					// パーティクル名
	int nCntError = 0;				// エラー用
	int nCntOffset = 0;				// オフセットのカウント
	int nId = 0;					// パーティクル番号
									// 一時保存用
									// ファイルが開かれていなかったら
	if ((pFile = fopen(PARTICLE_FILEPATH, "r")) == NULL)
	{
		return E_FAIL;
	}
	// スクリプトが読み込まれるまでループ
	while (strcmp(cComp, "SCRIPT") != 0)
	{
		// 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);

		// エラーカウントをインクリメント
		nCntError++;
		if (nCntError > 1048576)
		{// エラー
			nCntError = 0;
			return E_FAIL;
		}
	}
	// エンドスクリプトが読み込まれるまでループ
	while (strcmp(cComp, "END_SCRIPT") != 0)
	{// END_SCRIPTまでループ
	 // 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);

		// エラーカウントをインクリメント
		nCntError++;
		if (nCntError > 1048576)
		{// エラー
			nCntError = 0;
			fclose(pFile);
			return E_FAIL;
		}
		// # が読み込まれたら
		else if (strcmp(cComp, "#") == 0)
		{
			// 位置xの最大値と最小値を代入
			sscanf(cRead, "%s [%d] %s", &cEmpty,
				&nId,
				&cName
			);
		}
		else if (strcmp(cComp, "OFFSET") == 0)
		{
			// パーティクルオフセットの情報確保
			m_ParticleOffset[nCntOffset] = new PARTICLE_INFO;
			// パーティクル名の初期化
			cName[0] = '\0';
			while (strcmp(cComp, "END_OFFSET") != 0)
			{
				// 1行読み込む
				fgets(cRead, sizeof(cRead), pFile);
				// 読み込んど文字列代入
				sscanf(cRead, "%s", &cComp);
				// セット位置が来たら
				// ->入る
				if (strcmp(cComp, "SET_POSRAND") == 0)
				{
					// エンドセット位置がくるまで
					// ->ループ
					while (strcmp(cComp, "ENDSET_POSRAND") != 0)
					{
						// 文字列の初期化
						cComp[0] = '\0';
						// 1行読み込む
						fgets(cRead, sizeof(cRead), pFile);
						// 読み込んど文字列代入
						sscanf(cRead, "%s", &cComp);
						// 位置xが読み込まれたら
						if (strcmp(cComp, "POSX") == 0)
						{
							// 位置xの最大値と最小値を代入
							sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->PosXRand.nX,
								&m_ParticleOffset[nCntOffset]->PosXRand.nY
							);
						}
						// 位置yが読み込まれたら
						else if (strcmp(cComp, "POSY") == 0)
						{
							// 位置yの最大値と最小値を代入
							sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->PosYRand.nX,
								&m_ParticleOffset[nCntOffset]->PosYRand.nY
							);
						}
						// 位置zが読み込まれたら
						else if (strcmp(cComp, "POSZ") == 0)
						{
							// 位置zの最大値と最小値を代入
							sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->PosZRand.nX,
								&m_ParticleOffset[nCntOffset]->PosZRand.nY
							);
						}
					}
				}
				// セット色が来たら
				// ->入る
				else if (strcmp(cComp, "SET_COLRAND") == 0)
				{
					// エンドセットサイズがくるまで
					// ->ループ
					while (strcmp(cComp, "ENDSET_COLRAND") != 0)
					{
						// 文字列の初期化
						cComp[0] = '\0';
						// 1行読み込む
						fgets(cRead, sizeof(cRead), pFile);
						// 読み込んど文字列代入
						sscanf(cRead, "%s", &cComp);
						// 赤
						if (strcmp(cComp, "RED") == 0)
						{
							m_ParticleOffset[nCntOffset]->bRedRand = true;
						}
						// 緑
						else if (strcmp(cComp, "GREEN") == 0)
						{
							m_ParticleOffset[nCntOffset]->bGreenRand = true;
						}
						// 青
						else if (strcmp(cComp, "BLUE") == 0)
						{
							m_ParticleOffset[nCntOffset]->bBlueRand = true;
						}
						// 透明度
						else if (strcmp(cComp, "ALPHA") == 0)
						{
							m_ParticleOffset[nCntOffset]->bAlphaRand = true;
						}
					}
				}
				// セットサイズが来たら
				// ->入る
				else if (strcmp(cComp, "SET_SIZERAND") == 0)
				{
					// エンドセットサイズがくるまで
					// ->ループ
					while (strcmp(cComp, "ENDSET_SIZERAND") != 0)
					{
						// 文字列の初期化
						cComp[0] = '\0';
						// 1行読み込む
						fgets(cRead, sizeof(cRead), pFile);
						// 読み込んど文字列代入
						sscanf(cRead, "%s", &cComp);
						// サイズx
						if (strcmp(cComp, "SIZEX") == 0)
						{
							sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->SizeXRand.nX,
								&m_ParticleOffset[nCntOffset]->SizeXRand.nY
							);
						}
						// サイズy
						else if (strcmp(cComp, "SIZEY") == 0)
						{
							sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->SizeYRand.nX,
								&m_ParticleOffset[nCntOffset]->SizeYRand.nY
							);
						}
					}
				}
				// セット角度が来たら
				// ->入る
				else if (strcmp(cComp, "SET_ANGLERAND") == 0)
				{
					// エンドセット角度がくるまで
					// ->ループ
					while (strcmp(cComp, "ENDSET_ANGLERAND") != 0)
					{
						// 文字列の初期化
						cComp[0] = '\0';
						// 1行読み込む
						fgets(cRead, sizeof(cRead), pFile);
						// 読み込んど文字列代入
						sscanf(cRead, "%s", &cComp);
						// 最大角度
						if (strcmp(cComp, "MAX") == 0)
						{
							sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->nAngleRand.nX
							);
						}
						// 最小角度
						else if (strcmp(cComp, "MIN") == 0)
						{
							sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->nAngleRand.nY
							);
						}
					}
				}
				// セット方向が来たら
				// ->入る
				if (strcmp(cComp, "SET_VECTORRAND") == 0)
				{
					// エンドセット方向がくるまで
					// ->ループ
					while (strcmp(cComp, "ENDSET_VECTORRAND") != 0)
					{
						// 文字列の初期化
						cComp[0] = '\0';
						// 1行読み込む
						fgets(cRead, sizeof(cRead), pFile);
						// 読み込んど文字列代入
						sscanf(cRead, "%s", &cComp);
						// 方向xが読み込まれたら
						if (strcmp(cComp, "VECTORX") == 0)
						{
							// 方向xの最大値と最小値を代入
							sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->VectorXRand.nX,
								&m_ParticleOffset[nCntOffset]->VectorXRand.nY
							);
						}
						// 方向yが読み込まれたら
						else if (strcmp(cComp, "VECTORY") == 0)
						{
							// 方向yの最大値と最小値を代入
							sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->VectorYRand.nX,
								&m_ParticleOffset[nCntOffset]->VectorYRand.nY
							);
						}
						// 方向zが読み込まれたら
						else if (strcmp(cComp, "VECTORZ") == 0)
						{
							// 方向zの最大値と最小値を代入
							sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->VectorZRand.nX,
								&m_ParticleOffset[nCntOffset]->VectorZRand.nY
							);
						}
					}
				}
				// セットスピードが来たら
				// ->入る
				else if (strcmp(cComp, "SET_SPEEDRAND") == 0)
				{
					// エンドセットスピードが読み込まれるまで
					// ->ループ
					while (strcmp(cComp, "ENDSET_SPEEDRAND") != 0)
					{
						// 文字列の初期化
						cComp[0] = '\0';
						// 1行読み込む
						fgets(cRead, sizeof(cRead), pFile);
						// 読み込んど文字列代入
						sscanf(cRead, "%s", &cComp);
						// 最大スピードが読み込まれるまで
						if (strcmp(cComp, "MAX") == 0)
						{
							// スピードランダムの最大値を代入
							sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->nSpeedRand.nX
							);
						}
						// 最小スピードが読み込まれるまで
						else if (strcmp(cComp, "MIN") == 0)
						{
							// スピードランダムの最小値を代入
							sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->nSpeedRand.nY
							);
						}
					}
				}
				// セットライフランダムが来たら
				// ->入る
				else if (strcmp(cComp, "SET_LIFERAND") == 0)
				{
					// エンドセットライフランダムがくるまで
					// ->ループ
					while (strcmp(cComp, "ENDSET_LIFERAND") != 0)
					{
						// 文字列の初期化
						cComp[0] = '\0';
						// 1行読み込む
						fgets(cRead, sizeof(cRead), pFile);
						// 読み込んど文字列代入
						sscanf(cRead, "%s", &cComp);
						// 最大ライフ
						if (strcmp(cComp, "MAX") == 0)
						{
							sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->nLifeRand.nX
							);
						}
						// 最小ライフ
						else if (strcmp(cComp, "MIN") == 0)
						{
							sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
								&m_ParticleOffset[nCntOffset]->nLifeRand.nY
							);
						}
					}
				}
				// 位置が読み込まれたら
				else if (strcmp(cComp, "POS") == 0)
				{
					// 位置情報の代入
					sscanf(cRead, "%s %s %f %f %f", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->Pos.x,
						&m_ParticleOffset[nCntOffset]->Pos.y,
						&m_ParticleOffset[nCntOffset]->Pos.z);
					// 文字列の初期化
					cComp[0] = '\0';
				}
				// 回転が読み込まれたら
				else if (strcmp(cComp, "ROT") == 0)
				{
					// 回転情報の代入
					sscanf(cRead, "%s %s %f %f %f", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->Rot.x,
						&m_ParticleOffset[nCntOffset]->Rot.y,
						&m_ParticleOffset[nCntOffset]->Rot.z);
					// 文字列の初期化
					cComp[0] = '\0';
				}
				// 色が読み込まれたら
				else if (strcmp(cComp, "COL") == 0)
				{
					// 色情報の代入
					sscanf(cRead, "%s %s %f %f %f %f", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->Col.r,
						&m_ParticleOffset[nCntOffset]->Col.g,
						&m_ParticleOffset[nCntOffset]->Col.b,
						&m_ParticleOffset[nCntOffset]->Col.a);
					// 文字列の初期化
					cComp[0] = '\0';
				}
				// 色の減少状態が読み込まれたら
				else if (strcmp(cComp, "COL_DESCREASE") == 0)
				{
					// サイズの減少状態をtrueに
					m_ParticleOffset[nCntOffset]->bAlphaDecrease = true;
					// 文字列の初期化
					cComp[0] = '\0';
				}
				// サイズが読み込まれたら
				else if (strcmp(cComp, "SIZE") == 0)
				{
					// サイズの代入
					sscanf(cRead, "%s %s %f %f",
						&cEmpty,
						&cEmpty,
						&m_ParticleOffset[nCntOffset]->Size.x,
						&m_ParticleOffset[nCntOffset]->Size.y
					);
					// 文字列の初期化
					cComp[0] = '\0';
				}
				// サイズの減少が読み込まれたら
				else if (strcmp(cComp, "SIZE_DESCREASE") == 0)
				{
					// サイズの減少状態をtrueに
					m_ParticleOffset[nCntOffset]->bSizeDecrease = true;
					// 文字列の初期化
					cComp[0] = '\0';
				}
				// サイズの変化値が読み込まれたら
				else if (strcmp(cComp, "SIZE_CHANGE") == 0)
				{
					// サイズの変化値の代入
					sscanf(cRead, "%s %s %f",
						&cEmpty,
						&cEmpty,
						&m_ParticleOffset[nCntOffset]->fSizeChange
					);
					// 文字列の初期化
					cComp[0] = '\0';
				}
				// 方向が読み込まれたら
				else if (strcmp(cComp, "VECTOR") == 0)
				{
					// 方向情報の代入
					sscanf(cRead, "%s %s %f %f %f", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->Vector.x,
						&m_ParticleOffset[nCntOffset]->Vector.y,
						&m_ParticleOffset[nCntOffset]->Vector.z);
					// 文字列の初期化
					cComp[0] = '\0';
				}
				// スピードが読み込まれたら
				else if (strcmp(cComp, "SPEED") == 0)
				{
					// スピードの代入
					sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->fSpeed);
					// 文字列の初期化
					cComp[0] = '\0';
				}
				// ライフが読み込まれたら
				else if (strcmp(cComp, "LIFE") == 0)
				{
					// ライフの代入
					sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->nLife);
					// 文字列の初期化
					cComp[0] = '\0';
				}
				// フレームが読み込まれたら
				else if (strcmp(cComp, "FRAME") == 0)
				{
					// フレームの代入
					sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->nFrame);
					// 文字列の初期化
					cComp[0] = '\0';
				}
				// 番号が読み込まれたら
				else if (strcmp(cComp, "NUMBER") == 0)
				{
					// 番号の代入
					sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->nNumber);
					// 文字列の初期化
					cComp[0] = '\0';
				}
				// テクスチャータイプが読み込まれたら
				else if (strcmp(cComp, "TEXTYPE") == 0)
				{
					// テクスチャータイプの代入
					sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->nEffectTexType);
					// 文字列の初期化
					cComp[0] = '\0';
				}
				// アニメーションが読み込まれたら
				else if (strcmp(cComp, "ANIMATION") == 0)
				{
					/*
					// アニメーション情報のNULLチェック
					if (!m_ParticleOffset[nCntOffset]->Anim)
					{
					m_ParticleOffset[nCntOffset]->Anim = new ANIMATION_LOAD;
					}
					*/
					// アニメーション情報代入
					sscanf(cRead, "%s %s %d %d %d", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->Anim.nMaxCntAnim,
						&m_ParticleOffset[nCntOffset]->Anim.nMaxHorizonAnim,
						&m_ParticleOffset[nCntOffset]->Anim.nMaxVirticalAnim
					);
					// 水平アニメーションと垂直アニメーションの設定
					m_ParticleOffset[nCntOffset]->Anim.fHorizonSize = 1.0f / m_ParticleOffset[nCntOffset]->Anim.nMaxHorizonAnim;
					m_ParticleOffset[nCntOffset]->Anim.fVirticalSize = 1.0f / m_ParticleOffset[nCntOffset]->Anim.nMaxVirticalAnim;
					m_ParticleOffset[nCntOffset]->Anim.bUse = true;
					// 文字列の初期化
					cComp[0] = '\0';
				}
				// エフェクトタイプが読み込まれたら
				else if (strcmp(cComp, "EFFECTTYPE") == 0)
				{
					// エフェクトタイプの代入
					sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->nEffectType);
					// 文字列の初期化
					cComp[0] = '\0';
				}
				// パーティクルタイプが読み込まれたら
				else if (strcmp(cComp, "PARTICLETYPE") == 0)
				{
					// パーティクルタイプの代入
					sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->nParticleType);
					// 文字列の初期化
					cComp[0] = '\0';
				}
				// ブレンドタイプが読み込まれたら
				else if (strcmp(cComp, "BLEND") == 0)
				{
					// パーティクルタイプの代入
					sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
						&m_ParticleOffset[nCntOffset]->Blend);
					// 文字列の初期化
					cComp[0] = '\0';
				}
			}
			// オフセットカウントを進める
			nCntOffset++;
		}
	}
	fclose(pFile);
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 全リソース情報の開放
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Unload(void)
{
	// パーティクル情報の開放
	for (int nCntParticle = 0; nCntParticle < C3DParticle::PARTICLE_ID_MAX; nCntParticle++)
	{
		if (m_ParticleOffset[nCntParticle] == NULL)continue;
		delete m_ParticleOffset[nCntParticle];
		m_ParticleOffset[nCntParticle] = NULL;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// パーティクル番号設定
//	ParticleId	: パーティクル番号
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::SetParticle(
	PARTICLE_ID const	&ParticleId	// パーティクル番号
)
{
	m_offsetID = ParticleId;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期位置設定
//	origin	: 初期位置
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::SetOrigin(
	D3DXVECTOR3 const	&origin		// 初期位置
)
{
	m_Origin = origin;
}

#ifdef _DEBUG
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Debug(void)
{
}
#endif // _DEBUG