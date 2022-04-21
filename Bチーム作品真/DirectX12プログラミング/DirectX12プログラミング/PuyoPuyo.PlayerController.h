#pragma once
#include "PuyoPuyo.Puyo.h"
#include "PuyoPuyo.PuyoPiece.h"
#include "PuyoPuyo.System.h"
#include <vector>

namespace PuyoPuyo
{
	// プレイヤーインデックス列挙型
	enum class PlayerIndex
	{
		One,	// 1P
		Two,	// 2P
		Three,	// 3P
		Four,	// 4P
	};

	//---------------------------------------------------------------------------------------------------------------------------------------------
	// プレイヤーコントローラー (UnityのC#スクリプトに該当)
	//
	//		・フィールド、スコア、ピースの管理。
	// 
	//---------------------------------------------------------------------------------------------------------------------------------------------
	class PlayerController : public MonoBehaviour
	{
	private:
		// ゲームの進行状態を表す列挙型
		enum class State
		{
			Controllable,	// 操作可能状態
			Falling,		// 浮いているぷよ落下中
			Disappearing,	// ぷよ消滅中
			Lose,			// 負け
			Win,			// 勝ち
		};

	private:
		// ここにメンバ変数を宣言する
		PlayerIndex	m_playerIndex;									// プレイヤーインデックス
		GameObject* m_rotationAxis;									// 回転軸
		State		m_state;										// ゲームの進行状態
		Puyo		m_field[System::CellNumY][System::CellNumX];	// フィールド
		Puyo		m_floating[System::MaxNumFloatings];			// 浮いているぷよ配列
		int			m_numFloatings;									// 浮いているぷよの個数
		PuyoPiece	m_currPiece;									// 現在落下中の組ぷよ
		PuyoPiece	m_nextPiece[2];									// 次に落ちてくる組ぷよ
		bool        m_searched[System::CellNumY][System::CellNumX];	// 探索済みか？
		int			m_chainCount;									// 連鎖数
		friend class Scene;											// シーンクラスは友達
		friend class GameObject;									// ゲームオブジェクトクラスは友達
		friend class PuyoPiece;										// 組ぷよクラスは友達

	private:
		// コンストラクタ
		PlayerController() = default;

		// 仮想デストラクタ
		virtual ~PlayerController() = default;

		// MonoBehaviour::Awake()のオーバーライド
		void Awake() override;

		// MonoBehaviour::Start()のオーバーライド
		void Start() override;

		// MonoBehaviour::Update()のオーバーライド
		void Update() override;

		// 状態が「Controllable」時の更新処理
		void UpdateOnControllable();

		// 状態が「Falling」時の更新処理
		void UpdateOnFalling();

		// 状態が「Disappearing」時の更新処理
		void UpdateOnDisappearing();

		// 状態が「Lose」時の更新処理
		void UpdateOnLose();

		// 状態が「Win」時の更新処理
		void UpdateOnWin();

	public:
		// プレイヤーを作成します。
		void Create(PlayerIndex playerIndex, Transform* parent);

	private:
		// 1Pフレームを作成します。
		void CreateFrame1P(Transform* parent);

		// 2Pフレームを作成します。
		void CreateFrame2P(Transform* parent);

		// フィールドを作成します。
		void CreateField(Transform* parent);

		// 組ぷよを作成します。
		void CreatePuyoPieces(Transform* parent);

		// 浮いているぷよ配列を初期化します。
		void CreateFloatingPuyos(Transform* parent);

	private:
		// このプレイヤーを初期化します。
		void Reset();

		// フィールド上の指定した位置にぷよを置きます。
		void PlacePuyoOnField(int xInCells, int yInCells, PuyoType puyoType);

		// フィールド上の指定した位置にぷよの取得します。
		const Puyo& GetPuyoOnField(int xInCells, int yInCells) const;

		// フィールド上の浮いているぷよを全て探し出す。
		int SearchAllFloatingPuyos(Puyo floatings[System::MaxNumFloatings]);

		// 4個以上繋がっている同じ色のぷよを全て探し出す。
		bool SearchAllLinkedPuyo();

		// 位置を表す構造体
		struct Position
		{
			int x;
			int y;
		};

		// フィールドの(x, y)に指定した色のぷよがいた場合は登録する。
		// この関数は上下左右に再帰的に処理します。
		void SearchLinkedPuyosRecursive(int x, int y, PuyoType type, std::vector<Position>& linked);

		// 「次の組ぷよ」を落とす準備をします。
		void PrepareToDropNextPiece();

		// フィールド上にぷよが積みあがった場合は、負け状態に移行します。
		void TransitToLoseStateIfStackedUp();
	};
}



