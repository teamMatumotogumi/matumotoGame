#include "Precompiled.h"
#include "PuyoPuyo.PlayerController.h"

namespace PuyoPuyo
{
	void PlayerController::Awake()
	{
		//---------------------------------------------------------------------------------------------------------------------------------------------
		// ・コンストラクタの代替関数。主にメンバ変数の初期化を行う。
		// ・GameObject::Find()で他のゲームオブジェクトを探すことができる。
		// ・他のゲームオブジェクトが初期化されているかは不明。
		//---------------------------------------------------------------------------------------------------------------------------------------------
		m_playerIndex = PlayerIndex::One;
		m_rotationAxis = nullptr;
		m_state = State::Controllable;
		m_chainCount = 0;
	}


	void PlayerController::Start()
	{
		//---------------------------------------------------------------------------------------------------------------------------------------------
		// ・記述する内容はAwake()と同じ。
		// ・自身と他のゲームオブジェクトのAwake()は呼び出された後である。
		// ・他のゲームオブジェクトのStart()が呼び出された後かどうかは不明。
		//---------------------------------------------------------------------------------------------------------------------------------------------

	}


	void PlayerController::Update()
	{
		//---------------------------------------------------------------------------------------------------------------------------------------------
		// ・入力デバイス(キーボード、マウス、ゲームパッド)の状態を調べる。
		// ・移動、回転、スケーリングなど。
		// ・ゲームオブジェクトの新規作成
		// ・オーディオの再生開始、停止
		// ・1秒間に約60回呼び出されます。 (60fpsの場合)
		//---------------------------------------------------------------------------------------------------------------------------------------------

		switch (m_state)
		{
		case State::Controllable:
			UpdateOnControllable();
			break;

		case State::Falling:
			UpdateOnFalling();
			break;

		case State::Disappearing:
			UpdateOnDisappearing();
			break;

		case State::Lose:
			UpdateOnLose();
			break;

		case State::Win:
			UpdateOnWin();
			break;
		}
	}


	void PlayerController::UpdateOnControllable()
	{
		float fallSpeed = 4.0f;

		// キーボードの'A'が押されたら…
		if (Keyboard::JustPressed('A'))
		{
			m_currPiece.Move(Direction::Left);
		}

		// キーボードの'D'が押されたら…
		if (Keyboard::JustPressed('D'))
		{
			m_currPiece.Move(Direction::Right);
		}

		// キーボードの'S'が押されたら…
		if (Keyboard::Pressed('S'))
		{
			fallSpeed *= 5;
		}

		// キーボードの十字キーの左が押されたら…
		if (Keyboard::JustPressed(VK_LEFT))
		{
			m_currPiece.Rotate(Direction::Left);
		}

		// キーボードの十字キーの右が押されたら…
		if (Keyboard::JustPressed(VK_RIGHT))
		{
			m_currPiece.Rotate(Direction::Right);
		}

		// 組ぷよを落下させる (Y座標を減らしていく)
		Transform* transform = m_currPiece.GetTransform();

		// 移動前の位置を保存しておく (参照変数はNG)
		const DirectX::XMFLOAT3 localPosition = transform->GetLocalPosition();

		// 落下させる
		transform->Translate(0, -fallSpeed, 0);

		// 「フィールドの底に到達した」または「フィールド上のぷよに衝突した」
		if (m_currPiece.HasReachedBottomOfField() || m_currPiece.IsHitToPuyoPlacedOnField())
		{
			// 移動前の位置に戻す
			transform->SetLocalPosition(localPosition);

			// 組ぷよをフィールドに固定する。
			m_currPiece.PlaceOnField();

			// 浮いているぷよを探し出す
			m_numFloatings = SearchAllFloatingPuyos(m_floating);

			// 浮いているぷよがいる場合は、
			if (m_numFloatings > 0)
			{
				// 組ぷよを画面外に追い出す
				m_currPiece.GetTransform()->SetLocalPosition(-1000, 0, 0);

				// 状態を「Falling」に移行させる
				m_state = State::Falling;
			}
			else
			{
				// 同じ色のぷよが4個以上繋がっている場合は消す
				if (SearchAllLinkedPuyo())
				{
					// 連鎖数のリセット
					m_chainCount = 0;

					// 消したことによって浮いたぷよを探し出す
					m_numFloatings = SearchAllFloatingPuyos(m_floating);
					if (m_numFloatings > 0)
					{
						// 組ぷよを画面外に追い出す
						m_currPiece.GetTransform()->SetLocalPosition(-1000, 0, 0);

						// 状態を「Falling」に移行させる
						m_state = State::Falling;
					}
					else
					{
						//「次の組ぷよ」を落とす準備をする
						PrepareToDropNextPiece();
					}
				}
				else
				{
					//「次の組ぷよ」を落とす準備をする
					PrepareToDropNextPiece();
				}
			}
		}

		TransitToLoseStateIfStackedUp();
	}

