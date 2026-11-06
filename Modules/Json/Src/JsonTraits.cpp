#include "JsonTraits.h"
#include "GameMath.h"
#include "JsonArray.h"
#include "JsonObject.h"

template<>
struct JsonValueConverter<Vector3>
{
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

    static bool Set(rapidjson::Value& parent, const char* name, const Vector3& inValue, rapidjson::Document::AllocatorType& allocator)
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

    static bool Set(rapidjson::Value& parent, const char* name, const Quaternion& inValue, rapidjson::Document::AllocatorType& allocator)
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

template<>
struct JsonValueConverter<JsonArray>
{
    static bool JsonToValue(const rapidjson::Value& value, JsonArray& outValue)
    {
        if(!value.IsArray())
        {
            return false;
        }
        
        for(rapidjson::SizeType i = 0; i < value.Size(); ++i)
        {
            outValue.PushBack(value[i]);
        }

        return true;
    }

    static bool Set(rapidjson::Value& parent, const char* name, const JsonArray& inValue, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value arrayValue(rapidjson::kArrayType);
        arrayValue.Reserve(inValue.Size(), allocator);
        for(rapidjson::SizeType i = 0; i < inValue.Size(); ++i)
        {
            inValue.Get(i, arrayValue[i]);
        }
        parent.AddMember(rapidjson::StringRef(name), arrayValue, allocator);
        return true;
    }

    static bool PushBack(rapidjson::Value& inArray, const JsonArray& inValue, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value arrayValue(rapidjson::kArrayType);
        arrayValue.Reserve(inValue.Size(), allocator);
        for(rapidjson::SizeType i = 0; i < inValue.Size(); ++i)
        {
            inValue.Get(i, arrayValue[i]);
        }
        inArray.PushBack(arrayValue, allocator);
        return true;
    }
};

template<>
struct JsonValueConverter<JsonObject>
{
    static bool JsonToValue(const rapidjson::Value& value, JsonObject& outValue)
    {
        if(!value.IsObject())
        {
            return false;
        }
        
        rapidjson::Document document;
        rapidjson::Document::AllocatorType& docAllocator = document.GetAllocator();

        for(auto iter = value.MemberBegin(); iter != value.MemberEnd(); ++iter)
        {
            outValue.AddMember(iter->name.GetString(), iter->value, docAllocator);
        }

        return true;
    }

    static bool Set(rapidjson::Value& parent, const char* name, const JsonObject& inValue, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value objectValue(rapidjson::kObjectType);
        for (const auto& member : inValue.GetMembers())
        {
            objectValue.AddMember(rapidjson::StringRef(member.first.c_str()), *member.second, allocator);
        }
        parent.AddMember(rapidjson::StringRef(name), objectValue, allocator);
        return true;
    }

    static bool PushBack(rapidjson::Value& inArray, const JsonObject& inValue, rapidjson::Document::AllocatorType& allocator)
    {
        rapidjson::Value objectValue(rapidjson::kObjectType);
        for (const auto& member : inValue.GetMembers())
        {
            objectValue.AddMember(rapidjson::StringRef(member.first.c_str()), *member.second, allocator);
        }
        inArray.PushBack(objectValue, allocator);
        return true;
    }
};