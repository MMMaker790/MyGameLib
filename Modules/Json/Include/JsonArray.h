#pragma once

#include "JsonTraits.h"
#include <rapidjson/document.h>

class JsonArray
{
public:
    JsonArray()
    {
        mValuePtr = new rapidjson::Value(rapidjson::kArrayType);
    }

    // 値の取得
    template <typename T>
    bool Get(size_t index, T &outValue) const
    {
        if (index >= mValuePtr->Size())
        {
            return false;
        }

        return JsonValueConverter<T>::JsonToValue((*mValuePtr)[index], outValue);
    }

    // 値の追加
    template <typename T>
    bool PushBack(const T &value)
    {
        return JsonValueConverter<T>::PushBack(*mValuePtr, value, mValuePtr->GetAllocator());
    }

    // 配列のサイズを取得
    size_t Size() const { return mValuePtr->Size(); }

private:
    // JsonData, JsonObjectクラスからのみ値を設定できるようにする
    friend class JsonData;
    friend class JsonObject;

    JsonArray(rapidjson::Value *valuePtr) : mValuePtr(valuePtr) {}

    void SetValuePtr(rapidjson::Value *valuePtr) { mValuePtr = valuePtr; }
    rapidjson::Value *GetValuePtr() const { return mValuePtr; }

private:
    rapidjson::Value *mValuePtr = nullptr;
};
