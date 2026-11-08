#pragma once

#include "JsonData.h"
#include "JsonTraits.h"
#include <rapidjson/document.h>
#include <vector>

class JsonArray
{
public:
    JsonArray()
        : mValuePtr(new rapidjson::Value(rapidjson::kArrayType))
        , mJsonData(nullptr)
        , mOwnsValue(true)
    {
    }

    // コピー禁止（rapidjson::Value* の浅いコピーによる二重解放を防ぐ）
    JsonArray(const JsonArray&) = delete;
    JsonArray& operator=(const JsonArray&) = delete;

    // ムーブ許可。移動元は空の有効なオブジェクトとして残る
    JsonArray(JsonArray&& other) noexcept
        : mValuePtr(other.mValuePtr)
        , mJsonData(other.mJsonData)
        , mOwnsValue(other.mOwnsValue)
    {
        other.mValuePtr = new rapidjson::Value(rapidjson::kArrayType);
        other.mJsonData = nullptr;
        other.mOwnsValue = true;
    }

    JsonArray& operator=(JsonArray&& other) noexcept
    {
        if (this != &other)
        {
            if (mOwnsValue && mValuePtr)
            {
                delete mValuePtr;
            }

            mValuePtr   = other.mValuePtr;
            mJsonData   = other.mJsonData;
            mOwnsValue  = other.mOwnsValue;

            other.mValuePtr = new rapidjson::Value(rapidjson::kArrayType);
            other.mJsonData = nullptr;
            other.mOwnsValue = true;
        }
        return *this;
    }

    ~JsonArray()
    {
        if (mOwnsValue && mValuePtr)
        {
            delete mValuePtr;
        }
    }

    // 値の取得
    template <typename T>
    bool Get(size_t index, T &outValue) const
    {
        if (index >= mValuePtr->Size())
        {
            return false;
        }

        if constexpr (std::is_same_v<T, JsonArray>)
        {
            outValue.SetJsonData(mJsonData);
        }
        else if constexpr (std::is_same_v<T, JsonObject>)
        {
            outValue.SetJsonData(mJsonData);
        }

        return JsonValueConverter<T>::JsonToValue((*mValuePtr)[index], outValue);
    }

    // 値の追加
    template <typename T>
    bool PushBack(const T &value, JsonData &jsonData)
    {
        return JsonValueConverter<T>::PushBack(*mValuePtr, value, jsonData.GetAllocator());
    }

    // 配列のサイズを取得
    size_t Size() const { return mValuePtr->Size(); }

private:
    // JsonData, JsonObjectクラスからのみ値を設定できるようにする
    friend class JsonData;
    friend class JsonObject;

    JsonArray(rapidjson::Value *valuePtr) : mValuePtr(valuePtr), mJsonData(nullptr), mOwnsValue(false) {}
    JsonArray(rapidjson::Value *valuePtr, JsonData *jsonData) : mValuePtr(valuePtr), mJsonData(jsonData), mOwnsValue(false) {}

    void SetValuePtr(rapidjson::Value *valuePtr) { mValuePtr = valuePtr; }
    rapidjson::Value *GetValuePtr() const { return mValuePtr; }
    void SetJsonData(JsonData *jsonData) { mJsonData = jsonData; }
    JsonData *GetJsonData() const { return mJsonData; }

    // アロケータ指定のPushBack
    friend struct JsonValueConverter<JsonArray>;
    bool PushBack(const rapidjson::Value &value, rapidjson::Document::AllocatorType &allocator)
    {
        rapidjson::Value copy(value, allocator);
        mValuePtr->PushBack(copy, allocator);
        return true;
    }

private:
    rapidjson::Value *mValuePtr = nullptr;
    JsonData *mJsonData = nullptr;
    bool mOwnsValue = false;
};


// JsonArrayのJsonValueConverterの特殊化
// JsonArray ⇔ JsonTraitsの循環参照を避けるためここで定義
template<>
struct JsonValueConverter<JsonArray>
{
    static bool IsType(const rapidjson::Value& value)
    {
        return value.IsArray();
    }

    static bool JsonToValue(const rapidjson::Value& value, JsonArray& outValue)
    {
        if(!value.IsArray())
        {
            return false;
        }
        
        for(rapidjson::SizeType i = 0; i < value.Size(); ++i)
        {
            outValue.PushBack(value[i], outValue.GetJsonData()->GetAllocator());
        }

        return true;
    }

    static bool Set(const char* name, const JsonArray& inValue, rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value arrayValue(rapidjson::kArrayType);
        arrayValue.Reserve(inValue.Size(), allocator);

        for(auto& element : inValue.GetValuePtr()->GetArray())
        {
            arrayValue.PushBack(element, allocator);
        }
        parent.AddMember(rapidjson::StringRef(name), arrayValue, allocator);
        return true;
    }

    static bool PushBack(rapidjson::Value& inArray, const JsonArray& inValue, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value arrayValue(rapidjson::kArrayType);
        arrayValue.Reserve(inValue.Size(), allocator);

        for(auto& element : inValue.GetValuePtr()->GetArray())
        {
            arrayValue.PushBack(element, allocator);
        }
        inArray.PushBack(arrayValue, allocator);
        return true;
    }
};

