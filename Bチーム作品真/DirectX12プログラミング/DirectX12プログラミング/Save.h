#pragma once

class PlayData
{
public:
	// パスを設定
	void SetPath(const char* path);

	// セーブ
	void Save(int score);

	// ロード
	int Load();
private:
	
	// 文字列を入れる入れ物
	std::string m_path;

};