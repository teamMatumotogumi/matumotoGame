#include "GridLinesRenderer.h"
#include "Color.h"


const TypeInfo& GridLinesRenderer::GetTypeInfo()
{
	static TypeInfo typeInfo(TypeID::GridLinesRenderer, "GridLinesRenderer");
	return typeInfo;
}


GridLineStyle::GridLineStyle(GridLineType type, const Color& color)
	: m_type(type)
	, m_color(color)
{

}


struct GridLinesRenderer::VertexLayout
{
	Vector3 position;
	Color   color;
};


struct GridLinesRenderer::ConstantBufferLayout
{
	DirectX::XMFLOAT4X4 world;
};


GridLinesRenderer::GridLinesRenderer()
	: m_majorGridLines(10)
	, m_majorGridLineStyle(GridLineType::Solid, Color::Black)
	, m_majorGridLineInterval(1.0f)
	, m_minorPerMajor(5)
	, m_minorGridLineStyle(GridLineType::Solid, Color::Gray)
	, m_isGeometryDirty(true)
{

}


GridLinesRenderer::~GridLinesRenderer()
{
}


void GridLinesRenderer::UploadGeometryData()
{
	if (m_isGeometryDirty)
	{

		// TODO: ジオメトリデータの生成


		m_isGeometryDirty = false;
	}
}


void GridLinesRenderer::Render()
{
	// このレンダラーが無効な場合は描画しない
	if (!IsEnabled())
		return;

	// このレンダラーがカメラの視野外にある場合は描画しない
	if (!IsVisible())
		return;

	UploadGeometryData();
}
