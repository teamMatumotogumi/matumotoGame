#pragma once

//----------------------------------------------------------------------------------
// Cランタイムライブラリ (CRT)
//----------------------------------------------------------------------------------
#include <stdio.h>						// 標準入出力機能。 C言語では<stdio.h>   C++言語では<cstdio>  を使おう。
#include <stdlib.h>						// 疑似乱数など。
#include <time.h>                       // 時間の取得ができる
#include <string.h>						// 文字列操作系の関数
#include <locale.h>                     // ロケール設定(文字列処理などに影響を与える)ができる
#include <tchar.h>                      // 文字セット(マルチバイト文字とワイド文字)の差異を吸収する


//----------------------------------------------------------------------------------
// C++標準ライブラリ
//----------------------------------------------------------------------------------
#include <exception>					// 例外処理
#include <cassert>						// アサート機能
#include <limits>						// 下限値、上限値などの定義


//----------------------------------------------------------------------------------
// C++標準テンプレートライブラリ (STL)
//----------------------------------------------------------------------------------
#include <string>						// 文字列クラス			std::string
#include <vector>						// 可変長配列クラス		std::vector
#include <list>							// 双方向リストクラス	std::list
#include <unordered_map>				// 連想配列				std::unordered_map
#include <unordered_map>				// 集合				    std::unordered_set
#include <stack>						// スタック				std::stack
#include <queue>						// キュー				std::queue
#include <deque>						// デキュー				std::deque
#include <functional>					// クロージャ			std::function
#include <algorithm>
#include <iterator>
#include <memory>
#include <utility>


//----------------------------------------------------------------------------------
// Windows SDK (Software Development Kit)
//----------------------------------------------------------------------------------
#include <windows.h>					// Windowsアプリ開発用API (Application Programming Interfaceの略)
#include <process.h>					// プロセスやスレッドに関するAPI
#include <mmsystem.h>					// マルチメディアに関するAPI
#include <wrl.h>						// COM用のスマートポインタが使える

//----------------------------------------------------------------------------------
// Direct3D 12  (正式名称は DirectXGraphics)
//----------------------------------------------------------------------------------
#include <d3d12.h>						// DirectX12に含まれる「DirectXGraphics」コンポーネントのコアライブラリ
#include <dxgi1_6.h>					// DXGI (Microsoft DirectX Graphics Infrastructureの略) バージョン1.6
#include <d3dcompiler.h>				// シェーダーファイルのコンパイル機能
#include <DirectXMath.h>				// DirectX系の数学ライブラリ

//----------------------------------------------------------------------------------
// 入力デバイス関連
//----------------------------------------------------------------------------------
#include "Keyboard.h"					// キーボードからのキー入力を検出する


//----------------------------------------------------------------------------------
// オーディオ関連
//----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
// ゲームエンジン
//----------------------------------------------------------------------------------

// システム
#include "TypeInfo.h"					// このゲームエンジン内で主要なクラスを表す数値
#include "Object.h"						// このゲームエンジン内の主要なクラスの基底
#include "ReferenceCounter.h"			// オブジェクトの寿命管理 (参照カウント方式)

// 数学
#include "Mathf.h"						// 数学における定数や変換処理などを定義
#include "Color.h"						// RGBAカラー
#include "Rect.h"						// 矩形
#include "Vector2.h"					// 2次元ベクトル
#include "Vector3.h"					// 3次元ベクトル
#include "Vector4.h"					// 4次元ベクトル
#include "Matrix4x4.h"					// 4x4行列

// グラフィックスエンジン
#include "PIX.h"                        // D3D12グラフィックスアナライザー (デバッグ用)
#include "GraphicsEngine.h"				// 2D・3Dグラフィックスエンジン
#include "FrameResources.h"				// フレームごとに使用するリソース
#include "RootSignatureBuilder.h"		// ルートシグネチャ (ビルダーパターン)
#include "RootParameter.h"				// ルートパラメーター
#include "StaticSampler.h"				// スタティックサンプラー
#include "PipelineStateBuilder.h"		// グラフィックスパイプラインステート (ビルダーパターン)
#include "RenderTargetBlend.h"			// レンダーターゲットカラーブレンディング

// バッファリソース
#include "BufferResource.h"				// バッファリソースの基底
#include "VertexBuffer.h"				// 頂点バッファ
#include "IndexBuffer.h"				// インデックスバッファ
#include "ConstantBuffer.h"				// 定数バッファ
#include "DepthStencilFormat.h"			// 深度ステンシルフォーマット
#include "DepthStencil.h"				// 深度ステンシルバッファ

// テクスチャ
#include "FilterMode.h"					// 
#include "GraphicsFormat.h"				// 
#include "TextureWrapMode.h"			// テクスチャの(u,v,w)におけるラップモード
#include "Texture.h"					// 「2次元テクスチャ」と「レンダーテクスチャ」の基底クラス
#include "Texture2D.h"					// 2次元テクスチャ
#include "RenderTextureDescriptor.h"	// レンダーテクスチャ詳細情報
#include "RenderTexture.h"				// レンダーテクスチャ

// シェーダー
#include "ShaderBytecode.h"				// シェーダーバイトコード
#include "ShaderReflection.h"			// シェーダーリフレクション (メタ情報)

// その他
#include "Sprite.h"						// スプライト (= 四角形ポリゴン + テクスチャ)
#include "Material.h"					// マテリアル

// ゲームオブジェクト
#include "GameObject.h"					// ゲームオブジェクト (複数のコンポーネントを付加できる)

// コンポーネント
#include "Component.h"					// 全てのコンポーネントの基底クラス
#include "Transform.h"					// スケール、向き、位置、親子関係を扱う
#include "SpriteRenderer.h"				// スプライトの描画機能を持つ
#include "Camera.h"						// 3Dカメラとしての機能を持つ
#include "Behaviour.h"					// 
#include "MonoBehaviour.h"				// スクリプトの基底クラス

// シーン管理
#include "Scene.h"						// ゲーム内のシーン
#include "SceneManager.h"				// ゲーム内の全てのシーンを管理する


//----------------------------------------------------------------------------------
// 前方宣言
//----------------------------------------------------------------------------------














