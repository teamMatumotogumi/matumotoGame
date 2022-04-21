#pragma once
#include <windows.h>

// 音源ファイルを開きます。
// この関数は識別IDを返します。
WORD OpenAudio(const char* path);

// 音源を再生します。
void PlayAudio(WORD id);

// 音源を停止します。
void StopAudio(WORD id);

// 音源ファイルを閉じます。
void CloseAudio(WORD id);
