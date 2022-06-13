#pragma once
#include<Windows.h>

// 音楽ファイルを開く
// パスで識別
WORD OpenAudio(const char* path);

// 音楽を開く
void PlayAudio(WORD deviceID);

// 音楽を一時停止する
void StopAudio(WORD deviceID);

// 音楽を閉じる
void closeAudio(WORD deviceID);