#pragma once
#include "Behaviour.h"
#include "RenderTexture.h"
#include "Color.h"
#include "Rect.h"
#include <d3d12.h>

class FrameResources;

// カメラがどのようにレンダーターゲットをクリアするか。
enum class CameraClearFlags
{
	None,		// 何もしない
	Depth,		// 深度バッファのみをクリアする
	SolidColor,	// 指定した色でクリアする
	Skybox,		// スカイボックスでクリアする
};

// カメラコンポーネントクラス
class Camera : public Behaviour
{
private:
	RenderTexture* m_targetTexture;				// レンダリング先となるテクスチャ。 (nullptr の場合はバックバッファ)
	bool m_orthographic;						// true:正射影  false:透視射影
	float m_fieldOfView;						// 鉛直方向視野角 (単位はラジアン)
	float m_aspect;								// アスペクト比 (= 水平方向視野角 / 鉛直方向視野角)
	float m_nearClipPlane;						// カメラから前方クリッピング平面までの距離
	float m_farClipPlane;						// カメラから後方クリッピング平面までの距離
	float m_orthographicSize;					// カメラの鉛直方向視野サイズの半分。
	CameraClearFlags m_clearFlags;				// レンダーターゲットをクリアする際の方法
	Color m_backgroundColor;					// 背景色 (SolidColorの時のみ有効)
	Rect m_rect;								// 正規化されたビューポート矩形
	mutable DirectX::XMFLOAT4X4 m_viewMatrix;	// ビュー変換行列
	mutable DirectX::XMFLOAT4X4 m_projMatrix;	// プロジェクション変換行列
	friend class Scene;							// Sceneクラスは友達
	friend class GameObject;					// GameObjectクラスは友達

protected:
	// コンストラクタ
	Camera();

	// デストラクタ
	virtual ~Camera();

public:
	// レンダリング先となるテクスチャを取得します。
	RenderTexture* GetTargetTexture() const { return m_targetTexture; }

	// レンダリング先となるテクスチャを設定します。
	void SetTargetTexture(RenderTexture* targetTexture) { m_targetTexture = targetTexture; }

	// プロジェクションタイプが正射影の場合は true を返します。
	bool IsOrthographic() const { return m_orthographic; }

	// プロジェクションタイプを正射影にする場合は true を設定します。
	void SetOrthographic(bool enable) { m_orthographic = enable; }

	// 鉛直方向視野角を取得します。
	float GetFieldOfView() const { return m_fieldOfView; }

	// 鉛直方向視野角を設定します。
	// (プロジェクションタイプが正射影の場合、設定した値は無視されます)
	void SetFieldOfView(float fieldOfView) { m_fieldOfView = fieldOfView; }

	// アスペクト比を取得します。
	float GetAspect() const { return m_aspect; }

	// アスペクト比を設定します。
	void SetAspect(float aspect) { m_aspect = aspect; }

	// カメラから前方クリッピング平面までの距離を取得します。
	float GetNearClipPlane() const { return m_nearClipPlane; }

	// カメラから前方クリッピング平面までの距離を設定します。
	void SetNearClipPlane(float distance) { m_nearClipPlane = distance; }

	// カメラから後方クリッピング平面までの距離を取得します。
	float GetFarClipPlane() const { return m_farClipPlane; }

	// カメラから後方クリッピング平面までの距離を設定します。
	void SetFarClipPlane(float distance) { m_farClipPlane = distance; }

	// 正射影時の鉛直方向視野サイズの半分の値を取得します。
	float GetOrthographicSize() const { return m_orthographicSize; }

	// 正射影時の鉛直方向視野サイズの半分の値を設定します。
	void SetOrthographicSize(float size) { m_orthographicSize = size; }

	// レンダーターゲットのクリア方法を取得します。
	CameraClearFlags GetClearFlags() const { return m_clearFlags; }

	// レンダーターゲットのクリア方法を設定します。
	void SetClearFlags(CameraClearFlags clearFlags) { m_clearFlags = clearFlags; }

	// レンダーターゲットのクリア時の色を取得します。
	const Color& GetBackgroundColor() const { return m_backgroundColor; }

	// レンダーターゲットのクリア時の色を設定します。
	void SetBackgroundColor(const Color& color) { m_backgroundColor = color; }

	// 正規化済みビューポート矩形を取得します。
	// (レンダリング先がテクスチャの場合はこのビューポート矩形は無視され、常にテクスチャ全体にレンダリングされます)
	const Rect& GetNormalizedViewportRect() const { return m_rect; }

	// 正規化済みビューポート矩形を設定します。
	// (レンダリング先がテクスチャの場合はこのビューポート矩形は無視され、常にテクスチャ全体にレンダリングされます)
	void SetNormalizedViewportRect(const Rect& rect) { m_rect = rect; }

	// ビュー変換行列を取得します。
	const DirectX::XMFLOAT4X4& GetViewMatrix() const;

	// プロジェクション変換行列を取得します。
	const DirectX::XMFLOAT4X4& GetProjMatrix() const;

public:
	// Component::OnAttach()をオーバーライドします。
	void OnAttach() override;

	// Component::Render()をオーバーライドします。
	void Render() override {  }

	// このカメラが所属するシーン内の全てのゲームオブジェクトをレンダリングします。
	// このメンバ関数はシーンクラスから呼び出されます。
	void Render(FrameResources* currentFrameResources);

	void SetViewport(ID3D12GraphicsCommandList* commandList);
	void SetScissorRects(ID3D12GraphicsCommandList* commandList);
	void ClearRenderTarget(ID3D12GraphicsCommandList* commandList);
};


