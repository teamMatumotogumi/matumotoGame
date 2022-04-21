#pragma once
#include <string>

// このゲームエンジン内で使用するデータ型を表す数値
enum class TypeID
{
    //----------------------------
    // Unity準拠
    //----------------------------
    Object = -1,
    Undefined = 0,
    GameObject = 1,
    Component = 2,
    Transform = 4,
    Behaviour = 8,
    Camera = 20,
    Renderer = 25,
    Texture2D = 28,
    MonoBehaviour = 114,
    SpriteRenderer = 212,
    Sprite = 213,

    //----------------------------
    // Unity非準拠
    //----------------------------
    UserTypeIDStart = 2000,
    AxisRenderer,
    GridLinesRenderer,
};


//---------------------------------------------------------------------------------------------------------------------------------------------
// データ型に関する情報クラス
// 
//      ・C/C++にはC#のようなリフレクション機能が無い。
//      ・データ型に関する情報(メタ情報)を取得する手段がないので、
//        それをエミュレートする為の簡易的なクラス。
//        
//---------------------------------------------------------------------------------------------------------------------------------------------
class TypeInfo
{
private:
    // データ型を表す一意な整数値
    TypeID m_typeID;

    // データ型名を表す一意な文字列
    std::string m_typeName;

public:
    // コンストラクタ
    TypeInfo(TypeID typeID, const std::string& typeName);

    // データ型を表す一意な整数値を返します。
    TypeID GetTypeID() const { return m_typeID; }

    // データ型名を表す一意な文字列を返します。
    const std::string& GetTypeName() const { return m_typeName; }
};


