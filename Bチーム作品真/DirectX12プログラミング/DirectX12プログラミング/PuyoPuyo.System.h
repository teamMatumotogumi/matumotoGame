#pragma once
#include "PuyoPuyo.PuyoType.h"
#include "Audio.h"
#include <vector>

namespace PuyoPuyo
{
	//---------------------------------------------------------------------------------------------------------------------------------------------
	// PuyoPuyo::Systemクラス
	//
	//		・全体を通して使用するリソースを管理する。
	//		・キャラクター情報を管理する。
	//		・共有音源を管理する。
	// 
	//---------------------------------------------------------------------------------------------------------------------------------------------
	class System
	{
	public:
		// 定数
		static const int PieceStartPositionX = 1;					// ピースの落下開始位置X (単位はセル)
		static const int PieceStartPositionY = 11;					// ピースの落下開始位置Y (単位はセル)
		static const int CellSizeX = 64;							// セルの横幅 (単位はピクセル)
		static const int CellSizeY = 60;							// セルの高さ (単位はピクセル)
		static const int CellNumX = 6;								// フィールド横方向のセル数
		static const int CellNumY = 14;								// フィールド縦方向のセル数
		static const int MaxNumFloatings = CellNumX * CellNumY;		// 浮遊ぷよの最大数
		static constexpr float GravityAcceleration = -0.49f;		// 重力加速度

	public:
		// 効果音ID
		enum class SoundEffectID
		{
			Unknown,				// 謎
			Chain01,				// 1連鎖
			Chain02,				// 2連鎖
			Chain03,				// 3連鎖
			Chain04,				// 4連鎖
			Chain05,				// 5連鎖
			Chain06,				// 6連鎖
			Chain07,				// 7連鎖
			PieceMove,				// 組ぷよの移動音
			PieceRotate,			// 組ぷよの回転音
			MaxNumSoundEffects,		// 効果音の最大数
		};

		// 背景音ID
		enum class BackgroundMusicID
		{
			es38_heppoko,
			MaxNumBackgroundMusics,	// 背景音の最大数
		};

	private:
		static System* s_singletonInstance;	// シングルトンインスタンス
		Sprite* m_puyoSprites[5];			// ぷよスプライト配列
		WORD m_sharedSE[(size_t)SoundEffectID::MaxNumSoundEffects];				// 共有する効果音
		WORD m_sharedBGM[(size_t)BackgroundMusicID::MaxNumBackgroundMusics];	// 共有する背景音

	private:
		// コンストラクタ
		System();

		// デストラクタ
		~System() = default;

		// 効果音をメモリ上に読み込みます。
		void LoadSharedSoundEffects();

		// 背景音をメモリ上に読み込みます。
		void LoadSharedBackgroundMusics();

	public:
		// シングルトンインスタンスを作成します。
		static void CreateSingletonInstance();

		// シングルトンインスタンスを破棄します。
		static void DestroySingletonInstance();

		// シングルトンインスタンスを取得します。
		static System& Instance() { return *s_singletonInstance; }

		// ゲームを起動します。
		void Run();

		// 共有効果音を再生します。
		void PlaySharedSE(SoundEffectID id);

		// 共有背景音を再生します。
		void PlaySharedBGM(BackgroundMusicID id);

		// 指定したタイプのぷよスプライトを取得します。
		Sprite* GetSprite(PuyoType type) const;
	};
}
