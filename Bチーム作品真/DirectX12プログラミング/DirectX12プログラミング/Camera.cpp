#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector2.h"
#include "GraphicsEngine.h"
#include "FrameResources.h"
#include "DepthStencil.h"
#include "Scene.h"

using namespace DirectX;

Camera::Camera()
	: m_targetTexture(nullptr)
	, m_orthographic(false)
	, m_fieldOfView(DirectX::XMConvertToRadians(45))
	, m_aspect(1.666f)
	, m_nearClipPlane(0.3f)
	, m_farClipPlane(1000.0f)
	, m_orthographicSize(5.0f)
	, m_clearFlags(CameraClearFlags::SolidColor)
	, m_backgroundColor(Color::Red)
	, m_rect(0.0f, 0.0f, 1.0f, 1.0f)
{
	XMStoreFloat4x4(&m_viewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_projMatrix, XMMatrixIdentity());
}


Camera::~Camera()
{
}


void Camera::OnAttach()
{
	// ゲームオブジェクトの所属するシーンにこのカメラを追加します。
	GetGameObject()->GetScene()->AddCamera(this);
}


const DirectX::XMFLOAT4X4& Camera::GetViewMatrix() const
{
	// このカメラの所有者であるゲームオブジェクトを取得
	const GameObject* gameObject = GetGameObject();

	// そのゲームオブジェクトが持つTransformコンポーネントを取得
	const Transform* transform = gameObject->GetTransform();

	// ビュー変換行列の作成
	m_viewMatrix = transform->GetWorldToLocalMatrix();

	return m_viewMatrix;
}


const DirectX::XMFLOAT4X4& Camera::GetProjMatrix() const
{
	if (m_orthographic)
	{
		// 正射影
		const float viewHeight = m_orthographicSize * 2;
		const float viewWidth = viewHeight * m_aspect;
		const DirectX::XMMATRIX ortho = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, m_nearClipPlane, m_farClipPlane);

		// 「計算用の行列」を「保存用の行列」として格納する
		DirectX::XMStoreFloat4x4(&m_projMatrix, ortho);
	}
	else
	{
		// 透視射影
		const DirectX::XMMATRIX pers = DirectX::XMMatrixPerspectiveFovLH(m_fieldOfView, m_aspect, m_nearClipPlane, m_farClipPlane);

		// 「計算用の行列」を「保存用の行列」として格納する
		DirectX::XMStoreFloat4x4(&m_projMatrix, pers);
	}

	return m_projMatrix;
}



void Camera::SetViewport(ID3D12GraphicsCommandList* commandList)
{
    //---------------------------------------------------------------------------------------------------------------------------------------------
	// 「ビューポート配列の設定」コマンドをコマンドリストに追加する。
	// 
	// ビューポートはレンダーターゲット内のどの場所にレンダリングするのかを表す矩形情報のこと。
	// 画面分割型ゲームを開発する際などに便利。
	//---------------------------------------------------------------------------------------------------------------------------------------------

	D3D12_VIEWPORT viewports[1];
	memset(viewports, 0, sizeof(viewports));
	viewports[0].MinDepth = 0.0f;
	viewports[0].MaxDepth = 1.0f;

	if (m_targetTexture)
	{
		viewports[0].TopLeftX = 0.0f;
		viewports[0].TopLeftY = 0.0f;
		viewports[0].Width = (float)m_targetTexture->GetWidth();
		viewports[0].Height = (float)m_targetTexture->GetHeight();
	}
	else
	{
		const auto& swapChainDesc = GraphicsEngine::Instance().GetSwapChainDesc();
		viewports[0].TopLeftX = swapChainDesc.Width * m_rect.x;
		viewports[0].TopLeftY = swapChainDesc.Height * m_rect.y;
		viewports[0].Width = swapChainDesc.Width * m_rect.width;
		viewports[0].Height = swapChainDesc.Height * m_rect.height;
	}

	commandList->RSSetViewports(_countof(viewports), viewports);
}


