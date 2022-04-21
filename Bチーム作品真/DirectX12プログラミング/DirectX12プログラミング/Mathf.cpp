#include "Mathf.h"

void Mathf::Transpose(DirectX::XMFLOAT4X4& matrix)
{
    // 4x4型行列の転置
    float temp;
    temp = matrix._12;    matrix._12 = matrix._21;    matrix._21 = temp;
    temp = matrix._13;    matrix._13 = matrix._31;    matrix._31 = temp;
    temp = matrix._14;    matrix._14 = matrix._41;    matrix._41 = temp;
    temp = matrix._23;    matrix._23 = matrix._32;    matrix._32 = temp;
    temp = matrix._24;    matrix._24 = matrix._42;    matrix._42 = temp;
    temp = matrix._34;    matrix._34 = matrix._43;    matrix._43 = temp;
}


void Mathf::Transpose(DirectX::XMFLOAT4X4& destination, const DirectX::XMFLOAT4X4& source)
{
    // 4x4型行列の転置
    destination._11 = source._11;
    destination._12 = source._21;
    destination._13 = source._31;
    destination._14 = source._41;

    destination._21 = source._12;
    destination._22 = source._22;
    destination._23 = source._32;
    destination._24 = source._42;

    destination._31 = source._13;
    destination._32 = source._23;
    destination._33 = source._33;
    destination._34 = source._43;

    destination._41 = source._14;
    destination._42 = source._24;
    destination._43 = source._34;
    destination._44 = source._44;
}