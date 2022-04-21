#include "SpriteRenderer.hlsli"

// フレーム定数バッファ
struct Frame
{
	matrix viewMatrix;	// ビュー変換行列
	matrix projMatrix;	// プロジェクション変換行列
};
ConstantBuffer<Frame> cFrame : register(b0);


// オブジェクト定数バッファ
struct Object
{
	matrix worldMatrix;		// ワールド変換行列
	float4 tint;			// 色合い
};
ConstantBuffer<Object> cObject : register(b1);


// 入力頂点
struct VSInput
{
	float2 positionOS   : POSITION;		// モデル空間座標(x, y)
	float2 texcoord		: TEXCOORD;		// テクスチャ座標(u, v)
};


//----------------------------------------------------------------------------------------------------------------------
// 頂点シェーダーのエントリーポイント関数
//----------------------------------------------------------------------------------------------------------------------
VSOutput main(in VSInput input)
{
	// 2次元のオブジェクト空間座標を4次元に拡張 (ただし z=0, w=1)
	const float4 positionOS = float4(input.positionOS, 0, 1);

	// 座標変換
	const float4 positionWS = mul(positionOS, cObject.worldMatrix);	// ワールド空間座標 = モデル空間座標 × ワールド変換行列
	const float4 positionVS = mul(positionWS, cFrame.viewMatrix);	// ビュー空間座標 = ワールド空間座標 × ビュー変換行列
	const float4 positionCS = mul(positionVS, cFrame.projMatrix);	// プロジェクション空間座標 = ビュー空間座標 × プロジェクション変換行列

	// 出力用変数
	VSOutput output = (VSOutput)0;
	output.positionCS = positionCS;
	output.color = cObject.tint;
	output.texcoord = input.texcoord;

	return output;
}

