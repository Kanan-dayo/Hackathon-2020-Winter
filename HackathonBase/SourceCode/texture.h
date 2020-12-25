//*************************************************************************************************************
//
// テクスチャ処理[texture.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "manager.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CTexture
{
public:
	enum
	{
		NAME_NONE = -1,					// 無し
		NAME_FRAME,						// 枠
		NAME_GAGE,						// ゲージ
		NAME_NUMBER,					// 数字
		NAME_PLAYER,					// プレイヤー
		NAME_PleasePressButton000,		// ボタンを押してください
		NAME_PAIR,						// 撃破ペア数
		NAME_READY,						// レディ
		NAME_GO,						// ゴー
		NAME_FINISH,					// フィニッシュ

		NAME_RESULT,					// リザルト
		NAME_RESULT_TEXT,				// リザルトのテキスト
		NAME_RESULT_NUMPAIR,			// リザルトの組数

		NAME_RANKING,					// ランキング
		NAME_RANKING_1st,				// 1位
		NAME_RANKING_2nd,				// 2位
		NAME_RANKING_3rd,				// 3位
		NAME_RANKING_4th,				// 4位
		NAME_RANKING_5th,				// 5位

		NAME_HEART,						// ハート
		NAME_STUDENT_0,					// 学生 男 右
		NAME_STUDENT_1,					// 学生 女 左
		NAME_COUPLE_0,					// 夫婦 男 右
		NAME_COUPLE_1,					// 夫婦 女 左
		NAME_NEWLYWED_0,				// 新婚 女 右
		NAME_NEWLYWED_1,				// 新婚 男 左

		NAME_TITLE_BG,					// タイトル背景
		NAME_TITLE_NAME,				// タイトル名
		NAME_TITLE_PRESSSTART,			// タイトルプレススタート

		NAME_STORY_SANTA,				// ストーリーサンタ
		NAME_STORY_SANTAIKARI,			// ストーリーサンタ怒り
		NAME_STORY_SLID1,				// ストーリースライド１
		NAME_STORY_SLID2,				// ストーリースライド２
		NAME_STORY_TUTORIALSKIP,		// ストーリーチュートリアルスキップ

		NAME_TUTORIAL_BG,				// チュートリアル背景
		NAME_TUTORIAL_TUTORIALSKIP,		// チュートリアルスキップ

		NAME_MAX						// 最大数
	};
	enum
	{
		TEXTURE_TOPID_NONE     = -1,						// 無し
		TEXTURE_TOPID_MAX      = NAME_MAX,					// 最大数
	};
	CTexture();																												// コンストラクタ
	~CTexture();																											// デストラクタ
	static void               Load(void);																					// 読み込み
	static void               Unload(void);																					// 開放
	static void               CreateTexture(void);																			// テクスチャの生成
	static LPDIRECT3DTEXTURE9 GetTextureInfo(int nIndex);																	// テクスチャの取得
	static int                GetNumTextureMax(void);																		// テクスチャ数の最大
	static void               GetFileName(const int nIndex, STRING output);													// ファイル名の取得
	static CONST_STRING       GetFileName(const int nIndex);																// ファイル名の取得
private:
	/* メンバ関数 */
	static void               GetResource(void);																			// 資源の調達
	static void               ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData);		// 1行から情報を読み取る
	static void               NumLoad(CONST_STRING cnpLine);																// 個数の読み込み
	static void               FileNameLoad(CONST_STRING cnpLine);															// ファイル名の読み込み
	/* メンバ変数 */
	static MyVector<LPDIRECT3DTEXTURE9> m_pTexture;																			// テクスチャ情報のポインタ
	static MyVector<CString>            m_pFileName;																		// ファイル名のポインタ
	static int                          m_nNumTextureMax;																	// 最大テクスチャ数
};
#endif
