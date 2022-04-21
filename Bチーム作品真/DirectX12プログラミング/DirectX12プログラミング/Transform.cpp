#include "Transform.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"

const TypeInfo& Transform::GetTypeInfo()
{
    static TypeInfo typeInfo(TypeID::Transform, "Transform");
    return typeInfo;
}


Transform::Transform()
    : m_parent(nullptr)
    , m_localScale(1, 1, 1)
    , m_localRotation(0, 0, 0, 1)
    , m_localPosition(0, 0, 0)
{
    // 単位行列で初期化
    //  | 1  0  0  0 |
    //  | 0  1  0  0 |
    //  | 0  0  1  0 |
    //  | 0  0  0  1 |
    DirectX::XMStoreFloat4x4(&m_localToWorldMatrix, DirectX::XMMatrixIdentity());
    DirectX::XMStoreFloat4x4(&m_worldToLocalMatrix, DirectX::XMMatrixIdentity());
}


void Transform::SetParent(Transform* parent, bool worldPositionStay)
{
    // ワールド変換行列を取得する。
    const DirectX::XMFLOAT4X4& localToWorldMatrix = this->GetLocalToWorldMatrix();

    // ワールド変換行列から「ワールド空間内での位置」を抜き出す。
    //
    //    | _11  _12  _13  _14 |
    //    | _21  _22  _23  _24 |
    //    | _31  _32  _33  _34 |
    //    | _41  _42  _43  _44 |
    //
    const DirectX::XMVECTOR worldPosition = DirectX::XMVectorSet(
        localToWorldMatrix._41,     // ワールド空間内での位置 (x座標)
        localToWorldMatrix._42,     // ワールド空間内での位置 (y座標)
        localToWorldMatrix._43,     // ワールド空間内での位置 (z座標)
        0.0f                        // 未使用なので 0 でよい
    );

    // 既に親がいる場合は、新しい親に切り替える
    if (m_parent)
    {
        m_parent->DetachChild(this);
    }

    // 新しい親が nullptr でない場合は、
    // その親の子として自分を登録する。
    if (parent)
    {
        parent->m_children.push_back(this);
    }

    Transform* before = m_parent;
    Transform* after = parent;

    // 新しい親として記憶しておく
    m_parent = parent;

    // 「親がいない状態」から「親がいる状態」に変化した場合、
    if ((before == nullptr) && (after != nullptr))
    {
        GetGameObject()->GetScene()->RemoveRootGameObject(this->GetGameObject());
    }
    // 「親がいる状態」から「親がいない状態」に変化した場合、
    else if ((before != nullptr) && (after == nullptr))
    {
        GetGameObject()->GetScene()->AddRootGameObject(this->GetGameObject());
    }

    if (worldPositionStay)
    {
        if (m_parent)
        {
            // 「ワールド空間での位置」を「新しい親の空間から見た位置」に逆変換する。
            const DirectX::XMFLOAT4X4& worldToLocalMatrix = m_parent->GetWorldToLocalMatrix();

            // 「保存用の行列」⇒「計算用の行列」に変換する
            const DirectX::XMMATRIX inverse = DirectX::XMLoadFloat4x4(&worldToLocalMatrix);

            // 「新しい親の空間から見た位置」=「ワールド空間での位置」×「逆行列」
            const DirectX::XMVECTOR localPosition = DirectX::XMVector3TransformCoord(worldPosition, inverse);

            // 「計算用のベクトル」⇒「保存用のベクトル」に変換する
            DirectX::XMStoreFloat3(&m_localPosition, localPosition);
        }
        else
        {
            // 親がいなくなった場合は、「ワールド空間での位置」と「ローカル空間での位置」は一致する。
            DirectX::XMStoreFloat3(&m_localPosition, worldPosition);
        }
    }
}

void Transform::DetachChild(Transform* child)
{
    assert(child);

    // 本当に自分の子供か？
    if (child->m_parent != this)
    {
        assert(0);  // 他人の子だ！
    }

    // 指定された子をリストから削除する
    m_children.remove(child);

    // お前の親はいなくなったぞ
    child->m_parent = nullptr;

    // 親がいなくなったのでルートゲームオブジェクトとしてシーンに追加する
    child->GetGameObject()->GetScene()->AddRootGameObject(child->GetGameObject());
}

void Transform::DetachChildren()
{
    // お前らの親はいなくなったぞ
    for (Transform* child : m_children)
    {
        child->m_parent = nullptr;

        // 親がいなくなったのでルートゲームオブジェクトとしてシーンに追加する
        child->GetGameObject()->GetScene()->AddRootGameObject(child->GetGameObject());
    }

    // 全ての子をリストから削除する
    m_children.clear();
}

void Transform::SetLocalScale(const DirectX::XMFLOAT3& localScale)
{
    m_localScale = localScale;
}

void Transform::SetLocalScale(float x, float y, float z)
{
    m_localScale.x = x;
    m_localScale.y = y;
    m_localScale.z = z;
}

void Transform::SetLocalRotation(const DirectX::XMFLOAT4& localRotation)
{
    m_localRotation = localRotation;
}

void Transform::SetLocalPosition(const DirectX::XMFLOAT3& localPosition)
{
    m_localPosition = localPosition;
}

void Transform::SetLocalPosition(float x, float y, float z)
{
    m_localPosition.x = x;
    m_localPosition.y = y;
    m_localPosition.z = z;
}

