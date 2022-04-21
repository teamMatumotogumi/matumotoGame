#pragma once
#include "Component.h"
#include <vector>

enum class ShadowCastingMode
{
    Off,                    // このオブジェクトからシャドウをキャストしません。
    On,                     // このオブジェクトからシャドウをキャストします。
    TwoSided,               // このオブジェクトからシャドウをキャストします。両面を処理します。
    ShadowsOnly,            // オブジェクトはシャドウをキャストしますが、それ以外の場合、シーンに表示されません。
};


enum class ReflectionProbeUsage
{
    Off,                    // Reflection Probe は無効。スカイボックスは反射のために使用されます。
    BlendProbes,            // Reflection Probe は有効。プローブ間でのみブレンドが発生します。屋内環境で便利です。
                            // リフレクションの近くに Reflection Probe がない場合、レンダラーはデフォルトのリフレクションを使用してください。
                            // デフォルトの反射と発生するプローブ間のブレンドは行われません。
    BlendProbesAndSkybox,   // Reflection Probe は有効。プローブ間やプローブとデフォルトのリフレクションの間でブレンドが発生します。屋外環境で便利です。
    Simple,                 // Reflection Probe は有効。しかし、2つの重複したボリュームがあるとき、プローブ間でブレンドは発生しません。
};

// 前方宣言
class Material;
class Bounds;

//---------------------------------------------------------------------------------------------------------------------------------------------
// レンダラークラス
//
//   ・各種レンダラーコンポーネントの基底クラス。
//	 ・レンダリング時に参照される0個以上のマテリアルを保持する。
//	 ・ソート時に参照される値を保持する。
// 
//---------------------------------------------------------------------------------------------------------------------------------------------
class Renderer : public Component
{
private:
    std::vector<Material*>	m_materials;		// レンダリング時に参照されるマテリアルの配列
	bool					m_isEnabled;		// レンダリングが有効な場合は true
	bool					m_isVisible;		// カメラにレンダラーが表示されている場合は true
    int						m_sortingLayerID;	// ソート時に使用されるレイヤーのID
    int						m_sortingOrder;		// ソート時に使用される順序を表す値
    friend class GameObject;					// ゲームオブジェクトクラスは友達

protected:
    // コンストラクタ
	Renderer();

    // 仮想デストラクタ
    virtual ~Renderer() = default;

public:
    // このデータ型の情報を返します。
    static const TypeInfo& GetTypeInfo();

    // このインスタンスのデータ型の情報を返します。
    virtual const TypeInfo& GetTypeInfoOfInstance() const { return GetTypeInfo(); }

	// レンダラーのバウンディングボリューム(読み取り専用)
	const Bounds& GetBounds() const;

	// レンダラーが静的にバッチ処理されている場合はtrueを返します(読み取り専用)
	bool IsPartOfStaticBatch() const;

	// カメラにレンダラーが表示されている場合は true を返します (読み取り専用)
	bool IsVisible() const { return m_isVisible; }

	// レンダリングが有効な場合は true を設定します。
	void SetEnabled(bool enabled) { m_isEnabled = enabled; }

	// レンダリングが有効な場合は true を返します。
	bool IsEnabled() const { return m_isEnabled; }

	// このレンダラーに適用されるベイクされたライトマップのインデックスを設定します
	void SetLightmapIndex(int index);

	// このレンダラーに適用されるベイクされたライトマップのインデックスを取得します
	int GetLightmapIndex() const;

	// ライトマップに使用されるUVスケールとオフセットを設定します
	void SetLightmapScaleOffset(const Vector4& scaleOffset) const;

	// ライトマップに使用されるUVスケールとオフセットを取得します
	const Vector4& GetLightmapScaleOffset() const;

	// ローカル座標からワールド座標へ変換する行列（読み取り専用）
	const Matrix4x4& GetLocalToWorldMatrix() const;

	// このレンダラーに適用されるベイクされたリアルタイムライトマップのインデックスを設定します
	void SetRealtimeLightmapIndex(int index) const;

