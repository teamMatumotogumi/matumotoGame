#pragma once
#include "PuyoPuyo.PuyoType.h"

// 前方宣言
class SpriteRenderer;

namespace PuyoPuyo
{
	// 前方宣言
	class System;

	enum class MoveDirection
	{
		Left,
		Right,
	};

	// ぷよ1体分を表すクラス
	class Puyo
	{
	private:
		SpriteRenderer* m_renderer;		// ぷよを描画するためのレンダラーへの参照
		PuyoType		m_type;			// ぷよの種類
		float			m_fallSpeed;	// ぷよの現在の落下スピード

	public:
		// コンストラクタ
		Puyo();

		// 新規ぷよを作成します。
		void Create(Transform* parent, PuyoType puyoType = PuyoType::None);

		// Transformコンポーネントを取得します。 (ショートカット)
		Transform* GetTransform() const;

		// ぷよの種類を設定します。
		void SetType(PuyoType type);

		// ぷよの種類を取得します。
		PuyoType GetType() const { return m_type; }

		// 現在の落下スピードを設定します。
		void SetFallSpeed(float fallSpeed) { m_fallSpeed = fallSpeed; }

		// 現在の落下スピードを取得します。
		float GetFallSpeed() const { return m_fallSpeed; }
	};
}

