#pragma once

#include "JsonTraits.h"
#include "JsonData.h"
#include <string>
#include <unordered_map>
#include <rapidjson/document.h>

class JsonObject
{
public:
    JsonObject()
        : mValuePtr(new rapidjson::Value(rapidjson::kObjectType))
        , mJsonData(nullptr)
        , mOwnsValue(true)
    {
    }

    // コピー禁止（rapidjson::Value* の浅いコピーによる二重解放を防ぐ）
    JsonObject(const JsonObject&) = delete;
    JsonObject& operator=(const JsonObject&) = delete;

    // ムーブ許可。移動元は空の有効なオブジェクトとして残る
    JsonObject(JsonObject&& other) noexcept
        : mValuePtr(other.mValuePtr)
        , mJsonData(other.mJsonData)
        , mOwnsValue(other.mOwnsValue)
    {
        other.mValuePtr = new rapidjson::Value(rapidjson::kObjectType);
        other.mJsonData = nullptr;
        other.mOwnsValue = true;
    }

    JsonObject& operator=(JsonObject&& other) noexcept
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

            other.mValuePtr = new rapidjson::Value(rapidjson::kObjectType);
            other.mJsonData = nullptr;
            other.mOwnsValue = true;
        }
        return *this;
    }

    ~JsonObject()
    {
        if (mOwnsValue && mValuePtr)
        {
            delete mValuePtr;
        }
    }

    // 値の取得
    template <typename T>
    bool Get(const char *propertyName, T &outValue) const
    {
        auto iter = mValuePtr->FindMember(propertyName);
        if (iter == mValuePtr->MemberEnd())
        {
            return false;
        }

        const auto &property = iter->value;

        if constexpr (std::is_same_v<T, JsonArray>)
        {
            outValue.SetJsonData(mJsonData);
        }
        else if constexpr (std::is_same_v<T, JsonObject>)
        {
            outValue.SetJsonData(mJsonData);
        }

        return JsonValueConverter<T>::JsonToValue(property, outValue);
    }

    // 値の追加
    template <typename T>
    bool AddMember(const char *propertyName, const T &value, JsonData &jsonData)
    {
        return JsonValueConverter<T>::Set(propertyName, value, *mValuePtr, jsonData.GetAllocator());
    }


    // メンバーの取得
    std::unordered_map<std::string, rapidjson::Value*> GetMembers() const
    {
        std::unordered_map<std::string, rapidjson::Value*> members;

        for (auto iter = mValuePtr->MemberBegin(); iter != mValuePtr->MemberEnd(); ++iter)
        {
            const auto &name = iter->name;
            const auto &property = iter->value;
            members.emplace(name.GetString(), const_cast<rapidjson::Value*>(&property));
        }

        return members;
    }

private:
    // JsonData, JsonArrayクラスからのみ値を設定できるようにする
    friend class JsonData;
    friend class JsonArray;

    JsonObject(rapidjson::Value *valuePtr) : mValuePtr(valuePtr), mJsonData(nullptr), mOwnsValue(false) {}
    JsonObject(rapidjson::Value *valuePtr, JsonData *jsonData) : mValuePtr(valuePtr), mJsonData(jsonData), mOwnsValue(false) {}

    void SetValuePtr(rapidjson::Value *valuePtr) { mValuePtr = valuePtr; }
    rapidjson::Value *GetValuePtr() const { return mValuePtr; }

    void SetJsonData(JsonData *jsonData) { mJsonData = jsonData; }
    JsonData *GetJsonData() const { return mJsonData; }

    // アロケータ指定のAddMember
    friend struct JsonValueConverter<JsonObject>;
    bool AddMember(const char *propertyName, const rapidjson::Value &value, rapidjson::Document::AllocatorType &allocator)
    {
        rapidjson::Value copy(value, allocator);
        mValuePtr->AddMember(rapidjson::StringRef(propertyName), copy, allocator);
        return true;
    }

private:
    rapidjson::Value *mValuePtr = nullptr;
    JsonData *mJsonData = nullptr;
    bool mOwnsValue = false;
};

// JsonObjectのJsonValueConverterの特殊化
// JsonObject ⇔ JsonTraitsの循環参照を避けるためここで定義
template<>
struct JsonValueConverter<JsonObject>
{
    static bool IsType(const rapidjson::Value& value)
    {
        return value.IsObject();
    }

    static bool JsonToValue(const rapidjson::Value& value, JsonObject& outValue)
    {
        if(!value.IsObject())
        {
            return false;
        }

        for(auto iter = value.MemberBegin(); iter != value.MemberEnd(); ++iter)
        {
            outValue.AddMember(iter->name.GetString(), iter->value, outValue.GetJsonData()->GetAllocator());
        }

        return true;
    }

    static bool Set(const char* name, const JsonObject& inValue, rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value objectValue(rapidjson::kObjectType);
        for (const auto& member : inValue.GetMembers())
        {
            rapidjson::Value key(member.first.c_str(), allocator);
            objectValue.AddMember(key, *member.second, allocator);
        }
        rapidjson::Value keyName(name, allocator);
        parent.AddMember(keyName, objectValue, allocator);
        return true;
    }

    static bool PushBack(rapidjson::Value& inArray, const JsonObject& inValue, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value objectValue(rapidjson::kObjectType);
        for (const auto& member : inValue.GetMembers())
        {
            rapidjson::Value key(member.first.c_str(), allocator);
            objectValue.AddMember(key, *member.second, allocator);
        }
        inArray.PushBack(objectValue, allocator);
        return true;
    }
};