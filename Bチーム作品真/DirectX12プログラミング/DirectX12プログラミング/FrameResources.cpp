#include "FrameResources.h"
#include <cassert>


FrameResources::FrameResources()
    : m_backBufferIndex(0)
    , m_backBufferResource(nullptr)
    , m_commandAllocator(nullptr)
    , m_commandList(nullptr)
    , m_fence(nullptr)
    , m_fenceEvent(nullptr)
{
}


void FrameResources::WaitForCompletion(ID3D12CommandQueue* commandQueue)
{
    // 今回の目標となるフェンス値 = 前回のフェンス値 + 1
    const UINT64 completionValue = m_fence->GetCompletedValue() + 1;

    // GPUにシグナルコマンドを送信する。
    // GPU処理がこのコマンドまで到達した場合はフェンス値が第2引数の値に更新される。
    if (FAILED(commandQueue->Signal(m_fence, completionValue)))
    {
        assert(0);
    }

    // GPUがまだ処理を完了していない場合は・・・
    if (m_fence->GetCompletedValue() < completionValue)
    {
        // 完了するまで永久に待つ
        if (FAILED(m_fence->SetEventOnCompletion(completionValue, m_fenceEvent)))
        {
            assert(0);
        }
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }
}
