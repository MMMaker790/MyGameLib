#pragma once

#include "GameMath.h"
#include <rapidjson/document.h>

template<typename T>
struct JsonValueConverter
{
    static bool IsType(const rapidjson::Value& value)
    {
        return value.Is<T>();
    }

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


template<>
struct JsonValueConverter<Vector3>
{
    static bool IsType(const rapidjson::Value& value)
    {
        return value.IsArray() && value.Size() == 3;
    }

    static bool JsonToValue(const rapidjson::Value& value, Vector3& outValue)
    {
        if(!value.IsArray() || value.Size() != 3)
        {
            return false;
        }
        outValue.x = value[0].GetFloat();
        outValue.y = value[1].GetFloat();
        outValue.z = value[2].GetFloat();
        return true;
    }

    static bool Set(const char* name, const Vector3& inValue, rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value vecValue(rapidjson::kArrayType);
        vecValue.PushBack(inValue.x, allocator);
        vecValue.PushBack(inValue.y, allocator);
        vecValue.PushBack(inValue.z, allocator);
        parent.AddMember(rapidjson::StringRef(name), vecValue, allocator);
        return true;
    }

    static bool PushBack(rapidjson::Value& inArray, const Vector3& inValue, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value vecValue(rapidjson::kArrayType);
        vecValue.PushBack(inValue.x, allocator);
        vecValue.PushBack(inValue.y, allocator);
        vecValue.PushBack(inValue.z, allocator);
        inArray.PushBack(vecValue, allocator);
        return true;
    }
};

template<>
struct JsonValueConverter<Quaternion>
{
    static bool IsType(const rapidjson::Value& value)
    {
        return value.IsArray() && value.Size() == 4;
    }

    static bool JsonToValue(const rapidjson::Value& value, Quaternion& outValue)
    {
        if(!value.IsArray() || value.Size() != 4)
        {
            return false;
        }
        outValue.x = value[0].GetFloat();
        outValue.y = value[1].GetFloat();
        outValue.z = value[2].GetFloat();
        outValue.w = value[3].GetFloat();
        return true;
    }

    static bool Set(const char* name, const Quaternion& inValue, rapidjson::Value& parent, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value quatValue(rapidjson::kArrayType);
        quatValue.PushBack(inValue.x, allocator);
        quatValue.PushBack(inValue.y, allocator);
        quatValue.PushBack(inValue.z, allocator);
        quatValue.PushBack(inValue.w, allocator);
        parent.AddMember(rapidjson::StringRef(name), quatValue, allocator);
        return true;
    }

    static bool PushBack(rapidjson::Value& inArray, const Quaternion& inValue, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value quatValue(rapidjson::kArrayType);
        quatValue.PushBack(inValue.x, allocator);
        quatValue.PushBack(inValue.y, allocator);
        quatValue.PushBack(inValue.z, allocator);
        quatValue.PushBack(inValue.w, allocator);
        inArray.PushBack(quatValue, allocator);
        return true;
    }
};