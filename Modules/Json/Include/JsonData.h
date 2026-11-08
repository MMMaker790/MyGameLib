#pragma once

#include "GameMath.h"
#include <rapidjson/document.h>
#include <string>

class JsonData
{
public:
    JsonData()
    {
        mDocument.SetObject();
    }

    // JSONファイルのロード
    bool LoadJSON(const std::string &fileName);
    bool SaveJSON(const std::string &fileName);

    // JSON配列、JSONオブジェクトからJsonDataを作成する
    bool CreateJsonData(const class JsonArray &jsonArray);
    bool CreateJsonData(const class JsonObject &jsonObject);

    // JSONドキュメントのアロケータを取得
    rapidjson::Document::AllocatorType& GetAllocator() { return mDocument.GetAllocator(); }

    // JSONオブジェクトの取得
    JsonObject GetObject();
    // JSON配列の取得
    JsonArray GetArray();

private:
    rapidjson::Document mDocument;
};

