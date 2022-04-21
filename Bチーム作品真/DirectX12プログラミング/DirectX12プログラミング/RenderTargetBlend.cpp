#include "Precompiled.h"

// 合成なし
const RenderTargetBlend RenderTargetBlend::Opaque
(
	false,
	false,
	BlendFactor::One,
	BlendFactor::Zero,
	BlendOperation::Add,
	BlendFactor::One,
	BlendFactor::Zero,
	BlendOperation::Add,
	LogicOperation::Noop,
	ColorWriteEnable::All
);

// 乗算合成
const RenderTargetBlend RenderTargetBlend::AlphaBlend
(
	true,
	false,
	BlendFactor::SourceAlpha,
	BlendFactor::InverseSourceAlpha,
	BlendOperation::Add,
	BlendFactor::One,
	BlendFactor::Zero,
	BlendOperation::Add,
	LogicOperation::Noop,
	ColorWriteEnable::All
);

// 加算合成
const RenderTargetBlend RenderTargetBlend::Additive
(
	true,
	false,
	BlendFactor::SourceAlpha,
	BlendFactor::One,
	BlendOperation::Add,
	BlendFactor::One,
	BlendFactor::Zero,
	BlendOperation::Add,
	LogicOperation::Noop,
	ColorWriteEnable::All
);

// 乗算済みではない
const RenderTargetBlend RenderTargetBlend::NonPremultiplied
(
	true,
	false,
	BlendFactor::SourceAlpha,
	BlendFactor::InverseSourceAlpha,
	BlendOperation::Add,
	BlendFactor::SourceAlpha,
	BlendFactor::InverseSourceAlpha,
	BlendOperation::Add,
	LogicOperation::Noop,
	ColorWriteEnable::All
);


RenderTargetBlend::RenderTargetBlend()
	: RenderTargetBlend(Opaque)
{
}


RenderTargetBlend::RenderTargetBlend(const RenderTargetBlend& source)
{
	*this = source;
}


RenderTargetBlend::RenderTargetBlend(bool blendEnable, bool logicOpEnable, BlendFactor srcBlend, BlendFactor destBlend, BlendOperation blendOp, BlendFactor srcBlendAlpha, BlendFactor destBlendAlpha, BlendOperation blendOpAlpha, LogicOperation logicOp, ColorWriteEnable renderTargetWriteMask)
{
	this->BlendEnable = blendEnable;
	this->LogicOpEnable = logicOpEnable;
	this->SrcBlend = srcBlend;
	this->DestBlend = destBlend;
	this->BlendOp = blendOp;
	this->SrcBlendAlpha = srcBlendAlpha;
	this->DestBlendAlpha = destBlendAlpha;
	this->BlendOpAlpha = blendOpAlpha;
	this->LogicOp = logicOp;
	this->RenderTargetWriteMask = renderTargetWriteMask;
}

