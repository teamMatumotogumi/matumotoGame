#pragma once

class Matrix4x4
{
public:
	union // 無名共用体
	{
		struct // 無名構造体
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};

public:
	// デフォルトコンストラクタ
	Matrix4x4();

	// 引数付きコンストラクタ
	Matrix4x4(
		float _11, float _12, float _13, float _14,
		float _21, float _22, float _23, float _24,
		float _31, float _32, float _33, float _34,
		float _41, float _42, float _43, float _44
	);
};