	void PlayerController::UpdateOnFalling()
	{
		int skipCount = 0;

		for (int i = 0; i < m_numFloatings; i++)
		{
			// 既にフィールドに固定されたぷよは処理をスキップ
			if (m_floating[i].GetType() == PuyoType::None)
			{
				skipCount++;
				continue;
			}

			// 落下速度に加速度を加える
			m_floating[i].SetFallSpeed(m_floating[i].GetFallSpeed() + System::GravityAcceleration);

			// 現在位置に落下速度を加える
			Transform* transform = m_floating[i].GetTransform();
			transform->Translate(0, m_floating[i].GetFallSpeed(), 0);

			// フィールドの底に到達、または、
			// フィールド上のぷよに衝突した場合は固定する。
			DirectX::XMFLOAT3 localPosition = transform->GetLocalPosition();

			if (localPosition.y <= 0)
			{
				// フィールドの底に到達した

				// このぷよが居るフィールドのセルの位置を求める。
				const int xInCells = (int)(localPosition.x / System::CellSizeX);
				const int yInCells = 0;

				// このセルの位置に固定する
				m_field[yInCells][xInCells].SetType(m_floating[i].GetType());

				// フィールドに固定したのでもう浮いていない
				m_floating[i].SetType(PuyoType::None);
			}
			else
			{
				// このぷよが居るフィールドのセルの位置を求める。
				const int xInCells = (int)(localPosition.x / System::CellSizeX);
				const int yInCells = (int)(localPosition.y / System::CellSizeY);

				// そのセルの位置に他のぷよが居る場合は、
				if (m_field[yInCells][xInCells].GetType() != PuyoType::None)
				{
					// ひとつ上のセルに固定する。
					m_field[yInCells + 1][xInCells].SetType(m_floating[i].GetType());

					// フィールドに固定したのでもう浮いていない
					m_floating[i].SetType(PuyoType::None);
				}
			}
		}

		// 浮いているぷよが全てフィールドに固定されたら
		if (skipCount >= m_numFloatings)
		{
			// 同じ色のぷよが4個以上繋がっている場合は消す
			if (SearchAllLinkedPuyo())
			{
				// 連鎖数をインクリメント
				m_chainCount++;

				// 消したことによって浮いたぷよを探す
				m_numFloatings = SearchAllFloatingPuyos(m_floating);
				if (m_numFloatings == 0)
				{
					//「次の組ぷよ」を落とす準備をする
					PrepareToDropNextPiece();

					// 次の状態に移行する
					m_state = State::Controllable;

					TransitToLoseStateIfStackedUp();

					// 連鎖数をリセット
					m_chainCount = 0;
				}
			}
			else
			{
				//「次の組ぷよ」を落とす準備をする
				PrepareToDropNextPiece();

				// 次の状態に移行する
				m_state = State::Controllable;

				TransitToLoseStateIfStackedUp();

				// 連鎖数をリセット
				m_chainCount = 0;
			}
		}
	}

	void PlayerController::UpdateOnDisappearing()
	{
	}

	void PlayerController::UpdateOnLose()
	{
		Transform* transform = m_rotationAxis->GetTransform();
		transform->Translate(0, -5, 0);

		// フィールドをZ軸周りに15°傾ける
		const DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(0, 0, 1, 0); // 回転軸ベクトル
		constexpr float rotationAngle = DirectX::XMConvertToRadians(-15);		 // 回転角度
		const DirectX::XMVECTOR rotation = DirectX::XMQuaternionRotationAxis(rotationAxis, rotationAngle);
		DirectX::XMFLOAT4 localRotation;
		DirectX::XMStoreFloat4(&localRotation, rotation);
		transform->SetLocalRotation(localRotation);
	}

	void PlayerController::UpdateOnWin()
	{
	}


