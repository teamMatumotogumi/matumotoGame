#include "SpriteRenderer.hlsli"

// デカールテクスチャ (シェーダーリソースレジスタ0番にバインド)
Texture2D<float4> _MainTex : register(t0);

// サンプラーステート (サンプラーレジスタ0番にバインド)
SamplerState _MainTexSampler : register(s0);

// 出力ピクセル
struct PSOutput
{
	float4 target0   : SV_TARGET0;		// レンダーターゲット[0]への出力値
};


//----------------------------------------------------------------------------------------------------------------------
// ピクセルシェーダーのエントリーポイント関数
//----------------------------------------------------------------------------------------------------------------------
PSOutput main(in VSOutput input)
{
	// テクスチャ内の指定したテクスチャ座標の色を抽出(サンプリング)する
	const float4 texelColor = _MainTex.Sample(_MainTexSampler, input.texcoord);

	// 出力用変数
	PSOutput output = (PSOutput)0;
	output.target0 = texelColor * input.color;
	return output;
}


