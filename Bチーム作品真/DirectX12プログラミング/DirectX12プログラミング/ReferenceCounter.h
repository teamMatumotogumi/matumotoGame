﻿#pragma once
#include <wrl.h>

//---------------------------------------------------------------------------------------------------------------------------------------------
// 参照カウントクラス
// 
//      ・比較的に長い寿命を持つオブジェクトの基底クラス。
//      ・そのオブジェクトの参照数を記録して破棄のタイミング(寿命)を管理する。
//---------------------------------------------------------------------------------------------------------------------------------------------
class ReferenceCounter : public IUnknown
{
private:
    // 参照カウント
    ULONG m_referenceCount;

public:
    // コンストラクタ
    ReferenceCounter();

    // 仮想デストラクタ
    virtual ~ReferenceCounter() = default;

    // オブジェクトの参照カウントを取得します。
    ULONG GetReferenceCount() const { return m_referenceCount; }


    //----------------------------------------------------------------------------------------------------------
    // COM(Component Object Model)
    //----------------------------------------------------------------------------------------------------------

    // オブジェクトの参照カウントをインクリメント(1だけ増加)します。
    ULONG __stdcall AddRef() override { return ++m_referenceCount; }

    // オブジェクトの参照カウントをデクリメント(1だけ減少)します。
    // 参照カウントが 0 になった場合はオブジェクトを delete します。
    ULONG __stdcall Release() override;

    // オブジェクトが特定のインターフェースをサポートしているかを識別します。
    // 指定されたインターフェイスがサポートされていた場合は、
    // オブジェクトの参照カウントをインクリメント(1だけ増加)します。
    HRESULT __stdcall QueryInterface(const IID& riid, void** ppvInterface) override { return E_NOTIMPL; }
};