	// このレンダラーに適用されるベイクされたリアルタイムライトマップのインデックスを取得します
	int GetRealtimeLightmapIndex() const;

	// リアルタイムライトマップに使用されるUVスケールとオフセットを設定します
	void SetRealtimeLightmapScaleOffset(const Vector4& scaleOffset) const;

	// リアルタイムライトマップに使用されるUVスケールとオフセットを取得します
	const Vector4& GetRealtimeLightmapScaleOffset() const;

	// このオブジェクトが影の影響を受ける場合はtrueを設定します。
	void SetReceiveShadows(bool enable);

	// このオブジェクトが影の影響を受ける場合はtrueを返します。
	// 通常、オブジェクトは影の影響を受けるべきですが、最適化目的でオフにすることができます。
	// この特定のオブジェクト上で影が必要とされないと分かっている場合です。
	// このフラグは遅延レンダリングパートでは使用されないので注意
	// 全てのオブジェクトはそこで影の影響を受けます。
	bool GetReceiveShadows() const;

	// このレンダラーの為に反射プローブが使用される場合はtrueを設定します
	void SetReflectionProbeUsage(ReflectionProbeUsage reflectionProbeUsage);

	// このレンダラーの為に反射プローブが使用される場合はtrueを返します
	ReflectionProbeUsage GetReflectionProbeUsage() const;

	// どのようにこのオブジェクトからシャドウをキャストするか
	void SetShadowCastingMode(ShadowCastingMode shadowCastingMode);

	// どのようにこのオブジェクトからシャドウをキャストするか
	ShadowCastingMode GetShadowCastingMode() const;

	// このレンダラーで使用する最初のマテリアルを設定します。
	// (引数で指定されたマテリアルを複製してレンダラーが保持します)
	void SetMaterial(const Material* material);

	// このレンダラーで使用する最初のマテリアルを取得します。
	const Material* GetMaterial() const { return m_materials[0]; }

	// このレンダラーで使用するマテリアルを設定します。
	// (引数で指定されたマテリアルを全て複製してレンダラーが保持します)
	void SetMaterials(const std::vector<Material*>& materials);

	// このレンダラーで使用するマテリアルを取得します。
	const std::vector<Material*>& GetMaterials() const;

	// 共有マテリアルを設定します
	// (非推奨！この共有マテリアルを変更すると、他のオブジェクトが使用していた場合にその外観も変更されます。
	//  また、プロジェクトに含まれている場合はその設定も変更されます)
	void SetSharedMaterial(Material* amterial);

	// 共有マテリアルを取得します
	const Material* GetSharedMaterial() const;

	// このオブジェクトの全ての共有マテリアルを取得します
	void SetSharedMaterials(const std::vector<Material*>& materials);

	// このオブジェクトの全ての共有マテリアルを取得します
	const std::vector<Material*>& GetSharedMaterials() const;

	// レンダラーの Sorting Layer のユニークIDを取得します
	// デフォルト値は常に0です
	int GetSortingLayerID() const;

	// レンダラーの Sorting Layer のユニークIDを設定します
	void SetSortingLayerID(int id);

	// レンダラーの Sorting Layer の名前を取得します
	void SetSortingLayerName(const std::string& layerName);

	// レンダラーの Sorting Layer の名前を取得します
	const std::string& GetSortingLayerName() const;

	// Sorting Layerによるレンダラーのオーダー順を設定します
	void SetSortingOrder(int order);

	// Sorting Layerによるレンダラーのオーダー順を取得します
	int GetSortingOrder() const;

	// このレンダラー用にライトプローブが使用される場合はtrueを指定します
	void SetUseLightProbes(bool enable);

	// このレンダラー用にライトプローブが使用される場合はtrueを返します
	bool GetUseLightProbes() const;

	// ワールド座標からローカル座標へ変換した行列（読み取り専用）
	const Matrix4x4& GetWorldToLocalMatrix() const;
};

