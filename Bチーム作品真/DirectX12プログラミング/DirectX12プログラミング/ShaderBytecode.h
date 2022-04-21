#pragma once
#include "ReferenceCounter.h"
#include <d3d12.h>
#include <cstdint>

//---------------------------------------------------------------------------------------------------------------------------------------------
// シェーダーバイトコードクラス
// 
//      ・HLSLで記述されたシェーダープログラムをコンパイルする。
//      ・コンパイルで生成されたシェーダーバイトコードを保持する。
//      ・シェーダーリフレクション等の機能はない。
//        
//---------------------------------------------------------------------------------------------------------------------------------------------
class ShaderBytecode : public ReferenceCounter
{
private:
    ID3DBlob* m_shaderBytecode;

public:
    // コンストラクタ
    //      第1引数 : [in] シェーダーが記述されたHLSLファイルへのパス
    //      第2引数 : [in] シェーダープロファイルを表す文字列
    //      第3引数 : [in] シェーダーのエントリーポイント関数名
    ShaderBytecode(const wchar_t* path, const char* shaderProfile, const char* entryPointName);

    // 仮想デストラクタ
    virtual ~ShaderBytecode();

    // シェーダーバイトコードを取得します。
    const void* GetBufferPointer() const { return m_shaderBytecode->GetBufferPointer(); }

    // シェーダーバイトコードのサイズ(単位はバイト)を取得します。
    SIZE_T GetBufferSize() const { return m_shaderBytecode->GetBufferSize(); }
};

