#pragma once
#include "PuyoPuyo.Puyo.h"

namespace PuyoPuyo
{
	// 前方宣言
	class PlayerController;

	enum class Direction
	{
		Left,
		Right,
	};

	//「組ぷよ」1個分を表すクラス
	class PuyoPiece
	{
	private:
		PlayerController* m_player;	// この組ぷよの所有者であるプレイヤーへの参照
		GameObject* m_piece;		// この組ぷよを構成しているぷよ達の親ゲームオブジェクト
		Puyo m_puyos[3][3];			// この組ぷよを構成しているぷよ達
		int m_num;					// この組ぷよを構成しているぷよの個数
		bool m_isBig;				// 大ぷよの場合は true

	public:
		// コンストラクタ
		PuyoPiece();

		// 組ぷよを作成します。
		void Create(PlayerController* playerController, Transform* parent);

		// 他の組ぷよからコピーします。
		void CopyTypeFrom(const PuyoPiece& other);

		// この組ぷよを構成しているぷよの個数を取得します。
		int GetNumPuyos() const { return m_num; }

		// 大ぷよの場合は true を返します。
		bool IsBig() const { return m_isBig; }

		// 指定した場所のぷよを取得します。 (const版)
		const Puyo& GetPuyo(int x, int y) const { return m_puyos[y][x]; }

		// 組ぷよをランダムにリセットします。
		void ResetRandomly();

		// Transformコンポーネントを取得します。 (ショートカット)
		Transform* GetTransform() const;

		// セル単位での位置を設定します。
		void SetPositionInCells(int xInCells, int yInCells);

		// 指定された方向に1セル分移動します。
		void Move(Direction direction);

		// 指定された方向に90度回転します。
		void Rotate(Direction direction);

		// 組ぷよをフィールド上に置きます。
		void PlaceOnField();

		// 組ぷよを構成しているぷよのいずれかがフィールドの底に到達したか？
		bool HasReachedBottomOfField() const;

		// 組ぷよを構成している全てのぷよがフィールド内にいるか？
		bool IsInField() const;

		// 組ぷよがフィールド上にぷよと衝突しているか？
		bool IsHitToPuyoPlacedOnField() const;
	};
}