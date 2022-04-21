#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "GraphicsEngine.h"
#include "ConstantBuffer.h"
#include "FrameResources.h"
#include "Sprite.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Color.h"
#include "Mathf.h"

const TypeInfo& SpriteRenderer::GetTypeInfo()
{
    static TypeInfo typeInfo(TypeID::SpriteRenderer, "SpriteRenderer");
    return typeInfo;
}


// 定数バッファのレイアウト
struct SpriteRenderer::ConstantBufferLayout
{
    DirectX::XMFLOAT4X4 world;
    Color   spriteColor;
};


SpriteRenderer::SpriteRenderer()
    : m_sprite(nullptr)
    , m_spriteColor(Color::White)
    , m_isFlippedX(false)
    , m_isFlippedY(false)
{
    // 定数バッファの作成
    m_constantBuffer = new ConstantBuffer(sizeof(ConstantBufferLayout));
}

SpriteRenderer::~SpriteRenderer()
{

}

void SpriteRenderer::SetSprite(Sprite* sprite)
{
    m_sprite = sprite;
}

void SpriteRenderer::SetColor(const Color& color)
{
    m_spriteColor = color;
}


void SpriteRenderer::Render()
{
    // このレンダラーが無効な場合は描画しない
    if (!IsEnabled())
        return;

    // このレンダラーがカメラの視野外にある場合は描画しない
    if (!IsVisible())
        return;

    // スプライトが設定されていない場合は描画しない。
    if (!m_sprite)
        return;

    // このスプライトレンダラーの所有者であるゲームオブジェクトを取得
    const GameObject* owner = GetGameObject();

    // Transformコンポーネントを取得
    const Transform* transform = owner->GetTransform();

    // ワールド変換行列を取得する
    const DirectX::XMFLOAT4X4 localToWorldMatrix = transform->GetLocalToWorldMatrix();

    // 定数バッファに「(転置した)ワールド変換行列」と「スプライトカラー」を書き込む
    ConstantBufferLayout* mapped = (ConstantBufferLayout*)m_constantBuffer->Map();
    Mathf::Transpose(mapped->world, localToWorldMatrix);
    mapped->spriteColor = m_spriteColor;
    m_constantBuffer->Unmap();

    // 現フレーム用のリソースセットを取得する
    FrameResources* frameResources = GraphicsEngine::Instance().GetCurrentFrameResources();

    // 現フレーム用のコマンドリストを取得する
    ID3D12GraphicsCommandList* commandList = frameResources->GetCommandList();

    // 頂点バッファビュー配列を設定する
    const D3D12_VERTEX_BUFFER_VIEW vertexBufferViews[] =
    {
        m_sprite->GetVertexBuffer()->GetVertexBufferView(),
    };
    commandList->IASetVertexBuffers(0, _countof(vertexBufferViews), vertexBufferViews);

    // インデックスバッファビューを設定する
    commandList->IASetIndexBuffer(&m_sprite->GetIndexBuffer()->GetIndexBufferView());

    // プリミティブトポロジーを設定する
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
 
    // ルートパラメーターに従って定数バッファを設定する
    commandList->SetGraphicsRootConstantBufferView(1, m_constantBuffer->GetNativeResource()->GetGPUVirtualAddress());

    ID3D12DescriptorHeap* const DesciptorHeaps[] = { m_sprite->GetTexture()->GetDescriptorHeap() };
    commandList->SetDescriptorHeaps(_countof(DesciptorHeaps), DesciptorHeaps);
    commandList->SetGraphicsRootDescriptorTable(3, m_sprite->GetTexture()->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());

    // ドローコール
    commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);


}


