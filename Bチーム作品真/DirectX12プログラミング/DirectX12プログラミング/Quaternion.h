#pragma once


// 四元数
class Quaternion
{
public:
	// よく使用する四元数を定数で定義しておくと便利
	static const Quaternion Identity;

public:
	float x;
	float y;
	float z;
	float w;

public:
	// デフォルトコンストラクタ
	Quaternion();

	// 引数付きコンストラクタ
	Quaternion(float x, float y, float z, float w);
};


// インライン実装ファイル
#include "Quaternion.inl"