void Transform::Translate(const DirectX::XMFLOAT3& deltaPosition)
{
    m_localPosition.x += deltaPosition.x;
    m_localPosition.y += deltaPosition.y;
    m_localPosition.z += deltaPosition.z;
}

void Transform::Translate(float dx, float dy, float dz)
{
    m_localPosition.x += dx;
    m_localPosition.y += dy;
    m_localPosition.z += dz;
}


static int SplitPath(std::string_view tokens[256], const char* path)
{
    int i = 0;

    //  "./"は除外する。
    // "../"は不正なパス。
    if (path[0] == '.')
    {
        assert(path[1] == '/');
        i = 2;
    }

    int numTokens = 0;
    int startIndex = i;
    while (path[i])
    {
        if (path[i] == '/')
        {
            std::string_view token{ &path[startIndex], (std::string_view::size_type)(i - startIndex) };
            tokens[numTokens++] = token;
            startIndex = i + 1;
        }
        i++;
    }

    if (i > startIndex)
    {
        std::string_view token{ &path[startIndex], (std::string_view::size_type)(i - startIndex) };
        tokens[numTokens++] = token;
    }

    return numTokens;
}


Transform* Transform::Find(const char* name) const
{
    std::string_view tokens[256];

    // セパレーター文字 '/' で区切る
    const int numTokens = SplitPath(tokens, name);

    Transform* current = (Transform*)this;
    for (int i = 0; i < numTokens; i++)
    {
        current = current->GetChildByName(tokens[i]);
        if (!current)
            break;
    }

    if (current == this)
        return nullptr;
    else
        return current;
}


Transform* Transform::GetChildByName(const std::string_view& name) const
{
    for (Transform* child : m_children)
    {
        const char* childName = child->GetName().c_str();
        if (!_strnicmp(childName, name.data(), name.size()))
        {
            return child;
        }
    }
    return nullptr;
}


void Transform::UpdateMatrices() const
{
    // 「自分のローカル変換行列」を取得する。
    const DirectX::XMMATRIX localMatrix = GetLocalMatrix();

    // 「自分のワールド変換行列」を格納する変数
    DirectX::XMMATRIX localToWorldMatrix;

    if (m_parent)
    {
        // 親Transformがいる場合・・・

        // 「親のワールド変換行列」を取得する
        const DirectX::XMMATRIX parentLocalToWorld = DirectX::XMLoadFloat4x4(&m_parent->GetLocalToWorldMatrix());

        // 「自分のワールド変換行列」 = 「自分のローカル変換行列」 × 「親のワールド変換行列」
        localToWorldMatrix = DirectX::XMMatrixMultiply(localMatrix, parentLocalToWorld);
    }
    else
    {
        // 親Transformがいない場合・・・

        // 「自分のワールド変換行列」 = 「自分のローカル変換行列」
        localToWorldMatrix = localMatrix;
    }

    // 「自分のワールド変換行列」の逆行列を計算する。
    const DirectX::XMMATRIX worldToLocalMatrix = DirectX::XMMatrixInverse(nullptr, localToWorldMatrix);

    // 「計算用変数」の値を変換して「保存用変数」に格納する
    DirectX::XMStoreFloat4x4(&m_localToWorldMatrix, localToWorldMatrix);
    DirectX::XMStoreFloat4x4(&m_worldToLocalMatrix, worldToLocalMatrix);
}


const DirectX::XMMATRIX Transform::GetLocalMatrix() const
{
    // ① スケーリング行列の作成
    //  | sx   0   0  0 |
    //  |  0  sy   0  0 |
    //  |  0   0  sz  0 |
    //  |  0   0   0  1 |
    //
    // DirectX::XMMatrixScaling(X軸方向のスケール値, Y軸方向のスケール値, Z軸方向のスケール値);
    const DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling(m_localScale.x, m_localScale.y, m_localScale.z);

    // ② 回転行列の作成
    //  | r  r  r  0 |
    //  | r  r  r  0 |
    //  | r  r  r  0 |
    //  | 0  0  0  1 |
    //
    // DirectX::XMMatrixRotationQuaternion(四元数)
    const DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&m_localRotation));

    // ③ 平行移動行列の作成
    //  |  1   0   0  0 |
    //  |  0   1   0  0 |
    //  |  0   0   1  0 |
    //  | tx  ty  tz  1 |
    //
    // DirectX::XMMatrixTranslation(位置X, 位置Y, 位置Z);
    const DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(m_localPosition.x, m_localPosition.y, m_localPosition.z);

    // ④ 行列を合成(①×②×③)
    // 
    // DirectX::XMMatrixMultiply(前から掛ける行列, 後ろから掛ける行列);
    return DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(scaling, rotation), translation);
}


const DirectX::XMFLOAT4X4& Transform::GetLocalToWorldMatrix() const
{
    UpdateMatrices();
    return m_localToWorldMatrix;
}


const DirectX::XMFLOAT4X4& Transform::GetWorldToLocalMatrix() const
{
    UpdateMatrices();
    return m_worldToLocalMatrix;
}


void Transform::Traverse(const std::function<void(Transform*)>& visitor)
{
    visitor(this);
    for (Transform* child : m_children)
    {
        child->Traverse(visitor);
    }
}
