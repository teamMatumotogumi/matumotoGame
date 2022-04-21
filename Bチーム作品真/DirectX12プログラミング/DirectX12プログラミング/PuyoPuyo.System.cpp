#include "PuyoPuyo.System.h"
#include "PuyoPuyo.MainScene.h"

namespace PuyoPuyo
{
    // 静的メンバ変数の実体を宣言
    System* System::s_singletonInstance = nullptr;


    void System::CreateSingletonInstance()
    {
        assert(!s_singletonInstance);
        s_singletonInstance = new System();
    }


    void System::DestroySingletonInstance()
    {
        assert(s_singletonInstance);
        delete s_singletonInstance;
        s_singletonInstance = nullptr;
    }


    System::System()
    {

    }

    void System::LoadSharedSoundEffects()
    {
        m_sharedSE[(size_t)SoundEffectID::Unknown] = OpenAudio("Assets/PuyoPuyo/SE/snd_main/00.wav");
        m_sharedSE[(size_t)SoundEffectID::Chain01] = OpenAudio("Assets/PuyoPuyo/SE/snd_main/01.wav");
        m_sharedSE[(size_t)SoundEffectID::Chain02] = OpenAudio("Assets/PuyoPuyo/SE/snd_main/02.wav");
        m_sharedSE[(size_t)SoundEffectID::Chain03] = OpenAudio("Assets/PuyoPuyo/SE/snd_main/03.wav");
        m_sharedSE[(size_t)SoundEffectID::Chain04] = OpenAudio("Assets/PuyoPuyo/SE/snd_main/04.wav");
        m_sharedSE[(size_t)SoundEffectID::Chain05] = OpenAudio("Assets/PuyoPuyo/SE/snd_main/05.wav");
        m_sharedSE[(size_t)SoundEffectID::Chain06] = OpenAudio("Assets/PuyoPuyo/SE/snd_main/06.wav");
        m_sharedSE[(size_t)SoundEffectID::Chain07] = OpenAudio("Assets/PuyoPuyo/SE/snd_main/07.wav");
        m_sharedSE[(size_t)SoundEffectID::PieceMove] = OpenAudio("Assets/PuyoPuyo/SE/snd_main/08.wav");
        m_sharedSE[(size_t)SoundEffectID::PieceRotate] = OpenAudio("Assets/PuyoPuyo/SE/snd_main/09.wav");
    }

    void System::LoadSharedBackgroundMusics()
    {
        m_sharedBGM[(size_t)BackgroundMusicID::es38_heppoko] = OpenAudio("Assets/PuyoPuyo/BGM/bgm_es38_heppoko.wav");
    }

    void System::Run()
    {
        // ぷよテクスチャのロード
        Texture2D* puyoTexture = Texture2D::FromFile(L"Assets/PuyoPuyo/Textures/puyo/puyo2P/puyo_aqua.png");

        // ぷよスプライト配列の作成
        m_puyoSprites[0] = Sprite::Create(puyoTexture, Rect(0, 72 * 0, 72, 72), Vector2(0.0f, 0.0f), 1.0f);
        m_puyoSprites[1] = Sprite::Create(puyoTexture, Rect(0, 72 * 1, 72, 72), Vector2(0.0f, 0.0f), 1.0f);
        m_puyoSprites[2] = Sprite::Create(puyoTexture, Rect(0, 72 * 2, 72, 72), Vector2(0.0f, 0.0f), 1.0f);
        m_puyoSprites[3] = Sprite::Create(puyoTexture, Rect(0, 72 * 3, 72, 72), Vector2(0.0f, 0.0f), 1.0f);
        m_puyoSprites[4] = Sprite::Create(puyoTexture, Rect(0, 72 * 4, 72, 72), Vector2(0.0f, 0.0f), 1.0f);

        // 共有音源のロード
        LoadSharedSoundEffects();
        LoadSharedBackgroundMusics();

        // ぷよぷよ「メイン画面」の作成
        MainScene* mainScene = new MainScene();

        // ぷよぷよ「メイン画面」をアクティブなシーンとして設定する
        SceneManager::SetActiveScene(mainScene);

        // ぷよぷよ「メイン画面」のアセットをロードする
        mainScene->LoadAssets();
    }

    void System::PlaySharedSE(SoundEffectID id)
    {
        PlayAudio(m_sharedSE[(size_t)id]);
    }

    void System::PlaySharedBGM(BackgroundMusicID id)
    {
        PlayAudio(m_sharedBGM[(size_t)id]);
    }


    Sprite* System::GetSprite(PuyoType type) const
    {
        switch (type)
        {
        case PuyoType::Red: return m_puyoSprites[0];
        case PuyoType::Green: return m_puyoSprites[1];
        case PuyoType::Blue: return m_puyoSprites[2];
        case PuyoType::Yellow: return m_puyoSprites[3];
        case PuyoType::Purple: return m_puyoSprites[4];
        }

        return nullptr;
    }
}