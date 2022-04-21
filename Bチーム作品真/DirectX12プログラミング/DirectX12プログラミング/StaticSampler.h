#pragma once
#include <d3d12.h>

class StaticSampler : public D3D12_STATIC_SAMPLER_DESC
{
public:
    static const StaticSampler PointWrap;
    static const StaticSampler PointClamp;
    static const StaticSampler LinearWrap;
    static const StaticSampler LinearClamp;
    static const StaticSampler AnisotropicWrap;
    static const StaticSampler AnisotropicClamp;
    static const StaticSampler Default;

public:
    // コンストラクタ
    StaticSampler
    (
        UINT                       shaderRegister,
        D3D12_FILTER               filter = D3D12_FILTER_ANISOTROPIC,
        D3D12_TEXTURE_ADDRESS_MODE addressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE addressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE addressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        FLOAT                      mipLODBias = 0.0f,
        UINT                       maxAnisotropy = 16,
        D3D12_COMPARISON_FUNC      comparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL,
        D3D12_STATIC_BORDER_COLOR  borderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
        FLOAT                      minLOD = 0,
        FLOAT                      maxLOD = D3D12_FLOAT32_MAX,
        D3D12_SHADER_VISIBILITY    shaderVisibility = D3D12_SHADER_VISIBILITY_ALL,
        UINT                       registerSpace = 0
    )
    {
        this->Filter = filter;
        this->AddressU = addressU;
        this->AddressV = addressV;
        this->AddressW = addressW;
        this->MipLODBias = mipLODBias;
        this->MaxAnisotropy = maxAnisotropy;
        this->ComparisonFunc = comparisonFunc;
        this->BorderColor = borderColor;
        this->MinLOD = minLOD;
        this->MaxLOD = maxLOD;
        this->ShaderRegister = shaderRegister;
        this->RegisterSpace = registerSpace;
        this->ShaderVisibility = shaderVisibility;
    }
};