void Camera::SetScissorRects(ID3D12GraphicsCommandList* commandList)
{
	//---------------------------------------------------------------------------------------------------------------------------------------------
	// 「シザー矩形配列の設定」コマンドをコマンドリストに追加する。
	//---------------------------------------------------------------------------------------------------------------------------------------------
	D3D12_RECT scissorRects[1];
	memset(scissorRects, 0, sizeof(scissorRects));

	if (m_targetTexture)
	{
		scissorRects[0].left   = 0;
		scissorRects[0].top    = 0;
		scissorRects[0].right  = m_targetTexture->GetWidth();
		scissorRects[0].bottom = m_targetTexture->GetHeight();
	}
	else
	{
		const DXGI_SWAP_CHAIN_DESC1& swapChainDesc = GraphicsEngine::Instance().GetSwapChainDesc();
		scissorRects[0].left   = (LONG)(swapChainDesc.Width  * m_rect.x);
		scissorRects[0].top    = (LONG)(swapChainDesc.Height * m_rect.y);
		scissorRects[0].right  = (LONG)(swapChainDesc.Width  * m_rect.width);
		scissorRects[0].bottom = (LONG)(swapChainDesc.Height * m_rect.height);
	}

	commandList->RSSetScissorRects(_countof(scissorRects), scissorRects);
}



void Camera::ClearRenderTarget(ID3D12GraphicsCommandList* commandList)
{
	FrameResources* currentFrameResources = GraphicsEngine::Instance().GetCurrentFrameResources();

	if (m_targetTexture)
	{

	}
	else
	{


	}

	// 「レンダーターゲットをクリアする(単色で塗りつぶす)」コマンドをコマンドリストに追加する。
	commandList->ClearRenderTargetView(currentFrameResources->GetCPUDescriptorHandleForRTV(), m_backgroundColor.components, 0, nullptr);

	// 「深度ステンシルをクリアする(指定した深度値で塗りつぶす)」コマンドをコマンドリストに追加する。
	//      深度は 1.0f の値でクリアするのが一般的。
	//      ステンシルは 0 の値でクリアするのが一般的。
	const DepthStencil* defaultDepthStencil = GraphicsEngine::Instance().GetDepthStencil();
	commandList->ClearDepthStencilView(defaultDepthStencil->GetCPUDescriptorHandle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
}



void Camera::Render(FrameResources* currentFrameResources)
{
	static const std::function<void(MonoBehaviour*)> OnPreCullFunction = [](MonoBehaviour* monoBehaviour) { monoBehaviour->OnPreRender(); };
	static const std::function<void(MonoBehaviour*)> OnPreRenderFunction = [](MonoBehaviour* monoBehaviour) { monoBehaviour->OnPostRender(); };
	static const std::function<void(MonoBehaviour*)> OnPostRenderFunction = [](MonoBehaviour* monoBehaviour) { monoBehaviour->OnPreCull(); };

	GameObject* gameObject = GetGameObject();
	gameObject->BroadcastMessage(OnPreCullFunction);

	//
	// TODO: ここでフラスタムカリング
	//

	gameObject->BroadcastMessage(OnPreRenderFunction);

	ID3D12GraphicsCommandList* commandList = currentFrameResources->GetCommandList();
	SetViewport(commandList);
	SetScissorRects(commandList);
	ClearRenderTarget(commandList);

	// 「プリミティブトポロジーを設定」コマンドをコマンドリストに追加する。
	//      点リスト          D3D_PRIMITIVE_TOPOLOGY_POINTLIST
	//      線リスト          D3D_PRIMITIVE_TOPOLOGY_LINELIST
	//      線ストリップ      D3D_PRIMITIVE_TOPOLOGY_LINESTRIP
	//      三角形リスト      D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	//      三角形ストリップ  D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 「レンダーターゲットと深度ステンシルの設定」コマンドをコマンドリストに追加する。
	const D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[] = { currentFrameResources->GetCPUDescriptorHandleForRTV(), };
	const D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = GraphicsEngine::Instance().GetDepthStencil()->GetCPUDescriptorHandle();
	commandList->OMSetRenderTargets(_countof(rtvHandles), rtvHandles, FALSE, &dsvHandle);

	// ここでゲームオブジェクトを描画する。
	const std::list<GameObject*>& rootGameObjects = GetGameObject()->GetScene()->GetRootGameObjects();
	for (GameObject* rootGameObject : rootGameObjects)
	{
		rootGameObject->Render();
	}

	gameObject->BroadcastMessage(OnPostRenderFunction);
}
