#pragma once
#include "Object.h"


//---------------------------------------------------------------------------------------------------------------------------------------------
// スプライトの一括描画
//
//   ・現在このクラスは実装途中です。
//   ・現在このクラスは使用できません。
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
class SpriteRendererBatch
{
private:
    struct ConstantBufferForSprite;
    struct ConstantBufferForCamera;
    struct SpriteVertex;

    struct Resources
    {
        std::vector<SpriteRenderer*> opaqueGeometries;
        std::vector<SpriteRenderer*> transparentGeometries;
        VertexBuffer* vertexBufer;
        ShaderBytecode* vertexShader;
        ShaderBytecode* pixelShader;
        ID3D12PipelineState* pipelineState;
        ID3D12RootSignature* rootSignature;
        ConstantBuffer* constantBufferForSprite;
        ConstantBuffer* constantBufferForCamera;
    };
    static Resources* s_resources;


private:
    static void CreateCommandListBundle();
    static void CreateDescriptorHeap();
    static void SortOpaqueGeometry();
    static void SortTransparentGeometry();
    static void RenderOpaqueGeometry();
    static void RenderTransparentGeometry();

public:
    static void LoadAssets();
    static void Push(SpriteRenderer* spriteRenderer);
    static void Render();
    static void UnloadAssets();
};
