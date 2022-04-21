#pragma once
#include "ReferenceCounter.h"
#include "DepthStencilFormat.h"
#include <d3d12.h>
#include <cstdint>

//---------------------------------------------------------------------------------------------------------------------------------------------
// 深度ステンシルクラス
// 
//      ・カメラを基点とした深度値を格納する為の深度バッファ。
//      ・任意のステンシル値を格納する為のステンシルバッファ。
//      ・一般的に深度バッファとステンシルバッファは1つのリソース内で実現される。
//      ・深度バッファは24ビット以上のフォーマットを持つことが望ましい。
//      ・ステンシルバッファが不要な場合、深度バッファは32ビットが望ましい。
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
class DepthStencil : public ReferenceCounter
{
private:
    ID3D12Resource*         m_depthStencilBuffer;
    ID3D12DescriptorHeap*   m_descriptorHeap;
    uint64_t                m_width;
    uint32_t                m_height;
    DepthStencilFormat      m_format;

private:
    void Create(uint64_t width, uint32_t height, DepthStencilFormat format);
    static DXGI_FORMAT ConvertFrom(DepthStencilFormat format);
    static ID3D12Resource* CreateDepthStencilBuffer(ID3D12Device* d3d12Device, uint64_t width, uint32_t height, DepthStencilFormat format);
    static ID3D12DescriptorHeap* CreateDescriptorHeapForDSV(ID3D12Device* d3d12Device, UINT numDescriptors);

public:
    // 深度ステンシルバッファを生成します。
    //   第1引数 : [in] ピクセル単位での横幅
    //   第2引数 : [in] ピクセル単位での高さ
    //   第3引数 : [in] 深度ステンシルフォーマット
    DepthStencil(uint32_t width, uint32_t height, DepthStencilFormat format);

    /// 仮想デストラクタ
    virtual ~DepthStencil();

    // 深度ステンシルバッファを取得します。
    ID3D12Resource* GetNativeResource() const { return m_depthStencilBuffer; }

    // 深度ステンシルバッファの横幅(単位はピクセル)を取得します。
    uint32_t GetWidth() const { return (uint32_t)m_width; }

    // 深度ステンシルバッファの高さ(単位はピクセル)を取得します。
    uint32_t GetHeight() const { return m_height; }

    // 深度ステンシルフォーマットを取得します。
    DepthStencilFormat GetFormat() const { return m_format; }

    // 深度ステンシルフォーマットを取得します。
    DXGI_FORMAT GetFormatAsDXGI() const { return ConvertFrom(m_format); }

    // 深度ステンシルビュー(DSV)のディスクリプタハンドルを取得します。
    D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle() const { return m_descriptorHeap->GetCPUDescriptorHandleForHeapStart(); }
};