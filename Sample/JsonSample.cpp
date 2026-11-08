#include "GameMath.h"
#include "JsonData.h"
#include "JsonArray.h"
#include "JsonObject.h"
#include <iostream>

bool InputSample();
bool OutputSample();

int main()
{
    std::cout << "InputSample" << std::endl;
    if(!InputSample())
    {
        std::cout << "InputSample failed" << std::endl;
        return -1;
    }
    
    std::cout << "OutputSample" << std::endl;
    if(!OutputSample())
    {
        std::cout << "OutputSample failed" << std::endl;
        return -1;
    }

    std::cin.get();

    return 0;
}

bool InputSample()
{
    // 読込
    JsonData inputJson;
    if(!inputJson.LoadJSON("./InputSample.json"))
    {
        std::cout << "LoadJSON failed" << std::endl;
        return false;
    }
    JsonObject root = inputJson.GetObject();

    int id;
    JsonObject properties;

    // id
    if(!root.Get("id", id))
    {
        std::cout << "failed to get id" << std::endl;
        return false;
    }
    std::cout << "id: " << id << std::endl;

    // properties
    if(!root.Get("properties", properties))
    {
        std::cout << "failed to get properties" << std::endl;
        return false;
    }

    Vector3 pos;
    JsonArray array;

    // pos
    if(!properties.Get("pos", pos))
    {
        std::cout << "failed to get pos" << std::endl;
        return false;
    }
    std::cout << "pos: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;

    // array
    if(!properties.Get("array", array))
    {
        std::cout << "failed to get array" << std::endl;
        return false;
    }
    for(int i = 0; i < array.Size(); i++)
    {
        int tmp;
        if(!array.Get(i, tmp))
        {
            std::cout << "failed to get array element" << std::endl;
            return false;
        }
        std::cout << "array[" << i << "]: " << tmp << std::endl;
    }

    return true;
}

bool OutputSample()
{
    // 書込
    JsonData outputJson;
    JsonObject root;

    int id = 1;
    Vector3 pos(1.0f, 2.0f, 3.0f);
    JsonArray array;
    array.PushBack(10, outputJson);
    array.PushBack(20, outputJson);
    array.PushBack(30, outputJson);

    JsonObject properties;
    properties.AddMember("pos", pos, outputJson);
    properties.AddMember("array", array, outputJson);

    root.AddMember("id", id, outputJson);
    root.AddMember("properties", properties, outputJson);

    outputJson.CreateJsonData(root);

    if(!outputJson.SaveJSON("./OutputSample.json"))
    {
        std::cout << "SaveJSON failed" << std::endl;
        return false;
    }

    return true;
}