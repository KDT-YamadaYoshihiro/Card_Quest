#pragma once
template <typename T>
// 継承して使えるSingletonヘッダー
class Singleton {
public:
    // インスタンスの取得
    static T& GetInstance()
    {
        static T instance;
        return instance;
    }
    // コピーコンストラクタと代入演算子を禁止
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // ムーブも禁止
    Singleton(Singleton&&) = delete;
    // ムーブ代入も禁止
    Singleton& operator=(Singleton&&) = delete;

protected:  // 継承先でも使えるようにprotected:

    Singleton() = default;
    // 仮想デストラクタ
    virtual ~Singleton() = default;
};