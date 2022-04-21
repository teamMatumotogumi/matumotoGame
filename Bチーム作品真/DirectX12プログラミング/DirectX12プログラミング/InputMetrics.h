#pragma once
#include <cstdint>

// システムを通過するスループットのレベルに関する情報を提供します。
class InputMetrics
{
public:
    // Update()ごとに受信したイベントバッファの平均サイズを取得します。
    float GetAverageEventBytesPerFrame() const;

    // イベントが生成されてから処理されるまでにかかる平均時間を取得します。
    double GetAverageLagTimePerEvent() const;

    // 個々のInputEventの処理に費やされる平均時間(単位は秒)を取得します。
    double GetAverageProcessingTimePerEvent() const;

    // システム内のデバイスで現在稼働しているInputControlの総数を取得します。
    int32_t GetCurrentControlCount() const;

    // システム内のデバイスで現在稼働しているInputControlの総数を設定します。
    void SetCurrentControlCount(int32_t value);

    // 現在登録されているレイアウトの総数を取得します。
    int32_t GetCurrentLayoutCount() const;

    // 現在登録されているレイアウトの総数を設定します。
    void SetCurrentLayoutCount(int32_t value);

    // 現在システムに追加されているデバイスの数を取得します。
    int32_t GetCurrentNumDevices() const;

    // 現在システムに追加されているデバイスの数を設定します。
    void SetCurrentNumDevices(int32_t value);

    // 現在の全てのデバイスの結合された状態メモリの合計サイズを取得します。
    int32_t GetCurrentStateSizeInBytes() const;

    // 現在の全てのデバイスの結合された状態メモリの合計サイズを設定します。
    void SetCurrentStateSizeInBytes(int32_t value);

    // システムに同時に追加されたデバイスの最大数を取得します。
    int32_t GetMaxNumDevices() const;

    // システムに同時に追加されたデバイスの最大数を設定します。
    void SetMaxNumDevices(int32_t value);

    // 全てのデバイスの合計状態メモリの最大値を取得します。
    int32_t GetMaxStateSizeInBytes() const;

    // 全てのデバイスの合計状態メモリの最大値を設定します。
    void SetMaxStateSizeInBytes(int32_t value);

    // これまでに消費されたInputEventsの合計バイト数を取得します。
    int32_t GetTotalEventBytes() const;

    // これまでに消費されたInputEventsの合計バイト数を設定します。
    void SetTotalEventBytes(int32_t value);

    // これまでに消費されたInputEventsの総数を取得します。
    int32_t GetTotalEventCount() const;

    // これまでに消費されたInputEventsの総数を設定します。
    void SetTotalEventCount(int32_t value);

    // イベントが処理された時と比較してイベントが生成された時の間に経過した合計累積時間を取得します。
    double GetTotalEventLagTime() const;

    // イベントが処理された時と比較してイベントが生成された時の間に経過した合計累積時間を設定します。
    void SetTotalEventLagTime(double value);

    // これまでにInputEventsの処理に費やされた合計時間(単位は秒)を取得します。
    double GetTotalEventProcessingTime() const;

    // これまでにInputEventsの処理に費やされた合計時間(単位は秒)を設定します。
    void SetTotalEventProcessingTime(double value);

    // これまでに実行された入力システム更新の総数を取得します。
    int32_t GetTotalUpdateCount() const;
 
    // これまでに実行された入力システム更新の総数を設定します。
    void SetTotalUpdateCount(int32_t value);
};


