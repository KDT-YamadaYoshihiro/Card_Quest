#pragma once
template <typename T>
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
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

protected:

    Singleton() = default;
    virtual ~Singleton() = default;
};