#include<iostream>
#include<chrono>
#include"Timer.h"

int Timer()
{
	std::chrono::system_clock::time_point start, end;
	//計測時間開始
	start = std::chrono::system_clock::now();
	//計測終了時間
	end = std::chrono::system_clock::now();
	//処理に要した時間
	double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

}