	void PlayerController::Create(PlayerIndex playerIndex, Transform* parent)
	{
		assert(parent);
		m_playerIndex = playerIndex;

		// フィールド回転軸
		m_rotationAxis = new GameObject("フィールド回転軸");
		m_rotationAxis->GetTransform()->SetParent(parent->GetTransform());

		// フィールド原点
		GameObject* fieldOrigin = new GameObject("フィールド原点");
		fieldOrigin->GetTransform()->SetParent(m_rotationAxis->GetTransform());
		fieldOrigin->GetTransform()->SetLocalPosition(-196, -4, 0);

		// フィールド背景
		Texture2D* fieldBGTexture = Texture2D::FromFile(L"Assets/PuyoPuyo/Textures/puyo/field_bg2p/field_bg2p_arl.tzip/field_arl.png");
		GameObject::CreateWithSprite("フィールド背景", fieldBGTexture, Rect(0, 0, 400, 725), Vector2(0.0f, 0.0f), 1.0f, Vector3(-200, 0, 0), m_rotationAxis->GetTransform());

		switch (m_playerIndex)
		{
		case PlayerIndex::One:
			m_rotationAxis->GetTransform()->SetLocalPosition(472, 198, 0);
			CreateFrame1P(m_rotationAxis->GetTransform());
			break;

		case PlayerIndex::Two:
			m_rotationAxis->GetTransform()->SetLocalPosition(1920 - 472, 198, 0);
			CreateFrame2P(m_rotationAxis->GetTransform());
			break;

		default:
			assert(0);
			break;
		}

		CreateField(fieldOrigin->GetTransform());
		CreatePuyoPieces(fieldOrigin->GetTransform());
		CreateFloatingPuyos(fieldOrigin->GetTransform());
		Reset();
	}


