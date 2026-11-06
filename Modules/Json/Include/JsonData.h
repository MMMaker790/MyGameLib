#pragma once

#include "GameMath.h"
#include <cstddef>
#include <rapidjson/document.h>
#include <string>

class JsonData
{
public:
    // JSONファイルのロード
    bool LoadJSON(const std::string &fileName);
    bool SaveJSON(const std::string &fileName);

    // JSON配列、JSONオブジェクトからJsonDataを作成する
    bool CreateJsonData(const class JsonArray &jsonArray);
    bool CreateJsonData(const class JsonObject &jsonObject);

    // 型チェック
    template <typename T>
    bool IsType() const
    {
        if constexpr (std::is_same_v<T, Vector3>)
        {
            return mDocument.IsArray() && mDocument.Size() == 3;
        }
        else if constexpr (std::is_same_v<T, Quaternion>)
        {
            return mDocument.IsArray() && mDocument.Size() == 4;
        }
        else if constexpr (std::is_same_v<T, JsonObject>)
        {
            return mDocument.IsObject();
        }
        else if constexpr (std::is_same_v<T, JsonArray>)
        {
            return mDocument.IsArray();
        }

        return mDocument.Is<T>();
    }

    template <typename T>
    static bool IsType(const rapidjson::Value &value)
    {
        if constexpr (std::is_same_v<T, Vector3>)
        {
            return value.IsArray() && value.Size() == 3;
        }
        else if constexpr (std::is_same_v<T, Quaternion>)
        {
            return value.IsArray() && value.Size() == 4;
        }
        else if constexpr (std::is_same_v<T, JsonObject>)
        {
            return value.IsObject();
        }
        else if constexpr (std::is_same_v<T, JsonArray>)
        {
            return value.IsArray();
        }

        return value.Is<T>();
    }

    // JSONドキュメントのアロケータを取得
    rapidjson::Document::AllocatorType& GetAllocator() { return mDocument.GetAllocator(); }

    // JSONオブジェクトの取得
    const class JsonObject GetObject();
    // JSON配列の取得
    const class JsonArray GetArray();

private:
    rapidjson::Document mDocument;
};

