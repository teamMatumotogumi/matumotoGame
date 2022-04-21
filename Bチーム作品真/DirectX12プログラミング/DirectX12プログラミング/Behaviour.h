#pragma once
#include "Component.h"

//---------------------------------------------------------------------------------------------------------------------------------------------
// ビヘイビアクラス
//
//   ・振る舞いを表すクラスの基底クラス
//   ・派生先として「MonoBehaviour」がある。
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
class Behaviour : public Component
{
private:
	bool m_enabled;				// 更新が有効な場合は true。
	friend class GameObject;	// ゲームオブジェクトクラスは友達

protected:
	// コンストラクタ
	Behaviour();

	// 仮想デストラクタ
	virtual ~Behaviour() = default;

public:
	// このデータ型の情報を返します。
	static const TypeInfo& GetTypeInfo();

	// このインスタンスのデータ型の情報を返します。
	virtual const TypeInfo& GetTypeInfoOfInstance() const { return GetTypeInfo(); }

	// このスクリプトを有効にする場合は true、無効にする場合は false を指定します。
	void SetEnabled(bool enabled) { m_enabled = enabled; }

	// このスクリプトが有効な場合は true を返します。
	bool IsEnabled() const { return m_enabled; }

	// 所属するゲームオブジェクトがアクティブ状態、かつ、このスクリプトが有効な場合は true を返します。
	bool IsActiveAndEnabled() const;
};

