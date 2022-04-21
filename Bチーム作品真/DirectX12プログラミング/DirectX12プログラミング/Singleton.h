#pragma once
#include <memory>
#include <cassert>

template<typename BaseClass, typename... Args>
class Singleton
{
private:
	// 唯一のインスタンス
	static BaseClass* s_instance;

protected:
	// デフォルトコンストラクタ
	Singleton() = default;

	// 仮想デストラクタ
	virtual ~Singleton() = default;

	// コピーコンストラクタ
	Singleton(const BaseClass&) = delete;

	// ムーブコンストラクタ
	Singleton(BaseClass&&) = delete;

	// 代入演算子
	void operator=(const Singleton&) = delete;

	// ムーブ演算子
	void operator=(Singleton&&) = delete;

public:
	// シングルトンインスタンスを作成します。
	static void CreateSingletonInstance(Args... args)
	{
		assert(!s_instance);
		s_instance = new BaseClass(args...);
	}

	static BaseClass& Instance()
	{
		return *s_instance;
	}

	// シングルトンインスタンスを破棄します。
	static void DestroySingletonInstance()
	{
		assert(s_instance);
		delete s_instance;
		s_instance = nullptr;
	}
};


template<typename BaseClass, typename... Args>
__declspec(selectany) BaseClass* Singleton<BaseClass, Args...>::s_instance = nullptr;

