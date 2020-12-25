//*************************************************************************************************************
//
// サウンド処理 [sound.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _SOUND_H_
#define _SOUND_H_

#include "manager.h"
//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CSound
{
public:
	// サウンドラベル
	typedef enum
	{
		SOUND_LABEL_NONE = -1,				// 無し
		/* ----- BGM ----- */
		SOUND_LABEL_BGM_TITLE = 0,			// タイトル
		SOUND_LABEL_BGM_STORY,				// ストーリー
		SOUND_LABEL_BGM_TUTORIAL,			// チュートリアル
		SOUND_LABEL_BGM_GAME,				// ゲーム
		SOUND_LABEL_BGM_RESULT,				// リザルト
		/* ----- SE ----- */
		SOUND_LABEL_SE_DECIDE,
		SOUND_LABEL_SE_ENEMYDAMAGE,
		SOUND_LABEL_SE_PLAYERDAMAGE,
		SOUND_LABEL_SE_SMASH,
		/* ----- 最大数 ----- */
		SOUND_LABEL_MAX,
	} SOUND_LABEL;// サウンドラベル

	// サウンドパラメータ
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント BGM = -1 ; SE = 1
	} SOUNDPARAM;

	CSound();																								// コンストラクタ
	~CSound();																								// デストラクタ
	HRESULT     InitSound(HWND hWnd);																		// 初期化
	void        UninitSound(void);																			// 終了
	HRESULT     PlaySound(SOUND_LABEL label);																// セグメント再生(再生中なら停止)
	HRESULT     DoNotOverlapPlaySound(SOUND_LABEL label);													// セグメント再生(同じSEが重ならないようにする)
	void        StopSound(SOUND_LABEL label);																// セグメント停止(ラベル指定)
	void        StopSound(void);																			// セグメント停止(全て)
	static      CSound *Create(HWND hWnd);																	// 生成
private:
	/* メンバ関数 */
	HRESULT     CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);		// チャンクのチェック
	HRESULT     ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);		// チャンクデータの読み込み
	void        UnLoadFileName(void);																		// サウンドファイル名の開放
	static void GetSoundInfo(void);																			// サウンド情報を取得する
	static void ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData);	// 1行から情報を読み取る
	/* メンバ変数 */
	static IXAudio2*               m_pXAudio2;																// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice* m_pMasteringVoice;														// マスターボイス
	static IXAudio2SourceVoice*    m_apSourceVoice[SOUND_LABEL_MAX];										// ソースボイス
	static BYTE*                   m_apDataAudio[SOUND_LABEL_MAX];											// オーディオデータ
	static DWORD                   m_aSizeAudio[SOUND_LABEL_MAX];											// オーディオデータサイズ
	static SOUNDPARAM              m_aParam[SOUND_LABEL_MAX];												// 各音素材のパラメータ
	static int                     m_nNumParam;																// パラメータ数
};

#endif
