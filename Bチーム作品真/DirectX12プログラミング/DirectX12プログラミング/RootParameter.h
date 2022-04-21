#pragma once
#include <d3d12.h>


class RootParameter : public D3D12_ROOT_PARAMETER1
{
public:
    RootParameter(D3D12_ROOT_PARAMETER_TYPE parameterType, D3D12_SHADER_VISIBILITY shaderVisibility)
    {
        this->ParameterType = parameterType;
        this->ShaderVisibility = shaderVisibility;
    }
};


class RootParameterCBV : public RootParameter
{
public:
    // 定数バッファビュー(CBV)としてルートパラメーターを生成します。
    RootParameterCBV(UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL)
        : RootParameter(D3D12_ROOT_PARAMETER_TYPE_CBV, shaderVisibility)
    {
        this->Descriptor.ShaderRegister = shaderRegister;
        this->Descriptor.RegisterSpace = registerSpace;
        this->Descriptor.Flags = D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE;
    }
};


class RootParameterSRV : public RootParameter
{
public:
    // シェーダーリソースビュー(SRV)としてルートパラメーターを生成します。
    RootParameterSRV(UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL)
        : RootParameter(D3D12_ROOT_PARAMETER_TYPE_SRV, shaderVisibility)
    {
        this->Descriptor.ShaderRegister = shaderRegister;
        this->Descriptor.RegisterSpace = registerSpace;
        this->Descriptor.Flags = D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE;
    }
};


class RootParameterUAV : public RootParameter
{
public:
    // アンオーダードアクセスビュー(UAV)としてルートパラメーターを生成します。
    RootParameterUAV(UINT shaderRegister, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL)
        : RootParameter(D3D12_ROOT_PARAMETER_TYPE_UAV, shaderVisibility)
    {
        this->Descriptor.ShaderRegister = shaderRegister;
        this->Descriptor.RegisterSpace = registerSpace;
        this->Descriptor.Flags = D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE;
    }
};


class DescriptorRange : public D3D12_DESCRIPTOR_RANGE1
{
public:
    DescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT baseShaderRegister, UINT numDescriptors, UINT registerSpace = 0, UINT offsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
    {
        this->RangeType = rangeType;
        this->BaseShaderRegister = baseShaderRegister;
        this->NumDescriptors = numDescriptors;
        this->RegisterSpace = registerSpace;
        this->OffsetInDescriptorsFromTableStart = offsetInDescriptorsFromTableStart;
        this->Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE;
    }
};


class RootParameterDescriptorTable : public RootParameter
{
public:
    // ディスクリプタテーブルとしてルートパラメーターを生成します。
    RootParameterDescriptorTable(UINT numDesriptorRanges, const DescriptorRange desriptorRanges[], D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL)
        : RootParameter(D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, shaderVisibility)
    {
        this->DescriptorTable.NumDescriptorRanges = numDesriptorRanges;
        this->DescriptorTable.pDescriptorRanges = desriptorRanges;
    }
};

