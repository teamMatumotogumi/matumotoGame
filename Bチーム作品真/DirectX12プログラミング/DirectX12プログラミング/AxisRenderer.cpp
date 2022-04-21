#include "AxisRenderer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "Vector3.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mathf.h"
#include "GraphicsEngine.h"
#include "FrameResources.h"


AxisStyle::AxisStyle()
	: m_type(AxisType::Solid)
	, m_color(Color::White)
	, m_length(1.0f)
	, m_negative(false)
{
}


AxisStyle::AxisStyle(AxisType axisType, const Color& axisColor, float axisLength, bool negative)
	: m_type(axisType)
	, m_color(axisColor)
	, m_length(axisLength)
	, m_negative(negative)
{

}


const TypeInfo& AxisRenderer::GetTypeInfo()
{
	static TypeInfo typeInfo(TypeID::AxisRenderer, "AxisRenderer");
	return typeInfo;
}


AxisRenderer::AxisRenderer()
	: m_xStyle(AxisType::Solid, Color::Red,   1.0f, true)
	, m_yStyle(AxisType::Solid, Color::Green, 1.0f, true)
	, m_zStyle(AxisType::Solid, Color::Blue,  1.0f, true)
	, m_vertexBuffer(nullptr)
	, m_vertexCount(0)
	, m_pipelineState(0)
	, m_isGeometryDirty(true)
{

}


AxisRenderer::~AxisRenderer()
{

}


struct AxisRenderer::VertexLayout
{
	Vector3	position;
	Color	color;
};


struct AxisRenderer::ConstantBufferLayout
{
	DirectX::XMFLOAT4X4 world;
};


uint32_t AxisRenderer::AddAxis(VertexLayout vertices[], const AxisStyle& axisStyle, const Vector3& direction)
{
	switch (axisStyle.GetType())
	{
	case AxisType::Solid:
		vertices[0].position = Vector3::Zero;
		vertices[0].color = axisStyle.GetColor();
		vertices[1].position = axisStyle.GetLength() * direction;
		vertices[1].color = axisStyle.GetColor();
		return 2;

	default:
		return 0;
	}
}


void AxisRenderer::UploadGeometryData()
{
	if (m_isGeometryDirty)
	{
		VertexLayout vertices[12];
		memset(vertices, 0, sizeof(vertices));

		m_vertexCount = 0;
		m_vertexCount += AddAxis(&vertices[m_vertexCount], m_xStyle, Vector3::Right);
		m_vertexCount += AddAxis(&vertices[m_vertexCount], m_xStyle, Vector3::Left);
		m_vertexCount += AddAxis(&vertices[m_vertexCount], m_yStyle, Vector3::Up);
		m_vertexCount += AddAxis(&vertices[m_vertexCount], m_yStyle, Vector3::Down);
		m_vertexCount += AddAxis(&vertices[m_vertexCount], m_zStyle, Vector3::Forward);
		m_vertexCount += AddAxis(&vertices[m_vertexCount], m_zStyle, Vector3::Back);

		if (m_vertexCount > 0)
		{
			if (!m_vertexBuffer)
			{
				m_vertexBuffer = new VertexBuffer(sizeof(VertexLayout), _countof(vertices), vertices);
			}
			else
			{
				void* mapped = m_vertexBuffer->Map();
				memcpy(mapped, vertices, sizeof(vertices));
				m_vertexBuffer->Unmap();
			}
		}

		if (!m_constantBuffer)
		{
			m_constantBuffer = new ConstantBuffer(sizeof(ConstantBufferLayout));
		}

		m_isGeometryDirty = false;
	}
}

void AxisRenderer::Render()
{
	// このレンダラーが無効な場合は描画しない
	if (!IsEnabled())
		return;

	// このレンダラーがカメラの視野外にある場合は描画しない
	if (!IsVisible())
		return;

	// ジオメトリなしの場合は描画しない。
	if (!m_vertexCount)
		return;

	UploadGeometryData();

	// このスプライトレンダラーの所有者であるゲームオブジェクトを取得
	GameObject* owner = GetGameObject();

	// Transformコンポーネントを取得
	Transform* transform = owner->GetTransform();

	// ワールド変換行列を取得する
	DirectX::XMFLOAT4X4 localToWorldMatrix = transform->GetLocalToWorldMatrix();

	// ワールド変換行列を転置する
	DirectX::XMFLOAT4X4 localToWorldMatrixTransposed;
	Mathf::Transpose(localToWorldMatrixTransposed, localToWorldMatrix);

	// 定数バッファに「(転置した)ワールド変換行列」と「スプライトカラー」を書き込む
	ConstantBufferLayout* mapped = (ConstantBufferLayout*)m_constantBuffer->Map();
	mapped->world = localToWorldMatrixTransposed;
	m_constantBuffer->Unmap();

	// 現フレーム用のリソースセットを取得する
	FrameResources* frameResources = GraphicsEngine::Instance().GetCurrentFrameResources();

	// 現フレーム用のコマンドリストを取得する
	ID3D12GraphicsCommandList* commandList = frameResources->GetCommandList();

	// 頂点バッファビュー配列を設定する
	const D3D12_VERTEX_BUFFER_VIEW vertexBufferViews[] =
	{
		m_vertexBuffer->GetVertexBufferView(),
	};
	commandList->IASetVertexBuffers(0, _countof(vertexBufferViews), vertexBufferViews);

	// プリミティブトポロジーを設定する
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	// ルートパラメーターに従って定数バッファを設定する
	commandList->SetGraphicsRootConstantBufferView(1, m_constantBuffer->GetNativeResource()->GetGPUVirtualAddress());

//	ID3D12DescriptorHeap* const DesciptorHeaps[] = { m_sprite->GetTexture()->GetDescriptorHeap() };
//	commandList->SetDescriptorHeaps(_countof(DesciptorHeaps), DesciptorHeaps);
//	commandList->SetGraphicsRootDescriptorTable(3, m_sprite->GetTexture()->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());

	// ドローコール
	commandList->DrawInstanced(m_vertexCount, 1, 0, 0);
}



