#include "PuyoPuyo.Puyo.h"
#include "PuyoPuyo.System.h"

namespace PuyoPuyo
{
	Puyo::Puyo()
		: m_type(PuyoType::None)
		, m_renderer(nullptr)
		, m_fallSpeed(0.0f)
	{

	}


	void Puyo::Create(Transform* parent, PuyoType puyoType)
	{
		// ぷよ1体分を表すゲームオブジェクトを作成する。
		GameObject* gameObject = new GameObject();

		// スプライトレンダラーを追加する
		m_renderer = gameObject->AddComponent<SpriteRenderer>();

		// 親子関係と位置を変更する
		Transform* transform = gameObject->GetTransform();
		transform->SetParent(parent);

		// 最初のぷよの種類は「None」
		SetType(PuyoType::None);
	}


	Transform* Puyo::GetTransform() const
	{
		return m_renderer->GetTransform();
	}


	void Puyo::SetType(PuyoType type)
	{
		m_type = type;
		m_renderer->SetSprite(System::Instance().GetSprite(m_type));
	}
}

