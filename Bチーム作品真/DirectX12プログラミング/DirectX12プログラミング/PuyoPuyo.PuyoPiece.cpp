#include "PuyoPuyo.PuyoPiece.h"
#include "PuyoPuyo.PlayerController.h"
#include "PuyoPuyo.System.h"

namespace PuyoPuyo
{
	PuyoPiece::PuyoPiece()
		: m_player(nullptr)
		, m_num(0)
		, m_isBig(false)
	{
	}


	void PuyoPiece::Create(PlayerController* playerController, Transform* parent)
	{
		m_player = playerController;

		// 組ぷよ用ゲームオブジェクトの作成
		m_piece = new GameObject();
		m_piece->GetTransform()->SetParent(parent);

		// フィールド上の全てのマスで子ゲームオブジェクトを作成する
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				m_puyos[y][x].Create(m_piece->GetTransform());
				m_puyos[y][x].GetTransform()->SetLocalPosition((float)(System::CellSizeX * x), (float)(System::CellSizeY * y), 0);
			}
		}
	}


	void PuyoPiece::CopyTypeFrom(const PuyoPiece& other)
	{
		m_num = other.m_num;
		m_isBig = other.m_isBig;

		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				m_puyos[y][x].SetType(other.m_puyos[y][x].GetType());
			}
		}
	}


	Transform* PuyoPiece::GetTransform() const
	{
		return m_piece->GetTransform();
	}


	void PuyoPiece::SetPositionInCells(int xInCells, int yInCells)
	{
		GetTransform()->SetLocalPosition((float)System::CellSizeX * xInCells, (float)System::CellSizeY * yInCells, 0);
	}


	void PuyoPiece::ResetRandomly()
	{
		// ぷよ何体で構成されているか？ (2～4)
		m_num = 2 + rand() % 3;

		// 3×3マス全てを None で埋めておく
		//
		//	2 □□□
		//	1 □□□
		//	0 □□□
		//	   0 1 2
		//
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				m_puyos[y][x].SetType(PuyoType::None);
			}
		}

		// 左下を[0][0]とするのが良さそう
		switch (m_num)
		{
		case 2:
			//	2 □●□
			//	1 □●□
			//	0 □□□
			//	   0 1 2
			m_puyos[2][1].SetType((PuyoType)(rand() % 5));		// ランダムに決定
			m_puyos[1][1].SetType((PuyoType)(rand() % 5));		// ランダムに決定
			break;

		case 3:
			//	2 □●□
			//	1 □●●
			//	0 □□□
			//	   0 1 2
			m_puyos[2][1].SetType((PuyoType)(rand() % 5));		// ランダムに決定
			m_puyos[1][1].SetType(m_puyos[2][1].GetType());		// 上と同色
			m_puyos[1][2].SetType((PuyoType)(rand() % 5));		// ランダムに決定
			break;

		case 4:
			//	2 □●●
			//	1 □●●
			//	0 □□□
			//	   0 1 2
			m_puyos[2][1].SetType((PuyoType)(rand() % 5));		// ランダムに決定
			m_puyos[1][1].SetType(m_puyos[2][1].GetType());		// 上と同色
			m_puyos[2][2].SetType((PuyoType)(rand() % 5));		// ランダムに決定
			m_puyos[1][2].SetType(m_puyos[2][2].GetType());		// 上と同色
			m_isBig = (m_puyos[2][1].GetType() == m_puyos[2][2].GetType());	// 全て同色なら大ぷよ
			break;
		}
	}


	void PuyoPiece::Move(Direction direction)
	{
		// 移動量 (単位はピクセル)
		float offsetXInPixels;

		switch (direction)
		{
		case Direction::Left:
			offsetXInPixels = -(float)System::CellSizeX;
			break;

		case Direction::Right:
			offsetXInPixels = (float)System::CellSizeX;
			break;

		default:
			assert(0);
			break;
		}

		// 組ぷよを指定したセル分だけ移動させる
		Transform* transform = GetTransform();
		transform->Translate(offsetXInPixels, 0, 0);

		// フィールドの外側に出てしまった場合は
		if (!IsInField() || IsHitToPuyoPlacedOnField())
		{
			// 元の位置に戻す
			transform->Translate(-offsetXInPixels, 0, 0);
		}
		else
		{
			// 移動できたので移動音を再生する
			System::Instance().PlaySharedSE(System::SoundEffectID::PieceMove);
		}
	}


	void PuyoPiece::Rotate(Direction direction)
	{
		// 回転前の組ぷよのコピーをとる
		PuyoType copied[3][3];
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				copied[y][x] = m_puyos[y][x].GetType();
			}
		}

		if (m_num < 4)
		{
			// 「回転前の2次元配列」を基にして「回転後の2次元配列」を作成する。
			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					if (direction == Direction::Left)
					{
						// 左回転の場合は(x,y)にいるぷよを(2-y,x)に移動させる
						m_puyos[x][2 - y].SetType(copied[y][x]);
					}
					else
					{
						// 右回転の場合は(x,y)にいるぷよを(y,2-x)に移動させる
						m_puyos[2 - x][y].SetType(copied[y][x]);
					}
				}
			}

			// 回転後の組ぷよがフィールド外にいたり他のぷよにめり込んでいる場合は回転前の状態に戻す。
			if (!IsInField() || IsHitToPuyoPlacedOnField())
			{
				for (int y = 0; y < 3; y++)
				{
					for (int x = 0; x < 3; x++)
					{
						m_puyos[y][x].SetType(copied[y][x]);
					}
				}
			}
			else
			{
				// 回転できたので回転音を再生する
				System::Instance().PlaySharedSE(System::SoundEffectID::PieceRotate);
			}
		}
		else if (IsBig())
		{
			// 「大ぷよ」の場合は次の色に変化する。
			const int currTypeValue = (int)m_puyos[1][1].GetType();
			const int nextTypeValue = (currTypeValue + ((direction == Direction::Left) ? 4 : 1)) % 5;
			const PuyoType nextPuyoType = (PuyoType)nextTypeValue;
			m_puyos[1][1].SetType(nextPuyoType);
			m_puyos[1][2].SetType(nextPuyoType);
			m_puyos[2][1].SetType(nextPuyoType);
			m_puyos[2][2].SetType(nextPuyoType);

			// 回転できたので回転音を再生する
			System::Instance().PlaySharedSE(System::SoundEffectID::PieceRotate);
		}
		else
		{
			// 「大ぷよ」以外の場合は回転する。
			if (direction == Direction::Left)
			{
				m_puyos[1][1].SetType(copied[2][1]);
				m_puyos[2][1].SetType(copied[2][2]);
				m_puyos[2][2].SetType(copied[1][2]);
				m_puyos[1][2].SetType(copied[1][1]);
			}
			else
			{
				m_puyos[1][1].SetType(copied[1][2]);
				m_puyos[2][1].SetType(copied[1][1]);
				m_puyos[2][2].SetType(copied[2][1]);
				m_puyos[1][2].SetType(copied[2][2]);
			}

			// 回転できたので回転音を再生する
			System::Instance().PlaySharedSE(System::SoundEffectID::PieceRotate);
		}
	}



	void PuyoPiece::PlaceOnField()
	{
		// 現在の位置を取得する
		const DirectX::XMFLOAT3& localPosition = GetTransform()->GetLocalPosition();

		// 組ぷよの左下のマスは、フィールド上ではどのマスか？
		const int xInCells = (int)(localPosition.x / System::CellSizeX);
		const int yInCells = (int)((localPosition.y + System::CellSizeY) / System::CellSizeY) - 1;

		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				// [y][x]の場所にいるぷよを取得する
				const PuyoType puyoType = m_puyos[y][x].GetType();

				// その場所にぷよがいるか？
				if (puyoType != PuyoType::None)
				{
					m_player->PlacePuyoOnField(xInCells + x, yInCells + y, puyoType);
				}
			}
		}
	}


	bool PuyoPiece::HasReachedBottomOfField() const
	{
		// 現在の位置を取得する
		const DirectX::XMFLOAT3& localPosition = GetTransform()->GetLocalPosition();

		// 組ぷよがまだフィールドの底に達していない場合は false
		if (localPosition.y > 0)
			return false;

		// 組ぷよを構成する全てのぷよについて調べる
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				// [y][x]の場所にぷよがいるか？
				if (m_puyos[y][x].GetType() != PuyoType::None)
				{
					// そのぷよのY座標がフィールドの底(Y=0)よりも下か？
					if (localPosition.y + System::CellSizeY * y < 0.0f)
						return true;
				}
			}
		}

		return false;
	}


	bool PuyoPiece::IsInField() const
	{
		int xmin = 2;	// 最も左にいるぷよのセル位置X
		int xmax = 0;	// 最も右にいるぷよのセル位置X
		int ymin = 2;	// 最も下にいるぷよのセル位置Y
		int ymax = 0;	// 最も上にいるぷよのセル位置Y

		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				// その場所にぷよがいるか？
				if (m_puyos[y][x].GetType() != PuyoType::None)
				{
					if (x < xmin) xmin = x; // 更に左にいるぷよが見つかった
					if (x > xmax) xmax = x; // 更に右にいるぷよが見つかった
					if (y < ymin) ymin = y; // 更に下にいるぷよが見つかった
					if (y > ymax) ymax = y; // 更に上にいるぷよが見つかった
				}
			}
		}

		// 現在の位置を取得する
		const DirectX::XMFLOAT3 localPosition = GetTransform()->GetLocalPosition();

		// 矩形(Rectangle)で表現する (単位はピクセル)
		Rect boundingRect;
		boundingRect.x      = (float)System::CellSizeX * xmin;
		boundingRect.y      = (float)System::CellSizeY * ymin;
		boundingRect.width  = (float)System::CellSizeX * (xmax - xmin + 1);
		boundingRect.height = (float)System::CellSizeY * (ymax - ymin + 1);

		// フィールド原点からの位置に変換する
		boundingRect.x += localPosition.x;
		boundingRect.y += localPosition.y;

		// フィールドの左端から出ている
		if (boundingRect.x < 0)
			return false;

		// フィールドの右端から出ている
		if (boundingRect.x + boundingRect.width > (System::CellSizeX * System::CellNumX))
			return false;

		// フィールドの上端から出ている
		if (boundingRect.y + boundingRect.height > (System::CellSizeY * System::CellNumY))
			return false;

		// フィールドの下端から出ている
		if (boundingRect.y < 0)
			return false;

		return true;
	}


	bool PuyoPiece::IsHitToPuyoPlacedOnField() const
	{
		// 現在の位置を取得する
		const DirectX::XMFLOAT3& localPosition = GetTransform()->GetLocalPosition();

		// 「組ぷよの左下のセル」はフィールド上ではどのセルか？
		const int xInCells = (int)(localPosition.x / System::CellSizeX);
		const int yInCells = (int)((localPosition.y + System::CellSizeY) / System::CellSizeY) - 1;

		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				// [y][x]の場所にいるぷよいる
				if (m_puyos[y][x].GetType() != PuyoType::None)
				{
					// フィールド上の同じ場所にぷよがいる
					if (m_player->GetPuyoOnField(xInCells + x, yInCells + y).GetType() != PuyoType::None)
					{
						// フィールド上にいるぷよと衝突している
						return true;
					}
				}
			}
		}

		return false;
	}
}