	void PlayerController::CreateFrame1P(Transform* parent)
	{
		// 各種テクスチャのロード
		Texture2D* frameTexture = Texture2D::FromFile(L"Assets/PuyoPuyo/Textures/puyo/puyo2P/puyo2P.tzip/win_field_puyo_d4444.png");
		Texture2D* nextPieceTexture = Texture2D::FromFile(L"Assets/PuyoPuyo/Textures/puyo/puyo2P/puyo2P.tzip/pla_next_d4444.png");
		Texture2D* namePlateTexture = Texture2D::FromFile(L"Assets/PuyoPuyo/Textures/puyo/puyo2P/puyo2P.tzip/pla_username_d4444.png");

		// 1P
		GameObject::CreateWithSprite("1P枠上部", frameTexture, Rect(5, 4, 436, 52), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217, 724, 0), parent);
		GameObject::CreateWithSprite("1P枠左上", frameTexture, Rect(451, 4, 25, 356), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217, 372, 0), parent);
		GameObject::CreateWithSprite("1P枠左下", frameTexture, Rect(571, 4, 25, 364), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217, 8, 0), parent);
		GameObject::CreateWithSprite("1P枠右上", frameTexture, Rect(476, 4, 25, 356), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217 + 417, 372, 0), parent);
		GameObject::CreateWithSprite("1P枠右下", frameTexture, Rect(596, 4, 25, 364), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217 + 417, 8, 0), parent);
		GameObject::CreateWithSprite("1P枠下部", frameTexture, Rect(5, 64, 436, 64), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217, -56, 0), parent);
		GameObject::CreateWithSprite("1Pネクスト背景", nextPieceTexture, Rect(330, 4, 160, 300), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217 + 438, 446, 0), parent);
		GameObject::CreateWithSprite("1Pネクスト枠", nextPieceTexture, Rect(0, 4, 160, 306), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217 + 440, 438, 0), parent);
		GameObject::CreateWithSprite("1Pネームプレート", namePlateTexture, Rect(0, 0, 560, 64), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217 - 61, -128, 0), parent);
	}


	void PlayerController::CreateFrame2P(Transform* parent)
	{
		// 各種テクスチャのロード
		Texture2D* frameTexture = Texture2D::FromFile(L"Assets/PuyoPuyo/Textures/puyo/puyo2P/puyo2P.tzip/win_field_puyo_d4444.png");
		Texture2D* nextPieceTexture = Texture2D::FromFile(L"Assets/PuyoPuyo/Textures/puyo/puyo2P/puyo2P.tzip/pla_next_d4444.png");
		Texture2D* namePlateTexture = Texture2D::FromFile(L"Assets/PuyoPuyo/Textures/puyo/puyo2P/puyo2P.tzip/pla_username_d4444.png");

		// 2P
		GameObject::CreateWithSprite("2P枠上部", frameTexture, Rect(5, 4 + 136, 436, 52), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217, 724, 0), parent);
		GameObject::CreateWithSprite("2P枠左上", frameTexture, Rect(451 + 60, 4, 25, 356), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217, 372, 0), parent);
		GameObject::CreateWithSprite("2P枠左下", frameTexture, Rect(571 + 60, 4, 25, 364), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217, 8, 0), parent);
		GameObject::CreateWithSprite("2P枠右上", frameTexture, Rect(476 + 60, 4, 25, 356), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217 + 417, 372, 0), parent);
		GameObject::CreateWithSprite("2P枠右下", frameTexture, Rect(596 + 60, 4, 25, 364), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217 + 417, 8, 0), parent);
		GameObject::CreateWithSprite("2P枠下部", frameTexture, Rect(5, 64 + 136, 436, 64), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217, -56, 0), parent);
		GameObject::CreateWithSprite("2Pネクスト背景", nextPieceTexture, Rect(330 + 160, 4, 160, 300), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217 - 165, 446, 0), parent);
		GameObject::CreateWithSprite("2Pネクスト枠", nextPieceTexture, Rect(0 + 160, 4, 160, 306), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217 - 167, 438, 0), parent);
		GameObject::CreateWithSprite("2Pネームプレート", namePlateTexture, Rect(0, 0, 560, 64), Vector2(0.0f, 0.0f), 1.0f, Vector3(-217 - 61, -128, 0), parent);
	}


	void PlayerController::CreateField(Transform* parent)
	{
		// フィールド内の全てのぷよ用のゲームオブジェクトを作成する。
		for (int y = 0; y < System::CellNumY; y++)
		{
			for (int x = 0; x < System::CellNumX; x++)
			{
				m_field[y][x].Create(parent);
				m_field[y][x].GetTransform()->SetLocalPosition((float)(64 * x), (float)(60 * y), 0);
			}
		}
	}


	void PlayerController::CreatePuyoPieces(Transform* parent)
	{
		// 組ぷよの作成
		m_currPiece.Create(this, parent);
		m_nextPiece[0].Create(this, parent);
		m_nextPiece[1].Create(this, parent);
	}


	void PlayerController::CreateFloatingPuyos(Transform* parent)
	{
		for (int i = 0; i < System::MaxNumFloatings; i++)
		{
			// 浮いているぷよの作成
			m_floating[i].Create(parent);
		}
	}


	void PlayerController::Reset()
	{
		// フィールド
		for (int y = 0; y < System::CellNumY; y++)
		{
			for (int x = 0; x < System::CellNumX; x++)
			{
				// ぷよの色を全て None に戻す
				m_field[y][x].SetType(PuyoType::None);
			}
		}

		// 組ぷよをリセットする
		m_currPiece.ResetRandomly();
		m_nextPiece[0].ResetRandomly();
		m_nextPiece[1].ResetRandomly();

		// 「現在の組ぷよ」を初期位置に配置する
		m_currPiece.SetPositionInCells(System::PieceStartPositionX, System::PieceStartPositionY);

		// 「次の組ぷよ」「次の次の組ぷよ」を初期位置に配置する
		switch (m_playerIndex)
		{
		case PlayerIndex::One:
			m_nextPiece[0].SetPositionInCells(6, 9);
			m_nextPiece[1].SetPositionInCells(6, 7);
			break;

		case PlayerIndex::Two:
			m_nextPiece[0].SetPositionInCells(-4, 9);
			m_nextPiece[1].SetPositionInCells(-4, 7);
			break;
		}
	}


	void PlayerController::PlacePuyoOnField(int xInCells, int yInCells, PuyoType puyoType)
	{
		m_field[yInCells][xInCells].SetType(puyoType);
	}


	const Puyo& PlayerController::GetPuyoOnField(int xInCells, int yInCells) const
	{
		return m_field[yInCells][xInCells];
	}


	int PlayerController::SearchAllFloatingPuyos(Puyo floatings[System::MaxNumFloatings])
	{
		// 浮いているぷよの個数
		int numFloatings = 0;

		// フィールドの左下から右上にかけて走査する (ただし、一番下の行を調べる必要はない)
		for (int y = 1; y < System::CellNumY; y++)
		{
			for (int x = 0; x < System::CellNumX; x++)
			{
				// 現在のセルにぷよがいるか？
				if (m_field[y][x].GetType() != PuyoType::None)
				{
					// ひとつ下のセルにぷよがいない場合は
					if (m_field[y - 1][x].GetType() == PuyoType::None)
					{
						// 浮いているので「浮いているぷよ配列」の末尾に追加する
						floatings[numFloatings].SetType(m_field[y][x].GetType());
						floatings[numFloatings].SetFallSpeed(0.0f);

						// 位置も設定しておく
						Transform* transform = floatings[numFloatings].GetTransform();
						transform->SetLocalPosition((float)(System::CellSizeX * x), (float)(System::CellSizeY * y), 0);

						// 浮いているぷよが1つ増えた
						numFloatings++;

						// フィールドから取り除く
						m_field[y][x].SetType(PuyoType::None);
					}
				}
			}
		}

		return numFloatings;
	}


	bool PlayerController::SearchAllLinkedPuyo()
	{
		// ぷよを消した場合は true
		bool disappear = false;

		// 全てを未探索としてマークする
		memset(m_searched, 0, sizeof(m_searched));

		// リンクしているぷよの位置を格納する為の可変長配列
		std::vector<Position> linked;

		// フィールド左下から右上に向かって調べる
		for (int y = 0; y < System::CellNumY; y++)
		{
			for (int x = 0; x < System::CellNumX; x++)
			{
				// フィールドの(x,y)にいるぷよの色を取得する
				const PuyoType type = m_field[y][x].GetType();

				// ぷよがいない場合はスキップ
				if (type == PuyoType::None)
					continue;

				// 前回の情報をクリアする
				linked.clear();

				// 再帰関数を使って上下左右に探す
				SearchLinkedPuyosRecursive(x, y, type, linked);

				// 同じ色のぷよが4個以上繋がっているか？
				if (linked.size() >= 4)
				{
					// 試しに消してみる (デバッグ用)
					for (const Position& pos : linked)
					{
						m_field[pos.y][pos.x].SetType(PuyoType::None);
					}
					disappear = true;
				}
			}
		}

		if (disappear)
		{
			// ぷよが消えたので消滅音を再生する
			if (m_chainCount < 7)
			{
				System::Instance().PlaySharedSE((System::SoundEffectID)((int)System::SoundEffectID::Chain01 + m_chainCount));
			}
			else
			{
				System::Instance().PlaySharedSE(System::SoundEffectID::Chain07);
			}
		}

		return disappear;
	}

	void PlayerController::SearchLinkedPuyosRecursive(int x, int y, PuyoType type, std::vector<Position>& linked)
	{
		// フィールド外ならば何もしない
		if ((x < 0) || (x >= System::CellNumX) || (y < 0) || (y >= System::CellNumY))
			return;

		// すでに探索済みならば何もしない
		if (m_searched[y][x])
			return;

		// 探している色と違う場合は何もしない
		if (m_field[y][x].GetType() != type)
			return;

		// 探索済みとしてマークしておく
		m_searched[y][x] = true;

		// 指定された色のぷよが見つかったので登録しておく
		Position pos;
		pos.x = x;
		pos.y = y;
		linked.push_back(pos);

		// 上下左右に探しに行く (再帰処理)
		SearchLinkedPuyosRecursive(x, y + 1, type, linked); // 上
		SearchLinkedPuyosRecursive(x, y - 1, type, linked); // 下
		SearchLinkedPuyosRecursive(x - 1, y, type, linked); // 左
		SearchLinkedPuyosRecursive(x + 1, y, type, linked); // 右
	}

	void PlayerController::PrepareToDropNextPiece()
	{
		// 「次の組ぷよ」を「現在の組ぷよ」にコピーする
		m_currPiece.CopyTypeFrom(m_nextPiece[0]);

		// 「次の次の組ぷよ」を「次の組ぷよ」にコピーする
		m_nextPiece[0].CopyTypeFrom(m_nextPiece[1]);

		// 「次の次の組ぷよ」を新規作成する
		m_nextPiece[1].ResetRandomly();

		// 「現在の組ぷよ」を初期位置にリセットする。
		m_currPiece.SetPositionInCells(System::PieceStartPositionX, System::PieceStartPositionY);
	}

	void PlayerController::TransitToLoseStateIfStackedUp()
	{
		if (m_field[11][2].GetType() != PuyoType::None)
		{
			m_state = State::Lose;
		}
	}
}
