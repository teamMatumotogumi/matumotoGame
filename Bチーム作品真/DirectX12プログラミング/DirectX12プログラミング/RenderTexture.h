#pragma once
#include "RenderTextureDescriptor.h"

enum class RenderTextureReadWrite
{
	Default,	// プロジェクトの設定に基づくデフォルトの色空間の変換。
	Linear,		// レンダーテクスチャにリニア(色以外)のデータが含まれます。カラー変換は行われません。
	sRGB,		// レンダーテクスチャにsRGB(色)データが含まれており、[リニア <-> sRGB]変換が行われます。
};


class RenderBuffer
{
private:
	void* m_nativeBufferPtr;
	friend class RenderTexture;

public:
	// コンストラクタ
	RenderBuffer();
};


//---------------------------------------------------------------------------------------------------------------------------------------------
// レンダーテクスチャクラス
// 
//      ・レンダリング可能なカラーバッファと深度ステンシルバッファを持つ。
//      ・深度ステンシルバッファはオプション。
//		・Cameraコンポーネント等でレンダーターゲットとして指定できる。
//      ・明示的にCreate()が呼び出されていない場合でも、
//        カラーバッファ等が必要になったタイミングで暗黙的にCreate()が呼び出される。
//        
//---------------------------------------------------------------------------------------------------------------------------------------------
class RenderTexture : public Texture
{
private:
	static RenderTexture* s_active;
	RenderTextureDescriptor m_descriptor;
	RenderTextureReadWrite m_readWrite;
	RenderBuffer* m_colorBuffer;
	RenderBuffer* m_depthBuffer;
	bool m_isCreated;

private:
	// 共通のコンストラクション処理
	void Construct();

	// カラーバッファを作成します。
	RenderBuffer* CreateColorBuffer();

	// 深度/ステンシルバッファを作成します。
	RenderBuffer* CreateDepthBuffer();

public:
	// コンストラクタ
	RenderTexture(uint32_t width, uint32_t height, uint32_t depth, RenderTextureFormat format = RenderTextureFormat::Default, RenderTextureReadWrite readWrite = RenderTextureReadWrite::Default);

	// コンストラクタ
	RenderTexture(const RenderTextureDescriptor& descriptor);

	// コンストラクタ
	RenderTexture(const RenderTexture* textureToCopy);

	// 仮想デストラクタ
	virtual ~RenderTexture();

	// 作成済みの場合は true を返します。
	bool IsCreated() const { return m_isCreated; }

	// 実際にRenderTextureオブジェクトを作成します。
	bool Create();

	// レンダーテクスチャ作成に関する詳細情報を取得します。
	const RenderTextureDescriptor& GetDescriptor() const { return m_descriptor; }

	// レンダーテクスチャの横幅を取得します。 (単位はピクセル)
	uint32_t GetWidth() const { return m_descriptor.width; }

	// レンダーテクスチャの高さを取得します。 (単位はピクセル)
	uint32_t GetHeight() const { return m_descriptor.height; }

	// カラーバッファのフォーマットを取得します。
	RenderTextureFormat GetFormat() const { return m_descriptor.colorFormat; }
	
	// 深度/ステンシルバッファの精度を取得します。 (単位はビット)
	uint32_t GetDepth() const { return m_descriptor.depthBufferBits; }

	// カラーバッファを取得します。
	RenderBuffer* GetColorBuffer() const { return m_colorBuffer; }

	// 深度/ステンシルバッファを取得します。
	RenderBuffer* GetDepthBuffer() const { return m_depthBuffer; }

	// グラフィックスAPIに内在する深度バッファリソースを取得します。
	// (Direct3D12の場合はID3D12Resource型ポインタ)
	void* GetNativeDepthBufferPtr() const;

public:
	// descriptorを満たす一時的なレンダーテクスチャを取得します。
	static RenderTexture* GetTemporary(const RenderTextureDescriptor* descriptor);

	// 一時的なレンダーテクスチャを解放します。
	static void ReleaseTemporary(RenderTexture* temp);
};




