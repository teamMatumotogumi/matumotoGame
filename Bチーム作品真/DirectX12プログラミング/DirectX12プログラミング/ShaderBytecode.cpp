#include "ShaderBytecode.h"
#include <d3dcompiler.h>
#include <cstdio>
#include <cassert>


ShaderBytecode::ShaderBytecode(const wchar_t* path, const char* shaderProfile, const char* entryPointName)
{
    ID3DBlob* errorMessage;
    if (FAILED(D3DCompileFromFile(path, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPointName, shaderProfile, 0, 0, &m_shaderBytecode, &errorMessage)))
    {
        printf("[失敗] シェーダーバイトコードの作成\n");
        if (errorMessage)
        {
            printf("%s\n", (const char*)errorMessage->GetBufferPointer());
            errorMessage->Release();
        }
        assert(0);
    }
    printf("[成功] シェーダーバイトコードの作成 (アドレス: 0x%p)\n", m_shaderBytecode);
}


ShaderBytecode::~ShaderBytecode()
{
    if (m_shaderBytecode)
        m_shaderBytecode->Release();
}

