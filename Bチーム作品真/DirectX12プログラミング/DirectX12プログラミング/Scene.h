#pragma once
#include <list>
#include <functional>
#include <DirectXMath.h>

// 前方宣言
class GameObject;
class Camera;
class ConstantBuffer;

//---------------------------------------------------------------------------------------------------------------------------------------------
// シーンクラス
// 
//      ・全てのシーンの基底クラス。
//      ・ファイルからシーンをロードするUnityの仕様とは異なる。
//      ・全てのシーンはシーン管理クラス(SceneManager)によって管理される(予定)
//        
//---------------------------------------------------------------------------------------------------------------------------------------------
class Scene
{
private:
    std::list<GameObject*> m_rootGameObjects;   // ルートゲームオブジェクトリスト
    std::list<Camera*> m_allCameras;            // カメラコンポーネントリスト
    ConstantBuffer* m_constantBufferForCamera;  // 定数バッファ
    std::vector<Transform*> m_allTransforms;    // 作業用
    bool m_isUpdating;                          // Update()実行中は true

    struct ConstantBufferLayoutForCamera;       // 定数バッファレイアウト構造体
    friend class GameObject;                    // GameObjectクラスは友達
    friend class Transform;                     // Transformクラスは友達
    friend class Camera;                        // Cameraクラスは友達

private:
    // 新規ゲームオブジェクトとしてこのシーンに追加します。
    void AddNewGameObject(GameObject* newGameObject);

    // 親を持たないゲームオブジェクトとしてこのシーンに追加します。
    void AddRootGameObject(GameObject* rootGameObject);

    // 指定したルートゲームオブジェクトをこのシーンから削除します。
    void RemoveRootGameObject(GameObject* rootGameObject);

    // ルートゲームオブジェクトのリストへの参照を取得します。
    std::list<GameObject*>& GetRootGameObjects() { return m_rootGameObjects; }

    // ルートゲームオブジェクトのリストへの参照を取得します。 (const版)
    const std::list<GameObject*>& GetRootGameObjects() const { return m_rootGameObjects; }

    // カメラをこのシーンに追加します。
    void AddCamera(Camera* camera);

    // カメラをこのシーンから削除します。
    void RemoveCamera(Camera* camera);

    // このシーンに所属する全てのゲームオブジェクトを走査します。
    void Traverse(const std::function<void (Transform*)>& visitor);

public:
    // コンストラクタ
    Scene();

    // アセットをロードします。
    // (継承先でオーバーライドしてください)
    virtual void LoadAssets();

    // このシーンを更新します。
    // (継承先でオーバーライドしてください)
    virtual void Update();

    // このシーンを描画します。
    // (継承先でオーバーライドしてください)
    virtual void Render();

    // アセットをアンロードします。
    // (継承先でオーバーライドしてください)
    virtual void UnloadAssets();
};
