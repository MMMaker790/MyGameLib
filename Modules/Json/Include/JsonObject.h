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
    {
        mValuePtr = new rapidjson::Value(rapidjson::kObjectType);
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

    JsonObject(rapidjson::Value *valuePtr) : mValuePtr(valuePtr) {}

    void SetValuePtr(rapidjson::Value *valuePtr) { mValuePtr = valuePtr; }
    rapidjson::Value *GetValuePtr() const { return mValuePtr; }

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
};