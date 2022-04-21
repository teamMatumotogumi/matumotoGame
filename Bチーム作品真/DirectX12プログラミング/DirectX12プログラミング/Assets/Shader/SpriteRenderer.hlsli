
struct VSOutput
{
	float4 positionCS	: SV_POSITION;	// プロジェクション空間座標
	float4 color		: COLOR;		// カラー
	float2 texcoord		: TEXCOORD;		// テクスチャ座標
};

