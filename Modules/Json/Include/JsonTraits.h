#pragma once

#include <rapidjson/document.h>

template<typename T>
struct JsonValueConverter
{
    static bool JsonToValue(const rapidjson::Value& value, T& outValue)
    {
        if(!value.Is<T>())
        {
            return false;
        }
        outValue = value.Get<T>();
        return true;
    }

    static bool Set(const char* name, const T& inValue, rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator)
    {
        parent.AddMember(rapidjson::StringRef(name), inValue, allocator);
        return true;
    }

    static bool PushBack(rapidjson::Value& inArray, const T& inValue, rapidjson::Document::AllocatorType& allocator)
    {
        inArray.PushBack(inValue, allocator);
        return true;
    }
};

class JsonObject;
template<>
struct JsonValueConverter<JsonObject>;