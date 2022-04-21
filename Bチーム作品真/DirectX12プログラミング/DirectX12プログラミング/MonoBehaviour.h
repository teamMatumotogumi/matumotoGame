#pragma once
#include "Behaviour.h"

//---------------------------------------------------------------------------------------------------------------------------------------------
// Monoビヘイビアクラス
//
//   ・ゲームオブジェクトの振る舞いを決めるスクリプトの基底クラス。
//   ・ユーザー(プログラマー)は独自のスクリプトクラスでこのクラスを継承し、
//     用意された仮想関数をオーバーライドすることができる。
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
class MonoBehaviour : public Behaviour
{
private:
	friend class Camera;		// カメラクラスは友達
	friend class GameObject;	// ゲームオブジェクトクラスは友達

public:
	// このデータ型の情報を返します。
	static const TypeInfo& GetTypeInfo();

	// このインスタンスのデータ型の情報を返します。
	virtual const TypeInfo& GetTypeInfoOfInstance() const { return GetTypeInfo(); }

	// このクラスの派生クラスである場合は有効なアドレスを返します。
	// このクラスの派生クラスでない場合は nullptr を返します。
	// (派生クラスでこの仮想関数をオーバーライドすることはできません)
	MonoBehaviour* AsMonoBehaviour() const override sealed { return (MonoBehaviour*)this; }

protected:
	// コンストラクタ
	MonoBehaviour() = default;

	// 仮想デストラクタ
	virtual ~MonoBehaviour() = default;

	// ・このコンポーネントがゲームオブジェクトにアタッチされた際に1回だけ呼び出されます。
	// ・Awake()は常にStart()よりも前に呼び出されます。
	virtual void Awake() {  }

	// ・Start()は常にAwake()よりも後に呼び出されます。
	// ・Start()は最初のUpdate()の直前で1回だけ呼び出されます。
	virtual void Start() {  }

	// Update()はMonoBehaviourが「有効(enabled)」の場合に毎フレーム呼び出されます。
	virtual void Update() {  }

protected:
	// カメラがシーンをカリングする直前に呼び出されます。
	virtual void OnPreCull() {  };

	// カメラがシーンをレンダリングする直前に呼び出されます。
	virtual void OnPreRender() {  };

	// カメラがシーンをレンダリングした直後に呼び出されます。
	virtual void OnPostRender() {  };
};
